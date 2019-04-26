/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"
#include "vpr_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"

#include "read_opt_types.h"
#include "read_opt.h"
#include "shell_types.h"

#include "vpr_api.h"
#include "fpga_x2p_setup.h"

boolean shell_setup_fpga_x2p_setup(t_shell_env* env, t_opt_info* opts) {
  /* Setup the PowerOpts */  
  env->vpr_setup.FPGA_SPICE_Opts.do_fpga_spice = TRUE;

  if (NULL == env->arch.spice) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P Information has not been initialized in architecture!\nPlease redo read_arch by enabling read_xml_fpga_x2p option!\n");
    return FALSE;
  }

  /* Configure FPGA X2P options*/
  env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_parasitic_net_estimation = is_opt_set(opts, "parasitic_net_estimation", TRUE);
  env->vpr_setup.FPGA_SPICE_Opts.read_act_file = env->vpr_setup.FPGA_SPICE_Opts.SpiceOpts.fpga_spice_parasitic_net_estimation;
  

  env->vpr_setup.FPGA_SPICE_Opts.rename_illegal_port = is_opt_set(opts, "rename_illegal_port", FALSE);
  env->vpr_setup.FPGA_SPICE_Opts.signal_density_weight = get_opt_float_val(opts, "signal_density_weight", 1.);
  env->vpr_setup.FPGA_SPICE_Opts.sim_window_size = get_opt_float_val(opts, "sim_window_size", 0.5);

  return TRUE;
}

void shell_execute_fpga_x2p_setup(t_shell_env* env, t_opt_info* opts) {
  if (FALSE == shell_setup_fpga_x2p_setup(env, opts)) {
    return;
  }
  
  fpga_x2p_setup(env->vpr_setup, &(env->arch));

  return;
}
