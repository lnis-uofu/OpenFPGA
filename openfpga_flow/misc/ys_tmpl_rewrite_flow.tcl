# Rewrite the .blif to Verilog
# so that the pin sequence matches
yosys -import

read_blif rewritten_${OUTPUT_BLIF}
write_verilog ${OUTPUT_VERILOG}
