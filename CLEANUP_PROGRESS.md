# LiveMusicApp cleanup — progress tracker

Working notes for the ongoing code-quality pass (build system fixes,
uninitialized-var/missing-return fixes, and now inline/doc comments).
Commit this file alongside each patch so it survives across sessions —
point future-Claude at it to resume without re-deriving context.

## Branches merged so far
- `buildsystem/drop-dead-deps` — dropped dead xmltok/gladeui deps, fixed
  libxml2/X11/Xmu pkg-config flags, LT_INIT. See patch
  `0001-buildsystem-drop-dead-deps-fix-includes.patch`.
- `codequality/uninitialized-vars-missing-returns` — fixed all 10
  uninitialized-variable and 19 missing-return warnings across the
  codebase (see bug log below for the interesting ones). See patch
  `0002-fix-uninitialized-vars-and-missing-returns.patch`.

## Comment pass — status

Depth per function: full doc-comment header (what it does, why, any
non-obvious behavior) + inline comments on genuinely tricky logic.
Skip nothing, including small/boilerplate functions — give them an
accurate one-liner at minimum.

| File | Status | Functions done / total |
|---|---|---|
| `src/AlsaUtils.c` | ✅ DONE | 23/23 |
| `src/HTML.c` | ✅ DONE | 34/34 |
| `src/PrefsFile.c` | ✅ DONE | 7/7 |
| `src/Chorder.c` | ✅ DONE | 15/15 |
| `src/LiveMusicApp.c` | ⬜ not started | 0/~55 |
| `src/PrefsGui.c` | ⬜ not started | 0/22 |
| `src/transport.c` | ⬜ not started | 0/17 |
| `src/HIDGrab.c` | ✅ DONE | 5/5 |
| `src/Player.c` | ✅ DONE | 45/45 |
| `src/SooperOSC.c` | ✅ DONE | 12/12 |

### HTML.c — done (all 34 functions)
`SavePresetChanges`, `on_Back_clicked`, `on_Forward_clicked`,
`web_view_javascript_finished`, `scroll_js_finished_cb`, `ScrollCtrl`,
`ScrollGetPosition`, `on_DrumLoopButton_clicked`, `on_TapTempo_clicked`,
`on_patch_clicked`, `on_patch__release_handler`, `on_SaveWeb_clicked`
(both `#if 0`/`#else` versions), `ScalePage`, `PageLoaded`,
`NavigationPolicy`, `OpenSetListSong`, `on_SetList_clicked`, `InitHTML`,
`HoverLink_cb`, `on_RightMenu_clicked`, `DrumFav_cb`, `SaveFav_cb`,
`EditChart_cb`, `Search_in_File`, `AssignPreset`, `SetPatchTitles`,
`ishex`, `DecodeURI`, `load_finished_cb`/`viewport_recompute_cb`/
`viewport_changed_cb`/`viewport_valid_changed_cb` (dead WebKit1-era
code, annotated as a block), `DownloadRequestcb`/`PolicyRequestCD`
(dead WebKit1-era code, annotated as a block).

### Remaining files — function inventories (for the checklist, not yet annotated)

**LiveMusicApp.c**: `layout_click_handler`, `layout_release_handler`,
`my_keypress_function`, `printd`, `activate`, `main`, `parse_cmdline`,
`GTKIdel_cb`, `GetResourceDir`, `CheckForStartupDirs`, `SwitchToTab`,
`on_button_clicked`, `on_About_clicked`, `on_window1_destroy`,
`on_Tempo_Button`, `tab_focus_callback`, `ClearMainButtons`,
`UpdateStatus`, `SetMetronomeStatus`, `click_handler`, `release_handler`,
`Notebook_click_handler`, `CreateTabButtons`, `CreateMainButtons`,
`MidiToDB`, `SetScale4Label`, `VScale1..4_Changed`, `SetVolume1..4`,
`SetUpMainButtons`, `RaiseWindowsNum`, `IncrementLayoutMode`,
`GuitarMidiPreset`, `GuitarMidiPresetComplete`, `FindString`,
`InitHistoryFile`, `WriteToHistory`, `CloseHistoryFile`,
`ConvertToLogF`, `ConvertToLogF_OLD`, `ConvertToLogInt`,
`SetExpressionControl`, `GetExpressionControl`, `LEDControl`,
`CreateHTMLGuide`, `create_pixbuf`, `CheckConnectionData`,
`apply_font_to_widget`, `on_hscale1_value_changed`.
(Biggest file by function count — consider splitting across sessions.)

**PrefsGui.c**: `on_BasePath_changed`, `Prefs_click_spin_handler`,
`Prefs_click_text_handler`, `Prefs_Regenerate_handler` (x2, one per
system() call — see source), `Prefs_ReConnect_handler`,
`Prefs_about_close_handler`, `InitGuiPrefs`, `CreatePatchModel`,
`CreateStringModel`, `PatchListEdited`, `text_editing_started`,
`CreatePatchViewModel`, `StringListEdited`, `CreateStringViewModel`,
`PortsListEdited`, `CreatePortsModel`, `CreatePortsViewModel`,
`CreateLayoutModel`, `LayoutsListEdited`, `CreateLayoutsViewModel`,
`ApplicationsListEdited`, `CreateApplicationViewModel`,
`CreateApplicationModel`.

**transport.c**: `timebase`, `jack_poll`, `jack_shutdown`,
`signal_handler`, `com_activate`, `com_deactivate`, `com_exit`,
`com_locate`, `com_master`, `com_play`, `com_release`, `com_stop`,
`com_tempo`, `com_setBeats`, `com_timeout`, `com_toggle`,
`InitJackTransport`, `CloseJackTransport`.

**Player.c — done (all functions)**: `LivePlayerInit`, `SetMP3PlayVol`,
`SaveLoopPopup_cb`, `ResetPlayer`, `LivePlayerClose`, `SetPlayerFile`,
`OpenSavedLoopFile`, `SaveLoopFile`, `PlayerWrite`, `PlayerPoll`,
`PositionSlider_Changed`, `SetASlider_Changed`, `SetBSlider_Changed`,
`SetAFineTickBox_Changed`, `SetBFineTickBox_Changed`,
`SpeedSlider_Changed`, `VolumeSlider_Changed`, `SetA_click_handler`,
`SetB_click_handler`, `Speed_click_handler`, `NewLoop_click_handler`,
`NewLoopDialog`, `NewMarker_click_handler`, `NewMarkerDialog`,
`SaveSongMarkers` (dead code, annotated as a block),
`EnterLoop_click_handler`, `NextSeg_click_handler`,
`PrevSeg_click_handler`, `Loop_click_handler`, `StartPlayer`,
`plScrub`, `Play_click_handler`, `Stop_click_handler`, `plSetA`,
`plSetB`, `plLoopToggle`, `plPlay`, `plStop`, `plPausePlay`,
`plNextSeg`, `plPrevSeg`, `plSpeedUp`, `plSpeedDown`, `plSetPosition`,
`plSeekFw`, `plSeekBk`.

**IMPORTANT**: this file was re-synced from the user's actual current
local Player.c (uploaded 2026-09-02) — includes their pause/resume fix
(now sends real "pause"/"pause" instead of "stop") and PipeWire env
var additions to the mplayer/mpv launch commands. If resuming in a
fresh sandbox clone, re-request the user's current Player.c before
continuing — don't use the plain git clone version, it's stale.

## Bug log (found during comment/audit passes)

Already fixed (in patch 0002):
- `AlsaUtils.c` `SendMidi`: uninitialized `err`.
- `AlsaUtils.c`/`HTML.c`/`Chorder.c`/`LiveMusicApp.c`: assorted
  uninitialized vars and missing returns — see patch 0002 for full list.
- `HTML.c` `ScalePage`/`NavigationPolicy`: uninitialized `Horiz`/`Vert`
  (real "fit to page" zoom bug) and `FileName`.
- `Player.c` `NewMarker_click_handler`: was destroying an uninitialized
  `GtkWidget*` on every click — dead leftover code, removed.
- `SooperOSC.c` `OSCErrHandler`: wrong return type
  (`lo_err_handler` instead of `void`) causing a real function-pointer
  type mismatch at its `lo_server_new()` call site.

Flagged but NOT fixed (found during comment pass — behavioral changes,
deliberately out of scope for a comments-only pass):
1. `AlsaUtils.c` `SendMidiPatch`: `TestProgram++` immediately
   overwritten by `TestProgram = 20` — every patch change sends
   program 20 regardless. Unclear if intentional debug leftover or bug.
2. `HTML.c` `SavePresetChanges`: `remove(OutFile)` passes a `FILE*`
   where `remove()` expects a filename string — does nothing useful,
   probably meant `remove(FileLine)`.
3. `HTML.c` `ScrollGetPosition`: busy-waits on `WaitForCallBack` with
   no `g_main_context_iteration()` call anywhere in the codebase — the
   async JS callback that would set the wait variable likely can never
   fire while the main thread is spinning in this loop. Likely always
   returns stale/wrong data.
4. `HTML.c` `on_TapTempo_clicked`: `Time1` declared as `struct timeval`
   but passed to `clock_gettime()`, which expects `struct timespec` —
   nanoseconds get misread as microseconds, likely corrupting the BPM
   calculation by ~1000x in its sub-second component. **User asked to
   fix this one specifically once the comment pass is done.**
5. `HTML.c` `PageLoaded`: the final `webkit_web_view_set_zoom_level(
   web_view, 1)` is unreachable dead code — every branch of the
   `.mp3`/`.mid`/`else` chain above it returns, so the zoom-to-100%
   reset after page load never actually runs.
6. `HTML.c` `NavigationPolicy`: the `.rtf`/no-extension/`.html`
   branches all `return (TRUE)` with a comment "let WebKit handle it",
   but returning TRUE from this callback (per the function's own
   original doc) means "we're handling it", and none of those
   branches call `webkit_policy_decision_ignore()`/`_use()`. A
   commented-out `return (FALSE)` right above each suggests this was
   inverted at some point. Less certain than the others — worth
   testing before touching.
7. `HTML.c` `OpenSetListSong`: builds the target song path as
   `dirname(set-list-file) + "/." + filename` — the literal `.` looks
   like it could be a stray typo (`strcat(Copy, "/.")` instead of
   `strcat(Copy, "/")`) that would make it look for a dot-prefixed
   hidden file that doesn't actually exist. No other code in the file
   writes set-list entries with a leading dot. Lower confidence — the
   set-list HTML file's actual `href=` format wasn't available to
   check against.
8. `PrefsFile.c` `XMLParseFileElement`: missing `break;` between
   `case 2:` and `case 3:` in the Level-dispatch switch (case 0 and
   case 1 both have one). Every Level-2 XML node parse also falls
   through into the Level-3 "Layout Patch" handling logic using
   whatever `ElementName[2]` is stale-leftover from a prior recursion.
   Usually a silent no-op, but a real correctness bug.
9. `Chorder.c` `InitChorder`: calls `gtk_window_get_default_size(window,
   ...)` where `window` is actually a `GtkFrame` (the "ChordFrame"
   embedded widget, per its caller in LiveMusicApp.c), not a
   `GtkWindow`. Would normally fail GTK's own type check and log a
   critical warning rather than crash - likely harmless since
   Fwidth/Fheight are already set correctly by an earlier
   `gtk_window_get_size(MainWindow, ...)` call, but still real API
   misuse.
10. `Chorder.c` `delete_event`/`destroy`: both connected to that same
    "ChordFrame" GtkFrame rather than a real top-level window.
    `delete_event` (GTK-tutorial boilerplate) likely never fires in
    practice since "delete-event" is normally only emitted by a
    window manager closing an actual GtkWindow. `destroy` calling
    `gtk_main_quit()` is more plausible as an intentional "quit when
    the app tears down" hook, but worth confirming it isn't firing
    earlier than intended.
11. `HIDGrab.c` `CloseHIDGrab`: `ioctl(theHID_fd, EVIOCGRAB, 1)` -
    value 1 ACQUIRES exclusive access, the exact same call
    `InitHIDGrab()` uses to grab the device. Releasing before close
    should be `EVIOCGRAB, 0`. Looks like a copy-paste of the acquire
    call instead of the intended release.
12. `HIDGrab.c` `InitHIDGrab`: `ioctl(scan_fd, EVIOCGRAB)` is missing
    its value argument entirely (undefined behavior), and targets
    `scan_fd` (the probe fd) rather than `theHID_fd` (the fd actually
    used afterward) - so even if it did something, it's the wrong
    descriptor. `scan_fd` is also never closed once a matching device
    is found - effectively leaked.
13. `SooperOSC.c` `MyOSCLoadFile`: the reply-address arguments to
    `/load_session` are hardcoded to `"osc.udp://localhost:9951/"`
    instead of using `our_url` (this app's own actual OSC server URL,
    used correctly elsewhere e.g. in MyOSCPoll()). If the app's OSC
    server isn't on port 9951, SooperLooper's response goes nowhere
    this app listens.
14. `SooperOSC.c` `MyOSCJackMute`: neither of its two channel
    switches has a `case 4:` (typeOSCVolGuitarR) - unlike
    MyOSCJackVol()'s JackVoladdr switch, which does cover it. Muting
    channel 4 sends nothing via OSC to either JackVoladdr or Carla
    (only the MIDI pedal-LED update still happens). Would need the
    right Carla parameter index to fill in correctly.
15. `Player.c` `LivePlayerInit`: repeated GTK API misuse pattern -
    `gtk_window_set_resizable()` called on MainBox/theFrame/
    PositionBox/PlayerFrame/SavedFrame/VolumeFrame/ImageWidget/
    PositionSlider/PositionSliderBox, none of which are GtkWindow
    (boxes/frames/an image/a slider). Same class of bug as
    Chorder.c's InitChorder() but repeated ~8 times here - looks
    systemic rather than a one-off typo.
16. `Player.c` `OpenSavedLoopFile`: classic `while (!feof(fd))` +
    unchecked `fscanf()` bug pattern - runs one extra iteration past
    the real last line. Low practical impact here since the array is
    zeroed up front and the resulting blank entry gets filtered by
    the "all zero" check, but still the wrong pattern. Also: the
    MaxSavedLoops-overflow early `return` skips the fclose() a few
    lines below, leaking the file descriptor until the next call's
    own close-if-already-open guard happens to catch it.
17. `Player.c` `SaveLoopFile`: the per-loop "Measures" calculation
    reads `mySavedLoops[Loop + 1].Start` unconditionally before
    checking whether `Loop` is the last index (where the result gets
    discarded and overwritten anyway) - if `NumSavedLoops` ever equals
    `MaxSavedLoops` exactly, this reads one entry past the last
    populated one. Likely harmless (result discarded, still in array
    bounds) but worth restructuring to check the last-index case
    first.
18. `Player.c` `SetPlayerFile`: builds a player `"load \"%s\"\n"`
    command into `PlayerString` but never sends it (`PlayerWrite()` is
    never called on it in this function) - dead code, likely leftover
    from an earlier IPC-based file-loading design before the app
    switched to relaunching the player process with the new file as a
    command-line argument instead.
19. `Player.c` `SetASlider_Changed`/`SetBSlider_Changed`: read the new
    coarse Loop-Start/Loop-End slider value into a local variable and
    do nothing else with it - no player command, no state update, just
    a log line. Only the fine spin-button counterparts
    (`SetAFineTickBox_Changed`/`SetBFineTickBox_Changed`) actually
    have any effect. Worth confirming whether the coarse sliders are
    meant to be purely visual/rough indicators or whether this is an
    oversight.
20. `Player.c` `NewMarkerDialog`: calls
    `gtk_combo_box_set_active(SaveCombo, 1)` with the literal constant
    `1` after adding a new marker - not the new entry's actual index
    (`NumSavedLoops` before incrementing, or `NumSavedLoops - 1`
    after). Always jumps the combo box to the second saved item
    regardless of how many entries exist or where the new one landed.
21. `Player.c` `EnterLoop_click_handler`: updates the selected saved
    loop's Start/Length/Position in memory but never calls
    `SaveLoopFile()` afterward (unlike `NewLoopDialog()`/
    `NewMarkerDialog()`, which both do). An edit here could be lost if
    the app closes uncleanly before something else triggers a save.
22. `Player.c` `StartPlayer`: `OutputString` is computed from the
    `UsePipewire` check (itself pointless since both branches set the
    same literal, then it's overwritten again regardless) but never
    actually used in either mplayer launch command - fully vestigial.
23. `Player.c` `plScrub`: dead code, no callers anywhere in the
    codebase - declared and defined but unused.

## Player.c pause/resume fix - explained
Now fully understood and documented in `plPausePlay()`'s own doc
comment. The fix: pausing now sends a real `pause` command to the
still-running mplayer (keeping it alive/responsive) instead of `stop`
(which unloaded it and broke position tracking). `StartPlayer()`
ALWAYS kills and relaunches mplayer on resume regardless (that part
didn't change) - what mattered was that `CurrentSongPosition` stays
accurate during the paused interval (via PlayerPoll()'s ongoing
polling, which only works if mplayer is still alive to answer
`get_time_pos`), so the relaunch seeds from the right position.

## Notes for Player.c specifically
DONE — see the "Player.c pause/resume fix - explained" section above.
Player.c was re-synced from the user's actual uploaded file
(2026-09-02) before commenting, so all comments describe the real,
current (fixed) behavior.

## To resume in a new session
1. Re-clone/pull the repo, checkout the latest merged branch.
2. Read this file for status.
3. Continue with the next file/function listed above — same standard:
   full doc header + inline comments on tricky bits, verify build
   after each function or small group, patch + PR per file (or batch
   of files) same as the build-system and bugfix passes.
