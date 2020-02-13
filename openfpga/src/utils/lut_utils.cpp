/********************************************************************
 * This file includes most utilized functions to manipulate LUTs, 
 * especially their truth tables, in the OpenFPGA context
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "lut_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to adapt the truth table to a mapped physical LUT 
 * subject to a pin rotation map
 * The modification is applied to line by line
 *   - For unused inputs : insert dont care
 *   - For used inputs : find the bit in the truth table rows and move it by the given mapping
 *
 * The rotated pin map is the reference to adapt the truth table.
 * Each element of the map represents the input index in the original truth table
 * The sequence of the rotate pin map is the final sequence of how
 * each line of the original truth table should be shuffled
 * Example: 
 *                     output_value(we do not modify)
 *                               |
 *                               v
 *   Truth table line:       00111
 *   rotated_pin_map:        2310
 *   Adapt truth table line: 11001     
 *******************************************************************/
AtomNetlist::TruthTable lut_truth_table_adaption(const AtomNetlist::TruthTable& orig_tt, 
                                                 const std::vector<int>& rotated_pin_map) {
  AtomNetlist::TruthTable tt;

  for (auto row : orig_tt) {
    VTR_ASSERT(row.size() - 1 <= rotated_pin_map.size());

    std::vector<vtr::LogicValue> tt_line;
    /* We do not care about the last digit, which is the output value */
    for (size_t i = 0; i < rotated_pin_map.size(); ++i) {
      if (-1 == rotated_pin_map[i]) {
        tt_line.push_back(vtr::LogicValue::DONT_CARE);
      } else { 
        /* Ensure we never access the last digit, i.e., the output value! */
        VTR_ASSERT((size_t)rotated_pin_map[i] < row.size() - 1);
        tt_line.push_back(row[rotated_pin_map[i]]);
      }
    } 

    /* Do not miss the last digit in the final result */
    tt_line.push_back(row.back());
    tt.push_back(tt_line);
  }

  return tt;
} 

/********************************************************************
 * Convert a truth table to strings, which are ready to be printed out
 *******************************************************************/
std::vector<std::string> truth_table_to_string(const AtomNetlist::TruthTable& tt) { 
  std::vector<std::string> tt_str;
  for (auto row : tt) {
    std::string row_str;
    for (size_t i = 0; i < row.size(); ++i) {
      /* Add a gap between inputs and outputs */
      if (i == row.size() - 1) {
        row_str += std::string(" ");
      }
      switch (row[i]) {
        case vtr::LogicValue::TRUE:
          row_str += std::string("1");
          break;
        case vtr::LogicValue::FALSE:
          row_str += std::string("0");
          break;
        case vtr::LogicValue::DONT_CARE:
          row_str += std::string("-");
          break;
        default:
          VTR_ASSERT_MSG(false, "Valid single-output cover logic value");
      }
    }
    tt_str.push_back(row_str);
  }

  return tt_str;
}

} /* end namespace openfpga */

