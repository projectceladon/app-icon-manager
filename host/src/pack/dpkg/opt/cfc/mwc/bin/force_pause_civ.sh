#!/bin/bash

INPUT=$(mktemp -u) || exit -1
OUTPUT=$(mktemp -u) || exit -1
#QMP_PATH=$(realpath ~/.intel/.civ/.penguin-peak.qmp.unix.socket)
QEMU_PID=`pidof qemu-system-x86_64`

if [ -z "$QEMU_PID" ];
then
    echo "Qemu process doesn't exit, skip."
    exit 0
fi

#QMP_PATH=$(realpath /proc/$QEMU_PID/cwd/.intel/.civ/.penguin-peak.qmp.unix.socket)
USERNAME=`ps --no-headers -o euser -p $QEMU_PID`
QMP_PATH=$(realpath /home/$USERNAME/.intel/.civ/.penguin-peak.qmp.unix.socket)
echo "QMP_PATH:$QMP_PATH"

function clean_up()
{
    exec 4<&-
    exec 5<&-
    [[ ! -z $INPUT && -f $INPUT ]] && rm $INPUT
    [[ ! -z $OUTPUT && -f $OUTPUT ]] && rm $OUTPUT
}
trap clean_up EXIT

function connect_qmp()
{
    local attempts=0
    while ! nc -z -U "$QMP_PATH"; do
        sleep 0.1
        (( attempts++ ))
        if ((attempts == 100)); then
            echo "Error: QMP Socket is not ready yet!"
            return -1
        fi
    done

    mkfifo "$INPUT" || exit -1
    mkfifo "$OUTPUT" || exit -1
    (cat "$INPUT" | nc -U "$QMP_PATH" > "$OUTPUT") &
    exec 4> "$INPUT"
    exec 5< "$OUTPUT"

    local out
    read -u 5 -r out;
    echo "Socket connected: $out"
    if ! grep -q "{.*QMP.*version.*qemu.*package.*capabilities.*}" <<< "$out"; then
        return -1
    fi

    echo "Send: qmp_capabilities"
    echo '{ "execute": "qmp_capabilities" }' >&4
    read -u 5 -r out
    echo "QMP Capabilities: $out"
    if [[ ${#out} -eq 15 && "${out:0:14}" == '{"return": {}}' ]]; then
        echo "QMP connected!"
    else
        echo "Failed to connect QMP!"
        return -1
    fi
}

function pause_civ()
{
    echo "Send QMP: stop"
    local out
    local i
    echo "{ \"execute\": \"stop\"}" >&4
    for i in {0..9}; do
        read -u 5 -t 1 -r out
        echo "OUTPUT: $out"
        if [[ ${#out} -eq 15 && "${out:0:14}" == '{"return": {}}' ]]; then
            return 0
        fi
    done
    echo "Failed to Pause CiV!"
    return -1
}

connect_qmp || exit -1

pause_civ || exit -1
echo "Pause CIV executed."
