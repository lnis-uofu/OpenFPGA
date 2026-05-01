/********************************************************************
 * Add commands to the OpenFPGA shell interface,
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - repack : create physical pbs and redo packing
 *******************************************************************/
#include "openfpga_bitstream_command.h"

#include "openfpga_bitstream_command_template.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to add all the commands related to FPGA-Bitstream
 *******************************************************************/
void add_openfpga_bitstream_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  add_bitstream_command_templates<OpenfpgaContext>(shell);
}

} /* end namespace openfpga */
