/*********************************************************************
 * This file includes functions that are used for building bitstreams
 * for Look-Up Tables
 ********************************************************************/
#include <cmath>
#include <cstring>

#include "vtr_assert.h"
#include "util.h"
#include "vpr_types.h"
#include "string_token.h"

#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"

#include "build_lut_bitstream.h"

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
LutTruthTable build_post_routing_wired_lut_truth_table(const int& lut_output_vpack_net_num,
                                                       const size_t& lut_size, 
                                                       const std::vector<int>& lut_pin_vpack_net_num) {
  LutTruthTable tt;
  
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
  tt[0].resize(lut_size, '-');
  /* Fill the truth table !!! */
  for (size_t inet = 0; inet < lut_size; ++inet) {
    /* Find the vpack_num in the lut_input_pin, we fix it to be 1 */
    if (lut_output_vpack_net_num == lut_pin_vpack_net_num[inet]) {
      tt[0][inet] = '1'; 
    }
  }
  tt[0] += std::string(" 1");

  return tt;
}

/********************************************************************
 * Provide the truth table of a mapped logical block 
 * 1. Reorgainze the truth table to be consistent with the mapped nets of a LUT
 * 2. Allocate the truth table in a clean string and return
 ********************************************************************/
LutTruthTable build_post_routing_lut_truth_table(t_logical_block* mapped_logical_block,
                                                 const size_t& lut_size, 
                                                 const std::vector<int>& lut_pin_vpack_net_num) {
  if (NULL == mapped_logical_block) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, [LINE%d]) Invalid mapped_logical_block!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Create a map between the lut net ids and logical block net ids */
  std::vector<int> lut_to_lb_net_mapping(lut_size, OPEN);
  /* Find nets mapped to a logical block */
  std::vector<int> lb_pin_vpack_net_num = find_lut_logical_block_input_pin_vpack_net_num(mapped_logical_block);
  /* Create a pin-to-pin net_num mapping */
  for (size_t inet = 0; inet < lut_size; ++inet) {
    /* Bypass open nets */
    if (OPEN == lut_pin_vpack_net_num[inet]) {
      continue;
    }
    VTR_ASSERT_SAFE (OPEN != lut_pin_vpack_net_num[inet]);
    /* Find the position (offset) of each vpack_net_num in lb_pins */
    for (size_t jnet = 0; jnet < lb_pin_vpack_net_num.size(); ++jnet) {
      if (lut_pin_vpack_net_num[inet] == lb_pin_vpack_net_num[jnet]) {
        lut_to_lb_net_mapping[inet] = jnet; 
        break;
      }  
    } 
    /* Not neccesary to find a one, some luts just share part of their pins  */ 
  } 

  /* Count the lines of truth table stored in the mapped logical block */
  struct s_linked_vptr* head = mapped_logical_block->truth_table;

  /* Convert the truth_tables stored in the mapped logical block */
  LutTruthTable truth_table;

  /* Handle the truth table pin remapping 
   * Note that we cannot simply copy the original truth table from the mapped logical block
   * Due to the logic equivalence of LUT pins, the nets are not longer in the sequences   
   * that are defined in the original truth table
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
   */  
  while (head) {
    /* Cache a line of truth table */
    std::string tt_line;

    /* Reorganize the original truth table */
    for (size_t inet = 0; inet < lut_size; ++inet) {
      /* Open net implies a don't care, or some nets are not in the list  */
      if ( (OPEN == lut_pin_vpack_net_num[inet]) 
        || (OPEN == lut_to_lb_net_mapping[inet])) {
        tt_line.push_back('-');
        continue;
      }
      /* Find the desired truth table bit */
      tt_line.push_back(((char*)(head->data_vptr))[lut_to_lb_net_mapping[inet]]);
    }

    /* Copy the last two characters from original truth table, which is not changed even after VPR implementation */
    int lb_truth_table_size = strlen((char*)(head->data_vptr));
    tt_line += std::string((char*)(head->data_vptr) + lb_truth_table_size - 2, 2);

    /* Add the line to truth table */
    truth_table.push_back(tt_line);

    /* Go to next line */
    head = head->next;
  }

  return truth_table;
} 


/********************************************************************
 * Generate the mask bits for a truth table 
 * This function actually converts an integer to a binary vector 
 *******************************************************************/
static 
std::string generate_mask_bits(const size_t& mask_code, 
                               const size_t& num_mask_bits) {
  std::vector<size_t> mask_bits(num_mask_bits, 0);

  /* Make sure we do not have any overflow! */
  VTR_ASSERT ( (mask_code < pow(2., num_mask_bits)) );
  
  size_t temp = mask_code;
  for (size_t i = 0; i < num_mask_bits; ++i) {
    if (1 == temp % 2) { 
      mask_bits[i] = 1; /* Keep a good sequence of bits */
    }
    temp = temp / 2;
  }

  std::string mask_bits_str;
  for (const size_t& mask_bit : mask_bits) {
    VTR_ASSERT( 0 == mask_bit || 1 == mask_bit );
    if (0 == mask_bit) {
      mask_bits_str.push_back('1');
      continue;
    }
    mask_bits_str.push_back('0');
  }
 
  return mask_bits_str;
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
LutTruthTable adapt_truth_table_for_frac_lut(const CircuitLibrary& circuit_lib,
                                             t_pb_graph_pin* lut_out_pb_graph_pin, 
                                             const LutTruthTable& truth_table) {
  LutTruthTable adapt_truth_table;

  /* Find the output port of LUT that this logical block is mapped to */
  VTR_ASSERT(NULL != lut_out_pb_graph_pin);
  /* find the corresponding SPICE model output port and assoicated lut_output_mask */
  CircuitPortId lut_model_output_port = lut_out_pb_graph_pin->port->circuit_model_port;
  size_t lut_frac_level = circuit_lib.port_lut_frac_level(lut_model_output_port);

  /* No adaption required for when the lut_frac_level is not set */
  if (size_t(OPEN) == lut_frac_level) {
    return truth_table;
  }

  /* Find the corresponding circuit model output port and assoicated lut_output_mask */
  size_t lut_output_mask = circuit_lib.port_lut_output_masks(lut_model_output_port)[lut_out_pb_graph_pin->pin_number];

  /* Apply modification to the truth table */
  for (const std::string& tt_line : truth_table) {
    /* Last two chars are fixed */
    size_t lut_size = tt_line.length() - 2;
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
    std::string mask_bits_str = generate_mask_bits(temp, num_mask_bits);
    /* Copy the bits to the truth table line */
    std::string adapt_tt_line = tt_line;
    adapt_tt_line.replace(lut_frac_level, mask_bits_str.size(), mask_bits_str);

    /* Push to adapted truth table */
    adapt_truth_table.push_back(adapt_tt_line);
  }

  return adapt_truth_table;
}

/********************************************************************
 * Determine if the truth table of a LUT is a on-set or a off-set
 *******************************************************************/
static 
bool lut_truth_table_use_on_set(const LutTruthTable& truth_table) {
  bool on_set = false;
  bool off_set = false;

  for (const std::string& tt_line : truth_table) {
    switch (tt_line.back()) {
    case '1':
      on_set = true;
      break;
    case '0':
      off_set = true;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s,[LINE%d])Invalid truth_table_line ending(=%c)!\n",
                 __FILE__, __LINE__, tt_line.back());
      exit(1);
    }
  }

  /* Prefer on_set if both are true */
  if (true == on_set && true == off_set) {
    on_set = true; 
    off_set = false;
  }

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
std::string complete_truth_table_line(const size_t& lut_size,
                                      const std::string& input_truth_table_line) {
  std::string ret;

  /* Split one line of truth table line*/
  StringToken string_tokenizer(input_truth_table_line);
  std::vector<std::string> tokens = string_tokenizer.split(' ');
  /* Check, only 2 tokens*/
  /* Sometimes, the truth table is ' 0' or ' 1', which corresponds to a constant */
  if (1 == tokens.size()) {
    /* restore the token[0]*/
    tokens.insert(tokens.begin(), std::string("-"));
  }

  /* After processing, there should be 2 tokens. */
  VTR_ASSERT(2 == tokens.size());

  /* Complete the truth table line*/
  size_t cover_len = tokens[0].length(); 
  VTR_ASSERT( (cover_len < lut_size) || (cover_len == lut_size) );

  /* Copy the original truth table line */ 
  ret = tokens[0];

  /* Add the number of '-' we should add in the back !!! */
  for (size_t j = cover_len; j < lut_size; ++j) {
    ret.push_back('-');
  }

  /* Copy the original truth table line */ 
  ret.push_back(' ');
  ret.append(tokens[1]);

  /* Check if the size of ret matches our expectation */
  VTR_ASSERT(lut_size + 2 == ret.size());
  
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
                                      const std::string& truth_table_line,
                                      const size_t& start_point) {
  std::string temp_line(truth_table_line);

  /* Check the length of sram bits and truth table line */
  VTR_ASSERT(lut_size + 2 == truth_table_line.length()); /* lut_size + space + '1' */

  /* End of truth_table_line should be "space" and "1" */ 
  VTR_ASSERT( (0 == truth_table_line.compare(truth_table_line.length() - 2, 2, " 1"))
           || (0 == truth_table_line.compare(truth_table_line.length() - 2, 2, " 0")) );

  /* Make sure before start point there is no '-' */
  VTR_ASSERT(start_point < truth_table_line.length());
  for (size_t i = 0; i < start_point; ++i) {
    VTR_ASSERT('-' != truth_table_line[i]);
  }

  /* Configure sram bits recursively */
  for (size_t i = start_point; i < lut_size; ++i) {
    if ('-' == truth_table_line[i]) {
      /* if we find a dont_care, we don't do configure now but recursively*/
      /* '0' branch */
      temp_line[i] = '0'; 
      rec_build_lut_bitstream_per_line(lut_bitstream, lut_size, temp_line, start_point + 1);
      /* '1' branch */
      temp_line[i] = '1'; 
      rec_build_lut_bitstream_per_line(lut_bitstream, lut_size, temp_line, start_point + 1);
      return; 
    }
  }

  /* TODO: Use MuxGraph to decode this!!! */
  /* Decode bitstream only when there are only 0 or 1 in the truth table */
  size_t sram_id = 0;
  for (size_t i = 0; i < lut_size; ++i) {
    /* Should be either '0' or '1' */
    switch (truth_table_line[i]) {
    case '0':
      /* We assume the 1-lut pass sram1 when input = 0 */
      sram_id += (size_t)pow(2., (double)(i));
      break;
    case '1':
      /* We assume the 1-lut pass sram0 when input = 1 */
      break;
    case '-':
    default :
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d]) Invalid truth_table bit(%c), should be [0|1|]!\n",
                 __FILE__, __LINE__, truth_table_line[i]); 
      exit(1);
    }
  }
  /* Set the sram bit to '1'*/
  VTR_ASSERT(sram_id < lut_bitstream.size());
  if (0 == truth_table_line.compare(truth_table_line.length() - 2, 2, " 1")) {
    lut_bitstream[sram_id] = true; /* on set*/
  } else if (0 == truth_table_line.compare(truth_table_line.length() - 2, 2, " 0")) {
    lut_bitstream[sram_id] = false; /* off set */
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid truth_table_line ending(=%s)!\n",
               __FILE__, __LINE__, truth_table_line.substr(lut_size, 2));
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
std::vector<bool> build_single_output_lut_bitstream(const LutTruthTable& truth_table,
                                                    const MuxGraph& lut_mux_graph,
                                                    const size_t& default_sram_bit_value) {
  size_t lut_size = lut_mux_graph.num_memory_bits();
  size_t bitstream_size = lut_mux_graph.num_inputs();
  std::vector<bool> lut_bitstream(bitstream_size, false);
  LutTruthTable completed_truth_table;
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
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(File:%s,[LINE%d]) Invalid default_signal_init_value(=%lu)!\n",
                 __FILE__, __LINE__, default_sram_bit_value);
      exit(1);
    }
  } else {
    on_set = lut_truth_table_use_on_set(truth_table);
    off_set = !on_set;
  }

  /* Read in truth table lines, decode one by one */
  for (const std::string& tt_line : truth_table) {
    /* Complete the truth table line by line*/
    completed_truth_table.push_back(complete_truth_table_line(lut_size, tt_line));
  }

  /* Initial all the bits in the bitstream */
  if (true == on_set) {
    lut_bitstream.resize(bitstream_size, false);
  } else if (true == off_set) {
    lut_bitstream.resize(bitstream_size, true);
  }

  for (const std::string& tt_line : completed_truth_table) {
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
                                           t_phy_pb* lut_pb,
                                           const std::vector<LutTruthTable>& truth_tables,
                                           const size_t& default_sram_bit_value) {
  /* Initialization */
  std::vector<bool> lut_bitstream(lut_mux_graph.num_inputs(), default_sram_bit_value);

  for (int ilb = 0; ilb < lut_pb->num_logical_blocks; ++ilb) {
    /* Find the corresponding circuit model output port and assoicated lut_output_mask */
    CircuitPortId lut_model_output_port = lut_pb->lut_output_pb_graph_pin[ilb]->port->circuit_model_port;
    size_t lut_frac_level = circuit_lib.port_lut_frac_level(lut_model_output_port);

    /* Find the corresponding circuit model output port and assoicated lut_output_mask */
    size_t lut_output_mask = circuit_lib.port_lut_output_masks(lut_model_output_port)[lut_pb->lut_output_pb_graph_pin[ilb]->pin_number];

    /* Decode lut sram bits */
    std::vector<bool> temp_bitstream = build_single_output_lut_bitstream(truth_tables[ilb], lut_mux_graph, default_sram_bit_value); 

    /* Depending on the frac-level, we get the location(starting/end points) of sram bits */
    size_t length_of_temp_bitstream_to_copy = (size_t)pow(2., (double)(lut_frac_level)); 
    size_t bitstream_offset = length_of_temp_bitstream_to_copy * lut_output_mask; 
    /* Ensure the offset is in range */        
    VTR_ASSERT(bitstream_offset < lut_bitstream.size());
    VTR_ASSERT(bitstream_offset + length_of_temp_bitstream_to_copy <= lut_bitstream.size());

    /* Copy to the segment of bitstream */
    for (size_t bit = bitstream_offset; bit < bitstream_offset + length_of_temp_bitstream_to_copy; ++bit) {
      lut_bitstream[bit] = temp_bitstream[bit];
    }
  }

  return lut_bitstream; 
}
