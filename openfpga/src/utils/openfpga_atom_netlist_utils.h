#ifndef OPENFPGA_ATOM_NETLIST_UTILS_H
#define OPENFPGA_ATOM_NETLIST_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include "atom_netlist.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<std::string> find_atom_netlist_clock_port_names(const AtomNetlist& atom_nlist);

} /* end namespace openfpga */

#endif
