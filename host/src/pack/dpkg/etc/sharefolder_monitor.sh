#!/bin/bash

MONITOR_FOLDER=$HOME"/Android"
FOLDER_UPDATE='adb shell am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///storage/emulated/0'
FIRST_RUN=

function folder_notify() {
    inotifywait -rm -e create -e delete -e move $MONITOR_FOLDER
}

function tree_check() {
    tree -ifp $1
}

function folder_permission_check() {
    STAT=$(echo $1 | awk -F"  " '{print $1}')
    FILE_NAME=$(echo $1 | awk -F"  " '{print $2}')
    O_READ_STAT=${1:8:1}
    O_EXEC_STAT=${1:10:1}
    if [[ $1 == "["* ]];then
       if [[ -d "$2" ]];then
           if [[ "$O_READ_STAT" != "r" ]] || [[ "$O_EXEC_STAT" != "x" ]] ;then
               echo "The file/folder $2 has no read permission, add it for user"
               chmod o+rx $2
               if [[ "$FIRST_RUN" != "True" ]];then
                   file_update $2
               fi
           fi
       elif [[ -f "$2" ]];then
           file_permission_check $2
       fi
    fi
}

function file_permission_check() {
    STAT="ls -l $1"
    while read stat
    do
        if [[ "${stat:7:1}" != "r" ]];then
            echo "The file/folder $1 has no read permission, add it for user"
            chmod o+r $1
            if [[ "$FIRST_RUN" != "True" ]];then
                file_update $1
            fi
        fi
    done < <($STAT)
}

function file_update() {
    UPDATE_FILE=$(echo $1 | awk -F"Android" '{print $2}')
    SCAN_FILE=$FOLDER_UPDATE$UPDATE_FILE
    echo $SCAN_FILE
    $SCAN_FILE
}

function check_all_when_first_run() {
    FIRST_RUN="True"

    while read line
    do
        FILESTAT=$(echo $line | awk -F"  " '{print $1}')
        FILE=$(echo $line | awk -F"  " '{print $2}')
        folder_permission_check $FILESTAT $FILE
    done < <(tree_check $MONITOR_FOLDER)

    $FOLDER_UPDATE

}


check_all_when_first_run


while read -r line
do
    FILEPATH=$(echo $line | awk -F" " '{print $1}')
    FILENAME=$(echo $line | awk -F" " '{print $3}')
    echo $FILEPATH$FILENAME
    if [[ "$line" =~ "DELETE" ]] || [[ "$line" =~ "MOVED_FROM" ]];then
        echo $line
        file_update $FILEPATH$FILENAME
    elif [[ "$line" =~ "CREAT" ]] || [[ "$line" =~ "MOVED_TO" ]];then
        if [[ "$line" =~ "ISDIR" ]];then
            while read -r line
            do
                FILESTAT=$(echo $line | awk -F"  " '{print $1}')
                FILE=$(echo $line | awk -F"  " '{print $2}')
                folder_permission_check $FILESTAT $FILE
            done < <(tree_check $FILEPATH)
        else
            file_permission_check $FILEPATH$FILENAME
        fi
    fi
done < <(folder_notify)

