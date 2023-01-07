#ifndef CHECK_NETLIST_NAMING_CONFLICT_H
#define CHECK_NETLIST_NAMING_CONFLICT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "atom_netlist.h"
#include "command.h"
#include "command_context.h"
#include "vpr_netlist_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

size_t detect_netlist_naming_conflict(const AtomNetlist& atom_netlist,
                                      const std::string& sensitive_chars);

void fix_netlist_naming_conflict(const AtomNetlist& atom_netlist,
                                 const std::string& sensitive_chars,
                                 const std::string& fix_chars,
                                 VprNetlistAnnotation& vpr_netlist_annotation);

void print_netlist_naming_fix_report(
  const std::string& fname, const AtomNetlist& atom_netlist,
  const VprNetlistAnnotation& vpr_netlist_annotation);

} /* end namespace openfpga */

#endif
