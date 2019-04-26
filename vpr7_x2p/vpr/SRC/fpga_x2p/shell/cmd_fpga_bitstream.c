
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
#include "fpga_bitstream.h"

boolean shell_setup_fpga_bitstream(t_shell_env* env, t_opt_info* opts) {
  /* Setup the PowerOpts */  
  env->vpr_setup.FPGA_SPICE_Opts.BitstreamGenOpts.gen_bitstream = TRUE;
  env->vpr_setup.FPGA_SPICE_Opts.BitstreamGenOpts.bitstream_output_file = get_opt_val(opts, "output_file");

  if (NULL == env->arch.spice) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P Information has not been initialized in architecture!\nPlease redo read_arch by enabling read_xml_fpga_x2p option!\n");
    return FALSE;
  }

  return TRUE;
}

void shell_execute_fpga_bitstream(t_shell_env* env, t_opt_info* opts) {
  t_sram_orgz_info* sram_bitstream_orgz_info = NULL;

  if (FALSE == shell_setup_fpga_bitstream(env, opts)) {
    return;
  }
  
  vpr_fpga_bitstream_generator(env->vpr_setup, env->arch,
                 env->vpr_setup.FileNameOpts.CircuitName,
                 &sram_bitstream_orgz_info);

  free_sram_orgz_info(sram_bitstream_orgz_info,
                      sram_bitstream_orgz_info->type);

  return;
}
