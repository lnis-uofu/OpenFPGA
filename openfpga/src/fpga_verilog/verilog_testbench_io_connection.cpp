/********************************************************************
 * This file includes functions that are used to generate
 * a Verilog module of a pre-configured FPGA fabric
 *******************************************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_atom_netlist_utils.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_port.h"
#include "openfpga_reserved_words.h"
#include "verilog_constants.h"
#include "verilog_preconfig_top_module_utils.h"
#include "verilog_testbench_io_connection.h"
#include "verilog_testbench_utils.h"
#include "verilog_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to generate the I/O connections for a pre-configured FPGA
 *fabric.
 *******************************************************************/
int print_verilog_testbench_io_connection(
  const ModuleManager &module_manager, const FabricGlobalPortInfo &global_ports,
  const AtomContext &atom_ctx, const PlacementContext &place_ctx,
  const PinConstraints &pin_constraints, const BusGroup &bus_group,
  const IoLocationMap &io_location_map, const ModuleNameMap &module_name_map,
  const VprNetlistAnnotation &netlist_annotation,
  const std::string &circuit_name, const std::string &verilog_fname,
  const VerilogTestbenchOption &options) {
  std::string timer_message = std::string(
                                "Write I/O connections for pre-configured FPGA "
                                "fabric mapped to design '") +
                              circuit_name + std::string("'");

  int status = CMD_EXEC_SUCCESS;

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Generate a brief description on the Verilog file*/
  std::string title =
    std::string(
      "I/O connections for a pre-configured FPGA fabric mapped to design: ") +
    circuit_name;
  print_verilog_file_header(fp, title, options.time_stamp());

  /* Spot the dut module */
  ModuleId top_module =
    module_manager.find_module(module_name_map.name(options.dut_module()));
  if (!module_manager.valid_module_id(top_module)) {
    VTR_LOG_ERROR(
      "Unable to find the DUT module '%s'. Please check if you create "
      "dedicated module when building the fabric!\n",
      options.dut_module().c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  /* Note that we always need the core module as it contains the original port
   * names before possible renaming at top-level module. If there is no core
   * module, it means that the current top module is the core module */
  std::string core_module_name = generate_fpga_core_module_name();
  if (module_name_map.name_exist(core_module_name)) {
    core_module_name = module_name_map.name(core_module_name);
  }
  ModuleId core_module = module_manager.find_module(core_module_name);
  if (!module_manager.valid_module_id(core_module)) {
    core_module = top_module;
  }

  /* Find clock ports in benchmark */
  std::vector<std::string> benchmark_clock_port_names =
    find_atom_netlist_clock_port_names(atom_ctx.netlist(), netlist_annotation);

  bool little_endian = options.little_endian();
  /* Connect FPGA top module global ports to constant or benchmark global
   * signals! */
  status = print_verilog_preconfig_top_module_connect_global_ports(
    fp, module_manager, core_module, pin_constraints, atom_ctx,
    netlist_annotation, global_ports, benchmark_clock_port_names, std::string(),
    little_endian);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }

  /* Connect I/Os to benchmark I/Os or constant driver */
  print_verilog_testbench_connect_fpga_ios(
    fp, module_manager, core_module, atom_ctx, place_ctx, io_location_map,
    netlist_annotation, bus_group, std::string(), std::string(), std::string(),
    std::vector<std::string>(), (size_t)VERILOG_DEFAULT_SIGNAL_INIT_VALUE,
    little_endian);

  /* Close the file stream */
  fp.close();

  return status;
}

} /* end namespace openfpga */
