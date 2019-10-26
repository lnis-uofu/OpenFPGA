/********************************************************************
 * Header file for build_lut_bitstream.cpp
 ********************************************************************/
#ifndef BUILD_LUT_BITSTREAM_H
#define BUILD_LUT_BITSTREAM_H

#include <vector>
#include "circuit_library.h"
#include "mux_graph.h"
#include "vpr_types.h"

/* Alias name for data structure of LUT truth table */
typedef std::vector<std::string> LutTruthTable;

/* Declaration for functions */
LutTruthTable build_post_routing_wired_lut_truth_table(const int& lut_output_vpack_net_num,
                                                       const size_t& lut_size, 
                                                       const std::vector<int>& lut_pin_vpack_net_num);

LutTruthTable build_post_routing_lut_truth_table(t_logical_block* mapped_logical_block,
                                                 const size_t& lut_size, 
                                                 const std::vector<int>& lut_pin_vpack_net_num);

LutTruthTable adapt_truth_table_for_frac_lut(const CircuitLibrary& circuit_lib,
                                             t_pb_graph_pin* lut_out_pb_graph_pin, 
                                             const LutTruthTable& truth_table);

std::vector<bool> build_frac_lut_bitstream(const CircuitLibrary& circuit_lib,
                                           const MuxGraph& lut_mux_graph, 
                                           t_phy_pb* lut_pb,
                                           const std::vector<LutTruthTable>& truth_tables,
                                           const size_t& default_sram_bit_value);

#endif
