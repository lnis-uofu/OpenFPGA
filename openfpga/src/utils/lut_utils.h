#ifndef LUT_UTILS_H
#define LUT_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "atom_netlist.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

AtomNetlist::TruthTable lut_truth_table_adaption(const AtomNetlist::TruthTable& orig_tt, 
                                                 const std::vector<int>& rotated_pin_map);

std::vector<std::string> truth_table_to_string(const AtomNetlist::TruthTable& tt);

} /* end namespace openfpga */

#endif
