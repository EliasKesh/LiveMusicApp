#!/bin/bash

PORT="RtMidiIn Client:RtMidi input"
CHANNEL=1

for BANK in {3..3}; do
    for PROGRAM in {25..99}; do

        # Send MIDI
        pw-jack python3 - "$PORT" "$BANK" "$PROGRAM" "$CHANNEL" <<'PY'
import sys
import mido

port_name = sys.argv[1]
bank = int(sys.argv[2])
program = int(sys.argv[3])
channel = int(sys.argv[4]) - 1

matches = [p for p in mido.get_output_names() if port_name in p]

if not matches:
    sys.exit(1)

with mido.open_output(matches[0]) as out:
    out.send(mido.Message('control_change',
                          control=0,
                          value=0,
                          channel=channel))

    out.send(mido.Message('control_change',
                          control=32,
                          value=bank,
                          channel=channel))

    out.send(mido.Message('program_change',
                          program=program,
                          channel=channel))
PY

        # Print AFTER MIDI was sent
        echo "Bank = $BANK   PC = $PROGRAM"

        while true; do
            read -n 1 -s -r -p "Press n for next: " key
            echo
            [[ ${key,,} == n ]] && break
        done
#        read -n 1 -s -r -p "Press any key for next..."
        echo

    done
done

echo "Finished."

