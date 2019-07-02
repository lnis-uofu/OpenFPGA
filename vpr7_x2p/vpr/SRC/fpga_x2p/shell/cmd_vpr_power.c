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

boolean shell_setup_vpr_versa_power(t_shell_env* env, t_opt_info* opts) {
  /* Setup the PowerOpts */  
  env->vpr_setup.PowerOpts.do_power = TRUE;

  if ((NULL == env->arch.power)
     || (NULL == env->arch.clocks)
     || (NULL == g_clock_arch)) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "Power Information has not been initialized in architecture!\nPlease redo read_arch by enabling versa_power option!\n");
    return FALSE;
  }

  return TRUE;
}

void shell_execute_vpr_versapower(t_shell_env* env, t_opt_info* opts) {
  if (FALSE == shell_setup_vpr_versa_power(env, opts)) {
    return;
  }

  vpr_power_estimation(env->vpr_setup, env->arch);
  return;
}
