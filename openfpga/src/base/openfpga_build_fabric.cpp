/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from fabrickey library */
#include "read_xml_fabric_key.h"

#include "device_rr_gsb.h"
#include "device_rr_gsb_utils.h"
#include "build_device_module.h"
#include "fabric_hierarchy_writer.h"
#include "fabric_key_writer.h"
#include "build_fabric_io_location_map.h"
#include "build_fabric_global_port_info.h"
#include "openfpga_build_fabric.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Identify the unique GSBs from the Device RR GSB arrays
 * This function should only be called after the GSB builder is done
 *******************************************************************/
static 
void compress_routing_hierarchy(OpenfpgaContext& openfpga_ctx,
                                const bool& verbose_output) {
  vtr::ScopedStartFinishTimer timer("Identify unique General Switch Blocks (GSBs)");

  /* Build unique module lists */
  openfpga_ctx.mutable_device_rr_gsb().build_unique_module(g_vpr_ctx.device().rr_graph);

  /* Report the stats */
  VTR_LOGV(verbose_output, 
           "Detected %lu unique X-direction connection blocks from a total of %d (compression rate=%.2f%)\n",
           openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANX),
           find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANX),
           100. * ((float)find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANX) / (float)openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANX) - 1.));

  VTR_LOGV(verbose_output,
           "Detected %lu unique Y-direction connection blocks from a total of %d (compression rate=%.2f%)\n",
           openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANY),
           find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANY),
           100. * ((float)find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANY) / (float)openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANY) - 1.));

  VTR_LOGV(verbose_output,
           "Detected %lu unique switch blocks from a total of %d (compression rate=%.2f%)\n",
           openfpga_ctx.device_rr_gsb().get_num_sb_unique_module(),
           find_device_rr_gsb_num_sb_modules(openfpga_ctx.device_rr_gsb()),
           100. * ((float)find_device_rr_gsb_num_sb_modules(openfpga_ctx.device_rr_gsb()) / (float)openfpga_ctx.device_rr_gsb().get_num_sb_unique_module() - 1.));

  VTR_LOG("Detected %lu unique general switch blocks from a total of %d (compression rate=%.2f%)\n",
          openfpga_ctx.device_rr_gsb().get_num_gsb_unique_module(),
          find_device_rr_gsb_num_gsb_modules(openfpga_ctx.device_rr_gsb()),
          100. * ((float)find_device_rr_gsb_num_gsb_modules(openfpga_ctx.device_rr_gsb()) / (float)openfpga_ctx.device_rr_gsb().get_num_gsb_unique_module() - 1.));
}

/********************************************************************
 * Build the module graph for FPGA device
 *******************************************************************/
int build_fabric(OpenfpgaContext& openfpga_ctx,
                 const Command& cmd, const CommandContext& cmd_context) { 

  CommandOptionId opt_frame_view = cmd.option("frame_view");
  CommandOptionId opt_compress_routing = cmd.option("compress_routing");
  CommandOptionId opt_duplicate_grid_pin = cmd.option("duplicate_grid_pin");
  CommandOptionId opt_gen_random_fabric_key = cmd.option("generate_random_fabric_key");
  CommandOptionId opt_write_fabric_key = cmd.option("write_fabric_key");
  CommandOptionId opt_load_fabric_key = cmd.option("load_fabric_key");
  CommandOptionId opt_verbose = cmd.option("verbose");
  
  if (true == cmd_context.option_enable(cmd, opt_compress_routing)) {
    compress_routing_hierarchy(openfpga_ctx, cmd_context.option_enable(cmd, opt_verbose));
    /* Update flow manager to enable compress routing */
    openfpga_ctx.mutable_flow_manager().set_compress_routing(true);
  }

  VTR_LOG("\n");

  /* Record the execution status in curr_status for each command 
   * and summarize them in the final status
   */
  int curr_status = CMD_EXEC_SUCCESS;
  int final_status = CMD_EXEC_SUCCESS;

  /* Load fabric key from file */
  FabricKey predefined_fabric_key;
  if (true == cmd_context.option_enable(cmd, opt_load_fabric_key)) {
    std::string fkey_fname = cmd_context.option_value(cmd, opt_load_fabric_key);
    VTR_ASSERT(false == fkey_fname.empty());
    predefined_fabric_key = read_xml_fabric_key(fkey_fname.c_str());
  }

  VTR_LOG("\n");

  curr_status = build_device_module_graph(openfpga_ctx.mutable_module_graph(),
                                          openfpga_ctx.mutable_decoder_lib(),
                                          openfpga_ctx.mutable_blwl_shift_register_banks(),
                                          const_cast<const OpenfpgaContext&>(openfpga_ctx),
                                          g_vpr_ctx.device(),
                                          cmd_context.option_enable(cmd, opt_frame_view),
                                          cmd_context.option_enable(cmd, opt_compress_routing),
                                          cmd_context.option_enable(cmd, opt_duplicate_grid_pin),
                                          predefined_fabric_key,
                                          cmd_context.option_enable(cmd, opt_gen_random_fabric_key),
                                          cmd_context.option_enable(cmd, opt_verbose));

  /* If there is any error, final status cannot be overwritten by a success flag */
  if (CMD_EXEC_SUCCESS != curr_status) {
    final_status = curr_status;
  }

  /* Build I/O location map */
  openfpga_ctx.mutable_io_location_map() = build_fabric_io_location_map(openfpga_ctx.module_graph(),
                                                                        g_vpr_ctx.device().grid);

  /* Build fabric global port information */
  openfpga_ctx.mutable_fabric_global_port_info() = build_fabric_global_port_info(openfpga_ctx.module_graph(),
                                                                                 openfpga_ctx.arch().tile_annotations,
                                                                                 openfpga_ctx.arch().circuit_lib);

  /* Output fabric key if user requested */
  if (true == cmd_context.option_enable(cmd, opt_write_fabric_key)) {
    std::string fkey_fname = cmd_context.option_value(cmd, opt_write_fabric_key);
    VTR_ASSERT(false == fkey_fname.empty());
    curr_status = write_fabric_key_to_xml_file(openfpga_ctx.module_graph(),
                                               fkey_fname,
                                               openfpga_ctx.arch().config_protocol,
                                               cmd_context.option_enable(cmd, opt_verbose));
    /* If there is any error, final status cannot be overwritten by a success flag */
    if (CMD_EXEC_SUCCESS != curr_status) {
      final_status = curr_status;
    }
  }

  return final_status;
} 

/********************************************************************
 * Build the module graph for FPGA device
 *******************************************************************/
int write_fabric_hierarchy(const OpenfpgaContext& openfpga_ctx,
                           const Command& cmd, const CommandContext& cmd_context) { 

  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Check the option '--file' is enabled or not 
   * Actually, it must be enabled as the shell interface will check 
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  /* Default depth requirement, will not stop until the leaf */
  int depth = -1;
  CommandOptionId opt_depth = cmd.option("depth");
  if (true == cmd_context.option_enable(cmd, opt_depth)) {
    depth = std::atoi(cmd_context.option_value(cmd, opt_depth).c_str());
    /* Error out if we have negative depth */
    if (0 > depth) {
      VTR_LOG_ERROR("Invalid depth '%d' which should be 0 or a positive number!\n",
                    depth);
      return CMD_EXEC_FATAL_ERROR; 
    }
  }

  std::string hie_file_name = cmd_context.option_value(cmd, opt_file);

  /* Write hierarchy to a file */
  return write_fabric_hierarchy_to_text_file(openfpga_ctx.module_graph(),
                                             hie_file_name,
                                             size_t(depth),
                                             cmd_context.option_enable(cmd, opt_verbose));
}

} /* end namespace openfpga */
