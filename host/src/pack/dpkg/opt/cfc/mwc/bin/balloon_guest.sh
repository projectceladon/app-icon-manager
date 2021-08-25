#!/bin/bash

BALLOON_SIZE=2048
INPUT=$(mktemp -u) || exit -1
OUTPUT=$(mktemp -u) || exit -1
QMP_PATH=$(realpath ~/.intel/.civ/.penguin-peak.qmp.unix.socket)

function clean_up()
{
    exec 4<&-
    exec 5<&-
    [[ ! -z $INPUT && -f $INPUT ]] && rm $INPUT
    [[ ! -z $OUTPUT && -f $OUTPUT ]] && rm $OUTPUT
}
trap clean_up EXIT

function usage()
{
    echo -e "Usage: $0 \$BALLOON_SIZE_IN_MEGABYTES"
    echo -e "\tExample(balloon to 2048MB): $0 2048"
}

function parse_param()
{
    if [ $# != 1 ]; then
        usage
        return -1
    fi

    if [[ ! -z $1 && "$1" =~ ^[0-9]+$ ]]; then
        if ! [[ "$1" =~ ^[0-9]+$ ]]; then
            echo "Error: Invalid parameter: $1"
            usage
            return -1
        fi
        BALLOON_SIZE=$((10#$1))
    else
        echo "Error: Invalid parameter: $1"
        usage
        return -1
    fi
}

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

function balloon()
{

    local TARGET_BALLOON=$(($1*1024*1024))
    echo "Target ballon size is $((TARGET_BALLOON/1024/1024)) MB"

    local out

    echo "Send: query-balloon"
    echo '{ "execute": "query-balloon" }' >&4
    read -u 5 -r out
    echo "Current balloon: $out"
    local actual_sz=$(jq -r .return.actual <<< "$out")
    if ((actual_sz == TARGET_BALLOON)); then
        echo "Balloon size is already $((TARGET_BALLOON/1024/1024)) MB!"
        return 0
    fi

    echo "Send: balloon $TARGET_BALLOON"
    echo "{ \"execute\": \"balloon\", \"arguments\": { \"value\": $TARGET_BALLOON } }" >&4
    while read -t 10 -u 5 -r out; do
        echo "$out"
        local event=$(jq -r .event <<< $out)
        case $event in
            BALLOON_CHANGE)
                actual_sz=$(jq -r .data.actual <<< "$out")
                if ((actual_sz == TARGET_BALLOON));then
                    break
                fi
                ;;
            *)
                continue
                ;;
        esac
    done

    echo "Send: query-balloon"
    echo '{ "execute": "query-balloon" }' >&4
    read -u 5 -r out
    echo "$out"
    actual_sz=$(jq -r .return.actual <<< "$out")
    if ((actual_sz != TARGET_BALLOON)); then
        echo "Failed balloon to $((TARGET_BALLOON/1024/1024)) MB! Actual balloon size is $((actual_sz/1024/1024)) MB!"
        return -1
    else
        echo "Succeed balloon to $((TARGET_BALLOON/1024/1024)) MB!"
    fi
}

parse_param "$@" || exit -1
connect_qmp || exit -1
balloon $BALLOON_SIZE
