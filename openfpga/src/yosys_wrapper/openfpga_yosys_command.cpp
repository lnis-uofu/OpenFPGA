/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - generate_fabric_verilog : generate Verilog netlists about FPGA fabric
 * - generate_fabric_verilog_testbench : TODO: generate Verilog testbenches
 *******************************************************************/
#ifdef OPENFPGA_INCLUDE_YOSYS_COMMAND

#include "openfpga_yosys_command.h"

#include "openfpga_yosys_command_template.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_yosys_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  add_yosys_command_templates<OpenfpgaContext>(shell);
}

} /* end namespace openfpga */

#endif
