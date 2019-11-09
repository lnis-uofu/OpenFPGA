/********************************************************************
 * Useful APIs for SDC generator
 *******************************************************************/
#include <ctime>
#include "pnr_sdc_writer.h"

#include "sdc_api.h"

/********************************************************************
 * Top-level function to launch SDC generator
 *******************************************************************/
void fpga_sdc_generator(const SdcOption& sdc_options,
                        const float& critical_path_delay,
                        const MuxLibrary& mux_lib,
                        const CircuitLibrary& circuit_lib,
                        const ModuleManager& module_manager,
                        const std::vector<CircuitPortId>& global_ports) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "SDC generator starts...");

  /* Start time count */
  clock_t t_start = clock();

  if (true == sdc_options.generate_sdc_pnr()) {
    print_pnr_sdc(sdc_options, critical_path_delay, mux_lib, circuit_lib, module_manager, global_ports); 
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "SDC generation took %g seconds\n", 
             run_time_sec);  
}
