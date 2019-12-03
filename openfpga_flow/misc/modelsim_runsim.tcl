
echo "=========================="
pwd
echo "=========================="

set projectname ${PROJECTNAME}
set benchmark ${BENCHMARK}
set top_tb ${TOP_TB}
#in ms
set simtime ${SIMTIME}
set unit ${UNIT}

#Path were both tcl script are located
set verilog_path ${VERILOG_PATH}
set project_path "${MODELSIM_PROJ_DIR}/msim_projects/"

#Path were the verilog files are located
set verilog_path ${VERILOG_PATH}

set verilog_files [list \
  ${VERILOG_PATH}${VERILOG_FILE1} \
  ${VERILOG_PATH}${VERILOG_FILE2} \
  ${VERILOG_PATH}fpga_defines.v
  ]

#Source the tcl script
source ${MODELSIM_PROJ_DIR}/${BENCHMARK}_autocheck_proc.tcl

#Execute the top level procedure

try {
  top_create_new_project $$projectname $$verilog_files $$project_path $$simtime $$unit $$top_tb
} finally {
   quit
}

#Relaunch simulation
