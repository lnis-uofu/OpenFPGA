/********************************************************************
 * This file includes the main function to build module graphs
 * for the FPGA fabric
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgashell library */
#include "build_fpga_core_wrapper_module.h"
#include "command_exit_codes.h"
#include "openfpga_naming.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Add ports to top module based on I/O naming rules:
 * - Add ports which has been defined in the naming rules
 * - Add ports from the core module, which does not appear in the naming rules
 *******************************************************************/
static int create_fpga_top_module_ports_using_naming_rules(
  ModuleManager& module_manager, const ModuleId& wrapper_module,
  const ModuleId& core_module, const IoNameMap& io_naming,
  const bool& verbose) {
  for (BasicPort top_port : io_naming.fpga_top_ports()) {
    /* For dummy port, just add it. Port type should be defined from io naming
     * rules */
    if (io_naming.fpga_top_port_is_dummy(top_port)) {
      ModuleManager::e_module_port_type port_type =
        ModuleManager::e_module_port_type::MODULE_INOUT_PORT;
      if (IoNameMap::e_dummy_port_direction::INPUT ==
          io_naming.fpga_top_dummy_port_direction(top_port)) {
        port_type = ModuleManager::e_module_port_type::MODULE_INPUT_PORT;
      } else if (IoNameMap::e_dummy_port_direction::OUTPUT ==
                 io_naming.fpga_top_dummy_port_direction(top_port)) {
        port_type = ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT;
      } else if (IoNameMap::e_dummy_port_direction::INOUT ==
                 io_naming.fpga_top_dummy_port_direction(top_port)) {
        port_type = ModuleManager::e_module_port_type::MODULE_INOUT_PORT;
      } else {
        VTR_LOG_ERROR(
          "fpga_top dummy port '%s' has an invalid direction. Expect "
          "[input|output|inout]!\n",
          top_port.to_verilog_string().c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      module_manager.add_port(wrapper_module, top_port, port_type);
      VTR_LOGV(verbose,
               "Add dummy port '%s' to fpga_top by following naming rules\n",
               top_port.to_verilog_string().c_str());
      continue; /* Finish for this port addition */
    }
    /* Get the port type which should be same as the fpga_core port */
    BasicPort core_port = io_naming.fpga_core_port(top_port);
    if (!core_port.is_valid()) {
      VTR_LOG_ERROR("fpga_top port '%s' is not mapped to any fpga_core port!\n",
                    top_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    ModulePortId core_port_id =
      module_manager.find_module_port(core_module, core_port.get_name());
    if (!module_manager.valid_module_port_id(core_module, core_port_id)) {
      VTR_LOG_ERROR(
        "fpga_top port '%s' is mapped to an invalid fpga_core port '%s'!\n",
        top_port.to_verilog_string().c_str(),
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    ModuleManager::e_module_port_type top_port_type =
      module_manager.port_type(core_module, core_port_id);
    module_manager.add_port(wrapper_module, top_port, top_port_type);
    VTR_LOGV(verbose,
             "Add port '%s' to fpga_top (correspond to '%s' of fpga_core) by "
             "following naming rules\n",
             top_port.to_verilog_string().c_str(),
             core_port.to_verilog_string().c_str());
  }
  /* Now walk through the ports of fpga_core, if port which is not mapped to
   * fpga_top should be added */
  for (ModulePortId core_port_id : module_manager.module_ports(core_module)) {
    BasicPort core_port = module_manager.module_port(core_module, core_port_id);
    BasicPort top_port = io_naming.fpga_top_port(core_port);
    if (top_port.is_valid()) {
      continue; /* Port has been added in the previous loop, skip now */
    }
    /* Throw fatal error if part of the core port is mapped while other part is
     * not mapped. This is not allowed! */
    IoNameMap::e_port_mapping_status mapping_status =
      io_naming.fpga_core_port_mapping_status(core_port, true);
    if (mapping_status == IoNameMap::e_port_mapping_status::FULL) {
      continue;
    }
    if (mapping_status == IoNameMap::e_port_mapping_status::PARTIAL) {
      VTR_LOG_ERROR(
        "fpga_core port '%s' is partially mapped to fpga_top, which is not "
        "allowed. Please cover the full-sized port in naming rules!\n",
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (mapping_status == IoNameMap::e_port_mapping_status::OVERLAPPED) {
      VTR_LOG_ERROR(
        "fpga_core port '%s' is overlapped mapped to fpga_top, which is not "
        "allowed. Please cover the full-sized port in naming rules!\n",
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    VTR_ASSERT(mapping_status == IoNameMap::e_port_mapping_status::NONE);

    /* Add the port now */
    ModuleManager::e_module_port_type top_port_type =
      module_manager.port_type(core_module, core_port_id);
    module_manager.add_port(wrapper_module, core_port, top_port_type);
    VTR_LOG(
      "Add port '%s' to fpga_top in the same name as the port of "
      "fpga_core, since naming rules do not specify\n",
      core_port.to_verilog_string().c_str());
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Add nets between top module and core module based on I/O naming rules:
 * - Dummy ports do not need any nets
 * - For ports which are defined in the naming rules, create dedicated
 *connections
 * - For ports of the core module, which does not appear in the naming rules,
 *create direct connections
 *******************************************************************/
static int create_fpga_top_module_nets_using_naming_rules(
  ModuleManager& module_manager, const ModuleId& wrapper_module,
  const ModuleId& core_module, const IoNameMap& io_naming,
  const bool& verbose) {
  for (BasicPort top_port : io_naming.fpga_top_ports()) {
    if (io_naming.fpga_top_port_is_dummy(top_port)) {
      VTR_LOGV("Skip nets for dummy port '%s' at top module\n",
               top_port.to_verilog_string().c_str());
      continue;
    }
    /* Collect port-level information */
    ModulePortId top_port_id =
      module_manager.find_module_port(wrapper_module, top_port.get_name());
    if (!module_manager.valid_module_port_id(wrapper_module, top_port_id)) {
      VTR_LOG_ERROR("fpga_top port '%s' is not found at top module!\n",
                    top_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    BasicPort core_port = io_naming.fpga_core_port(top_port);
    if (!core_port.is_valid()) {
      VTR_LOG_ERROR("fpga_top port '%s' is not mapped to any fpga_core port!\n",
                    top_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    ModulePortId core_port_id =
      module_manager.find_module_port(core_module, core_port.get_name());
    if (!module_manager.valid_module_port_id(core_module, core_port_id)) {
      VTR_LOG_ERROR(
        "fpga_top port '%s' is mapped to an invalid fpga_core port '%s'!\n",
        top_port.to_verilog_string().c_str(),
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    ModuleManager::e_module_port_type top_port_type =
      module_manager.port_type(core_module, core_port_id);

    /* Create net for each pin-to-pin connection */
    if (top_port.get_width() != core_port.get_width()) {
      VTR_LOG_ERROR(
        "fpga_top port '%s' does not match the width of fpga_core port '%s'!\n",
        top_port.to_verilog_string().c_str(),
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    for (size_t ipin = 0; ipin < top_port.pins().size(); ++ipin) {
      ModuleNetId new_net = module_manager.create_module_net(wrapper_module);
      if (top_port_type !=
          ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT) {
        module_manager.add_module_net_source(wrapper_module, new_net,
                                             wrapper_module, 0, top_port_id,
                                             top_port.pins()[ipin]);
        module_manager.add_module_net_sink(wrapper_module, new_net, core_module,
                                           0, core_port_id,
                                           core_port.pins()[ipin]);
      } else {
        VTR_ASSERT_SAFE(top_port_type ==
                        ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT);
        module_manager.add_module_net_source(wrapper_module, new_net,
                                             core_module, 0, core_port_id,
                                             core_port.pins()[ipin]);
        module_manager.add_module_net_sink(wrapper_module, new_net,
                                           wrapper_module, 0, top_port_id,
                                           top_port.pins()[ipin]);
      }
      BasicPort top_pin(top_port.get_name(), top_port.pins()[ipin],
                        top_port.pins()[ipin]);
      BasicPort core_pin(core_port.get_name(), core_port.pins()[ipin],
                         core_port.pins()[ipin]);
      VTR_LOGV(
        verbose,
        "Add nets to connect between fpga_top '%s' and fpga_core '%s' by "
        "following naming rules\n",
        top_pin.to_verilog_string().c_str(),
        core_pin.to_verilog_string().c_str());
    }
  }
  /* Now walk through the ports of fpga_core, if port which is not mapped to
   * fpga_top, nets should be added */
  for (ModulePortId core_port_id : module_manager.module_ports(core_module)) {
    BasicPort core_port = module_manager.module_port(core_module, core_port_id);
    BasicPort top_port = io_naming.fpga_top_port(core_port);
    if (top_port.is_valid()) {
      continue; /* Port has been added in the previous loop, skip now */
    }
    /* Throw fatal error if part of the core port is mapped while other part is
     * not mapped. This is not allowed! */
    IoNameMap::e_port_mapping_status mapping_status =
      io_naming.fpga_core_port_mapping_status(core_port);
    if (mapping_status == IoNameMap::e_port_mapping_status::FULL) {
      continue;
    }
    if (mapping_status == IoNameMap::e_port_mapping_status::PARTIAL) {
      VTR_LOG_ERROR(
        "fpga_core port '%s' is partially mapped to fpga_top, which is not "
        "allowed. Please cover the full-sized port in naming rules!\n",
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (mapping_status == IoNameMap::e_port_mapping_status::OVERLAPPED) {
      VTR_LOG_ERROR(
        "fpga_core port '%s' is overlapped mapped to fpga_top, which is not "
        "allowed. Please cover the full-sized port in naming rules!\n",
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    VTR_ASSERT(mapping_status == IoNameMap::e_port_mapping_status::NONE);
    ModuleManager::e_module_port_type top_port_type =
      module_manager.port_type(core_module, core_port_id);
    /* Collect port-level information */
    ModulePortId top_port_id =
      module_manager.find_module_port(wrapper_module, core_port.get_name());
    top_port = module_manager.module_port(
      wrapper_module, top_port_id); /* Note: overwrite the top port */
    if (!module_manager.valid_module_port_id(wrapper_module, top_port_id)) {
      VTR_LOG_ERROR("fpga_top port '%s' is not found at top module!\n",
                    top_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    /* Create net for each pin-to-pin connection */
    if (top_port.get_width() != core_port.get_width()) {
      VTR_LOG_ERROR(
        "fpga_top port '%s' does not match the width of fpga_core port '%s'!\n",
        top_port.to_verilog_string().c_str(),
        core_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    for (size_t ipin = 0; ipin < core_port.pins().size(); ++ipin) {
      ModuleNetId new_net = module_manager.create_module_net(wrapper_module);
      if (top_port_type !=
          ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT) {
        module_manager.add_module_net_source(wrapper_module, new_net,
                                             wrapper_module, 0, top_port_id,
                                             top_port.pins()[ipin]);
        module_manager.add_module_net_sink(wrapper_module, new_net, core_module,
                                           0, core_port_id,
                                           core_port.pins()[ipin]);
      } else {
        VTR_ASSERT_SAFE(top_port_type ==
                        ModuleManager::e_module_port_type::MODULE_OUTPUT_PORT);
        module_manager.add_module_net_source(wrapper_module, new_net,
                                             core_module, 0, core_port_id,
                                             core_port.pins()[ipin]);
        module_manager.add_module_net_sink(wrapper_module, new_net,
                                           wrapper_module, 0, top_port_id,
                                           top_port.pins()[ipin]);
      }
      BasicPort core_pin(top_port.get_name(), core_port.pins()[ipin],
                         core_port.pins()[ipin]);
      VTR_LOGV(
        verbose,
        "Add nets to connect fpga_top '%s' in the same name as the port of "
        "fpga_core, since naming rules do not specify\n",
        core_pin.to_verilog_string().c_str());
    }
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Create a custom fpga_top module by applying naming rules
 *******************************************************************/
static int create_fpga_top_module_using_naming_rules(
  ModuleManager& module_manager, ModuleId& wrapper_module,
  const ModuleId& core_module, const std::string& top_module_name,
  const IoNameMap& io_naming, const std::string& instance_name,
  const bool& add_nets, const bool& verbose) {
  /* Create a new module with the given name */
  wrapper_module = module_manager.add_module(top_module_name);
  if (!wrapper_module) {
    return CMD_EXEC_FATAL_ERROR;
  }
  /* Add the existing module as an instance */
  module_manager.add_child_module(wrapper_module, core_module, false);
  module_manager.set_child_instance_name(wrapper_module, core_module, 0,
                                         instance_name);

  /* Add ports */
  if (CMD_EXEC_SUCCESS !=
      create_fpga_top_module_ports_using_naming_rules(
        module_manager, wrapper_module, core_module, io_naming, verbose)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Add nets */
  if (add_nets) {
    if (CMD_EXEC_SUCCESS !=
        create_fpga_top_module_nets_using_naming_rules(
          module_manager, wrapper_module, core_module, io_naming, verbose)) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  /* TODO: Update the fabric global ports */

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * The main function to be called for adding the fpga_core wrapper to a FPGA
 *fabric
 * - Rename existing fpga_top to fpga_core
 * - Create a wrapper module 'fpga_top' on the fpga_core
 *******************************************************************/
int add_fpga_core_to_device_module_graph(ModuleManager& module_manager,
                                         ModuleNameMap& module_name_map,
                                         const IoNameMap& io_naming,
                                         const std::string& core_inst_name,
                                         const bool& frame_view,
                                         const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;

  /* Execute the module graph api */
  std::string top_module_name =
    module_name_map.name(generate_fpga_top_module_name());
  ModuleId top_module = module_manager.find_module(top_module_name);
  if (!module_manager.valid_module_id(top_module)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Rename existing top module to fpga_core */
  std::string core_module_name = generate_fpga_core_module_name();
  module_manager.set_module_name(top_module, core_module_name);
  VTR_LOGV(verbose, "Rename current top-level module '%s' to '%s'\n",
           top_module_name.c_str(), core_module_name.c_str());

  /* Create a wrapper module under the existing fpga_top
   * - if there are no io naming rules, just use the default API to create a
   * wrapper
   * - if there are io naming rules, use dedicated function to handle
   */
  ModuleId new_top_module;
  if (io_naming.empty()) {
    new_top_module = module_manager.create_wrapper_module(
      top_module, top_module_name, core_inst_name, !frame_view);
  } else {
    status = create_fpga_top_module_using_naming_rules(
      module_manager, new_top_module, top_module, top_module_name, io_naming,
      core_inst_name, !frame_view, verbose);
    if (CMD_EXEC_SUCCESS != status) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  if (!module_manager.valid_module_id(new_top_module)) {
    VTR_LOGV_ERROR(verbose,
                   "Failed to create a wrapper module '%s' on top of '%s'!\n",
                   top_module_name.c_str(), core_module_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  VTR_LOGV(verbose, "Created a wrapper module '%s' on top of '%s'\n",
           top_module_name.c_str(), core_module_name.c_str());

  /* Update module name map */
  status =
    module_name_map.set_tag_to_name_pair(core_module_name, core_module_name);
  if (CMD_EXEC_SUCCESS != status) {
    VTR_LOG_ERROR(
      "Failed to register fpga core module '%s' in module name map!\n",
      core_module_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOGV(verbose, "Updated module name map\n");

  /* Now fpga_core should be the only configurable child under the top-level
   * module */
  module_manager.add_configurable_child(
    new_top_module, top_module, 0, ModuleManager::e_config_child_type::UNIFIED);

  return status;
}

} /* end namespace openfpga */
