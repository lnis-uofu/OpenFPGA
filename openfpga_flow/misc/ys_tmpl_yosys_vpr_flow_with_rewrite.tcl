# Yosys synthesis script for ${TOP_MODULE}
yosys -import

# Read verilog files
${READ_VERILOG_FILE}

# Technology mapping
hierarchy -top ${TOP_MODULE}
procs
techmap -D NO_LUT -map +/adff2dff.v

# Synthesis
synth -top ${TOP_MODULE} -flatten
clean

# LUT mapping
if { [info exists LUT_SIZE] } {
    abc -lut ${LUT_SIZE}
} else {
    abc -lut 4
}

# Check
synth -run check

# Clean and output blif
opt_clean -purge
write_blif rewritten_${OUTPUT_BLIF}
