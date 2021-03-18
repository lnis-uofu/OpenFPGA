/********************************************************************
 * This file includes functions that print SDC (Synopsys Design Constraint) 
 * files in physical design tools, i.e., Place & Route (PnR) tools
 * The SDC files are used to constrain the physical design for each module
 * in FPGA fabric, such as Configurable Logic Blocks (CLBs), 
 * Heterogeneous blocks, Switch Blocks (SBs) and Connection Blocks (CBs)
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
#include "openfpga_port.h"
#include "openfpga_wildcard_string.h"
#include "openfpga_digest.h"

#include "mux_utils.h"

#include "openfpga_naming.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "sdc_memory_utils.h"
#include "sdc_mux_utils.h"
#include "sdc_hierarchy_writer.h"
#include "pnr_sdc_global_port.h"
#include "pnr_sdc_routing_writer.h"
#include "pnr_sdc_grid_writer.h"
#include "pnr_sdc_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from
 * configurable memory cells. 
 * To handle this, we disable the outputs of memory cells
 *******************************************************************/
static 
void print_pnr_sdc_constrain_configurable_memory_outputs(const std::string& sdc_dir,
                                                         const bool& flatten_names,
                                                         const ModuleManager& module_manager,
                                                         const ModuleId& top_module) {

  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_CONFIG_MEM_OUTPUTS_FILE_NAME));

  /* Start time count */
  std::string timer_message = std::string("Write SDC to disable configurable memory outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable configurable memory outputs for PnR"));

  /* Go recursively in the module manager, starting from the top-level module: instance id of the top-level module is 0 by default */
  rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, flatten_names,
                                                              module_manager, top_module, 
                                                              format_dir_path(module_manager.module_name(top_module)));

  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * loops of multiplexers.
 * To handle this, we disable the timing at outputs of Switch blocks
 * This function is designed for flatten routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_flatten_routing_disable_switch_block_outputs(const std::string& sdc_dir,
                                                                const bool& flatten_names,
                                                                const ModuleManager& module_manager,
                                                                const ModuleId& top_module,
                                                                const DeviceRRGSB& device_rr_gsb) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_SB_OUTPUTS_FILE_NAME));

  /* Start time count */
  std::string timer_message = std::string("Write SDC to disable switch block outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable Switch Block outputs for PnR"));

  std::string root_path = format_dir_path(module_manager.module_name(top_module));

  /* Build wildcard names for the instance names of multiple-instanced-blocks (MIB) 
   * We will find all the instance names and see there are common prefix 
   * If so, we can use wildcards
   */
  std::map<ModuleId, std::vector<std::string>> wildcard_names;

  /* Get the range of SB array */
  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();
  /* Go for each SB */
  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);

      if (false == rr_gsb.is_sb_exist()) {
        continue;
      }

      std::string module_path = root_path;

      vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
      std::string sb_instance_name = generate_switch_block_module_name(gsb_coordinate); 

      ModuleId sb_module = module_manager.find_module(sb_instance_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

      if (false == flatten_names) { 
        /* Try to adapt to a wildcard name: replace all the numbers with a wildcard character '*' */
        WildCardString wildcard_str(sb_instance_name); 
        /* If the wildcard name is already in the list, we can skip this
         * Otherwise, we have to 
         *   - output this instance 
         *   - record the wildcard name in the map 
         */
        if ( (0 < wildcard_names.count(sb_module)) 
          && (wildcard_names.at(sb_module).end() != std::find(wildcard_names.at(sb_module).begin(),
                                                              wildcard_names.at(sb_module).end(),
                                                              wildcard_str.data())) ) {
          continue;
        }

        module_path += wildcard_str.data();
 
        wildcard_names[sb_module].push_back(wildcard_str.data());
      } else {
        module_path += sb_instance_name;
      }

      module_path = format_dir_path(module_path);

      std::vector<std::string> port_wildcard_names;

      /* Disable the outputs of the module */
      for (const BasicPort& output_port : module_manager.module_ports_by_type(sb_module, ModuleManager::MODULE_OUTPUT_PORT)) {
        std::string port_name = output_port.get_name();        

        if (false == flatten_names) { 
          /* Try to adapt to a wildcard name: replace all the numbers with a wildcard character '*' */
          WildCardString port_wildcard_str(output_port.get_name()); 
          /* If the wildcard name is already in the list, we can skip this
           * Otherwise, we have to 
           *   - output this port 
           *   - record the wildcard name in the vector
           */
          if (port_wildcard_names.end() != std::find(port_wildcard_names.begin(),
                                                     port_wildcard_names.end(),
                                                     port_wildcard_str.data())) {
            continue;
          }

          port_name = port_wildcard_str.data();
 
          port_wildcard_names.push_back(port_wildcard_str.data());
        }

        fp << "set_disable_timing ";
        fp << module_path;
        fp << port_name << std::endl;

        fp << std::endl;
      }
    }
  }
  
  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * loops of multiplexers.
 * To handle this, we disable the timing at outputs of Switch blocks
 * This function is designed for compact routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_compact_routing_disable_switch_block_outputs(const std::string& sdc_dir,
                                                                const bool& flatten_names,
                                                                const ModuleManager& module_manager,
                                                                const ModuleId& top_module,
                                                                const DeviceRRGSB& device_rr_gsb) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_SB_OUTPUTS_FILE_NAME));

  /* Start time count */
  std::string timer_message = std::string("Write SDC to disable switch block outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable Switch Block outputs for PnR"));

  std::string root_path = format_dir_path(module_manager.module_name(top_module));

  /* Build wildcard names for the instance names of multiple-instanced-blocks (MIB) 
   * We will find all the instance names and see there are common prefix 
   * If so, we can use wildcards
   */
  std::map<ModuleId, std::vector<std::string>> wildcard_names;

  /* Build unique switch block modules */
  for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& rr_gsb = device_rr_gsb.get_sb_unique_module(isb);

    if (false == rr_gsb.is_sb_exist()) {
      continue;
    }

    vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
    std::string sb_module_name = generate_switch_block_module_name(gsb_coordinate); 

    ModuleId sb_module = module_manager.find_module(sb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(sb_module));
    
    std::string module_path = root_path;

    /* Find all the instances in the top-level module */
    for (const size_t& instance_id : module_manager.child_module_instances(top_module, sb_module)) {
      std::string sb_instance_name = module_manager.instance_name(top_module, sb_module, instance_id);

      if (false == flatten_names) { 
        /* Try to adapt to a wildcard name: replace all the numbers with a wildcard character '*' */
        WildCardString wildcard_str(sb_instance_name); 
        /* If the wildcard name is already in the list, we can skip this
         * Otherwise, we have to 
         *   - output this instance 
         *   - record the wildcard name in the map 
         */
        if ( (0 < wildcard_names.count(sb_module)) 
          && (wildcard_names.at(sb_module).end() != std::find(wildcard_names.at(sb_module).begin(),
                                                              wildcard_names.at(sb_module).end(),
                                                              wildcard_str.data())) ) {
          continue;
        }

        module_path += wildcard_str.data();
 
        wildcard_names[sb_module].push_back(wildcard_str.data());
      } else {
        module_path += sb_instance_name;
      }

      module_path = format_dir_path(module_path);

      std::vector<std::string> port_wildcard_names;

      /* Disable the outputs of the module */
      for (const BasicPort& output_port : module_manager.module_ports_by_type(sb_module, ModuleManager::MODULE_OUTPUT_PORT)) {
        std::string port_name = output_port.get_name();        

        if (false == flatten_names) { 
          /* Try to adapt to a wildcard name: replace all the numbers with a wildcard character '*' */
          WildCardString port_wildcard_str(output_port.get_name()); 
          /* If the wildcard name is already in the list, we can skip this
           * Otherwise, we have to 
           *   - output this port 
           *   - record the wildcard name in the vector
           */
          if (port_wildcard_names.end() != std::find(port_wildcard_names.begin(),
                                                     port_wildcard_names.end(),
                                                     port_wildcard_str.data())) {
            continue;
          }

          port_name = port_wildcard_str.data();
 
          port_wildcard_names.push_back(port_wildcard_str.data());
        }

        fp << "set_disable_timing ";
        fp << module_path;
        fp << port_name << std::endl;

        fp << std::endl;
      }
    }
  }
  
  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Top-level function to print a number of SDC files in different purpose
 * This function will generate files upon the options provided by users
 * 1. Design constraints for CLBs
 * 2. Design constraints for Switch Blocks
 * 3. Design constraints for Connection Blocks 
 * 4. Design constraints for breaking the combinational loops in FPGA fabric
 *******************************************************************/
void print_pnr_sdc(const PnrSdcOption& sdc_options,
                   const DeviceContext& device_ctx,
                   const VprDeviceAnnotation& device_annotation,
                   const DeviceRRGSB& device_rr_gsb,
                   const ModuleManager& module_manager,
                   const MuxLibrary& mux_lib,
                   const CircuitLibrary& circuit_lib,
                   const FabricGlobalPortInfo& global_ports,
                   const SimulationSetting& sim_setting,
                   const bool& compact_routing_hierarchy) {

  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Constrain global ports */
  if (true == sdc_options.constrain_global_port()) {
    print_pnr_sdc_global_ports(sdc_options.sdc_dir(),
                               module_manager, top_module, global_ports,
                               sim_setting,
                               sdc_options.constrain_non_clock_global_port());
  }

  /* Output Design Constraints to disable outputs of memory cells */
  if (true == sdc_options.constrain_configurable_memory_outputs()) {
    print_pnr_sdc_constrain_configurable_memory_outputs(sdc_options.sdc_dir(),
                                                        sdc_options.flatten_names(),
                                                        module_manager,
                                                        top_module); 
  } 

  /* Break loops from Multiplexer Output */
  if (true == sdc_options.constrain_routing_multiplexer_outputs()) {
    print_sdc_disable_routing_multiplexer_outputs(sdc_options.sdc_dir(),
                                                  sdc_options.flatten_names(),
                                                  mux_lib, circuit_lib,
                                                  module_manager,
                                                  top_module);
  }

  /* Break loops from any SB output */
  if (true == sdc_options.constrain_switch_block_outputs()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_disable_switch_block_outputs(sdc_options.sdc_dir(),
                                                                 sdc_options.flatten_names(),
                                                                 module_manager, top_module,
                                                                 device_rr_gsb);
    } else {
      VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_disable_switch_block_outputs(sdc_options.sdc_dir(),
                                                                 sdc_options.flatten_names(),
                                                                 module_manager, top_module,
                                                                 device_rr_gsb);
    }
  }

  /* Output routing constraints for Switch Blocks */
  if (true == sdc_options.constrain_sb()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_constrain_sb_timing(sdc_options.sdc_dir(),
                                                        sdc_options.time_unit(),
                                                        sdc_options.hierarchical(),
                                                        module_manager,
                                                        top_module,
                                                        device_annotation,
                                                        device_ctx.grid,
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb,
                                                        sdc_options.constrain_zero_delay_paths());
    } else {
	  VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_constrain_sb_timing(sdc_options.sdc_dir(),
                                                        sdc_options.time_unit(),
                                                        sdc_options.hierarchical(),
                                                        module_manager,
                                                        top_module,
                                                        device_annotation,
                                                        device_ctx.grid,
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb,
                                                        sdc_options.constrain_zero_delay_paths());
    }
  }

  /* Output hierachy to plain text file */
  if ( (true == sdc_options.constrain_sb())
    && (true == sdc_options.output_hierarchy())
    && (true == compact_routing_hierarchy) ) {
    print_pnr_sdc_routing_sb_hierarchy(sdc_options.sdc_dir(),
                                       module_manager,
                                       top_module,
                                       device_rr_gsb);
  }

  /* Output routing constraints for Connection Blocks */
  if (true == sdc_options.constrain_cb()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_constrain_cb_timing(sdc_options.sdc_dir(),
                                                        sdc_options.time_unit(),
                                                        sdc_options.hierarchical(),
                                                        module_manager,
                                                        top_module,
                                                        device_annotation,
                                                        device_ctx.grid,
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb,
                                                        sdc_options.constrain_zero_delay_paths());
    } else {
	  VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_constrain_cb_timing(sdc_options.sdc_dir(),
                                                        sdc_options.time_unit(),
                                                        sdc_options.hierarchical(),
                                                        module_manager, 
                                                        top_module,
                                                        device_annotation,
                                                        device_ctx.grid,
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb,
                                                        sdc_options.constrain_zero_delay_paths());
    }
  }

  /* Output hierachy to plain text file */
  if ( (true == sdc_options.constrain_cb())
    && (true == sdc_options.output_hierarchy())
    && (true == compact_routing_hierarchy) ) {
    print_pnr_sdc_routing_cb_hierarchy(sdc_options.sdc_dir(),
                                       module_manager,
                                       top_module,
                                       CHANX,
                                       device_rr_gsb);

    print_pnr_sdc_routing_cb_hierarchy(sdc_options.sdc_dir(),
                                       module_manager,
                                       top_module,
                                       CHANY,
                                       device_rr_gsb);
  }

  /* Output Timing constraints for Programmable blocks */
  if (true == sdc_options.constrain_grid()) {
    print_pnr_sdc_constrain_grid_timing(sdc_options.sdc_dir(),
                                        sdc_options.time_unit(),
                                        sdc_options.hierarchical(),
                                        device_ctx,
                                        device_annotation,
                                        module_manager,
                                        top_module,
                                        sdc_options.constrain_zero_delay_paths());
  }

  if ( (true == sdc_options.constrain_grid())
    && (true == sdc_options.output_hierarchy()) ) {
    print_pnr_sdc_grid_hierarchy(sdc_options.sdc_dir(),
                                 device_ctx,
                                 device_annotation,
                                 module_manager,
                                 top_module);

  }
}

} /* end namespace openfpga */
