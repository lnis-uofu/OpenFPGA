/********************************************************************
 * This file includes most utilized functions to manipulate LUTs, 
 * especially their truth tables, in the OpenFPGA context
 *******************************************************************/
#include <cmath>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_decode.h"

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
 *
 * An illustrative example: 
 *          
 *          Original Truth Table       Post VPR Truth Table
 *
 *               +-------+                  +-------+
 *      net0 --->|       |          net1--->|       |
 *      net1 --->|  LUT  |          net0--->|  LUT  |
 *           ... |       |              ... |       |
 *               +-------+                  +-------+
 *
 *         Truth table line             Truth table line
 *     .names net0 net1 out          .names net1 net0 out
 *     01 1                          10 1
 *
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

/********************************************************************
 * Adapt the truth table from the short-wire connection 
 * from the input nets of a LUT to an output of a LUT
 * 
 *                  LUT
 *                +-------------+
 *   lut_input--->|----+        |
 *                |    |        |
 *                |    +------->|---> lut_output
 *                |             |
 *                +-------------+
 *
 * In this case, LUT is configured as a wiring module
 * This function will generate a truth for the wiring LUT
 *
 * For example: 
 * The truth table of the case where the 3rd input of 
 * a 4-input LUT is wired to output 
 *    
 *    --1- 1
 * 
 ********************************************************************/
AtomNetlist::TruthTable build_wired_lut_truth_table(const size_t& lut_size, 
                                                    const size_t& wire_input_id) {
  AtomNetlist::TruthTable tt;
  
  /* There is always only one line in this truth table */
  tt.resize(1);

  /* Pre-allocate the truth table: 
   * Each truth table line is organized in BLIF format:
   *     |<---LUT size--->|
   *   < a string of 0 or 1> <0 or 1>
   * The first <lut_size> of characters represent the input values of each LUT input
   * Here, we add 2 characters, which denote the space and a digit (0|1)
   * By default, we set all the inputs as don't care value '-'
   *
   * For more details, please refer to the BLIF format documentation
   */
  tt[0].resize(lut_size, vtr::LogicValue::DONT_CARE);
  /* Fill the truth table !!! */
  VTR_ASSERT(wire_input_id < lut_size);
  tt[0][wire_input_id] = vtr::LogicValue::TRUE; 
  tt[0].push_back(vtr::LogicValue::TRUE);

  return tt;
}

/********************************************************************
 * Adapt truth table for a fracturable LUT
 * Determine fixed input bits for this truth table:
 * 1. input bits within frac_level (all '-' if not specified) 
 * 2. input bits outside frac_level, decoded to its output mask (0 -> first part -> all '1') 
 *
 * For example: 
 *   A 4-input function is mapped to input[0..3] of a 6-input fracturable LUT 
 *   Plus, it uses the 2nd output of the fracturable LUT
 *   The truth table of the 4-input function is
 *     1001 1
 *   while truth table of a 6-input LUT requires 6 characters
 *   Therefore, it must be adapted by adding mask bits, which are
 *   a number of fixed digits to configure the fracturable LUT to
 *   operate in a 4-input LUT mode
 *   The mask bits can be decoded from the index of output used in the fracturable LUT
 *   For the 2nd output, it will be '01', the binary representation of index '1'
 *   Now the truth table will be adapt to
 *     100101 1
 *   where the first 4 digits come from the original truth table
 *   the 2 following digits are mask bits
 *   
 ********************************************************************/
AtomNetlist::TruthTable adapt_truth_table_for_frac_lut(const size_t& lut_frac_level,
                                                       const size_t& lut_output_mask,
                                                       const AtomNetlist::TruthTable& truth_table) {
  /* No adaption required for when the lut_frac_level is not set */
  if (size_t(OPEN) == lut_frac_level) {
    return truth_table;
  }

  AtomNetlist::TruthTable adapt_truth_table;

  /* Apply modification to the truth table */
  for (const std::vector<vtr::LogicValue>& tt_line : truth_table) {
    /* Last element is the output */
    size_t lut_size = tt_line.size() - 1;
    /* Get the number of bits to be masked (modified) */
    int num_mask_bits = lut_size - lut_frac_level;
    /* Check if we need to modify any bits */
    VTR_ASSERT(0 <= num_mask_bits);
    if ( 0 == num_mask_bits ) {
      /* No modification needed, push to adapted truth table */
      adapt_truth_table.push_back(tt_line);
      continue;
    }
    /* Modify bits starting from lut_frac_level */
    /* Decode the lut_output_mask to LUT input codes */ 
    int temp = pow(2., num_mask_bits) - 1 - lut_output_mask;
    VTR_ASSERT(0 <= temp);
    std::vector<size_t> mask_bits_vec = itobin_vec(temp, num_mask_bits);
    /* Copy the bits to the truth table line */
    std::vector<vtr::LogicValue> adapt_tt_line = tt_line;
    for (size_t itt = lut_frac_level; itt < lut_frac_level + mask_bits_vec.size(); ++itt) { 
      
      vtr::LogicValue logic_val = vtr::LogicValue::FALSE;
      VTR_ASSERT( (1 == mask_bits_vec[itt - lut_frac_level])
               || (0 == mask_bits_vec[itt - lut_frac_level]) );
      if (1 == mask_bits_vec[itt - lut_frac_level]) {
        logic_val = vtr::LogicValue::TRUE;
      }
      adapt_tt_line[itt] = logic_val;
    }

    /* Push to adapted truth table */
    adapt_truth_table.push_back(adapt_tt_line);
  }

  return adapt_truth_table;
}

/********************************************************************
 * Determine if the truth table of a LUT is a on-set or a off-set
 *  - An on-set is defined as the truth table lines are ended with logic '1'
 *  - An off-set is defined as the truth table lines are ended with logic '0'
 *******************************************************************/
bool lut_truth_table_use_on_set(const AtomNetlist::TruthTable& truth_table) {
  bool on_set = false;
  bool off_set = false;

  for (const std::vector<vtr::LogicValue>& tt_line : truth_table) {
    switch (tt_line.back()) {
    case vtr::LogicValue::TRUE :
      on_set = true;
      break;
    case vtr::LogicValue::FALSE :
      off_set = true;
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__, 
                     "Invalid truth_table_line ending '%s'!\n",
                     vtr::LOGIC_VALUE_STRING[size_t(tt_line.back())]);
      exit(1);
    }
  }

  /* Prefer on_set if both are true */
  if (true == on_set && true == off_set) {
    on_set = true; 
    off_set = false;
  }
  VTR_ASSERT(on_set == !off_set);

  return on_set;
}

/********************************************************************
 * Complete a line in truth table with a given lut size
 * Due to the size of truth table may be less than the lut size.
 * i.e. in LUT-6 architecture, there exists LUT1-6 in technology-mapped netlists
 * So, in truth table line, there may be 10- 1
 * In this case, we should complete it by --10- 1
 *******************************************************************/
static 
std::vector<vtr::LogicValue> complete_truth_table_line(const size_t& lut_size,
                                                       const std::vector<vtr::LogicValue>& tt_line) {
  std::vector<vtr::LogicValue> ret;

  VTR_ASSERT(0 < tt_line.size());

  /* Complete the truth table line*/
  size_t cover_len = tt_line.size() - 1; 
  VTR_ASSERT(cover_len <= lut_size);

  /* Copy the original truth table line */ 
  ret = tt_line;
  /* Kick out the last value for now as it is the output value */
  ret.pop_back();

  /* Add the number of '-' we should add in the back !!! */
  for (size_t j = cover_len; j < lut_size; ++j) {
    ret.push_back(vtr::LogicValue::DONT_CARE);
  }

  /* Copy the original truth table line */ 
  ret.push_back(tt_line.back());

  /* Check if the size of ret matches our expectation */
  VTR_ASSERT(lut_size + 1 == ret.size());
  
  return ret;
}

/********************************************************************
 * For each lut_bit_lines, we should recover the truth table,
 * and then set the sram bits to "1" if the truth table defines so.
 * Start_point: the position we start converting don't care sign '-'
 *              to explicit '0' or '1'
 *******************************************************************/
static 
void rec_build_lut_bitstream_per_line(std::vector<bool>& lut_bitstream, 
                                      const size_t& lut_size,
                                      const std::vector<vtr::LogicValue>& tt_line,
                                      const size_t& start_point) {
  std::vector<vtr::LogicValue> temp_line = tt_line;

  /* Check the length of sram bits and truth table line */
  VTR_ASSERT(lut_size + 1 == tt_line.size()); /* lut_size + '1|0' */

  /* End of truth_table_line should be "space" and "1" */ 
  VTR_ASSERT( (vtr::LogicValue::TRUE == tt_line.back())
           || (vtr::LogicValue::FALSE == tt_line.back()) );

  /* Make sure before start point there is no '-' */
  VTR_ASSERT(start_point < tt_line.size());
  for (size_t i = 0; i < start_point; ++i) {
    VTR_ASSERT(vtr::LogicValue::DONT_CARE != tt_line[i]);
  }

  /* Configure sram bits recursively */
  for (size_t i = start_point; i < lut_size; ++i) {
    if (vtr::LogicValue::DONT_CARE == tt_line[i]) {
      /* if we find a dont_care, we don't do configure now but recursively*/
      /* '0' branch */
      temp_line[i] = vtr::LogicValue::FALSE; 
      rec_build_lut_bitstream_per_line(lut_bitstream, lut_size, temp_line, start_point + 1);
      /* '1' branch */
      temp_line[i] = vtr::LogicValue::TRUE; 
      rec_build_lut_bitstream_per_line(lut_bitstream, lut_size, temp_line, start_point + 1);
      return; 
    }
  }

  /* TODO: Use MuxGraph to decode this!!! */
  /* Decode bitstream only when there are only 0 or 1 in the truth table */
  size_t sram_id = 0;
  for (size_t i = 0; i < lut_size; ++i) {
    /* Should be either '0' or '1' */
    switch (tt_line[i]) {
    case vtr::LogicValue::FALSE :
      /* We assume the 1-lut pass sram1 when input = 0 */
      sram_id += (size_t)pow(2., (double)(i));
      break;
    case vtr::LogicValue::TRUE :
      /* We assume the 1-lut pass sram0 when input = 1 */
      break;
    case vtr::LogicValue::DONT_CARE :
    default :
      VTR_LOGF_ERROR(__FILE__, __LINE__, 
                     "Invalid truth_table bit '%s', should be [0|1|]!\n",
                     vtr::LOGIC_VALUE_STRING[size_t(tt_line[i])]); 
      exit(1);
    }
  }
  /* Set the sram bit to '1'*/
  VTR_ASSERT(sram_id < lut_bitstream.size());
  if (vtr::LogicValue::TRUE == tt_line.back()) {
    lut_bitstream[sram_id] = true; /* on set*/
  } else if (vtr::LogicValue::FALSE == tt_line.back()) {
    lut_bitstream[sram_id] = false; /* off set */
  } else {
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "Invalid truth_table_line ending '%s'!\n",
                   vtr::LOGIC_VALUE_STRING[size_t(tt_line.back())]);
    exit(1);
  }
}

/********************************************************************
 * Generate the bitstream for a single-output LUT with a given truth table
 * As truth tables may come from different logic blocks, truth tables could be in on and off sets
 * We first build a base SRAM bits, where different parts are set to tbe on/off sets 
 * Then, we can decode SRAM bits as regular process 
 *******************************************************************/
static 
std::vector<bool> build_single_output_lut_bitstream(const AtomNetlist::TruthTable& truth_table,
                                                    const MuxGraph& lut_mux_graph,
                                                    const size_t& default_sram_bit_value) {
  size_t lut_size = lut_mux_graph.num_memory_bits();
  size_t bitstream_size = lut_mux_graph.num_inputs();
  std::vector<bool> lut_bitstream(bitstream_size, false);
  AtomNetlist::TruthTable completed_truth_table;
  bool on_set = false;
  bool off_set = false;

  /* if No truth_table, do default*/
  if (0 == truth_table.size()) {
    switch (default_sram_bit_value) {
    case 0:
      on_set = true;
      off_set = false;
      break;
    case 1:
      on_set = false;
      off_set = true;
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid default_signal_init_value '%lu'!\n",
                     default_sram_bit_value);
      exit(1);
    }
  } else {
    on_set = lut_truth_table_use_on_set(truth_table);
    off_set = !on_set;
  }

  /* Read in truth table lines, decode one by one */
  for (const std::vector<vtr::LogicValue>& tt_line : truth_table) {
    /* Complete the truth table line by line*/
    completed_truth_table.push_back(complete_truth_table_line(lut_size, tt_line));
  }

  /* Initial all the bits in the bitstream */
  if (true == off_set) {
    /* By default, the lut_bitstream is initialize for on_set
     * For off set, it should be flipped
     */
    lut_bitstream.clear(); 
    lut_bitstream.resize(bitstream_size, true);
  }

  for (const std::vector<vtr::LogicValue>& tt_line : completed_truth_table) {
    /* Update the truth table, sram_bits */
    rec_build_lut_bitstream_per_line(lut_bitstream, lut_size, tt_line, 0);
  }

  return lut_bitstream;
}

/********************************************************************
 * Generate bitstream for a fracturable LUT (also applicable to single-output LUT)
 * Check type of truth table of each mapped logical block
 * if it is on-set, we give a all 0 base bitstream
 * if it is off-set, we give a all 1 base bitstream
 *******************************************************************/
std::vector<bool> build_frac_lut_bitstream(const CircuitLibrary& circuit_lib,
                                           const MuxGraph& lut_mux_graph, 
                                           const VprDeviceAnnotation& device_annotation, 
                                           const std::map<const t_pb_graph_pin*, AtomNetlist::TruthTable>& truth_tables,
                                           const size_t& default_sram_bit_value) {
  /* Initialization */
  std::vector<bool> lut_bitstream(lut_mux_graph.num_inputs(), default_sram_bit_value);

  for (const std::pair<const t_pb_graph_pin*, AtomNetlist::TruthTable>& element : truth_tables) {
    /* Find the corresponding circuit model output port and assoicated lut_output_mask */
    CircuitPortId lut_model_output_port = device_annotation.pb_circuit_port(element.first->port);
    size_t lut_frac_level = circuit_lib.port_lut_frac_level(lut_model_output_port);
    /* By default, lut_frac_level will be the lut_size, i.e., number of levels of the mux graph */
    if (size_t(-1) == lut_frac_level) {
      lut_frac_level = lut_mux_graph.num_levels();
    }

    /* Find the corresponding circuit model output port and assoicated lut_output_mask */
    size_t lut_output_mask = circuit_lib.port_lut_output_mask(lut_model_output_port)[element.first->pin_number];

    /* Decode lut sram bits */
    std::vector<bool> temp_bitstream = build_single_output_lut_bitstream(element.second, lut_mux_graph, default_sram_bit_value); 

    /* Depending on the frac-level, we get the location(starting/end points) of sram bits */
    size_t length_of_temp_bitstream_to_copy = (size_t)pow(2., (double)(lut_frac_level)); 
    size_t bitstream_offset = length_of_temp_bitstream_to_copy * lut_output_mask; 
    /* Ensure the offset is in range */        
    VTR_ASSERT(bitstream_offset < lut_bitstream.size());
    VTR_ASSERT(bitstream_offset + length_of_temp_bitstream_to_copy <= lut_bitstream.size());

    /* Print debug information 
    bool verbose = true;
    VTR_LOGV(verbose, "Full truth table\n");
    for (const std::string& tt_line : truth_table_to_string(element.second)) {
      VTR_LOGV(verbose, "\t%s\n", tt_line.c_str());
    }
    VTR_LOGV(verbose, "\n");

    VTR_LOGV(verbose, "Bitstream (size = %ld)\n", temp_bitstream.size());
    for (const bool& bit : temp_bitstream) {
      if (true == bit) {
        VTR_LOGV(verbose, "1");
      } else {
        VTR_ASSERT(false == bit);
        VTR_LOGV(verbose, "0");
      }
    }
    VTR_LOGV(verbose, "\n");

    VTR_LOGV(verbose, "Bitstream offset = %d\n", bitstream_offset);
    VTR_LOGV(verbose, "Bitstream length to be used = %d\n", length_of_temp_bitstream_to_copy);
     */

    /* Copy to the segment of bitstream */
    for (size_t bit = bitstream_offset; bit < bitstream_offset + length_of_temp_bitstream_to_copy; ++bit) {
      lut_bitstream[bit] = temp_bitstream[bit];
    }
  }

  return lut_bitstream; 
}


} /* end namespace openfpga */
