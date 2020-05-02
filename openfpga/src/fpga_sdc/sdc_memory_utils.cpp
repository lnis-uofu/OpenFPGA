/********************************************************************
 * Most utilized function used to constrain memory cells in FPGA
 * fabric using SDC commands
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

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
 *******************************************************************/
void rec_print_pnr_sdc_disable_configurable_memory_module_output(std::fstream& fp, 
                                                                 const ModuleManager& module_manager, 
                                                                 const ModuleId& parent_module,
                                                                 const std::string& parent_module_path) {

  /* Keep tracking multiple-instanced-blocks (MIB) 
   * if they should be skipped as the unique module has been visited already
   */
  std::map<ModuleId, bool> skip_mib;
  /* For each configurable child, we will go one level down in priority */
  for (size_t child_index = 0; child_index < module_manager.configurable_children(parent_module).size(); ++child_index) {
    std::string child_module_path = parent_module_path;
    ModuleId child_module_id = module_manager.configurable_children(parent_module)[child_index];
    size_t child_instance_id = module_manager.configurable_child_instances(parent_module)[child_index];
    if (true == module_manager.instance_name(parent_module, child_module_id, child_instance_id).empty()) {
      if (0 < skip_mib.count(child_module_id)) {
        VTR_ASSERT(skip_mib.at(child_module_id) = true);
        continue;
      }
      /* Give a default name <module_name>_<instance_id>_ */
      child_module_path += module_manager.module_name(child_module_id); 
      child_module_path += "_";
      child_module_path += "*";
      //child_module_path += std::to_string(child_instance_id);
      child_module_path += "_";
      /* If we use wild card to disable all the other instance
       * So we can skip later if there is no specific instance name 
       */
      skip_mib[child_module_id] = true;
    } else {
      child_module_path += module_manager.instance_name(parent_module, child_module_id, child_instance_id);
    }
    child_module_path = format_dir_path(child_module_path);

    rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, module_manager, 
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
    for (const size_t& pin : output_port.pins()) {
      BasicPort output_pin(output_port.get_name(), pin, pin);
      fp << "set_disable_timing ";
      fp << parent_module_path << generate_sdc_port(output_pin);
      fp << std::endl;
    }
  }
}

} /* end namespace openfpga */
