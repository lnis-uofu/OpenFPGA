#!/bin/bash

# ==========================================================================
# Note: This shell script is parsed using python3 string templating system
# For correct parsing, please use $$ symbol in place of single \dollar sign
# variables can be declared using \dollar var_name
# Ref : https://docs.python.org/3/library/string.html#template-strings
# ==========================================================================
VTR_RUNTIME_ESTIMATE_SECONDS=1.70
VTR_MEMORY_ESTIMATE_BYTES=0

VTR_RUNTIME_ESTIMATE_HUMAN_READABLE="2 seconds"
VTR_MEMORY_ESTIMATE_HUMAN_READABLE="0.00 MiB"

#We redirect all command output to both stdout and the log file with 'tee'.

#Begin I/O redirection
{

    ${fpga_flow_script} \
    -conf ${conf_file} \
    -benchmark ${benchmark_list_file} \
    -rpt ${csv_rpt_file} \
    -vpr_fpga_verilog_dir ${verilog_output_path} \
    ${additional_params}

    #The IO redirection occurs in a sub-shell,
    #so we need to exit it with the correct code
    exit $$?

} |& tee vtr_flow.out
#End I/O redirection

#We used a pipe to redirect IO.
#To get the correct exit status we need to exit with the
#status of the first element in the pipeline (i.e. the real
#command run above)
exit $${PIPESTATUS[0]}
