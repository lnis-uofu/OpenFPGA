# OpenFPGA flow options

Usage -> **fpga_flow *-options <value>* **<br />
Mandatory options: <br />
- -conf <file> : *specify the basic configuration files for fpga_flow*
- -benchmark <file> : *the configuration file contains benchmark file names*
- -rpt <file> : *CSV file consists of data*
- -N <int> : *N-LUT/Matrix*
Other Options:<br />
## General
- -matlab_rpt <data_name> : *.m file consists of data compatible to matlab scripts. Specify the data name to be appeared in the script*
- -I <int> : *Number of inputs of a CLB, mandatory when mpack1 flow is chosen*
- -K <int> : *K-LUT, mandatory when standard flow is chosen*
- -M <int> : *M-Matrix, mandatory when mpack1 flow is chosen*
- -power : *run power estimation oriented flow*
- -black_box_ace: *run activity estimation with black box support. It increase the power.*
- -remove_designs: *remove all the old results.*
- -multi_thread <int>: *turn on the mutli-thread mode, specify the number of threads*
- -multi_task <int>: *turn on the mutli-task mode*
- -parse_results_only : *only parse the flow results and write CSV report.*
- -debug : *debug mode*
- -help : *print usage*
## ODIN II
- -min_hard_adder_size: *min. size of hard adder in carry chain defined in Arch XML.(Default:1)*
- -mem_size: *size of memory, mandatory when VTR/VTR_MCCL/VTR_MIG_MCCL flow is chosen*
- -odin2_carry_chain_support: *turn on the carry_chain support only valid for VTR_MCCL/VTR_MIG_MCCL flow *
## ABC
- -abc_scl : *run ABC optimization for sequential circuits, mandatory when VTR flow is selected.*
- -abc_verilog_rewrite : *run ABC to convert a blif netlist to a Verilog netlist.*
## ACE
- -ace_p <float> : *specify the default signal probablity of PIs in ACE2.*
- -ace_d <float> : *specify the default signal density of PIs in ACE2.*
## VPR - Original Version
- -vpr_timing_pack_off : *turn off the timing-driven pack for vpr.*
- -vpr_place_clb_pin_remap: *turn on place_clb_pin_remap in VPR.*
- -vpr_max_router_iteration <int> : *specify the max router iteration in VPR.*
- -vpr_route_breadthfirst : *use the breadth-first routing algorithm of VPR.*
- -vpr_use_tileable_route_chan_width: *turn on the conversion to tileable_route_chan_width in VPR.*
- -min_route_chan_width <float> : *turn on routing with <float>* min_route_chan_width.*
- -fix_route_chan_width : *turn on routing with a fixed route_chan_width, defined in benchmark configuration file.*
## VPR - FPGA-X2P Extension
- -vpr_fpga_x2p_rename_illegal_port : *turn on renaming illegal ports option of VPR FPGA SPICE*
- -vpr_fpga_x2p_signal_density_weight <float>: *specify the option signal_density_weight of VPR FPGA SPICE*
- -vpr_fpga_x2p_sim_window_size <float>: *specify the option sim_window_size of VPR FPGA SPICE*
- -vpr_fpga_x2p_compact_routing_hierarchy : *allow routing block modularization*
## VPR - FPGA-SPICE Extension
- -vpr_fpga_spice <task_file> : *turn on SPICE netlists print-out in VPR, specify a task file*
- -vpr_fpga_spice_sim_mt_num <int>: *specify the option sim_mt_num of VPR FPGA SPICE*
- -vpr_fpga_spice_print_component_tb : *print component-level testbenches in VPR FPGA SPICE*
- -vpr_fpga_spice_print_grid_tb : *print Grid-level testbenches in VPR FPGA SPICE*
- -vpr_fpga_spice_print_top_tb : *print full-chip testbench in VPR FPGA SPICE*
- -vpr_fpga_spice_leakage_only : *turn on leakage_only mode in VPR FPGA SPICE*
- -vpr_fpga_spice_parasitic_net_estimation_off : *turn off parasitic_net_estimation in VPR FPGA SPICE*
- -vpr_fpga_spice_testbench_load_extraction_off : *turn off testbench_load_extraction in VPR FPGA SPICE*
- -vpr_fpga_spice_simulator_path <string> : *Specify simulator path*
## VPR - FPGA-Verilog Extension
- -vpr_fpga_verilog : *turn on Verilog Generator of VPR FPGA SPICE*
- -vpr_fpga_verilog_dir <verilog_path>: *provides the path where generated verilog files will be written*
- -vpr_fpga_verilog_include_timing : *turn on printing delay specification in Verilog files*
- -vpr_fpga_verilog_include_signal_init : *turn on printing signal initialization in Verilog files*
- -vpr_fpga_verilog_print_autocheck_top_testbench: *turn on printing autochecked top-level testbench for Verilog Generator of VPR FPGA SPICE*
- -vpr_fpga_verilog_formal_verification_top_netlist : *turn on printing formal top Verilog files*
- -vpr_fpga_verilog_include_icarus_simulator : *Add syntax and definition required to use Icarus Verilog simulator*
- -vpr_fpga_verilog_print_user_defined_template : *Generates a template of hierarchy modules and their port mapping*
- -vpr_fpga_verilog_print_report_timing_tcl : *Generates tcl script useful for timing report generation*
- -vpr_fpga_verilog_report_timing_rpt_path <path_to_generate_reports> : *Specify path for report timing*
- -vpr_fpga_verilog_print_sdc_pnr : *Generates sdc file to constraint Hardware P&R*
- -vpr_fpga_verilog_print_sdc_analysis : *Generates sdc file to do STA*
- -vpr_fpga_verilog_print_top_tb : *turn on printing top-level testbench for Verilog Generator of VPR FPGA SPICE*
- -vpr_fpga_verilog_print_input_blif_tb : *turn on printing testbench for input blif file in Verilog Generator of VPR FPGA SPICE*
- -vpr_fpga_verilog_print_modelsim_autodeck <modelsim.ini_path>: *turn on printing modelsim simulation script*
## VPR - FPGA-Bitstream Extension
- -vpr_fpga_bitstream_generator: *turn on FPGA-SPICE bitstream generator*
