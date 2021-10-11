/***************************************************************************************
 * Output fabric key of Module Graph to file formats
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from archopenfpga library */
#include "write_xml_fabric_key.h"

#include "openfpga_naming.h"

#include "memory_utils.h"

#include "fabric_key_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Write the fabric key of top module to an XML file
 * We will use the writer API in libfabrickey
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 ***************************************************************************************/
int write_fabric_key_to_xml_file(const ModuleManager& module_manager,
                                 const std::string& fname,
                                 const ConfigProtocol& config_protocol,
                                 MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                 const bool& verbose) {
  std::string timer_message = std::string("Write fabric key to XML file '") + fname + std::string("'");

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
  if (true != module_manager.valid_module_id(top_module)) {
    VTR_LOGV_ERROR(verbose,
                   "Unable to find the top-level module '%s'!\n",
                   top_module_name.c_str());
    return 1;
  }
  
  /* Build a fabric key database by visiting all the configurable children */
  FabricKey fabric_key;
  size_t num_keys = module_manager.configurable_children(top_module).size(); 

  fabric_key.reserve_keys(num_keys);

  size_t num_regions = module_manager.regions(top_module).size();
  fabric_key.reserve_regions(num_regions);

  /* Create regions and build a id map */
  std::map<ConfigRegionId, FabricRegionId> region_id_map;
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    FabricRegionId fabric_region = fabric_key.create_region();
    region_id_map[config_region] = fabric_region;
  } 
 
  /* Create regions for the keys and load keys by region */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    FabricRegionId fabric_region = region_id_map[config_region];
    /* Each configuration protocol has some child which should not be in the list. They are typically decoders */
    size_t curr_region_num_config_child = module_manager.region_configurable_children(top_module, config_region).size();
    size_t num_child_to_skip = estimate_num_configurable_children_to_skip_by_config_protocol(config_protocol, curr_region_num_config_child);
    curr_region_num_config_child -= num_child_to_skip;

    fabric_key.reserve_region_keys(fabric_region, curr_region_num_config_child);

    for (size_t ichild = 0; ichild < curr_region_num_config_child; ++ichild) {
      ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[ichild];
      size_t child_instance = module_manager.region_configurable_child_instances(top_module, config_region)[ichild];
      vtr::Point<int> child_coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[ichild];

      FabricKeyId key = fabric_key.create_key();
      fabric_key.set_key_name(key, module_manager.module_name(child_module));
      fabric_key.set_key_value(key, child_instance);

      if (false == module_manager.instance_name(top_module, child_module, child_instance).empty()) {
        fabric_key.set_key_alias(key, module_manager.instance_name(top_module, child_module, child_instance));
      }

      /* Add key coordinate */
      fabric_key.set_key_coordinate(key, child_coord);
 
      /* Add keys to the region */
      fabric_key.add_key_to_region(fabric_region, key);
    }
  }

  /* Add BL shift register bank information, if there is any */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    FabricRegionId fabric_region = region_id_map[config_region];
    for (const FabricBitLineBankId& bank : blwl_sr_banks.bl_banks(config_region)) {
      FabricBitLineBankId fabric_bank = fabric_key.create_bl_shift_register_bank(fabric_region);
      for (const BasicPort& data_port : blwl_sr_banks.bl_bank_data_ports(config_region, bank)) {
        fabric_key.add_data_port_to_bl_shift_register_bank(fabric_region, fabric_bank, data_port);
      }
    }
  }

  /* Add WL shift register bank information, if there is any */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    FabricRegionId fabric_region = region_id_map[config_region];
    for (const FabricWordLineBankId& bank : blwl_sr_banks.wl_banks(config_region)) {
      FabricWordLineBankId fabric_bank = fabric_key.create_wl_shift_register_bank(fabric_region);
      for (const BasicPort& data_port : blwl_sr_banks.wl_bank_data_ports(config_region, bank)) {
        fabric_key.add_data_port_to_wl_shift_register_bank(fabric_region, fabric_bank, data_port);
      }
    }
  }

  VTR_LOGV(verbose,
           "Created %lu regions and %lu keys for the top module %s.\n",
           num_regions, num_keys, top_module_name.c_str());

  /* Call the XML writer for fabric key */
  int err_code = write_xml_fabric_key(fname.c_str(), fabric_key);

  return err_code;
}

} /* end namespace openfpga */
