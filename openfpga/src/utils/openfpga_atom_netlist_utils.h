#ifndef OPENFPGA_ATOM_NETLIST_UTILS_H
#define OPENFPGA_ATOM_NETLIST_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include "atom_netlist.h"
#include "vpr_netlist_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<std::string> find_atom_netlist_clock_port_names(const AtomNetlist& atom_nlist,
                                                            const VprNetlistAnnotation& netlist_annotation);

std::string remove_atom_block_name_prefix(const std::string& block_name);

} /* end namespace openfpga */

#endif
