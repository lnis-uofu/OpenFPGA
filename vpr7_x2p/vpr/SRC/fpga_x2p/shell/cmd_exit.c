
/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"
#include "vpr_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"

#include "read_opt_types.h"
#include "shell_types.h"

void shell_execute_exit(t_shell_env* env, t_opt_info* opts) {

  char* vpr_shell_name = "VPR7-OpenFPGA";

  vpr_printf(TIO_MESSAGE_INFO, 
             "Thank you for using %s!\n",
             vpr_shell_name);
  exit(1);
}
