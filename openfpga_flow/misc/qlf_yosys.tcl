# Yosys synthesis script for ${TOP_MODULE}
yosys -import
if { [info exists ::env(YOSYS)] } {
    set yosys_dir [file dirname $env(YOSYS)]
    if { [file exists $yosys_dir/../share/yosys/plugins/ql-qlf.so] == 1 } { plugin -i ql-qlf }
    yosys -import  ;# ingest plugin commands
} 

# Read verilog files
${READ_VERILOG_FILE}

synth_quicklogic -blif ${OUTPUT_BLIF} -top ${TOP_MODULE} ${YOSYS_ARGS}

write_verilog -noattr -nohex ${OUTPUT_VERILOG}
