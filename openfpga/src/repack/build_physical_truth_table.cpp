/***************************************************************************************
 * This file includes functions that are used to build truth tables of
 * the physical implementation of LUTs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "build_physical_truth_table.h"

#include "lut_utils.h"
#include "openfpga_naming.h"
#include "pb_type_utils.h"
#include "physical_pb.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Create pin rotation map for a LUT
 ***************************************************************************************/
static std::vector<int> generate_lut_rotated_input_pin_map(
  const std::vector<AtomNetId>& input_nets, const AtomContext& atom_ctx,
  const AtomBlockId& atom_blk, const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, const t_pb_graph_node* pb_graph_node) {
  /* Find the pin rotation status and record it ,
   * Note that some LUT inputs may not be used, we set them to be open by
   * default
   */
  std::vector<int> rotated_pin_map(input_nets.size(), -1);

  for (int iport = 0; iport < pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_input_pins[iport]; ++ipin) {
      /* Skip the input pin that do not drive by LUT MUXes */
      CircuitPortId circuit_port = device_annotation.pb_circuit_port(
        pb_graph_node->input_pins[iport][ipin].port);
      if (true == circuit_lib.port_is_harden_lut_port(circuit_port)) {
        continue;
      }

      /* The lut pb_graph_node may not be the primitive node
       * because VPR adds two default modes to its LUT pb_type
       * If so, we will use the LUT mode of the pb_graph node
       */
      t_port* lut_pb_type_in_port = pb_graph_node->input_pins[iport][ipin].port;
      if (0 != pb_graph_node->pb_type->num_modes) {
        VTR_ASSERT(2 == pb_graph_node->pb_type->num_modes);
        VTR_ASSERT(1 == pb_graph_node->pb_type->modes[VPR_PB_TYPE_LUT_MODE]
                          .num_pb_type_children);
        lut_pb_type_in_port =
          &(pb_graph_node->pb_type->modes[VPR_PB_TYPE_LUT_MODE]
              .pb_type_children[0]
              .ports[iport]);
        VTR_ASSERT(
          std::string(lut_pb_type_in_port->name) ==
          std::string(pb_graph_node->input_pins[iport][ipin].port->name));
        VTR_ASSERT(lut_pb_type_in_port->num_pins ==
                   pb_graph_node->input_pins[iport][ipin].port->num_pins);
      }

      /* Port exists (some LUTs may have no input and hence no port in the atom
       * netlist) */
      AtomPortId atom_port = atom_ctx.nlist.find_atom_port(
        atom_blk, lut_pb_type_in_port->model_port);
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
  }
  return rotated_pin_map;
}

/***************************************************************************************
 * This function will iterate over all the inputs and outputs of the LUT pb
 * and find truth tables that are mapped to each output pins
 * Note that a physical LUT may have multiple truth tables to be considered
 * as they may be fracturable
 ***************************************************************************************/
static void build_physical_pb_lut_truth_tables(
  PhysicalPb& physical_pb, const PhysicalPbId& lut_pb_id,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, const bool& verbose) {
  const t_pb_graph_node* pb_graph_node = physical_pb.pb_graph_node(lut_pb_id);

  CircuitModelId lut_model = device_annotation.pb_type_circuit_model(
    physical_pb.pb_graph_node(lut_pb_id)->pb_type);
  VTR_ASSERT(CIRCUIT_MODEL_LUT == circuit_lib.model_type(lut_model));

  /* Find all the nets mapped to each inputs */
  std::vector<AtomNetId> input_nets;
  for (int iport = 0; iport < pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_input_pins[iport]; ++ipin) {
      /* Skip the input pin that do not drive by LUT MUXes */
      CircuitPortId circuit_port = device_annotation.pb_circuit_port(
        pb_graph_node->input_pins[iport][ipin].port);
      if (true == circuit_lib.port_is_harden_lut_port(circuit_port)) {
        continue;
      }
      input_nets.push_back(physical_pb.pb_graph_pin_atom_net(
        lut_pb_id, &(pb_graph_node->input_pins[iport][ipin])));
    }
  }

  /* Find all the nets mapped to each outputs */
  for (int iport = 0; iport < pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_output_pins[iport]; ++ipin) {
      const t_pb_graph_pin* output_pin =
        &(pb_graph_node->output_pins[iport][ipin]);
      /* Skip the output ports that are not driven by LUT MUXes */
      CircuitPortId circuit_port =
        device_annotation.pb_circuit_port(output_pin->port);
      if (true == circuit_lib.port_is_harden_lut_port(circuit_port)) {
        continue;
      }

      AtomNetId output_net =
        physical_pb.pb_graph_pin_atom_net(lut_pb_id, output_pin);
      /* Bypass unmapped pins */
      if (AtomNetId::INVALID() == output_net) {
        continue;
      }
      /* Check if this is a LUT used as wiring */
      AtomNetlist::TruthTable adapt_tt;
      if (true == physical_pb.is_wire_lut_output(lut_pb_id, output_pin)) {
        /* Double check: ensure that the output nets appear in the input net !!!
         */
        if (!is_wired_lut(input_nets, output_net)) {
          VTR_LOGV(verbose, "Pb id: \'%lu\', output pin: \'%s\'\n",
                   size_t(lut_pb_id), output_pin->to_string().c_str());
          VTR_LOGV(verbose, "Input nets:\n");
          for (auto input_net : input_nets) {
            if (AtomNetId::INVALID() == input_net) {
              VTR_LOGV(verbose, "\tunconn\n");
            } else {
              VTR_ASSERT(AtomNetId::INVALID() != input_net);
              VTR_LOGV(verbose, "\t%s\n",
                       atom_ctx.nlist.net_name(input_net).c_str());
            }
          }
          VTR_LOGV(verbose, "Output nets:\n");
          VTR_LOGV(verbose, "\t%s\n",
                   atom_ctx.nlist.net_name(output_net).c_str());
        }
        VTR_ASSERT(true == is_wired_lut(input_nets, output_net));
        adapt_tt = build_wired_lut_truth_table(
          input_nets.size(),
          std::find(input_nets.begin(), input_nets.end(), output_net) -
            input_nets.begin());
      } else {
        /* Find the truth table from atom block which drives the atom net */
        const AtomBlockId& atom_blk =
          atom_ctx.nlist.net_driver_block(output_net);
        VTR_ASSERT(true == atom_ctx.nlist.valid_block_id(atom_blk));
        const AtomNetlist::TruthTable& orig_tt =
          atom_ctx.nlist.block_truth_table(atom_blk);

        std::vector<int> rotated_pin_map = generate_lut_rotated_input_pin_map(
          input_nets, atom_ctx, atom_blk, device_annotation, circuit_lib,
          pb_graph_node);
        adapt_tt = lut_truth_table_adaption(orig_tt, rotated_pin_map);

        VTR_LOGV(verbose, "Driver atom block: '%ld'\n", size_t(atom_blk));
        VTR_LOGV(verbose, "Pb atom blocks:");
        for (const AtomBlockId& pb_atom_blk :
             physical_pb.atom_blocks(lut_pb_id)) {
          VTR_LOGV(verbose, "'%ld', ", size_t(pb_atom_blk));
        }
        VTR_LOGV(verbose, "\n");
      }

      /* Adapt the truth table for fracturable lut implementation and add to
       * physical pb */
      CircuitPortId lut_model_output_port =
        device_annotation.pb_circuit_port(output_pin->port);
      size_t lut_frac_level =
        circuit_lib.port_lut_frac_level(lut_model_output_port);
      if (size_t(-1) == lut_frac_level) {
        lut_frac_level = input_nets.size();
      }
      size_t lut_output_mask = circuit_lib.port_lut_output_mask(
        lut_model_output_port)[output_pin->pin_number];
      const AtomNetlist::TruthTable& frac_lut_tt =
        adapt_truth_table_for_frac_lut(lut_frac_level, lut_output_mask,
                                       adapt_tt);
      physical_pb.set_truth_table(lut_pb_id, output_pin, frac_lut_tt);

      /* Print debug information */
      VTR_LOGV(verbose, "Input nets: ");
      for (const AtomNetId& net : input_nets) {
        if (AtomNetId::INVALID() == net) {
          VTR_LOGV(verbose, "unconn  ");
        } else {
          VTR_ASSERT(AtomNetId::INVALID() != net);
          VTR_LOGV(verbose, "%s  ", atom_ctx.nlist.net_name(net).c_str());
        }
      }
      VTR_LOGV(verbose, "\n");

      VTR_ASSERT(AtomNetId::INVALID() != output_net);
      VTR_LOGV(verbose, "Output net: %s\n",
               atom_ctx.nlist.net_name(output_net).c_str());

      VTR_LOGV(verbose, "Truth table before adaption to fracturable LUT'\n");
      for (const std::string& tt_line : truth_table_to_string(adapt_tt)) {
        VTR_LOGV(verbose, "\t%s\n", tt_line.c_str());
      }

      VTR_LOGV(verbose, "Add following truth table to pb_graph_pin '%s[%d]'\n",
               output_pin->port->name, output_pin->pin_number);
      for (const std::string& tt_line : truth_table_to_string(frac_lut_tt)) {
        VTR_LOGV(verbose, "\t%s\n", tt_line.c_str());
      }
      VTR_LOGV(verbose, "\n");
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
void build_physical_lut_truth_tables(
  VprClusteringAnnotation& cluster_annotation, const AtomContext& atom_ctx,
  const ClusteringContext& cluster_ctx,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build truth tables for physical LUTs");

  for (auto blk_id : cluster_ctx.clb_nlist.blocks()) {
    PhysicalPb& physical_pb = cluster_annotation.mutable_physical_pb(blk_id);
    VTR_LOGV(
      verbose,
      "Build truth tables for physical LUTs under clustered block '%s'...\n",
      cluster_ctx.clb_nlist.block_name(blk_id).c_str());
    /* Find the LUT physical pb id */
    for (const PhysicalPbId& primitive_pb : physical_pb.primitive_pbs()) {
      CircuitModelId circuit_model = device_annotation.pb_type_circuit_model(
        physical_pb.pb_graph_node(primitive_pb)->pb_type);
      VTR_ASSERT(true == circuit_lib.valid_model_id(circuit_model));
      if (CIRCUIT_MODEL_LUT != circuit_lib.model_type(circuit_model)) {
        continue;
      }

      /* Reach here, we have a LUT to deal with. Find the truth tables that
       * mapped to the LUT */
      build_physical_pb_lut_truth_tables(physical_pb, primitive_pb, atom_ctx,
                                         device_annotation, circuit_lib,
                                         verbose);
    }
  }
}

} /* end namespace openfpga */
