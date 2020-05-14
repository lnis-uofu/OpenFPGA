/********************************************************************
 * Most utilized function used to constrain routing multiplexers in FPGA
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

#include "sdc_mux_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print SDC commands to disable outputs of routing multiplexer modules
 * in a given module id 
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
void rec_print_pnr_sdc_disable_routing_multiplexer_outputs(std::fstream& fp, 
                                                           const bool& flatten_names,
                                                           const ModuleManager& module_manager, 
                                                           const ModuleId& parent_module,
                                                           const ModuleId& mux_module,
                                                           const std::string& parent_module_path) {

  /* Build wildcard names for the instance names of multiple-instanced-blocks (MIB) 
   * We will find all the instance names and see there are common prefix 
   * If so, we can use wildcards
   */
  std::map<ModuleId, std::vector<std::string>> wildcard_names;

  /* For each child, we will go one level down in priority */
  for (const ModuleId& child_module : module_manager.child_modules(parent_module)) {

    /* Iterate over the child instances*/
    for (const size_t& child_instance : module_manager.child_module_instances(parent_module, child_module)) {
      std::string child_module_path = parent_module_path;

      std::string child_instance_name;
      if (true == module_manager.instance_name(parent_module, child_module, child_instance).empty()) {
        child_instance_name = generate_instance_name(module_manager.module_name(child_module), child_instance);
      } else {
        child_instance_name = module_manager.instance_name(parent_module, child_module, child_instance);
      }

      if (false == flatten_names) { 
        /* Try to adapt to a wildcard name: replace all the numbers with a wildcard character '*' */
        WildCardString wildcard_str(child_instance_name); 
        /* If the wildcard name is already in the list, we can skip this
         * Otherwise, we have to 
         *   - output this instance 
         *   - record the wildcard name in the map 
         */
        if ( (0 < wildcard_names.count(child_module)) 
          && (wildcard_names.at(child_module).end() != std::find(wildcard_names.at(child_module).begin(),
                                                                 wildcard_names.at(child_module).end(),
                                                                 wildcard_str.data())) ) {
          continue;
        }

        child_module_path += wildcard_str.data();
 
        wildcard_names[child_module].push_back(wildcard_str.data());
      } else {
        child_module_path += child_instance_name;
      }
      
      child_module_path = format_dir_path(child_module_path);

      /* If this is NOT the MUX module we want, we go recursively */
      if (mux_module != child_module) {
        rec_print_pnr_sdc_disable_routing_multiplexer_outputs(fp, flatten_names,
                                                              module_manager, 
                                                              child_module, 
                                                              mux_module,
                                                              child_module_path);
        continue;
      }

      /* Validate file stream */
      valid_file_stream(fp);

      /* Reach here, this is the MUX module we want, disable the outputs */
      for (const BasicPort& output_port : module_manager.module_ports_by_type(mux_module, ModuleManager::MODULE_OUTPUT_PORT)) {
        fp << "set_disable_timing ";
        fp << child_module_path << output_port.get_name();
        fp << std::endl;
      }
    }
  }
}

} /* end namespace openfpga */
