/********************************************************************
 * This file includes functions that print SDC (Synopsys Design Constraint) 
 * files in physical design tools, i.e., Place & Route (PnR) tools
 * The SDC files are used to constrain the timing of configuration chain 
 *
 * Note that this is different from the SDC to constrain VPR Place&Route
 * engine! These SDCs are designed for PnR to generate FPGA layouts!!!
 *******************************************************************/
#include <ctime>
#include <fstream>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_scale.h"
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "openfpga_naming.h"

#include "sdc_writer_utils.h"
#include "configuration_chain_sdc_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print SDC commands to constrain the timing between outputs and inputs
 * of all the configurable memory modules
 *   
 *           |<------Max/Min delay-->|
 *           |                       | 
 *    +------+ out               in  +------+
 *    | CCFF |---------------------->| CCFF |
 *    +------+                       +------+
 *
 * This function will be executed in a recursive way, 
 * using a Depth-First Search (DFS) strategy
 * It will iterate over all the configurable children under each module
 * and print a SDC command
 *
 *******************************************************************/
static 
void rec_print_pnr_sdc_constrain_configurable_chain(std::fstream& fp, 
                                                    const float& tmax,
                                                    const float& tmin,
                                                    const ModuleManager& module_manager, 
                                                    const ModuleId& parent_module,
                                                    const std::string& parent_module_path,
                                                    std::string& previous_module_path,
                                                    ModuleId& previous_module) {

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

    child_module_path += child_instance_name;
    
    child_module_path = format_dir_path(child_module_path);

    rec_print_pnr_sdc_constrain_configurable_chain(fp,
                                                   tmax, tmin,
                                                   module_manager, 
                                                   child_module_id, 
                                                   child_module_path,
                                                   previous_module_path,
                                                   previous_module);
  }

  /* If there is no configurable children any more, this is a leaf module, print a SDC command for disable timing */
  if (0 < module_manager.configurable_children(parent_module).size()) {
    return;
  }

  /* Validate file stream */
  valid_file_stream(fp);

  /* Disable timing for each output port of this module */
  if (!previous_module_path.empty()) {
    bool first_port = true;
    for (const BasicPort& output_port : module_manager.module_ports_by_type(previous_module, ModuleManager::MODULE_OUTPUT_PORT)) {
      /* Only the first output port will be considered,
       * being consistent with build_memory_module.cpp:395
       */
      if (false == first_port) {
        continue;
      }

      for (const BasicPort& input_port : module_manager.module_ports_by_type(parent_module, ModuleManager::MODULE_INPUT_PORT)) {
        print_pnr_sdc_constrain_max_delay(fp, 
                                          previous_module_path, 
                                          generate_sdc_port(output_port),
                                          parent_module_path, 
                                          generate_sdc_port(input_port),
                                          tmax);

        print_pnr_sdc_constrain_min_delay(fp, 
                                          previous_module_path, 
                                          generate_sdc_port(output_port),
                                          parent_module_path, 
                                          generate_sdc_port(input_port),
                                          tmin);
      }
      
      first_port = false;
    }
  }

  /* Update previous module */
  previous_module_path = parent_module_path;
  previous_module = parent_module;
}


/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from
 * configurable memory cells. 
 * To handle this, we disable the outputs of memory cells
 *******************************************************************/
void print_pnr_sdc_constrain_configurable_chain(const std::string& sdc_fname,
                                                const float& time_unit,
                                                const float& max_delay,
                                                const float& min_delay,
                                                const ModuleManager& module_manager) {

  /* Create the directory */
  create_directory(find_path_dir_name(sdc_fname));

  /* Start time count */
  std::string timer_message = std::string("Write SDC to constrain configurable chain for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Timing constraints for configurable chains used in PnR"));

  /* Print time unit for the SDC file */
  print_sdc_timescale(fp, time_unit_to_string(time_unit));

  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Go recursively in the module manager, starting from the top-level module: instance id of the top-level module is 0 by default */
  std::string previous_module_path;
  ModuleId previous_module = ModuleId::INVALID();
  rec_print_pnr_sdc_constrain_configurable_chain(fp,
                                                 max_delay/time_unit, min_delay/time_unit, 
                                                 module_manager, top_module, 
                                                 format_dir_path(module_manager.module_name(top_module)),
                                                 previous_module_path,
                                                 previous_module);

  /* Close file handler */
  fp.close();
}

} /* end namespace openfpga */
