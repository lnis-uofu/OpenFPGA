
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
set project_path "${MODELSIM_PROJ_DIR}/msim_projects/"

#Path were the verilog files are located
set verilog_files ${VERILOG_PATH}/*_include_netlists_resolved.v

#Source the tcl script
source ${MODELSIM_PROJ_DIR}/${BENCHMARK}_autocheck_proc.tcl

#Execute the top level procedure
try {
  top_create_new_project $$projectname $$verilog_files $$project_path $$simtime $$unit $$top_tb
} finally {
   quit
}
