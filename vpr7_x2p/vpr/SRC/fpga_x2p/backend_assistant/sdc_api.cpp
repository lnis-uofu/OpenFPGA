/********************************************************************
 * Useful APIs for SDC generator
 *******************************************************************/
#include <ctime>
#include "pnr_sdc_writer.h"
#include "analysis_sdc_writer.h"

#include "sdc_api.h"

/********************************************************************
 * Top-level function to launch SDC generator
 *******************************************************************/
void fpga_sdc_generator(const SdcOption& sdc_options,
                        const float& critical_path_delay,
                        const std::vector<t_switch_inf>& rr_switches,
                        const DeviceRRGSB& L_device_rr_gsb,
                        const std::vector<t_logical_block>& L_logical_blocks,
                        const vtr::Point<size_t>& device_size,
                        const std::vector<std::vector<t_grid_tile>>& L_grids, 
                        const std::vector<t_block>& L_blocks,
                        const ModuleManager& module_manager,
                        const MuxLibrary& mux_lib,
                        const CircuitLibrary& circuit_lib,
                        const std::vector<CircuitPortId>& global_ports,
                        const bool& compact_routing_hierarchy) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "SDC generator starts...\n");

  /* Start time count */
  clock_t t_start = clock();

  if (true == sdc_options.generate_sdc_pnr()) {
    print_pnr_sdc(sdc_options, critical_path_delay, 
                  rr_switches, L_device_rr_gsb, 
                  module_manager, mux_lib, 
                  circuit_lib, global_ports, 
                  compact_routing_hierarchy); 
  }

  if (true == sdc_options.generate_sdc_analysis()) {
    print_analysis_sdc(sdc_options.sdc_dir(),
                       critical_path_delay,
                       L_device_rr_gsb, 
                       L_logical_blocks, device_size, L_grids,
                       L_blocks,
                       module_manager, 
                       circuit_lib, global_ports,
                       compact_routing_hierarchy);
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "SDC generation took %g seconds\n", 
             run_time_sec);  
}
