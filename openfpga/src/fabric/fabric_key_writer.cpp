/***************************************************************************************
 * Output fabric key of Module Graph to file formats
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "command_exit_codes.h"
#include "openfpga_digest.h"

/* Headers from archopenfpga library */
#include "fabric_key_writer.h"
#include "memory_utils.h"
#include "openfpga_naming.h"
#include "write_xml_fabric_key.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Add module-level keys to fabric key
 ***************************************************************************************/
static int add_module_keys_to_fabric_key(const ModuleManager& module_manager,
                                         const ModuleId& curr_module,
                                         FabricKey& fabric_key) {
  /* Bypass top-level module */
  std::string module_name = module_manager.module_name(curr_module);
  if (module_name == generate_fpga_top_module_name() ||
      module_name == generate_fpga_core_module_name()) {
    return CMD_EXEC_SUCCESS;
  }
  /* Bypass modules which does not have any configurable children */
  if (module_manager
        .configurable_children(curr_module,
                               ModuleManager::e_config_child_type::PHYSICAL)
        .empty()) {
    return CMD_EXEC_SUCCESS;
  }
  /* Now create the module and add subkey one by one */
  FabricKeyModuleId key_module_id = fabric_key.create_module(module_name);
  if (!key_module_id) {
    return CMD_EXEC_FATAL_ERROR;
  }
  size_t num_config_child =
    module_manager
      .configurable_children(curr_module,
                             ModuleManager::e_config_child_type::PHYSICAL)
      .size();
  for (size_t ichild = 0; ichild < num_config_child; ++ichild) {
    ModuleId child_module = module_manager.configurable_children(
      curr_module, ModuleManager::e_config_child_type::PHYSICAL)[ichild];
    size_t child_instance = module_manager.configurable_child_instances(
      curr_module, ModuleManager::e_config_child_type::PHYSICAL)[ichild];

    FabricSubKeyId sub_key = fabric_key.create_module_key(key_module_id);
    fabric_key.set_sub_key_name(sub_key,
                                module_manager.module_name(child_module));
    fabric_key.set_sub_key_value(sub_key, child_instance);

    if (false ==
        module_manager.instance_name(curr_module, child_module, child_instance)
          .empty()) {
      fabric_key.set_sub_key_alias(
        sub_key, module_manager.instance_name(curr_module, child_module,
                                              child_instance));
    }
  }
  return CMD_EXEC_SUCCESS;
}

/***************************************************************************************
 * Write the fabric key of top module to an XML file
 * We will use the writer API in libfabrickey
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 ***************************************************************************************/
int write_fabric_key_to_xml_file(
  const ModuleManager& module_manager, const std::string& fname,
  const ConfigProtocol& config_protocol,
  const MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const bool& include_module_keys, const bool& verbose) {
  int err_code = CMD_EXEC_SUCCESS;
  std::string timer_message =
    std::string("Write fabric key to XML file '") + fname + std::string("'");

  std::string dir_path = format_dir_path(find_path_dir_name(fname));

  /* Create directories */
  create_directory(dir_path);

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Use default name if user does not provide one */
  VTR_ASSERT(true != fname.empty());

  /* Find top-level module */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  std::string core_module_name = generate_fpga_core_module_name();
  ModuleId core_module = module_manager.find_module(core_module_name);
  if (!module_manager.valid_module_id(top_module) &&
      !module_manager.valid_module_id(core_module)) {
    VTR_LOGV_ERROR(
      verbose, "Unable to find the top-level/core-level module '%s' or '%s'!\n",
      top_module_name.c_str(), core_module_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (module_manager.valid_module_id(top_module) &&
      module_manager.valid_module_id(core_module)) {
    top_module = core_module;
  }

  /* Build a fabric key database by visiting all the configurable children */
  FabricKey fabric_key;
  size_t num_keys =
    module_manager
      .configurable_children(top_module,
                             ModuleManager::e_config_child_type::PHYSICAL)
      .size();

  fabric_key.reserve_keys(num_keys);

  size_t num_regions = module_manager.regions(top_module).size();
  fabric_key.reserve_regions(num_regions);

  /* Create regions and build a id map */
  std::map<ConfigRegionId, FabricRegionId> region_id_map;
  for (const ConfigRegionId& config_region :
       module_manager.regions(top_module)) {
    FabricRegionId fabric_region = fabric_key.create_region();
    region_id_map[config_region] = fabric_region;
  }

  /* Create regions for the keys and load keys by region */
  for (const ConfigRegionId& config_region :
       module_manager.regions(top_module)) {
    /* Must have a valid one-to-one region mapping  */
    auto result = region_id_map.find(config_region);
    VTR_ASSERT_SAFE(result != region_id_map.end());
    FabricRegionId fabric_region = result->second;

    /* Each configuration protocol has some child which should not be in the
     * list. They are typically decoders */
    size_t curr_region_num_config_child =
      module_manager.region_configurable_children(top_module, config_region)
        .size();
    size_t num_child_to_skip =
      estimate_num_configurable_children_to_skip_by_config_protocol(
        config_protocol, curr_region_num_config_child);
    curr_region_num_config_child -= num_child_to_skip;

    fabric_key.reserve_region_keys(fabric_region, curr_region_num_config_child);

    for (size_t ichild = 0; ichild < curr_region_num_config_child; ++ichild) {
      ModuleId child_module = module_manager.region_configurable_children(
        top_module, config_region)[ichild];
      size_t child_instance =
        module_manager.region_configurable_child_instances(
          top_module, config_region)[ichild];
      vtr::Point<int> child_coord =
        module_manager.region_configurable_child_coordinates(
          top_module, config_region)[ichild];

      FabricKeyId key = fabric_key.create_key();
      fabric_key.set_key_name(key, module_manager.module_name(child_module));
      fabric_key.set_key_value(key, child_instance);

      if (false ==
          module_manager.instance_name(top_module, child_module, child_instance)
            .empty()) {
        fabric_key.set_key_alias(
          key, module_manager.instance_name(top_module, child_module,
                                            child_instance));
      }

      /* Add key coordinate */
      fabric_key.set_key_coordinate(key, child_coord);

      /* Add keys to the region */
      fabric_key.add_key_to_region(fabric_region, key);
    }
  }

  /* Skip invalid region, some architecture may not have BL/WL banks */
  if (0 < blwl_sr_banks.regions().size()) {
    /* Add BL shift register bank information, if there is any */
    for (const ConfigRegionId& config_region :
         module_manager.regions(top_module)) {
      auto result = region_id_map.find(config_region);
      /* Must have a valid one-to-one region mapping  */
      VTR_ASSERT_SAFE(result != region_id_map.end());
      FabricRegionId fabric_region = result->second;
      for (const FabricBitLineBankId& bank :
           blwl_sr_banks.bl_banks(config_region)) {
        FabricBitLineBankId fabric_bank =
          fabric_key.create_bl_shift_register_bank(fabric_region);
        for (const BasicPort& data_port :
             blwl_sr_banks.bl_bank_data_ports(config_region, bank)) {
          fabric_key.add_data_port_to_bl_shift_register_bank(
            fabric_region, fabric_bank, data_port);
        }
      }
    }

    /* Add WL shift register bank information, if there is any */
    for (const ConfigRegionId& config_region :
         module_manager.regions(top_module)) {
      auto result = region_id_map.find(config_region);
      /* Must have a valid one-to-one region mapping  */
      VTR_ASSERT_SAFE(result != region_id_map.end());
      FabricRegionId fabric_region = result->second;

      for (const FabricWordLineBankId& bank :
           blwl_sr_banks.wl_banks(config_region)) {
        FabricWordLineBankId fabric_bank =
          fabric_key.create_wl_shift_register_bank(fabric_region);
        for (const BasicPort& data_port :
             blwl_sr_banks.wl_bank_data_ports(config_region, bank)) {
          fabric_key.add_data_port_to_wl_shift_register_bank(
            fabric_region, fabric_bank, data_port);
        }
      }
    }
  }

  VTR_LOGV(verbose, "Created %lu regions and %lu keys for the top module %s.\n",
           num_regions, num_keys, top_module_name.c_str());

  /* Output module subkeys if specified */
  if (include_module_keys) {
    for (ModuleId submodule : module_manager.modules()) {
      err_code =
        add_module_keys_to_fabric_key(module_manager, submodule, fabric_key);
      if (err_code != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  /* Call the XML writer for fabric key */
  err_code = write_xml_fabric_key(fname.c_str(), fabric_key);

  return err_code;
}

} /* end namespace openfpga */
