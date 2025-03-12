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
#include "bitstream_manager_utils.h"
#include "openfpga_atom_netlist_utils.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_port.h"
#include "openfpga_reserved_words.h"
#include "verilog_constants.h"
#include "verilog_preconfig_top_module_utils.h"
#include "verilog_template_testbench.h"
#include "verilog_testbench_utils.h"
#include "verilog_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to generate a template testbench for a FPGA fabric.
 *
 *   Testbench
 *   +--------------------------------------------
 *   |
 *   |          FPGA fabric
 *   |          +-------------------------------+
 *   |          |                               |
 *   |  0/1---->|FPGA global ports              |
 *   |          |                               |
 *   |--------->|FPGA_clock                     |
 *   |          |                               |
 *   |--------->|FPGA mapped I/Os               |
 *   |          |                               |
 *   |<---------|FPGA mapped I/Os               |
 *   |          |                               |
 *   |  0/1---->|FPGA unmapped I/Os             |
 *   |          |                               |
 *   |--------->|Internal_configuration_ports   |
 *   |          +-------------------------------+
 *   |
 *   +-------------------------------------------
 *******************************************************************/
int print_verilog_template_testbench(const ModuleManager &module_manager,
                                     const IoNameMap &io_name_map,
                                     const ModuleNameMap &module_name_map,
                                     const std::string &verilog_fname,
                                     const VerilogTestbenchOption &options) {
  std::string timer_message = std::string(
    "Write a template Verilog testbench for pre-configured FPGA top-level "
    "netlist");

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
    std::string("A template Verilog testbench for pre-configured FPGA fabric");
  print_verilog_file_header(fp, title, options.time_stamp());

  print_verilog_comment(fp,
                        std::string("Require an adaption to your needs before "
                                    "used for design verification!!!"));

  print_verilog_default_net_type_declaration(fp, options.default_net_type());

  /* Module declaration */
  fp << "module " << options.top_module();
  fp << ";" << std::endl;

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

  bool little_endian = options.little_endian();
  /* Print internal wires */
  print_verilog_preconfig_top_module_internal_wires(
    fp, module_manager, core_module, std::string(), little_endian);

  /* Instanciate FPGA top-level module */
  print_verilog_testbench_fpga_instance(
    fp, module_manager, top_module, core_module,
    std::string(FORMAL_VERIFICATION_TOP_MODULE_UUT_NAME),
    std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX), io_name_map,
    options.explicit_port_mapping(), little_endian);

  /* Testbench ends*/
  print_verilog_module_end(fp, options.top_module(),
                           options.default_net_type());

  /* Close the file stream */
  fp.close();

  return status;
}

} /* end namespace openfpga */
