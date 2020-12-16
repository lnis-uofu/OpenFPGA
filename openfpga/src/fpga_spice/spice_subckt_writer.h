#ifndef SPICE_SUBCKT_WRITER_H
#define SPICE_SUBCKT_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void write_spice_subckt_to_file(std::fstream& fp,
                                const ModuleManager& module_manager,
                                const ModuleId& module_id);

} /* end namespace openfpga */

#endif
