#!/bin/bash
#title           : openfpga.sh
#description     : This script provides shortcut commands <bash functions>
#                  for several simple operations in OpenFPGA project
#author          : Ganesh Gore <ganesh.gore@utah.edu>
#==============================================================================

export OPENFPGA_PATH="$(pwd)"
export OPENFPGA_TASK_PATH="$(pwd)/openfpga_flow/tasks"

# This function checks the path and
# raises warning if the command is not executing
# inside current OpendFPGA folder
check_execution_path (){
    if [[ $1 != *"${OPENFPGA_PATH}"* ]]; then
        echo -e "\e[33mCommand is not executed from configured OPNEFPGA directory\e[0m"
    fi
}

# lists all the configure task in task directory
list-tasks () {
    check_execution_path "$(pwd)"
    ls -tdalh ${OPENFPGA_TASK_PATH}/* | awk '{printf("%-4s | %s %-3s | ", $5, $6, $7) ;system("basename " $9)}'
}

# Switch directory to root of OpenFPGA
goto-root () {
    cd $OPENFPGA_PATH
}

# Changes directory to task directory [goto_task <task_name> <run_num[default 0]>]
goto-task () {
    if [ -z $1 ]; then
        echo "requires task name goto_task <task_name> <run_num[default 0]>"
        return
    fi
    goto_path=$OPENFPGA_TASK_PATH/$1
    run_num="latest"
    if [ ! -d $goto_path ]; then echo "Task directory not found"; return; fi
    if [[ $2 == '^[0-9]+$' ]] ; then
        echo "Second argumetn provided"
        if ! [[ $2 == '0' ]] ; then run_num="$(printf run%03d $2)" else run_num="latest" fi
        if [ ! -d "$goto_path/$run_num" ]; then run_num="latest" fi
    fi
    if [ ! -d $goto_path/$run_num ]; then
        echo "\e[33mTask run directory not found -" $goto_path/$run_num "\e[0m";
    else
        cd $goto_path/$run_num
    fi
}

# Clears enviroment variables and fucntions
unset_openfpga (){
    unset -v OPENFPGA_PATH
    unset -f list-tasks goto-task goto-root >/dev/null 2>&1
}