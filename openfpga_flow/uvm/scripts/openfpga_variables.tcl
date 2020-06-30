global vars

#First we source the common variables script
source "./scripts/common_variables.tcl"
source "./scripts/modelsim_procs.tcl"



#Name of the testbench module
set vars(module_tb_name) openfpga_tb
#Name of the top module which is instantiated in the tb (should be fpga_top)
set vars(module_name) FPGA_DUT
#To modify:
set vars(projectname) $vars(module_name)


set vars(simtime) 20 ;#in ms
set vars(unit) ns

set vars(vlog_options) ""
#Yes if post PnR, no if RTL simulation
set vars(mapped) "no"

#Should be 10_5t_12nm or 9t_14nm
set vars(library_type) "9t_14nm"

#Verilog testbench
set vars(verilog_files_tb) 			"$vars(tb_path)/uvm_tb.sv"
set vars(verilog_files_rtl) 		"$vars(verilog_rtl_path)/fabric_netlists.v"
set vars(ref_model)			"$vars(tb_path)/and.sv" 
#set vars(verilog_files_mapped) 	"$vars(verilog_mapped_path)/xor2_mapped.v" 
#set vars(sdf_file) 						"$vars(sdf_path)/xor2_mapped.sdf"


#if {$vars(mapped) == "yes"} {
#  set vars(projectname_modelsim) $vars(projectname)_mapped

#	if {$vars(library_type) == "9t_14nm"} {
#		set vars(sc_gpio_verilog_files) "$vars(sclib_14nm_9t_verilog) $vars(gpio_14nm_verilog)"
#	} elseif {$vars(library_type) == "10_5t_12nm"} {
#		set vars(sc_gpio_verilog_files) "$vars(sclib_12nm_10_5t_verilog) $vars(gpio_12nm_verilog)"
#	}
 # set vars(verilog_files) "$vars(sc_gpio_verilog_files) $vars(verilog_files_mapped) $vars(verilog_files_tb)"
#  set vars(verilog_files_recompile) "$vars(verilog_files_mapped) $vars(verilog_files_tb)"

#	} else {
  set vars(projectname_modelsim) $vars(projectname)_rtl
	set vars(sdf_file) ""
	set vars(verilog_files) "$vars(ref_model) $vars(verilog_files_rtl) $vars(verilog_files_tb)"
#}
