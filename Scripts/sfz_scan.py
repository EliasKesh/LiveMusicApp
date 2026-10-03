#!/usr/bin/env python3
"""
sfz_scan.py - walk a tree of SFZ instruments and pick out the ones likely to work.

Read-only with respect to your SoundFonts: it only writes its reports into
--out-dir (default ./sfz_scan_out) and temporary WAVs for --render.

Static checks per .sfz (no sfizz needed):
  * expands #include and #define, honours <control> default_path
  * extracts every sample= and classifies it:
      - generator (*sine, *saw, ...)  -> placeholder tone, not a real instrument
      - real file: must exist (case-sensitive, as on Linux), be non-empty,
        not be a Git-LFS pointer, and have a valid wav/flac/ogg/aiff header
  * flags files with no <region>, missing includes, case mismatches

Status values:
  OK         every referenced sample file is present and looks valid
  PARTIAL    >= --min-ratio of samples valid (default 0.9), some missing
  GENERATOR  only built-in oscillators (the "just a tone" patches)
  BROKEN     too many samples missing/invalid
  EMPTY      no regions / no sample= at all
  UNREADABLE could not read the file
  FRAGMENT   file is #included by another scanned .sfz and fails on its own
             (normal for include pieces; judge the including file instead)

Optional --render: for candidates, run sfizz_render on a short built-in test
MIDI and check the output WAV is not silent. This is the authoritative test;
the static check can only say "should load".

Python 3.8+, standard library only.
"""

import argparse
import array
import csv
import os
import re
import shutil
import subprocess
import sys
import tempfile
import wave
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

DEFAULT_ROOT = "/home/SoundFonts/SFZFonts"
GENERATORS = {"sine", "saw", "square", "triangle", "tri", "noise", "gnoise", "silence"}

COMMENT_RE = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
INCLUDE_RE = re.compile(r'^\s*#include\s+"([^"]+)"')
DEFINE_RE = re.compile(r"^\s*#define\s+(\$\w+)\s+(.*?)\s*$")
# An opcode value ends at the next " opcode=", the next <header>, or end of line.
_END = r"(?=\s+[A-Za-z_]\w*=|\s*<|\s*$)"
SAMPLE_RE = re.compile(r"(?<![\w])sample\s*=\s*(.+?)" + _END, re.M)
DEFPATH_RE = re.compile(r"(?<![\w])default_path\s*=\s*(.*?)" + _END, re.M)
REGION_RE = re.compile(r"<region>", re.I)


# ----------------------------------------------------------------- file helpers
def read_text(path: str) -> str:
    with open(path, "rb") as f:
        raw = f.read()
    for enc in ("utf-8-sig", "latin-1"):
        try:
            return raw.decode(enc)
        except UnicodeDecodeError:
            continue
    return raw.decode("latin-1", errors="replace")


_ls_cache: Dict[str, Dict[str, str]] = {}
INCLUDED: set = set()  # real paths pulled in by some other .sfz via #include


def _ls_lower(d: str) -> Dict[str, str]:
    r = _ls_cache.get(d)
    if r is None:
        try:
            r = {n.lower(): n for n in os.listdir(d)}
        except OSError:
            r = {}
        _ls_cache[d] = r
    return r


def resolve(base: str, rel: str) -> Optional[Tuple[str, str]]:
    """Return ('exact'|'case', real_path) or None. Backslashes are treated as '/'."""
    rel = rel.replace("\\", "/")
    full = os.path.normpath(os.path.join(base, rel))
    if os.path.isfile(full):
        return "exact", full
    cur, kind = os.sep, "exact"
    for part in full.split(os.sep):
        if part == "":
            continue
        actual = _ls_lower(cur).get(part.lower())
        if actual is None:
            return None
        if actual != part:
            kind = "case"
        cur = os.path.join(cur, actual)
    return (kind, cur) if os.path.isfile(cur) else None


def check_audio(path: str) -> str:
    """'ok' or a short reason the file is unusable."""
    try:
        size = os.path.getsize(path)
        with open(path, "rb") as f:
            head = f.read(64)
    except OSError:
        return "unreadable"
    if size == 0:
        return "empty-file"
    if head.startswith(b"version https://git-lfs"):
        return "git-lfs-pointer"
    ext = os.path.splitext(path)[1].lower()
    if ext == ".wav":
        ok = head[:4] in (b"RIFF", b"RF64", b"BW64") and head[8:12] == b"WAVE"
    elif ext == ".flac":
        ok = head[:4] == b"fLaC"
    elif ext in (".ogg", ".oga", ".opus"):
        ok = head[:4] == b"OggS"
    elif ext in (".aif", ".aiff", ".aifc"):
        ok = head[:4] == b"FORM"
    elif ext == ".wv":
        ok = head[:4] == b"wvpk"
    else:
        return "unsupported-ext"
    return "ok" if ok else "bad-header"


# ------------------------------------------------------------------- SFZ parse
def expand(path: str, root_dir: str, defines: Dict[str, str], problems: List[str],
           depth: int = 0, seen: Tuple[str, ...] = ()) -> str:
    text = COMMENT_RE.sub("", read_text(path))
    out: List[str] = []
    for line in text.splitlines():
        m = DEFINE_RE.match(line)
        if m:
            defines[m.group(1)] = m.group(2)
            continue
        m = INCLUDE_RE.match(line)
        if m:
            inc = apply_defines(m.group(1), defines)
            hit = resolve(os.path.dirname(path), inc) or resolve(root_dir, inc)
            if hit is None:
                problems.append(f"missing include: {inc}")
                continue
            real = os.path.realpath(hit[1])
            INCLUDED.add(real)
            if depth >= 16 or real in seen:
                problems.append(f"include loop/too deep: {inc}")
                continue
            try:
                out.append(expand(hit[1], root_dir, defines, problems, depth + 1, seen + (real,)))
            except OSError:
                problems.append(f"unreadable include: {inc}")
            continue
        out.append(line)
    return "\n".join(out)


def apply_defines(s: str, defines: Dict[str, str]) -> str:
    for k in sorted(defines, key=len, reverse=True):
        s = s.replace(k, defines[k])
    return s


@dataclass
class Result:
    path: str
    status: str = "UNREADABLE"
    regions: int = 0
    real_total: int = 0
    real_ok: int = 0
    generators: int = 0
    notes: List[str] = field(default_factory=list)
    render: str = ""

    @property
    def ratio(self) -> float:
        return self.real_ok / self.real_total if self.real_total else 0.0


def analyze(p: str, audio_cache: Dict[str, str], min_ratio: float) -> Result:
    r = Result(path=p)
    root_dir = os.path.dirname(p)
    defines: Dict[str, str] = {}
    problems: List[str] = []
    try:
        text = expand(p, root_dir, defines, problems, 0, (os.path.realpath(p),))
    except OSError as e:
        r.notes.append(f"unreadable: {e}")
        return r

    m = DEFPATH_RE.search(text)
    default_path = apply_defines(m.group(1).strip(), defines) if m else ""
    sample_base = os.path.join(root_dir, default_path.replace("\\", "/"))

    r.regions = len(REGION_RE.findall(text))
    seen = set()
    samples: List[str] = []
    for m in SAMPLE_RE.finditer(text):
        s = apply_defines(m.group(1).strip().strip('"'), defines)
        if s and s not in seen:
            seen.add(s)
            samples.append(s)

    bad: Dict[str, List[str]] = {}
    for s in samples:
        if s.startswith("*"):
            if s[1:].lower() in GENERATORS:
                r.generators += 1
            else:
                r.real_total += 1
                bad.setdefault("unknown generator", []).append(s)
            continue
        r.real_total += 1
        hit = resolve(sample_base, s)
        if hit is None:
            bad.setdefault("missing", []).append(s)
            continue
        kind, real = hit
        if kind == "case":
            bad.setdefault("case-mismatch", []).append(f"{s} -> {os.path.basename(real)}")
            continue
        st = audio_cache.get(real)
        if st is None:
            st = audio_cache[real] = check_audio(real)
        if st == "ok":
            r.real_ok += 1
        else:
            bad.setdefault(st, []).append(s)

    for reason, items in bad.items():
        r.notes.append(f"{reason} x{len(items)} (e.g. {items[0]})")
    r.notes.extend(problems)

    if r.regions == 0 or (r.real_total == 0 and r.generators == 0):
        r.status = "EMPTY"
    elif r.real_total == 0:
        r.status = "GENERATOR"
    elif r.ratio == 1.0:
        r.status = "PARTIAL" if problems else "OK"
    elif r.ratio >= min_ratio:
        r.status = "PARTIAL"
    else:
        r.status = "BROKEN"
    if r.status == "OK" and r.generators:
        r.notes.append(f"mixed: {r.generators} generator region(s)")
    return r


# ---------------------------------------------------------------- render check
def varlen(n: int) -> bytes:
    b = [n & 0x7F]
    n >>= 7
    while n:
        b.append((n & 0x7F) | 0x80)
        n >>= 7
    return bytes(reversed(b))


def make_test_midi(path: str) -> None:
    """Format-0 MIDI, ch1, spread of notes at velocity 100, 480 tpq."""
    keys = [28, 36, 43, 48, 55, 60, 67, 72, 79, 84, 91]
    trk = b""
    for i, k in enumerate(keys):
        trk += varlen(0 if i == 0 else 120) + bytes([0x90, k, 100])
        trk += varlen(240) + bytes([0x80, k, 0])
    trk += varlen(960) + b"\xff\x2f\x00"
    with open(path, "wb") as f:
        f.write(b"MThd" + (6).to_bytes(4, "big") + (0).to_bytes(2, "big")
                + (1).to_bytes(2, "big") + (480).to_bytes(2, "big"))
        f.write(b"MTrk" + len(trk).to_bytes(4, "big") + trk)


def wav_peak(path: str) -> Optional[float]:
    try:
        with wave.open(path) as w:
            sw = w.getsampwidth()
            data = w.readframes(w.getnframes())
    except Exception:
        return None
    if sw == 2:
        a = array.array("h")
        a.frombytes(data[: len(data) // 2 * 2])
        return max((abs(x) for x in a), default=0) / 32768.0
    if sw == 4:
        a = array.array("i")
        a.frombytes(data[: len(data) // 4 * 4])
        return max((abs(x) for x in a), default=0) / 2147483648.0
    if sw == 3:
        peak = 0
        for i in range(0, len(data) - 2, 3 * 8):
            peak = max(peak, abs(int.from_bytes(data[i:i + 3], "little", signed=True)))
        return peak / 8388608.0
    return None


def render_one(r: Result, sfizz: str, midi: str, timeout: int) -> None:
    with tempfile.TemporaryDirectory() as td:
        out = os.path.join(td, "o.wav")
        try:
            cp = subprocess.run([sfizz, "--sfz", r.path, "--midi", midi, "--wav", out],
                                capture_output=True, text=True, errors="replace", timeout=timeout)
        except subprocess.TimeoutExpired:
            r.render = "TIMEOUT"
            return
        except OSError as e:
            r.render = "ERROR"
            r.notes.append(f"render: {e}")
            return
        if cp.returncode != 0 or not os.path.isfile(out):
            r.render = "FAILED"
            tail = (cp.stderr or cp.stdout or "").strip().splitlines()
            if tail:
                r.notes.append(f"render: {tail[-1][:120]}")
            return
        peak = wav_peak(out)
        if peak is None:
            r.render = "RENDERED" if os.path.getsize(out) > 1000 else "FAILED"
        else:
            r.render = "OK" if peak > 1e-4 else "SILENT"


# ------------------------------------------------------------------------ main
def find_sfz(root: str) -> List[str]:
    found, visited = [], set()
    for d, dirs, files in os.walk(root, followlinks=True):
        real = os.path.realpath(d)
        if real in visited:
            dirs[:] = []
            continue
        visited.add(real)
        dirs[:] = [x for x in dirs if x != "__MACOSX"]
        for fn in files:
            if fn.lower().endswith(".sfz") and not fn.startswith("._"):
                found.append(os.path.join(d, fn))
    return sorted(found)


def main() -> int:
    ap = argparse.ArgumentParser(description="Find SFZ files likely to work with sfizz.")
    ap.add_argument("root", nargs="?", default=DEFAULT_ROOT)
    ap.add_argument("--out-dir", default="sfz_scan_out")
    ap.add_argument("--min-ratio", type=float, default=0.9,
                    help="fraction of samples that must be valid for PARTIAL (default 0.9)")
    ap.add_argument("--include-generators", action="store_true",
                    help="also list generator-only (*sine etc.) patches as working")
    ap.add_argument("--render", action="store_true",
                    help="smoke-test candidates with sfizz_render and drop silent/failed ones")
    ap.add_argument("--sfizz-render", default=None, help="path to sfizz_render")
    ap.add_argument("--jobs", type=int, default=min(4, os.cpu_count() or 1))
    ap.add_argument("--timeout", type=int, default=60, help="per-file render timeout (s)")
    args = ap.parse_args()

    root = os.path.abspath(args.root)
    if not os.path.isdir(root):
        print(f"not a directory: {root}", file=sys.stderr)
        return 2
    sfizz = args.sfizz_render or shutil.which("sfizz_render")
    if args.render and not sfizz:
        print("--render needs sfizz_render in PATH (or --sfizz-render PATH)", file=sys.stderr)
        return 2

    files = find_sfz(root)
    print(f"Found {len(files)} .sfz files under {root}", file=sys.stderr)
    audio_cache: Dict[str, str] = {}
    results: List[Result] = []
    for i, p in enumerate(files, 1):
        results.append(analyze(p, audio_cache, args.min_ratio))
        if i % 200 == 0:
            print(f"  scanned {i}/{len(files)}", file=sys.stderr)

    for r in results:
        if r.status in ("BROKEN", "EMPTY") and os.path.realpath(r.path) in INCLUDED:
            r.status = "FRAGMENT"

    candidates = [r for r in results if r.status in ("OK", "PARTIAL")
                  or (args.include_generators and r.status == "GENERATOR")]

    if args.render:
        midi_dir = tempfile.mkdtemp(prefix="sfz_scan_")
        midi = os.path.join(midi_dir, "test.mid")
        make_test_midi(midi)
        print(f"Rendering {len(candidates)} candidates with {sfizz} ...", file=sys.stderr)
        with ThreadPoolExecutor(max_workers=max(1, args.jobs)) as ex:
            list(ex.map(lambda r: render_one(r, sfizz, midi, args.timeout), candidates))
        shutil.rmtree(midi_dir, ignore_errors=True)
        working = [r for r in candidates if r.render in ("OK", "RENDERED")]
    else:
        working = candidates

    os.makedirs(args.out_dir, exist_ok=True)

    def write_list(name: str, items: List[Result]) -> str:
        p = os.path.join(args.out_dir, name)
        with open(p, "w") as f:
            for r in items:
                f.write(r.path + "\n")
        return p

    p_work = write_list("sfz_working.txt", working)
    write_list("sfz_generators.txt", [r for r in results if r.status == "GENERATOR"])
    write_list("sfz_broken.txt", [r for r in results if r.status in ("BROKEN", "EMPTY", "UNREADABLE")])
    if args.render:
        write_list("sfz_render_failed.txt", [r for r in candidates if r.render not in ("OK", "RENDERED")])

    csv_path = os.path.join(args.out_dir, "sfz_report.csv")
    with open(csv_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["path", "status", "render", "regions", "real_samples", "valid",
                    "ratio", "generators", "notes"])
        for r in results:
            w.writerow([r.path, r.status, r.render, r.regions, r.real_total, r.real_ok,
                        f"{r.ratio:.2f}", r.generators, " | ".join(r.notes)])

    counts: Dict[str, int] = {}
    for r in results:
        counts[r.status] = counts.get(r.status, 0) + 1
    print("\nStatic scan:")
    for k in ("OK", "PARTIAL", "GENERATOR", "BROKEN", "EMPTY", "FRAGMENT", "UNREADABLE"):
        if counts.get(k):
            print(f"  {k:<10} {counts[k]}")
    if args.render:
        rc: Dict[str, int] = {}
        for r in candidates:
            rc[r.render] = rc.get(r.render, 0) + 1
        print("Render test: " + ", ".join(f"{k}={v}" for k, v in sorted(rc.items())))
    print(f"\n{len(working)} likely-working instruments -> {p_work}")
    print(f"Full report -> {csv_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
