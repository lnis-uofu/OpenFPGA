#ifndef ANALYSIS_SDC_WRITER_H
#define ANALYSIS_SDC_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "vpr_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_analysis_sdc(const std::string& sdc_dir,
                        const float& critical_path_delay,
                        const VprContext& vpr_ctx, 
                        const OpenfpgaContext& openfpga_ctx,
                        const std::vector<CircuitPortId>& global_ports,
                        const bool& compact_routing_hierarchy);

} /* end namespace openfpga */

#endif
