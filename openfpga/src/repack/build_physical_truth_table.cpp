/***************************************************************************************
 * This file includes functions that are used to build truth tables of
 * the physical implementation of LUTs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "openfpga_naming.h"

#include "lut_utils.h"
#include "physical_pb.h"
#include "build_physical_truth_table.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Identify if LUT is used as wiring 
 * In this case, LUT functions as a buffer
 *         +------+
 *  in0 -->|---   |
 *         |   \  |
 *  in1 -->|    --|--->out
 *  ...
 *
 *  Note that this function judge the LUT operating mode from the input nets and output
 *  nets that are mapped to inputs and outputs. 
 *  If the output net appear in the list of input nets, this LUT is used as a wire 
 ***************************************************************************************/
static 
bool is_wired_lut(const std::vector<AtomNetId>& input_nets,
                  const AtomNetId& output_net) {
  for (const AtomNetId& input_net : input_nets) {
    if (input_net == output_net) {
      return true;
    }
  }
  
  return false;
}

/***************************************************************************************
 * Create pin rotation map for a LUT
 ***************************************************************************************/
static 
std::vector<int> generate_lut_rotated_input_pin_map(const std::vector<AtomNetId>& input_nets,
                                                    const AtomContext& atom_ctx,
                                                    const AtomBlockId& atom_blk,
                                                    const t_pb_graph_node* pb_graph_node) { 
  /* Find the pin rotation status and record it ,
   * Note that some LUT inputs may not be used, we set them to be open by default
   */
  std::vector<int> rotated_pin_map(input_nets.size(), -1);

  VTR_ASSERT(1 == pb_graph_node->num_input_ports);
  for (int ipin = 0; ipin < pb_graph_node->num_input_pins[0]; ++ipin) {
    /* Port exists (some LUTs may have no input and hence no port in the atom netlist) */
    AtomPortId atom_port = atom_ctx.nlist.find_atom_port(atom_blk, pb_graph_node->input_pins[0][ipin].port->model_port);
    if (!atom_port) { 
      continue;
    }

    for (AtomPinId atom_pin : atom_ctx.nlist.port_pins(atom_port)) {
      AtomNetId atom_pin_net = atom_ctx.nlist.pin_net(atom_pin);
      if (atom_pin_net == input_nets[ipin]) {
        rotated_pin_map[ipin] = atom_ctx.nlist.pin_port_bit(atom_pin);
        break;
      }
    }
  }
  return rotated_pin_map;
}

/***************************************************************************************
 * This function will iterate over all the inputs and outputs of the LUT pb 
 * and find truth tables that are mapped to each output pins
 * Note that a physical LUT may have multiple truth tables to be considered
 * as they may be fracturable
 ***************************************************************************************/
static 
void build_physical_pb_lut_truth_tables(PhysicalPb& physical_pb,
                                        const PhysicalPbId& lut_pb_id,
                                        const AtomContext& atom_ctx,
                                        const VprDeviceAnnotation& device_annotation,
                                        const CircuitLibrary& circuit_lib) {
  const t_pb_graph_node* pb_graph_node = physical_pb.pb_graph_node(lut_pb_id);

  CircuitModelId lut_model = device_annotation.pb_type_circuit_model(physical_pb.pb_graph_node(lut_pb_id)->pb_type);
  VTR_ASSERT(CIRCUIT_MODEL_LUT == circuit_lib.model_type(lut_model));

  /* Find all the nets mapped to each inputs */
  std::vector<AtomNetId> input_nets;
  VTR_ASSERT(1 == pb_graph_node->num_input_ports);
  for (int ipin = 0; ipin < pb_graph_node->num_input_pins[0]; ++ipin) {
    input_nets.push_back(physical_pb.pb_graph_pin_atom_net(lut_pb_id, &(pb_graph_node->input_pins[0][ipin]))); 
  }

  /* Find all the nets mapped to each outputs */
  for (int iport = 0; iport < pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_output_pins[iport]; ++ipin) {
      const t_pb_graph_pin* output_pin = &(pb_graph_node->output_pins[iport][ipin]);
      AtomNetId output_net = physical_pb.pb_graph_pin_atom_net(lut_pb_id, output_pin); 
      /* Bypass unmapped pins */
      if (AtomNetId::INVALID() == output_net) {
        continue;
      }
      /* Check if this is a LUT used as wiring */
      if (true == is_wired_lut(input_nets, output_net)) {
        AtomNetlist::TruthTable wire_tt = build_wired_lut_truth_table(input_nets.size(), std::find(input_nets.begin(), input_nets.end(), output_net) - input_nets.begin()); 
        physical_pb.set_truth_table(lut_pb_id, output_pin, wire_tt);
        continue;
      }

      /* Find the truth table from atom block which drives the atom net */
      const AtomBlockId& atom_blk = atom_ctx.nlist.net_driver_block(output_net); 
      VTR_ASSERT(true == atom_ctx.nlist.valid_block_id(atom_blk));
      const AtomNetlist::TruthTable& orig_tt = atom_ctx.nlist.block_truth_table(atom_blk);

      std::vector<int> rotated_pin_map = generate_lut_rotated_input_pin_map(input_nets, atom_ctx, atom_blk, pb_graph_node);
      const AtomNetlist::TruthTable& adapt_tt = lut_truth_table_adaption(orig_tt, rotated_pin_map);

      /* Adapt the truth table for fracturable lut implementation and add to physical pb */
      CircuitPortId lut_model_output_port = device_annotation.pb_circuit_port(output_pin->port);
      size_t lut_frac_level = circuit_lib.port_lut_frac_level(lut_model_output_port);
      if (size_t(-1) == lut_frac_level) {
        lut_frac_level = input_nets.size();
      }
      size_t lut_output_mask = circuit_lib.port_lut_output_mask(lut_model_output_port)[output_pin->pin_number];
      const AtomNetlist::TruthTable& frac_lut_tt = adapt_truth_table_for_frac_lut(lut_frac_level, lut_output_mask, adapt_tt);
      physical_pb.set_truth_table(lut_pb_id, output_pin, frac_lut_tt);
    }
  }
}

/***************************************************************************************
 * This function will iterate over all the physical pb that are
 * binded to clustered blocks and build the truth tables for the
 * physical Look-Up Table (LUT) implementations.
 * Note that the truth table built here is different from the atom
 * netlists in VPR context. We consider fracturable LUT features
 * and LUTs operating as wires
 ***************************************************************************************/
void build_physical_lut_truth_tables(VprClusteringAnnotation& cluster_annotation,
                                     const AtomContext& atom_ctx,
                                     const ClusteringContext& cluster_ctx,
                                     const VprDeviceAnnotation& device_annotation,
                                     const CircuitLibrary& circuit_lib) {
  vtr::ScopedStartFinishTimer timer("Build truth tables for physical LUTs");

  for (auto blk_id : cluster_ctx.clb_nlist.blocks()) {
    PhysicalPb& physical_pb = cluster_annotation.mutable_physical_pb(blk_id);
    /* Find the LUT physical pb id */
    for (const PhysicalPbId& primitive_pb : physical_pb.primitive_pbs()) {
      CircuitModelId circuit_model = device_annotation.pb_type_circuit_model(physical_pb.pb_graph_node(primitive_pb)->pb_type);
      VTR_ASSERT(true == circuit_lib.valid_model_id(circuit_model));
      if (CIRCUIT_MODEL_LUT != circuit_lib.model_type(circuit_model)) {
        continue;
      }
    
      /* Reach here, we have a LUT to deal with. Find the truth tables that mapped to the LUT */
      build_physical_pb_lut_truth_tables(physical_pb, primitive_pb, atom_ctx, device_annotation, circuit_lib);
    }
  }
}

} /* end namespace openfpga */
