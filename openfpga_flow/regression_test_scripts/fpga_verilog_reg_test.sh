#!/bin/bash

set -e
source openfpga.sh
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-Verilog Feature Tests";

echo -e "Testing Verilog generation for LUTs: a single mode LUT6 FPGA using micro benchmarks";
run-task fpga_verilog/lut_design/single_mode $@

echo -e "Testing Verilog generation for LUTs: simple fracturable LUT4 ";
run-task fpga_verilog/lut_design/frac_lut4 $@

echo -e "Testing Verilog generation for LUTs: fracturable LUT4 with embedded carry logic";
run-task fpga_verilog/lut_design/frac_lut4_arith $@

echo -e "Testing Verilog generation for LUTs: native fracturable LUT4 ";
run-task fpga_verilog/lut_design/frac_native_lut4 $@

echo -e "Testing Verilog generation for LUTs: simple fracturable LUT4 using AND gate to switch modes";
run-task fpga_verilog/lut_design/frac_lut4_and_switch $@

echo -e "Testing Verilog generation for LUTs: simple fracturable LUT6 ";
run-task fpga_verilog/lut_design/frac_lut6 $@

echo -e "Testing Verilog generation for LUTs: LUT6 with intermediate buffers";
run-task fpga_verilog/lut_design/intermediate_buffer $@

echo -e "Testing Verilog generation with VPR's untileable routing architecture ";
run-task fpga_verilog/untileable $@

echo -e "Testing Verilog generation with hard adder chain in CLBs ";
run-task fpga_verilog/adder/hard_adder $@

echo -e "Testing Verilog generation with soft adder chain in CLBs ";
run-task fpga_verilog/adder/soft_adder $@

echo -e "Testing Verilog generation with 1k block RAMs ";
run-task fpga_verilog/bram/dpram1k $@

echo -e "Testing Verilog generation with 1k block RAMs spanning two columns ";
run-task fpga_verilog/bram/wide_dpram1k $@

echo -e "Testing Verilog generation with heterogeneous fabric using 8-bit single-mode multipliers ";
run-task fpga_verilog/dsp/single_mode_mult_8x8 $@

echo -e "Testing Verilog generation with heterogeneous fabric using 16-bit multi-mode multipliers ";
run-task fpga_verilog/dsp/multi_mode_mult_16x16 $@

echo -e "Testing Verilog generation with heterogeneous fabric using 16-bit multi-mode multipliers with optional input registers ";
run-task fpga_verilog/dsp/multi_mode_regin_mult_16x16 $@

echo -e "Testing Verilog generation with heterogeneous fabric using multi-width 16-bit multi-mode multipliers ";
run-task fpga_verilog/dsp/wide_multi_mode_mult_16x16 $@

echo -e "Testing Verilog generation with heterogeneous fabric using 8-bit single-mode registerable multipliers ";
run-task fpga_verilog/dsp/single_mode_mult_8x8_reg $@

echo -e "Testing Verilog generation with heterogeneous fabric using 8-bit multiplier + dual port ram 1k ";
run-task fpga_verilog/dsp/mult_8x8_chain_dpram16k $@
run-task fpga_verilog/dsp/mult_8x8_chain_dpram16k_negz $@
run-task fpga_verilog/dsp/dual_mult_8x8_chain_dpram16k_negz $@
run-task fpga_verilog/dsp/mult_8x8_chain_dpram16k_supertile $@
run-task fpga_verilog/dsp/mult_8x8_chain_dpram16k_supertile_negz $@

echo -e "Testing Verilog generation with different I/O capacities on each side of an FPGA ";
run-task fpga_verilog/io/multi_io_capacity $@

echo -e "Testing Verilog generation with I/Os only on left and right sides of an FPGA ";
run-task fpga_verilog/io/reduced_io $@

echo -e "Testing Verilog generation with embedded I/Os for an FPGA ";
run-task fpga_verilog/io/embedded_io $@

echo -e "Testing Verilog generation with SoC I/Os for an FPGA ";
run-task fpga_verilog/io/soc_io $@

echo -e "Testing Verilog generation with registerable I/Os for an FPGA ";
run-task fpga_verilog/io/registerable_io $@

echo -e "Testing Verilog generation with adder chain across an FPGA";
run-task fpga_verilog/fabric_chain/adder_chain $@

echo -e "Testing Verilog generation with shift register chain across an FPGA";
run-task fpga_verilog/fabric_chain/register_chain $@

echo -e "Testing Verilog generation with scan chain across an FPGA";
run-task fpga_verilog/fabric_chain/scan_chain $@

echo -e "Testing Verilog generation with routing multiplexers implemented by tree structure";
run-task fpga_verilog/mux_design/tree_structure $@

echo -e "Testing Verilog generation with routing multiplexers implemented by standard cell MUX2";
run-task fpga_verilog/mux_design/stdcell_mux2 $@
run-task fpga_verilog/mux_design/stdcell_mux2_last_stage $@
run-task fpga_verilog/mux_design/stdcell_mux2_last_stage_size2 $@

echo -e "Testing Verilog generation with routing multiplexers implemented by local encoders";
run-task fpga_verilog/mux_design/local_encoder $@

echo -e "Testing Verilog generation with routing multiplexers without buffers";
run-task fpga_verilog/mux_design/debuf_mux $@

echo -e "Testing Verilog generation with routing multiplexers with input buffers only";
run-task fpga_verilog/mux_design/inbuf_only_mux $@

echo -e "Testing Verilog generation with routing multiplexers with output buffers only";
run-task fpga_verilog/mux_design/outbuf_only_mux $@

echo -e "Testing Verilog generation with routing multiplexers with constant gnd input";
run-task fpga_verilog/mux_design/const_input_gnd $@

echo -e "Testing Verilog generation with routing multiplexers without constant inputs";
run-task fpga_verilog/mux_design/no_const_input $@

echo -e "Testing Verilog generation with behavioral description";
run-task fpga_verilog/verilog_netlist_formats/behavioral_verilog $@
run-task fpga_verilog/verilog_netlist_formats/behavioral_verilog_little_endian $@
run-task fpga_verilog/verilog_netlist_formats/behavioral_verilog_default_nettype_wire $@

echo -e "Testing synthesizable Verilog generation with external standard cells";
run-task fpga_verilog/verilog_netlist_formats/synthesizable_verilog $@
run-task fpga_verilog/verilog_netlist_formats/synthesizable_verilog_little_endian $@

echo -e "Testing implicit Verilog generation";
run-task fpga_verilog/verilog_netlist_formats/implicit_verilog $@
run-task fpga_verilog/verilog_netlist_formats/implicit_verilog_default_nettype_wire $@

echo -e "Testing explicit Verilog generation";
run-task fpga_verilog/verilog_netlist_formats/explicit_port_mapping_default_nettype_wire $@
run-task fpga_verilog/verilog_netlist_formats/explicit_port_mapping_default_nettype_wire_little_endian $@

echo -e "Testing undriven net wiring in Verilog generation";
run-task fpga_verilog/verilog_netlist_formats/undriven_input_none $@
run-task fpga_verilog/verilog_netlist_formats/undriven_input_bus0 $@
run-task fpga_verilog/verilog_netlist_formats/undriven_input_bus1 $@
run-task fpga_verilog/verilog_netlist_formats/undriven_input_bit0 $@
run-task fpga_verilog/verilog_netlist_formats/undriven_input_bit1 $@

echo -e "Testing Verilog generation with flatten routing modules";
run-task fpga_verilog/flatten_routing $@

echo -e "Testing Verilog generation with duplicated grid output pins";
run-task fpga_verilog/duplicated_grid_pin $@

echo -e "Testing Verilog generation with spy output pads";
run-task fpga_verilog/spypad $@

echo -e "Testing Power-gating designs";
run-task fpga_verilog/power_gated_design/power_gated_inverter --show_thread_logs --debug

echo -e "Testing Depopulated crossbar in local routing";
run-task fpga_verilog/depopulate_crossbar $@

echo -e "Testing Fully connected output crossbar in local routing";
run-task fpga_verilog/fully_connected_output_crossbar $@

echo -e "Testing through channels in tileable routing";
run-task fpga_verilog/thru_channel/thru_narrow_tile $@
run-task fpga_verilog/thru_channel/thru_wide_tile $@

echo -e "Testing wire concatation in tileable routing";
run-task fpga_verilog/rr_concat_wire $@

echo -e "Testing the generation of preconfigured fabric wrapper for different HDL simulators";
run-task fpga_verilog/verilog_netlist_formats/embed_bitstream_none $@
run-task fpga_verilog/verilog_netlist_formats/embed_bitstream_modelsim $@

echo -e "Testing the netlist generation by forcing the use of relative paths";
run-task fpga_verilog/verilog_netlist_formats/use_relative_path $@
run-task fpga_verilog/verilog_netlist_formats/preconfig_testbench_use_relative_path $@
