
/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"
#include "vpr_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"

#include "read_opt_types.h"
#include "read_opt.h"
#include "shell_types.h"

#include "linkedlist.h"
#include "fpga_x2p_utils.h"
#include "verilog_api.h"

boolean shell_setup_fpga_verilog(t_shell_env* env, t_opt_info* opts) {
  /* Setup the PowerOpts */  
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_syn_verilog = TRUE;
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.syn_verilog_dump_dir = get_opt_val(opts, "output_dir");

  if (NULL == env->arch.spice) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P Information has not been initialized in architecture!\nPlease redo read_arch by enabling read_xml_fpga_x2p option!\n");
    return FALSE;
  }

  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_testbench = is_opt_set(opts, "print_top_testbench", TRUE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_autocheck_top_testbench = is_opt_set(opts, "print_autocheck_top_testbench", TRUE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.reference_verilog_benchmark_file = get_opt_val(opts, "reference_verilog_benchmark_file");
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_input_blif_testbench = is_opt_set(opts, "print_input_blif_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_formal_verification_top_netlist = is_opt_set(opts, "print_formal_verification_top_netlist", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.include_timing = is_opt_set(opts, "include_timing", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.include_signal_init = is_opt_set(opts, "include_signal_init", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.include_icarus_simulator = is_opt_set(opts, "include_icarus_simulator", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_modelsim_autodeck = is_opt_set(opts, "print_modelsim_autodeck", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.modelsim_ini_path = get_opt_val(opts, "modelsim_ini_path");
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_user_defined_template = is_opt_set(opts, "print_user_defined_template", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_report_timing_tcl = is_opt_set(opts, "print_report_timing_tcl", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.report_timing_path = get_opt_val(opts, "report_timing_dir_path");
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_sdc_pnr = is_opt_set(opts, "print_sdc_pnr", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_sdc_analysis = is_opt_set(opts, "print_sdc_analysis", FALSE);

  return TRUE;
}

void shell_execute_fpga_verilog(t_shell_env* env, t_opt_info* opts) {

  if (FALSE == shell_setup_fpga_verilog(env, opts)) {
    return;
  }
  
  vpr_fpga_verilog(env->module_manager, env->vpr_setup, env->arch,
                   env->vpr_setup.FileNameOpts.CircuitName);

  return;
}
