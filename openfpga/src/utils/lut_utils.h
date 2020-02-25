#ifndef LUT_UTILS_H
#define LUT_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include <map>
#include "atom_netlist.h"
#include "mux_graph.h"
#include "physical_types.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

AtomNetlist::TruthTable lut_truth_table_adaption(const AtomNetlist::TruthTable& orig_tt, 
                                                 const std::vector<int>& rotated_pin_map);

std::vector<std::string> truth_table_to_string(const AtomNetlist::TruthTable& tt);

AtomNetlist::TruthTable build_post_routing_wired_lut_truth_table(const size_t& lut_size, 
                                                                 const size_t& wire_input_id);

AtomNetlist::TruthTable adapt_truth_table_for_frac_lut(const size_t& lut_frac_level,
                                                       const size_t& lut_output_mask,
                                                       const AtomNetlist::TruthTable& truth_table);

bool lut_truth_table_use_on_set(const AtomNetlist::TruthTable& truth_table);

std::vector<bool> build_frac_lut_bitstream(const CircuitLibrary& circuit_lib,
                                           const MuxGraph& lut_mux_graph, 
                                           const VprDeviceAnnotation& device_annotation, 
                                           const std::map<const t_pb_graph_pin*, AtomNetlist::TruthTable>& truth_tables,
                                           const size_t& default_sram_bit_value);

} /* end namespace openfpga */

#endif
