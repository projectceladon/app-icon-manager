#!/bin/bash

MONITOR_FOLDER_1=$HOME"/Android/Download"
MONITOR_FOLDER_2=$HOME"/Android/Pictures"

INPUT=$(mktemp -u) || exit -1
OUTPUT=$(mktemp -u) || exit -1
QMP_PATH=$(realpath ~/.intel/.civ/.penguin-peak.qmp.unix.socket)

function cleanup_tmp()
{
    exec 6<&-
    exec 7>&-

    for pid in $(jobs -r -p); do
        # kill all child process of jobs, in case of child processes
        # become child of systemd and unable to release resouces(e.g.: nc)
        pkill -P $pid
    done

    rm $INPUT
    rm $OUTPUT
}

function connect_qmp() {

    local attempts=0
    while ! timeout 1 nc -z -U "$QMP_PATH"; do
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
    exec 6> "$INPUT"
    exec 7< "$OUTPUT"

    local out
    read -u 7 -r out
    echo "Socket connected: $out"
    if ! grep -q "{.*QMP.*version.*qemu.*package.*capabilities.*}" <<< "$out"; then
        return -1
    fi

    echo "Send: qmp_capabilities"
    echo '{ "execute": "qmp_capabilities" }' >&6
    read -u 7 -r out
    echo "QMP Capabilities: $out"
    if [[ ${#out} -eq 15 && "${out:0:14}" == '{"return": {}}' ]]; then
        echo "QMP connected!"
    else
        echo "Failed to connect QMP!"
        return -1
    fi
}

function disconnect_qmp() {
    echo "disconnect QMP"
    cleanup_tmp || exit -1
}

function folder_update() {

    connect_qmp || exit -1

    echo "Send QMP: query status"
    local out

    echo "{ \"execute\": \"query-status\"}" >&6
    read -u 7 -t 1 -r out

    if [[ "${out:23:7}" =~ "running" ]]; then
        local ADB_CONNECT='adb connect vsock:3:5555'

        local FOLDER_1_UPDATE='adb -s vsock:3:5555 shell am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///storage/emulated/0/Download' < /dev/null
        local FOLDER_2_UPDATE='adb -s vsock:3:5555 shell am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///storage/emulated/0/Pictures' < /dev/null
        $ADB_CONNECT
        $FOLDER_1_UPDATE
        $FOLDER_2_UPDATE
    fi
    disconnect_qmp
}

function folder_notify() {
    inotifywait -rm -e create -e delete -e move $MONITOR_FOLDER_1 $MONITOR_FOLDER_2
}

function check_wechat_folder() {
    if [[ ! -d $MONITOR_FOLDER_1"/WeChat" ]];then
        mkdir -p $MONITOR_FOLDER_1"/WeChat"
    fi
    if [[ ! -d $MONITOR_FOLDER_1"/WeiXin" ]];then
        mkdir -p $MONITOR_FOLDER_1"/WeiXin"
    fi
    if [[ ! -d $MONITOR_FOLDER_2"/WeChat" ]];then
        mkdir -p $MONITOR_FOLDER_2"/WeChat"
    fi
    if [[ ! -d $MONITOR_FOLDER_2"/WeiXin" ]];then
        mkdir -p $MONITOR_FOLDER_2"/WeiXin"
    fi
}

function check_all_when_first_run() {
    chmod -R o+rwx $MONITOR_FOLDER_1
    chmod -R o+rwx $MONITOR_FOLDER_2

    check_wechat_folder

    folder_update
}

check_all_when_first_run

folder_notify | while read -r line
do
    echo $line
    if [[ "$line" =~ "CREATE" ]] || [[ "$line" =~ "MOVED_TO" ]];then
        chmod -R o+rwx $MONITOR_FOLDER_1
        chmod -R o+rwx $MONITOR_FOLDER_2
    fi

    if [[ "$line" =~ "DELETE" ]] || [[ "$line" =~ "MOVED_FROM" ]];then
        check_wechat_folder
    fi

    folder_update

done

