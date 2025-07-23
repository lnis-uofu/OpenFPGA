/******************************************************************************
 * This files includes most utilized functions
 * for data structures for module management.
 ******************************************************************************/

#include <algorithm>
#include <cmath>
#include <map>

/* Headers from vtrutil library */
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "build_decoder_modules.h"
#include "circuit_library_utils.h"
#include "decoder_library_utils.h"
#include "memory_utils.h"
#include "module_manager_memory_utils.h"
#include "module_manager_utils.h"
#include "openfpga_naming.h"
#include "openfpga_port.h"
#include "openfpga_reserved_words.h"
#include "pb_type_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Compare the configurable children list with a given list of fabric sub-keys
 * Return true if exact naming-matches are found
 * When searching for matching, we consider
 * - alias is treated as No. 1 reference
 * - the <name, value> pair as No. 2 reference
 *******************************************************************/
static bool submodule_memory_modules_match_fabric_key(
  ModuleManager& module_manager, const ModuleId& module_id,
  const FabricKey& fabric_key, const FabricKeyModuleId& key_module_id) {
  /* If the length does not match, conclusion is easy to be made */
  size_t len_module_memory =
    module_manager
      .configurable_children(module_id,
                             ModuleManager::e_config_child_type::PHYSICAL)
      .size();
  size_t len_fabric_sub_key = fabric_key.sub_keys(key_module_id).size();
  if (len_module_memory != len_fabric_sub_key) {
    return false;
  }
  /* Now walk through the child one by one */
  for (size_t ikey = 0; ikey < len_module_memory; ++ikey) {
    FabricSubKeyId key_id = fabric_key.sub_keys(key_module_id)[ikey];
    std::pair<ModuleId, size_t> inst_info(ModuleId::INVALID(), 0);
    /* Try to match the alias */
    if (!fabric_key.sub_key_alias(key_id).empty()) {
      if (!fabric_key.sub_key_name(key_id).empty()) {
        inst_info.first =
          module_manager.find_module(fabric_key.sub_key_name(key_id));
        inst_info.second = module_manager.instance_id(
          module_id, inst_info.first, fabric_key.sub_key_alias(key_id));
      } else {
        inst_info = find_module_manager_instance_module_info(
          module_manager, module_id, fabric_key.sub_key_alias(key_id));
      }
    } else {
      inst_info.first =
        module_manager.find_module(fabric_key.sub_key_name(key_id));
      inst_info.second = fabric_key.sub_key_value(key_id);
    }
    if (inst_info.first !=
          module_manager.configurable_children(
            module_id, ModuleManager::e_config_child_type::PHYSICAL)[ikey] ||
        inst_info.second !=
          module_manager.configurable_child_instances(
            module_id, ModuleManager::e_config_child_type::PHYSICAL)[ikey]) {
      return false;
    }
  }
  return true;
}

/********************************************************************
 * Update the configurable children list based on fabric key definitions
 *******************************************************************/
static bool update_submodule_memory_modules_from_fabric_key(
  ModuleManager& module_manager, const ModuleId& module_id,
  const CircuitLibrary& circuit_lib, const ConfigProtocol& config_protocol,
  const ModuleManager::e_config_child_type& config_child_type,
  const FabricKey& fabric_key, const FabricKeyModuleId& key_module_id) {
  /* Reset the configurable children */
  module_manager.clear_configurable_children(module_id);

  for (FabricSubKeyId key_id : fabric_key.sub_keys(key_module_id)) {
    std::pair<ModuleId, size_t> inst_info(ModuleId::INVALID(), 0);
    /* Try to match the alias */
    if (!fabric_key.sub_key_alias(key_id).empty()) {
      if (!fabric_key.sub_key_name(key_id).empty()) {
        inst_info.first =
          module_manager.find_module(fabric_key.sub_key_name(key_id));
        inst_info.second = module_manager.instance_id(
          module_id, inst_info.first, fabric_key.sub_key_alias(key_id));
      } else {
        inst_info = find_module_manager_instance_module_info(
          module_manager, module_id, fabric_key.sub_key_alias(key_id));
      }
    } else {
      inst_info.first =
        module_manager.find_module(fabric_key.sub_key_name(key_id));
      inst_info.second = fabric_key.sub_key_value(key_id);
    }
    if (false == module_manager.valid_module_id(inst_info.first)) {
      if (!fabric_key.sub_key_alias(key_id).empty()) {
        VTR_LOG_ERROR("Invalid key alias '%s'!\n",
                      fabric_key.sub_key_alias(key_id).c_str());
      } else {
        VTR_LOG_ERROR("Invalid key name '%s'!\n",
                      fabric_key.sub_key_name(key_id).c_str());
      }
      return CMD_EXEC_FATAL_ERROR;
    }

    if (false == module_manager.valid_module_instance_id(
                   module_id, inst_info.first, inst_info.second)) {
      if (!fabric_key.sub_key_alias(key_id).empty()) {
        VTR_LOG_ERROR("Invalid key alias '%s'!\n",
                      fabric_key.sub_key_alias(key_id).c_str());
      } else {
        VTR_LOG_ERROR("Invalid key value '%ld'!\n", inst_info.second);
      }
      return CMD_EXEC_FATAL_ERROR;
    }

    /* If the the child has not configuration bits, error out */
    if (0 == find_module_num_config_bits(
               module_manager, inst_info.first, circuit_lib,
               config_protocol.memory_model(), config_protocol.type())) {
      if (!fabric_key.sub_key_alias(key_id).empty()) {
        VTR_LOG_ERROR(
          "Invalid key alias '%s' which has zero configuration bits!\n",
          fabric_key.sub_key_alias(key_id).c_str());
      } else {
        VTR_LOG_ERROR(
          "Invalid key name '%s' which has zero configuration bits!\n",
          fabric_key.sub_key_name(key_id).c_str());
      }
      return CMD_EXEC_FATAL_ERROR;
    }

    /* Now we can add the child to configurable children of the top module */
    module_manager.add_configurable_child(module_id, inst_info.first,
                                          inst_info.second, config_child_type,
                                          vtr::Point<int>(-1, -1));
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Remove the nets around the configuration chain (ccff_head and ccff_tails)
 *******************************************************************/
static int remove_submodule_nets_cmos_memory_chain_config_bus(
  ModuleManager& module_manager, const ModuleId& parent_module,
  const e_config_protocol_type& sram_orgz_type,
  const ModuleManager::e_config_child_type& config_child_type) {
  for (size_t mem_index = 0;
       mem_index <
       module_manager.configurable_children(parent_module, config_child_type)
         .size();
       ++mem_index) {
    ModuleId net_src_module_id;
    size_t net_src_instance_id;
    ModulePortId net_src_port_id;

    if (0 == mem_index) {
      /* Find the port name of configuration chain head */
      std::string src_port_name =
        generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_INPUT);
      net_src_module_id = parent_module;
      net_src_instance_id = 0;
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);
    } else {
      /* Find the port name of previous memory module */
      std::string src_port_name = generate_configuration_chain_tail_name();
      net_src_module_id = module_manager.configurable_children(
        parent_module, config_child_type)[mem_index - 1];
      net_src_instance_id = module_manager.configurable_child_instances(
        parent_module, config_child_type)[mem_index - 1];
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);
    }

    /* Get the pin id for source port */
    BasicPort net_src_port =
      module_manager.module_port(net_src_module_id, net_src_port_id);

    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
      /* Find the net from which the source node is driving */
      ModuleNetId net = module_manager.module_instance_port_net(
        parent_module, net_src_module_id, net_src_instance_id, net_src_port_id,
        net_src_port.pins()[pin_id]);
      /* Remove the net including sources and sinks */
      module_manager.clear_module_net_sinks(parent_module, net);
    }
  }

  /* For the last memory module:
   *    net source is the configuration chain tail of the previous memory module
   *    net sink is the configuration chain tail of the primitive module
   */
  /* Find the port name of previous memory module */
  std::string src_port_name = generate_configuration_chain_tail_name();
  ModuleId net_src_module_id =
    module_manager.configurable_children(parent_module, config_child_type)
      .back();
  size_t net_src_instance_id =
    module_manager
      .configurable_child_instances(parent_module, config_child_type)
      .back();
  ModulePortId net_src_port_id =
    module_manager.find_module_port(net_src_module_id, src_port_name);

  /* Get the pin id for source port */
  BasicPort net_src_port =
    module_manager.module_port(net_src_module_id, net_src_port_id);

  /* Create a net for each pin */
  for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
    /* Find the net from which the source node is driving */
    ModuleNetId net = module_manager.module_instance_port_net(
      parent_module, net_src_module_id, net_src_instance_id, net_src_port_id,
      net_src_port.pins()[pin_id]);
    /* Remove the net including sources and sinks */
    module_manager.clear_module_net_sinks(parent_module, net);
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Remove the nets around the configurable children for a given module which
 *should be in CMOS type
 *******************************************************************/
static int remove_submodule_nets_cmos_memory_config_bus(
  ModuleManager& module_manager, const ModuleId& module_id,
  const e_config_protocol_type& sram_orgz_type,
  const ModuleManager::e_config_child_type& config_child_type) {
  switch (sram_orgz_type) {
    case CONFIG_MEM_SCAN_CHAIN: {
      return remove_submodule_nets_cmos_memory_chain_config_bus(
        module_manager, module_id, sram_orgz_type, config_child_type);
      break;
    }
    case CONFIG_MEM_STANDALONE:
    case CONFIG_MEM_QL_MEMORY_BANK:
      /* TODO:
      add_module_nets_cmos_memory_bank_bl_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
      add_module_nets_cmos_memory_bank_wl_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
      add_module_nets_cmos_memory_bank_wl_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_WLR);
       */
      break;
    case CONFIG_MEM_MEMORY_BANK:
      /* TODO:
      add_module_nets_cmos_flatten_memory_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
      add_module_nets_cmos_flatten_memory_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
       */
      break;
    case CONFIG_MEM_FRAME_BASED:
      /* TODO:
      add_module_nets_cmos_memory_frame_config_bus(module_manager, decoder_lib,
                                                   parent_module);
       */
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid type of SRAM organization!\n");
      return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_FATAL_ERROR;
}

/********************************************************************
 * Remove the nets around the configurable children for a given module
 *******************************************************************/
static int remove_submodule_configurable_children_nets(
  ModuleManager& module_manager, const ModuleId& module_id,
  const CircuitLibrary& circuit_lib, const ConfigProtocol& config_protocol,
  const ModuleManager::e_config_child_type& config_child_type) {
  switch (circuit_lib.design_tech_type(config_protocol.memory_model())) {
    case CIRCUIT_MODEL_DESIGN_CMOS:
      return remove_submodule_nets_cmos_memory_config_bus(
        module_manager, module_id, config_protocol.type(), config_child_type);
      break;
    case CIRCUIT_MODEL_DESIGN_RRAM:
      /* TODO: */
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid type of memory design technology!\n");
      return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_FATAL_ERROR;
}

/********************************************************************
 * Rebuild the nets(only sinks) around the configuration chain (ccff_head and
 *ccff_tails)
 *******************************************************************/
static int rebuild_submodule_nets_cmos_memory_chain_config_bus(
  ModuleManager& module_manager, const ModuleId& parent_module,
  const e_config_protocol_type& sram_orgz_type,
  const ModuleManager::e_config_child_type& config_child_type) {
  for (size_t mem_index = 0;
       mem_index <
       module_manager.configurable_children(parent_module, config_child_type)
         .size();
       ++mem_index) {
    ModuleId net_src_module_id;
    size_t net_src_instance_id;
    ModulePortId net_src_port_id;

    ModuleId net_sink_module_id;
    size_t net_sink_instance_id;
    ModulePortId net_sink_port_id;

    if (0 == mem_index) {
      /* Find the port name of configuration chain head */
      std::string src_port_name =
        generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_INPUT);
      net_src_module_id = parent_module;
      net_src_instance_id = 0;
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);

      /* Find the port name of next memory module */
      std::string sink_port_name = generate_configuration_chain_head_name();
      net_sink_module_id = module_manager.configurable_children(
        parent_module, config_child_type)[mem_index];
      net_sink_instance_id = module_manager.configurable_child_instances(
        parent_module, config_child_type)[mem_index];
      net_sink_port_id =
        module_manager.find_module_port(net_sink_module_id, sink_port_name);
    } else {
      /* Find the port name of previous memory module */
      std::string src_port_name = generate_configuration_chain_tail_name();
      net_src_module_id = module_manager.configurable_children(
        parent_module, config_child_type)[mem_index - 1];
      net_src_instance_id = module_manager.configurable_child_instances(
        parent_module, config_child_type)[mem_index - 1];
      net_src_port_id =
        module_manager.find_module_port(net_src_module_id, src_port_name);

      /* Find the port name of next memory module */
      std::string sink_port_name = generate_configuration_chain_head_name();
      net_sink_module_id = module_manager.configurable_children(
        parent_module, config_child_type)[mem_index];
      net_sink_instance_id = module_manager.configurable_child_instances(
        parent_module, config_child_type)[mem_index];
      net_sink_port_id =
        module_manager.find_module_port(net_sink_module_id, sink_port_name);
    }

    /* Get the pin id for source port */
    BasicPort net_src_port =
      module_manager.module_port(net_src_module_id, net_src_port_id);
    /* Get the pin id for sink port */
    BasicPort net_sink_port =
      module_manager.module_port(net_sink_module_id, net_sink_port_id);
    /* Port sizes of source and sink should match */
    VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());

    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(
        module_manager, parent_module, net_src_module_id, net_src_instance_id,
        net_src_port_id, net_src_port.pins()[pin_id]);
      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id,
                                         net_sink_instance_id, net_sink_port_id,
                                         net_sink_port.pins()[pin_id]);
    }
  }

  /* For the last memory module:
   *    net source is the configuration chain tail of the previous memory module
   *    net sink is the configuration chain tail of the primitive module
   */
  /* Find the port name of previous memory module */
  std::string src_port_name = generate_configuration_chain_tail_name();
  ModuleId net_src_module_id =
    module_manager.configurable_children(parent_module, config_child_type)
      .back();
  size_t net_src_instance_id =
    module_manager
      .configurable_child_instances(parent_module, config_child_type)
      .back();
  ModulePortId net_src_port_id =
    module_manager.find_module_port(net_src_module_id, src_port_name);

  /* Find the port name of next memory module */
  std::string sink_port_name =
    generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_OUTPUT);
  ModuleId net_sink_module_id = parent_module;
  size_t net_sink_instance_id = 0;
  ModulePortId net_sink_port_id =
    module_manager.find_module_port(net_sink_module_id, sink_port_name);

  /* Get the pin id for source port */
  BasicPort net_src_port =
    module_manager.module_port(net_src_module_id, net_src_port_id);
  /* Get the pin id for sink port */
  BasicPort net_sink_port =
    module_manager.module_port(net_sink_module_id, net_sink_port_id);
  /* Port sizes of source and sink should match */
  VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());

  /* Create a net for each pin */
  for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
    /* Create a net and add source and sink to it */
    ModuleNetId net = create_module_source_pin_net(
      module_manager, parent_module, net_src_module_id, net_src_instance_id,
      net_src_port_id, net_src_port.pins()[pin_id]);
    /* Add net sink */
    module_manager.add_module_net_sink(parent_module, net, net_sink_module_id,
                                       net_sink_instance_id, net_sink_port_id,
                                       net_sink_port.pins()[pin_id]);
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Rebuild the nets around the configurable children for a given module which
 *should be in CMOS type
 *******************************************************************/
static int rebuild_submodule_nets_cmos_memory_config_bus(
  ModuleManager& module_manager, const ModuleId& module_id,
  const e_config_protocol_type& sram_orgz_type,
  const ModuleManager::e_config_child_type& config_child_type) {
  switch (sram_orgz_type) {
    case CONFIG_MEM_SCAN_CHAIN: {
      return rebuild_submodule_nets_cmos_memory_chain_config_bus(
        module_manager, module_id, sram_orgz_type, config_child_type);
      break;
    }
    case CONFIG_MEM_STANDALONE:
    case CONFIG_MEM_QL_MEMORY_BANK:
      /* TODO:
      add_module_nets_cmos_memory_bank_bl_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
      add_module_nets_cmos_memory_bank_wl_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
      add_module_nets_cmos_memory_bank_wl_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_WLR);
       */
      break;
    case CONFIG_MEM_MEMORY_BANK:
      /* TODO:
      add_module_nets_cmos_flatten_memory_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_BL);
      add_module_nets_cmos_flatten_memory_config_bus(
        module_manager, parent_module, sram_orgz_type, CIRCUIT_MODEL_PORT_WL);
       */
      break;
    case CONFIG_MEM_FRAME_BASED:
      /* TODO:
      add_module_nets_cmos_memory_frame_config_bus(module_manager, decoder_lib,
                                                   parent_module);
       */
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid type of SRAM organization!\n");
      return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_FATAL_ERROR;
}

/********************************************************************
 * Rebuild the nets(only sinks) around the configurable children for a given
 *module
 *******************************************************************/
static int rebuild_submodule_configurable_children_nets(
  ModuleManager& module_manager, const ModuleId& module_id,
  const CircuitLibrary& circuit_lib, const ConfigProtocol& config_protocol,
  const ModuleManager::e_config_child_type& config_child_type) {
  switch (circuit_lib.design_tech_type(config_protocol.memory_model())) {
    case CIRCUIT_MODEL_DESIGN_CMOS:
      return rebuild_submodule_nets_cmos_memory_config_bus(
        module_manager, module_id, config_protocol.type(), config_child_type);
      break;
    case CIRCUIT_MODEL_DESIGN_RRAM:
      /* TODO: */
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid type of memory design technology!\n");
      return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_FATAL_ERROR;
}

/********************************************************************
 * Load and update the configurable children of a given module (not a top-level
 *module) Compare the configurable children list with fabric sub-keys.
 * - If match, nothing should be done
 * - If not match,
 *   - remove the nets related to configurable children
 *   - rebuild the configurable children list
 *   - add the nets related to configurable children
 *******************************************************************/
static int load_and_update_submodule_memory_modules_from_fabric_key(
  ModuleManager& module_manager, const ModuleId& module_id,
  const CircuitLibrary& circuit_lib, const ConfigProtocol& config_protocol,
  const FabricKey& fabric_key, const FabricKeyModuleId& key_module_id,
  const bool& group_config_block) {
  int status = CMD_EXEC_SUCCESS;
  /* Compare the configurable children list */
  if (submodule_memory_modules_match_fabric_key(module_manager, module_id,
                                                fabric_key, key_module_id)) {
    return CMD_EXEC_SUCCESS;
  }
  /* Do not match, now remove all the nets for the configurable children */
  status = remove_submodule_configurable_children_nets(
    module_manager, module_id, circuit_lib, config_protocol,
    ModuleManager::e_config_child_type::PHYSICAL);
  if (status == CMD_EXEC_FATAL_ERROR) {
    return status;
  }
  /* Overwrite the configurable children list */
  status = update_submodule_memory_modules_from_fabric_key(
    module_manager, module_id, circuit_lib, config_protocol,
    group_config_block ? ModuleManager::e_config_child_type::PHYSICAL
                       : ModuleManager::e_config_child_type::UNIFIED,
    fabric_key, key_module_id);
  if (status == CMD_EXEC_FATAL_ERROR) {
    return status;
  }
  /* TODO: Create the nets for the new list of configurable children */
  status = rebuild_submodule_configurable_children_nets(
    module_manager, module_id, circuit_lib, config_protocol,
    ModuleManager::e_config_child_type::PHYSICAL);
  if (status == CMD_EXEC_FATAL_ERROR) {
    return status;
  }
  return status;
}

/********************************************************************
 * Load and update the configurable children of a given list of modules (not a
 *top-level module)
 *******************************************************************/
int load_submodules_memory_modules_from_fabric_key(
  ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const ConfigProtocol& config_protocol, const FabricKey& fabric_key,
  const bool& group_config_block) {
  int status = CMD_EXEC_SUCCESS;
  for (FabricKeyModuleId key_module_id : fabric_key.modules()) {
    std::string module_name = fabric_key.module_name(key_module_id);
    /* Ensure this is not a top module! */
    if (module_name == std::string(FPGA_TOP_MODULE_NAME)) {
      VTR_LOG_ERROR(
        "Expect a non-top-level name for the sub-module '%s' in fabric key!\n",
        module_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    ModuleId module_id = module_manager.find_module(module_name);
    if (module_id) {
      /* This is a valid module, try to load and update */
      status = load_and_update_submodule_memory_modules_from_fabric_key(
        module_manager, module_id, circuit_lib, config_protocol, fabric_key,
        key_module_id, group_config_block);
      if (status == CMD_EXEC_FATAL_ERROR) {
        return status;
      }
    } else {
      /* Not a valid module, report error */
      VTR_LOG_ERROR(
        "The sub-module '%s' in fabric key is not a valid module in FPGA "
        "fabric!\n",
        module_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  return status;
}

} /* end namespace openfpga */
