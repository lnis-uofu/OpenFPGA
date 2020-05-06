/********************************************************************
 * Most utilized function used to constrain memory cells in FPGA
 * fabric using SDC commands
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_wildcard_string.h"
#include "openfpga_digest.h"

#include "openfpga_naming.h"

#include "sdc_writer_utils.h"

#include "sdc_memory_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print SDC commands to disable outputs of all the configurable memory modules
 * in a given module 
 * This function will be executed in a recursive way, 
 * using a Depth-First Search (DFS) strategy
 * It will iterate over all the configurable children under each module
 * and print a SDC command to disable its outputs
 *
 * Note:
 *   - When flatten_names is true
 *     this function will not apply any wildcard to names
 *   - When flatten_names is false
 *     It will straightforwardly output the instance name and port name
 *     This function will try to apply wildcard to names
 *     so that SDC file size can be minimal 
 *******************************************************************/
void rec_print_pnr_sdc_disable_configurable_memory_module_output(std::fstream& fp, 
                                                                 const bool& flatten_names,
                                                                 const ModuleManager& module_manager, 
                                                                 const ModuleId& parent_module,
                                                                 const std::string& parent_module_path) {

  /* Build wildcard names for the instance names of multiple-instanced-blocks (MIB) 
   * We will find all the instance names and see there are common prefix 
   * If so, we can use wildcards
   */
  std::map<ModuleId, std::vector<std::string>> wildcard_names;

  /* For each configurable child, we will go one level down in priority */
  for (size_t child_index = 0; child_index < module_manager.configurable_children(parent_module).size(); ++child_index) {
    std::string child_module_path = parent_module_path;
    ModuleId child_module_id = module_manager.configurable_children(parent_module)[child_index];
    size_t child_instance_id = module_manager.configurable_child_instances(parent_module)[child_index];
    std::string child_instance_name;
    if (true == module_manager.instance_name(parent_module, child_module_id, child_instance_id).empty()) {
      child_instance_name = generate_instance_name(module_manager.module_name(child_module_id), child_instance_id);
    } else {
      child_instance_name = module_manager.instance_name(parent_module, child_module_id, child_instance_id);
    }

    if (false == flatten_names) { 
      /* Try to adapt to a wildcard name: replace all the numbers with a wildcard character '*' */
      WildCardString wildcard_str(child_instance_name); 
      /* If the wildcard name is already in the list, we can skip this
       * Otherwise, we have to 
       *   - output this instance 
       *   - record the wildcard name in the map 
       */
      if ( (0 < wildcard_names.count(child_module_id)) 
        && (wildcard_names.at(child_module_id).end() != std::find(wildcard_names.at(child_module_id).begin(),
                                                                  wildcard_names.at(child_module_id).end(),
                                                                  wildcard_str.data())) ) {
        continue;
      }

      child_module_path += wildcard_str.data();
 
      wildcard_names[child_module_id].push_back(wildcard_str.data());
    } else {
      child_module_path += child_instance_name;
    }
    
    child_module_path = format_dir_path(child_module_path);

    rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, flatten_names,
                                                                module_manager, 
                                                                child_module_id, 
                                                                child_module_path);
  }

  /* If there is no configurable children any more, this is a leaf module, print a SDC command for disable timing */
  if (0 < module_manager.configurable_children(parent_module).size()) {
    return;
  }

  /* Validate file stream */
  valid_file_stream(fp);

  /* Disable timing for each output port of this module */
  for (const BasicPort& output_port : module_manager.module_ports_by_type(parent_module, ModuleManager::MODULE_OUTPUT_PORT)) {
    fp << "set_disable_timing ";
    fp << parent_module_path << output_port.get_name();
    fp << std::endl;
  }
}

} /* end namespace openfpga */
