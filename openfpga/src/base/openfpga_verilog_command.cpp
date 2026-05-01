/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - generate_fabric_verilog : generate Verilog netlists about FPGA fabric
 * - generate_fabric_verilog_testbench : TODO: generate Verilog testbenches
 *******************************************************************/
#include "openfpga_verilog_command.h"

#include "openfpga_verilog_command_template.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_verilog_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  add_verilog_command_templates<OpenfpgaContext>(shell);
}

} /* end namespace openfpga */
