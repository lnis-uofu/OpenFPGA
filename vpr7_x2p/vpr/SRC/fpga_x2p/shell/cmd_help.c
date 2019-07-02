
/* Include vpr structs*/
#include "util.h"
#include "arch_types.h"
#include "vpr_types.h"

/* SPICE Support Headers */
#include "read_xml_spice_util.h"

#include "read_opt_types.h"
#include "shell_types.h"
#include "shell_utils.h"

void shell_execute_help(t_shell_env* env, t_opt_info* opts) {
  
  shell_print_usage(env);

  return;
}
