#ifndef VERILOG_FORMAL_RANDOM_TOP_TESTBENCH
#define VERILOG_FORMAL_RANDOM_TOP_TESTBENCH

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "vpr_context.h"
#include "simulation_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_random_top_testbench(const std::string& circuit_name,
                                        const std::string& verilog_fname,
                                        const std::string& verilog_dir,
                                        const AtomContext& atom_ctx,
                                        const VprNetlistAnnotation& netlist_annotation,
                                        const SimulationSetting& simulation_parameters);

} /* end namespace openfpga */

#endif
