#ifndef OPENFPGA_BUILD_FABRIC_TEMPLATE_H
#define OPENFPGA_BUILD_FABRIC_TEMPLATE_H
/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
#include "build_device_module.h"
#include "build_fabric_global_port_info.h"
#include "build_fabric_io_location_map.h"
#include "build_fpga_core_wrapper_module.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "device_rr_gsb.h"
#include "device_rr_gsb_utils.h"
#include "fabric_hierarchy_writer.h"
#include "fabric_key_writer.h"
#include "globals.h"
#include "openfpga_naming.h"
#include "read_unique_blocks_bin.h"
#include "read_unique_blocks_xml.h"
#include "read_xml_fabric_key.h"
#include "read_xml_io_name_map.h"
#include "read_xml_module_name_map.h"
#include "read_xml_tile_config.h"
#include "rename_modules.h"
#include "report_reference.h"
#include "vtr_log.h"
#include "vtr_time.h"
#include "write_unique_blocks_bin.h"
#include "write_unique_blocks_xml.h"
#include "write_xml_fabric_pin_physical_location.h"
#include "write_xml_module_name_map.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Identify the unique GSBs from the Device RR GSB arrays
 * This function should only be called after the GSB builder is done
 *******************************************************************/
template <class T>
void compress_routing_hierarchy_template(T& openfpga_ctx,
                                         const bool& verbose_output) {
  vtr::ScopedStartFinishTimer timer(
    "Identify unique General Switch Blocks (GSBs)");

  /* Build unique module lists */
  openfpga_ctx.mutable_device_rr_gsb().build_unique_module(
    g_vpr_ctx.device().rr_graph);

  /* Report the stats */
  VTR_LOGV(
    verbose_output,
    "Detected %lu unique X-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANX),
    find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANX),
    100. *
      ((float)find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(),
                                                CHANX) /
         (float)openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANX) -
       1.));

  VTR_LOGV(
    verbose_output,
    "Detected %lu unique Y-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANY),
    find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANY),
    100. *
      ((float)find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(),
                                                CHANY) /
         (float)openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANY) -
       1.));

  VTR_LOGV(
    verbose_output,
    "Detected %lu unique switch blocks from a total of %d (compression "
    "rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_sb_unique_module(),
    find_device_rr_gsb_num_sb_modules(openfpga_ctx.device_rr_gsb(),
                                      g_vpr_ctx.device().rr_graph),
    100. * ((float)find_device_rr_gsb_num_sb_modules(
              openfpga_ctx.device_rr_gsb(), g_vpr_ctx.device().rr_graph) /
              (float)openfpga_ctx.device_rr_gsb().get_num_sb_unique_module() -
            1.));

  VTR_LOG(
    "Detected %lu unique general switch blocks from a total of %d (compression "
    "rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_gsb_unique_module(),
    find_device_rr_gsb_num_gsb_modules(openfpga_ctx.device_rr_gsb(),
                                       g_vpr_ctx.device().rr_graph),
    100. * ((float)find_device_rr_gsb_num_gsb_modules(
              openfpga_ctx.device_rr_gsb(), g_vpr_ctx.device().rr_graph) /
              (float)openfpga_ctx.device_rr_gsb().get_num_gsb_unique_module() -
            1.));
}

/********************************************************************
 * Build the module graph for FPGA device
 *******************************************************************/
template <class T>
int build_fabric_template(T& openfpga_ctx, const Command& cmd,
                          const CommandContext& cmd_context) {
  CommandOptionId opt_frame_view = cmd.option("frame_view");
  CommandOptionId opt_compress_routing = cmd.option("compress_routing");
  CommandOptionId opt_duplicate_grid_pin = cmd.option("duplicate_grid_pin");
  CommandOptionId opt_gen_random_fabric_key =
    cmd.option("generate_random_fabric_key");
  CommandOptionId opt_write_fabric_key = cmd.option("write_fabric_key");
  CommandOptionId opt_load_fabric_key = cmd.option("load_fabric_key");
  CommandOptionId opt_group_tile = cmd.option("group_tile");
  CommandOptionId opt_group_config_block = cmd.option("group_config_block");
  CommandOptionId opt_name_module_using_index =
    cmd.option("name_module_using_index");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Report conflicts with options:
   * - group tile does not support duplicate_grid_pin
   * - group tile requires compress_routing to be enabled
   */
  if (cmd_context.option_enable(cmd, opt_group_tile)) {
    if (cmd_context.option_enable(cmd, opt_duplicate_grid_pin)) {
      VTR_LOG_ERROR(
        "Option '%s' requires options '%s' to be disabled due to a conflict!\n",
        cmd.option_name(opt_group_tile).c_str(),
        cmd.option_name(opt_duplicate_grid_pin).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  /* Conflicts: duplicate_grid_pin does not support any port merge */
  if (cmd_context.option_enable(cmd, opt_duplicate_grid_pin)) {
    if (openfpga_ctx.arch().tile_annotations.tiles_to_merge_ports().size() >
        0) {
      VTR_LOG_ERROR(
        "Option '%s' requires no tile ports to be merged due to a conflict!\n",
        cmd.option_name(opt_duplicate_grid_pin).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  if (true == cmd_context.option_enable(cmd, opt_compress_routing) &&
      false == openfpga_ctx.device_rr_gsb().is_compressed()) {
    compress_routing_hierarchy_template<T>(
      openfpga_ctx, cmd_context.option_enable(cmd, opt_verbose));
    /* Update flow manager to enable compress routing */
    openfpga_ctx.mutable_flow_manager().set_compress_routing(true);
  } else if (true == openfpga_ctx.device_rr_gsb().is_compressed()) {
    openfpga_ctx.mutable_flow_manager().set_compress_routing(true);
  }

  if (cmd_context.option_enable(cmd, opt_group_tile)) {
    if (!openfpga_ctx.device_rr_gsb().is_compressed()) {
      VTR_LOG_ERROR(
        "Option '%s' requires unique blocks to be valid due to a conflict!\n",
        cmd.option_name(opt_group_tile).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
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

  /* Build tile-level information:
   * - This feature only supports when compress routing is enabled
   * - Read the tile organization configuration file
   * - Build tile info
   */
  TileConfig tile_config;
  if (cmd_context.option_enable(cmd, opt_group_tile)) {
    if (!openfpga_ctx.device_rr_gsb().is_compressed()) {
      VTR_LOG_ERROR(
        "Group tile is applicable only when compress routing is enabled!\n");
      return CMD_EXEC_FATAL_ERROR;
    }
    curr_status = read_xml_tile_config(
      cmd_context.option_value(cmd, opt_group_tile).c_str(), tile_config);
    if (CMD_EXEC_SUCCESS != curr_status) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  curr_status = build_device_module_graph(
    openfpga_ctx.mutable_module_graph(), openfpga_ctx.mutable_decoder_lib(),
    openfpga_ctx.mutable_blwl_shift_register_banks(),
    openfpga_ctx.mutable_fabric_tile(), openfpga_ctx.mutable_module_name_map(),
    const_cast<const T&>(openfpga_ctx), g_vpr_ctx.device(),
    cmd_context.option_enable(cmd, opt_frame_view),
    openfpga_ctx.device_rr_gsb().is_compressed(),
    cmd_context.option_enable(cmd, opt_duplicate_grid_pin),
    predefined_fabric_key, tile_config,
    cmd_context.option_enable(cmd, opt_group_config_block),
    cmd_context.option_enable(cmd, opt_name_module_using_index),
    cmd_context.option_enable(cmd, opt_gen_random_fabric_key),
    cmd_context.option_enable(cmd, opt_verbose));

  /* If there is any error, final status cannot be overwritten by a success flag
   */
  if (CMD_EXEC_SUCCESS != curr_status) {
    final_status = curr_status;
  }

  /* Build I/O location map */
  openfpga_ctx.mutable_io_location_map() = build_fabric_io_location_map(
    openfpga_ctx.module_graph(), g_vpr_ctx.device().grid,
    cmd_context.option_enable(cmd, opt_group_tile));

  /* Build fabric global port information */
  openfpga_ctx.mutable_fabric_global_port_info() =
    build_fabric_global_port_info(
      openfpga_ctx.module_graph(), openfpga_ctx.arch().config_protocol,
      openfpga_ctx.arch().tile_annotations, openfpga_ctx.arch().circuit_lib);

  /* Output fabric key if user requested */
  if (true == cmd_context.option_enable(cmd, opt_write_fabric_key)) {
    std::string fkey_fname =
      cmd_context.option_value(cmd, opt_write_fabric_key);
    VTR_ASSERT(false == fkey_fname.empty());
    curr_status = write_fabric_key_to_xml_file(
      openfpga_ctx.module_graph(), fkey_fname,
      openfpga_ctx.arch().config_protocol,
      openfpga_ctx.blwl_shift_register_banks(), false,
      cmd_context.option_enable(cmd, opt_verbose));
    /* If there is any error, final status cannot be overwritten by a success
     * flag */
    if (CMD_EXEC_SUCCESS != curr_status) {
      final_status = curr_status;
    }
  }

  return final_status;
}

/********************************************************************
 * Write fabric key of the module graph for FPGA device to a file
 *******************************************************************/
template <class T>
int write_fabric_key_template(const T& openfpga_ctx, const Command& cmd,
                              const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_include_module_keys = cmd.option("include_module_keys");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  /* Write fabric key to a file */
  return write_fabric_key_to_xml_file(
    openfpga_ctx.module_graph(), cmd_context.option_value(cmd, opt_file),
    openfpga_ctx.arch().config_protocol,
    openfpga_ctx.blwl_shift_register_banks(),
    cmd_context.option_enable(cmd, opt_include_module_keys),
    cmd_context.option_enable(cmd, opt_verbose));
}

/********************************************************************
 * Write hierarchy of the module graph for FPGA device to a file
 *******************************************************************/
template <class T>
int write_fabric_hierarchy_template(const T& openfpga_ctx, const Command& cmd,
                                    const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_exclude_empty_modules =
    cmd.option("exclude_empty_modules");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  CommandOptionId opt_module = cmd.option("module");
  std::string root_module =
    openfpga_ctx.module_name_map().name(generate_fpga_top_module_name());
  if (true == cmd_context.option_enable(cmd, opt_module)) {
    root_module = cmd_context.option_value(cmd, opt_module);
  }

  CommandOptionId opt_filter = cmd.option("filter");
  std::string filter("*");
  if (true == cmd_context.option_enable(cmd, opt_filter)) {
    filter = cmd_context.option_value(cmd, opt_filter);
  }

  /* Default depth requirement, will not stop until the leaf */
  int depth = -1;
  CommandOptionId opt_depth = cmd.option("depth");
  if (true == cmd_context.option_enable(cmd, opt_depth)) {
    depth = std::atoi(cmd_context.option_value(cmd, opt_depth).c_str());
    /* Error out if we have negative depth */
    if (0 > depth) {
      VTR_LOG_ERROR(
        "Invalid depth '%d' which should be 0 or a positive number!\n", depth);
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  std::string hie_file_name = cmd_context.option_value(cmd, opt_file);

  /* Write hierarchy to a file */
  return write_fabric_hierarchy_to_text_file(
    openfpga_ctx.module_graph(), openfpga_ctx.module_name_map(), hie_file_name,
    root_module, filter, size_t(depth),
    cmd_context.option_enable(cmd, opt_exclude_empty_modules),
    cmd_context.option_enable(cmd, opt_verbose));
}

/********************************************************************
 * Write the I/O information of module graph to a file
 *******************************************************************/
template <class T>
int write_fabric_io_info_template(const T& openfpga_ctx, const Command& cmd,
                                  const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string file_name = cmd_context.option_value(cmd, opt_file);

  /* Write hierarchy to a file */
  return openfpga_ctx.io_location_map().write_to_xml_file(
    file_name, !cmd_context.option_enable(cmd, opt_no_time_stamp),
    cmd_context.option_enable(cmd, opt_verbose));
}

/********************************************************************
 * Add fpga_core module to the module graph
 *******************************************************************/
template <class T>
int add_fpga_core_to_fabric_template(T& openfpga_ctx, const Command& cmd,
                                     const CommandContext& cmd_context) {
  CommandOptionId opt_frame_view = cmd.option("frame_view");
  bool frame_view = cmd_context.option_enable(cmd, opt_frame_view);
  CommandOptionId opt_verbose = cmd.option("verbose");
  bool verbose_output = cmd_context.option_enable(cmd, opt_verbose);

  CommandOptionId opt_inst_name = cmd.option("instance_name");
  std::string core_inst_name = generate_fpga_core_instance_name();
  if (true == cmd_context.option_enable(cmd, opt_inst_name)) {
    core_inst_name = cmd_context.option_value(cmd, opt_inst_name);
  }

  /* Handle I/O naming rules if defined */
  CommandOptionId opt_io_naming = cmd.option("io_naming");
  if (true == cmd_context.option_enable(cmd, opt_io_naming)) {
    read_xml_io_name_map(cmd_context.option_value(cmd, opt_io_naming).c_str(),
                         openfpga_ctx.mutable_io_name_map());
  }

  return add_fpga_core_to_device_module_graph(
    openfpga_ctx.mutable_module_graph(), openfpga_ctx.mutable_module_name_map(),
    openfpga_ctx.io_name_map(), core_inst_name, frame_view, verbose_output);
}

/********************************************************************
 * Rename modules in module graph with a set of given rules
 *******************************************************************/
template <class T>
int rename_modules_template(T& openfpga_ctx, const Command& cmd,
                            const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string file_name = cmd_context.option_value(cmd, opt_file);

  int status = CMD_EXEC_SUCCESS;
  ModuleNameMap user_module_name_map;
  status = read_xml_module_name_map(file_name.c_str(), user_module_name_map);
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Apply renaming on the user version */
  status = partial_rename_fabric_modules(
    openfpga_ctx.mutable_module_graph(), user_module_name_map,
    cmd_context.option_enable(cmd, opt_verbose));
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Update the internal version of module name map based on users' version */
  return update_module_name_map_with_user_version(
    openfpga_ctx.mutable_module_name_map(), user_module_name_map,
    cmd_context.option_enable(cmd, opt_verbose));
}

/********************************************************************
 * Write module naming rules to a file
 *******************************************************************/
template <class T>
int write_module_naming_rules_template(const T& openfpga_ctx,
                                       const Command& cmd,
                                       const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string file_name = cmd_context.option_value(cmd, opt_file);

  /* Write hierarchy to a file */
  return write_xml_module_name_map(
    file_name.c_str(), openfpga_ctx.module_name_map(),
    !cmd_context.option_enable(cmd, opt_no_time_stamp),
    cmd_context.option_enable(cmd, opt_verbose));
}

/********************************************************************
 * Write fabric pin physical location to a file
 *******************************************************************/
template <class T>
int write_fabric_pin_physical_location_template(
  const T& openfpga_ctx, const Command& cmd,
  const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_show_invalid_side = cmd.option("show_invalid_side");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string file_name = cmd_context.option_value(cmd, opt_file);

  std::string module_name("*"); /* Use a wildcard for everything */
  CommandOptionId opt_module = cmd.option("module");
  if (true == cmd_context.option_enable(cmd, opt_module)) {
    module_name = cmd_context.option_value(cmd, opt_module);
  }

  /* Write hierarchy to a file */
  return write_xml_fabric_pin_physical_location(
    file_name.c_str(), module_name, openfpga_ctx.module_graph(),
    cmd_context.option_enable(cmd, opt_show_invalid_side),
    !cmd_context.option_enable(cmd, opt_no_time_stamp),
    cmd_context.option_enable(cmd, opt_verbose));
}

template <class T>
int read_unique_blocks_template(T& openfpga_ctx, const Command& cmd,
                                const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_file = cmd.option("file");
  CommandOptionId opt_type = cmd.option("type");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string file_name = cmd_context.option_value(cmd, opt_file);
  std::string file_type = cmd_context.option_value(cmd, opt_type);
  /* read unique blocks from a file */
  if (file_type == "xml") {
    return read_xml_unique_blocks(openfpga_ctx.mutable_device_rr_gsb(),
                                  file_name.c_str(),
                                  cmd_context.option_enable(cmd, opt_verbose));
  } else if (file_type == "bin") {
    return read_bin_unique_blocks(openfpga_ctx.mutable_device_rr_gsb(),
                                  file_name.c_str(),
                                  cmd_context.option_enable(cmd, opt_verbose));
  } else {
    VTR_LOG_ERROR("file type %s not supported", file_type.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
}

template <class T>
int write_unique_blocks_template(T& openfpga_ctx, const Command& cmd,
                                 const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_file = cmd.option("file");
  CommandOptionId opt_type = cmd.option("type");

  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string file_name = cmd_context.option_value(cmd, opt_file);
  std::string file_type = cmd_context.option_value(cmd, opt_type);
  /* Write unique blocks to a file */
  /* add check flag */
  if (file_type == "xml") {
    return write_xml_unique_blocks(openfpga_ctx.device_rr_gsb(),
                                   file_name.c_str(),
                                   cmd_context.option_enable(cmd, opt_verbose));
  } else if (file_type == "bin") {
    return write_bin_unique_blocks(openfpga_ctx.mutable_device_rr_gsb(),
                                   file_name.c_str(),
                                   cmd_context.option_enable(cmd, opt_verbose));
  } else {
    VTR_LOG_ERROR("file type %s not supported", file_type.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
}

/********************************************************************
 *  Report reference to a file
 *******************************************************************/
template <class T>
int report_reference_template(const T& openfpga_ctx, const Command& cmd,
                              const CommandContext& cmd_context) {
  CommandOptionId opt_verbose = cmd.option("verbose");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");

  CommandOptionId opt_file = cmd.option("file");

  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());
  std::string file_name = cmd_context.option_value(cmd, opt_file);

  std::string module_name("*"); /* Use a wildcard for everything */
  CommandOptionId opt_module = cmd.option("module");
  if (true == cmd_context.option_enable(cmd, opt_module)) {
    module_name = cmd_context.option_value(cmd, opt_module);
  }
  /* Write hierarchy to a file */
  return report_reference(file_name.c_str(), module_name,
                          openfpga_ctx.module_graph(),
                          !cmd_context.option_enable(cmd, opt_no_time_stamp),
                          cmd_context.option_enable(cmd, opt_verbose));
}

} /* end namespace openfpga */

#endif
