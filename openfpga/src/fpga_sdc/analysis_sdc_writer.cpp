/********************************************************************
 * This file includes functions that are used to output a SDC file
 * that constrain a FPGA fabric (P&Red netlist) using a benchmark 
 *******************************************************************/
#include <ctime>
#include <fstream>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_port.h"

#include "mux_utils.h"

#include "openfpga_naming.h"
#include "openfpga_atom_netlist_utils.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "sdc_memory_utils.h"

#include "analysis_sdc_grid_writer.h"
#include "analysis_sdc_routing_writer.h"
#include "analysis_sdc_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate SDC constaints for inputs and outputs
 * We consider the top module in formal verification purpose here
 * which is easier 
 *******************************************************************/
static 
void print_analysis_sdc_io_delays(std::fstream& fp,
                                  const AtomContext& atom_ctx,
                                  const PlacementContext& place_ctx,
                                  const VprNetlistAnnotation& netlist_annotation,
                                  const IoLocationMap& io_location_map,
                                  const ModuleManager& module_manager,
                                  const ModuleId& top_module,
                                  const CircuitLibrary& circuit_lib,
                                  const std::vector<CircuitPortId>& global_ports,
                                  const float& critical_path_delay) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Create clock                                    " << std::endl;
  fp << "##################################################" << std::endl; 

  /* Get clock port from the global port */
  std::vector<BasicPort> operating_clock_ports;
  for (const CircuitPortId& clock_port : global_ports) {
    if (CIRCUIT_MODEL_PORT_CLOCK != circuit_lib.port_type(clock_port)) {
      continue;
    }
    /* We only constrain operating clock here! */
    if (true == circuit_lib.port_is_prog(clock_port)) {
      continue;
    }
   
    /* Find the module port and Update the operating port list */
    ModulePortId module_port = module_manager.find_module_port(top_module, circuit_lib.port_prefix(clock_port)); 
    operating_clock_ports.push_back(module_manager.module_port(top_module, module_port));
  }

  for (const BasicPort& operating_clock_port : operating_clock_ports) {
    /* Reach here, it means a clock port and we need print constraints */
    fp << "create_clock ";
    fp << generate_sdc_port(operating_clock_port);
    fp << " -period " << std::setprecision(10) << critical_path_delay;
    fp << " -waveform {0 " << std::setprecision(10) << critical_path_delay / 2 << "}";
    fp << std::endl;

    /* Add an empty line as a splitter */
    fp << std::endl;
  }

  /* There should be only one operating clock!
   * TODO: this should be changed when developing multi-clock support!!!
   */
  VTR_ASSERT(1 == operating_clock_ports.size());

  /* In this function, we support only 1 type of I/Os */
  std::vector<BasicPort> module_io_ports = module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT);

  for (const BasicPort& module_io_port : module_io_ports) {
    /* Keep tracking which I/Os have been used */
    std::vector<bool> io_used(module_io_port.get_width(), false);

    /* Find clock ports in benchmark */
    std::vector<std::string> benchmark_clock_port_names = find_atom_netlist_clock_port_names(atom_ctx.nlist, netlist_annotation);

    /* Print comments */
    fp << "##################################################" << std::endl; 
    fp << "# Create input and output delays for used I/Os    " << std::endl;
    fp << "##################################################" << std::endl; 

    for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
      /* Bypass non-I/O atom blocks ! */
      if ( (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk))
        && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) ) {
        continue;
      }

      /* clock net or constant generator should be disabled in timing analysis */
      if (benchmark_clock_port_names.end() != std::find(benchmark_clock_port_names.begin(), benchmark_clock_port_names.end(), atom_ctx.nlist.block_name(atom_blk))) {
        continue;
      }

      /* Find the index of the mapped GPIO in top-level FPGA fabric */
      size_t io_index = io_location_map.io_index(place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.x,
                                                 place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.y,
                                                 place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.z);

      /* Ensure that IO index is in range */
      BasicPort module_mapped_io_port = module_io_port; 
      /* Set the port pin index */ 
      VTR_ASSERT(io_index < module_mapped_io_port.get_width());
      module_mapped_io_port.set_width(io_index, io_index);

      /* For input I/O, we set an input delay constraint correlated to the operating clock
       * For output I/O, we set an output delay constraint correlated to the operating clock
       */
      if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
        print_sdc_set_port_input_delay(fp, module_mapped_io_port,
                                       operating_clock_ports[0], critical_path_delay);
      } else {
        VTR_ASSERT(AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk));
        print_sdc_set_port_output_delay(fp, module_mapped_io_port,
                                        operating_clock_ports[0], critical_path_delay);
      }

      /* Mark this I/O has been used/wired */
      io_used[io_index] = true;
    }

    /* Add an empty line as a splitter */
    fp << std::endl;

    /* Print comments */
    fp << "##################################################" << std::endl; 
    fp << "# Disable timing for unused I/Os    " << std::endl;
    fp << "##################################################" << std::endl; 

    /* Wire the unused iopads to a constant */
    for (size_t io_index = 0; io_index < io_used.size(); ++io_index) {
      /* Bypass used iopads */
      if (true == io_used[io_index]) {
        continue;
      }

      /* Wire to a contant */
      BasicPort module_unused_io_port = module_io_port; 
      /* Set the port pin index */ 
      module_unused_io_port.set_width(io_index, io_index);
      print_sdc_disable_port_timing(fp, module_unused_io_port);
    }

    /* Add an empty line as a splitter */
    fp << std::endl;
  }
}

/********************************************************************
 * Disable the timing for all the global port except the operating clock ports
 *******************************************************************/
static 
void print_analysis_sdc_disable_global_ports(std::fstream& fp,
                                             const ModuleManager& module_manager,
                                             const ModuleId& top_module,
                                             const CircuitLibrary& circuit_lib,
                                             const std::vector<CircuitPortId>& global_ports) {
  /* Validate file stream */
  valid_file_stream(fp);

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Disable timing for global ports                 " << std::endl;
  fp << "##################################################" << std::endl; 

  for (const CircuitPortId& global_port : global_ports) {
    /* Skip operating clock here! */
    if ( (CIRCUIT_MODEL_PORT_CLOCK == circuit_lib.port_type(global_port)) 
      && (false == circuit_lib.port_is_prog(global_port)) ) {
      continue;
    }

    ModulePortId module_port = module_manager.find_module_port(top_module, circuit_lib.port_prefix(global_port)); 
    BasicPort port_to_disable = module_manager.module_port(top_module, module_port);

    print_sdc_disable_port_timing(fp, port_to_disable);
  }
}

/********************************************************************
 * Top-level function outputs a SDC file
 * that constrain a FPGA fabric (P&Red netlist) using a benchmark 
 *******************************************************************/
void print_analysis_sdc(const AnalysisSdcOption& option,
                        const float& critical_path_delay,
                        const VprContext& vpr_ctx, 
                        const OpenfpgaContext& openfpga_ctx,
                        const std::vector<CircuitPortId>& global_ports,
                        const bool& compact_routing_hierarchy) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(option.sdc_dir() + std::string(SDC_ANALYSIS_FILE_NAME));

  std::string timer_message = std::string("Generating SDC for Timing/Power analysis on the mapped FPGA '")
                            + sdc_fname
                            + std::string("'");

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  /* Validate file stream */
  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Constrain for Timing/Power analysis on the mapped FPGA"));

  /* Find the top_module */
  ModuleId top_module = openfpga_ctx.module_graph().find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == openfpga_ctx.module_graph().valid_module_id(top_module));

  /* Create clock and set I/O ports with input/output delays */
  print_analysis_sdc_io_delays(fp,
                               vpr_ctx.atom(), vpr_ctx.placement(),
                               openfpga_ctx.vpr_netlist_annotation(), openfpga_ctx.io_location_map(),
                               openfpga_ctx.module_graph(), top_module, 
                               openfpga_ctx.arch().circuit_lib, global_ports,
                               critical_path_delay);

  /* Disable the timing for global ports */
  print_analysis_sdc_disable_global_ports(fp,
                                          openfpga_ctx.module_graph(), top_module,
                                          openfpga_ctx.arch().circuit_lib, global_ports);

  /* Disable the timing for configuration cells */ 
  rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, 
                                                              openfpga_ctx.module_graph(), top_module, 
                                                              format_dir_path(openfpga_ctx.module_graph().module_name(top_module)));


  /* Disable timing for unused routing resources in connection blocks */
  print_analysis_sdc_disable_unused_cbs(fp,
                                        vpr_ctx.atom(), 
                                        openfpga_ctx.module_graph(),
                                        vpr_ctx.device().rr_graph,
                                        openfpga_ctx.vpr_routing_annotation(),
                                        openfpga_ctx.device_rr_gsb(), 
                                        compact_routing_hierarchy);

  /* Disable timing for unused routing resources in switch blocks */
  print_analysis_sdc_disable_unused_sbs(fp,
                                        vpr_ctx.atom(), 
                                        openfpga_ctx.module_graph(),
                                        vpr_ctx.device().rr_graph,
                                        openfpga_ctx.vpr_routing_annotation(),
                                        openfpga_ctx.device_rr_gsb(), 
                                        compact_routing_hierarchy);

  /* Disable timing for unused routing resources in grids (programmable blocks) */
  print_analysis_sdc_disable_unused_grids(fp,
                                          vpr_ctx.device().grid,
                                          openfpga_ctx.vpr_device_annotation(),
                                          openfpga_ctx.vpr_clustering_annotation(),
                                          openfpga_ctx.vpr_placement_annotation(),
                                          openfpga_ctx.module_graph());

  /* Close file handler */
  fp.close();
}

} /* end namespace openfpga */
