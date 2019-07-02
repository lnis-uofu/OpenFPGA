/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"
#include "vpr_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"

#include "read_opt_types.h"
#include "read_opt.h"
#include "shell_types.h"

#include "spice_api.h"

boolean shell_setup_fpga_spice(t_shell_env* env, t_opt_info* opts) {
  /* Setup the PowerOpts */  
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.do_spice = TRUE;
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.spice_dir = get_opt_val(opts, "output_dir");

  /* TODO: this could be more flexible*/
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.include_dir = "include/";
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.subckt_dir = "subckt/";


  if (NULL == env->arch.spice) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P Information has not been initialized in architecture!\nPlease redo read_arch by enabling read_xml_fpga_x2p option!\n");
    return FALSE;
  }

  /* Configure FPGA X2P options*/
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_testbench_load_extraction = is_opt_set(opts, "testbench_load_extraction", TRUE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_parasitic_net_estimation = is_opt_set(opts, "parasitic_net_estimation", TRUE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_leakage_only = is_opt_set(opts, "leakage_only", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_top_testbench = is_opt_set(opts, "print_top_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_pb_mux_testbench = is_opt_set(opts, "print_pb_mux_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_cb_mux_testbench = is_opt_set(opts, "print_cb_mux_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_sb_mux_testbench = is_opt_set(opts, "print_sb_mux_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_cb_testbench = is_opt_set(opts, "print_cb_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_sb_testbench = is_opt_set(opts, "print_sb_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_lut_testbench = is_opt_set(opts, "print_lut_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_hardlogic_testbench = is_opt_set(opts, "print_hardlogic_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_io_testbench = is_opt_set(opts, "print_io_testbench", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_grid_testbench = is_opt_set(opts, "print_grid_testbench", FALSE);

  /* Set default options */
  if ((TRUE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.do_spice)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_top_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_grid_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_pb_mux_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_cb_mux_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_sb_mux_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_cb_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_sb_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_lut_testbench)
    &&(FALSE == env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_hardlogic_testbench)) {
    env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_pb_mux_testbench = TRUE;
    env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_cb_mux_testbench = TRUE;
    env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_sb_mux_testbench = TRUE;
    env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_lut_testbench = TRUE;
    env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_print_hardlogic_testbench = TRUE;
  }

  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_sim_multi_thread_num = get_opt_int_val(opts, "sim_multi_thread_num", 8);
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.simulator_path = get_opt_val(opts, "simulator_path");

  return TRUE;
}

void shell_execute_fpga_spice(t_shell_env* env, t_opt_info* opts) {
  if (FALSE == shell_setup_fpga_spice(env, opts)) {
    return;
  }
  
  vpr_fpga_spice(env->vpr_setup, env->arch,
                 env->vpr_setup.FileNameOpts.CircuitName);

  return;
}
