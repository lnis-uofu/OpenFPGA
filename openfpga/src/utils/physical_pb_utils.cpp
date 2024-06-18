/************************************************************************
 * Function to perform fundamental operation for the physical pb using
 * data structures
 ***********************************************************************/
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "lut_utils.h"
#include "openfpga_naming.h"
#include "openfpga_tokenizer.h"
#include "pb_type_utils.h"
#include "physical_pb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Allocate an empty physical pb graph based on pb_graph
 * This function should start with an empty physical pb object!!!
 * Suggest to check this before executing this function
 *   VTR_ASSERT(true == phy_pb.empty());
 ***********************************************************************/
static void rec_alloc_physical_pb_from_pb_graph(
  PhysicalPb& phy_pb, const t_pb_graph_node* pb_graph_node,
  const VprDeviceAnnotation& device_annotation) {
  t_pb_type* pb_type = pb_graph_node->pb_type;

  t_mode* physical_mode = device_annotation.physical_mode(pb_type);

  PhysicalPbId cur_phy_pb_id = phy_pb.create_pb(pb_graph_node);
  VTR_ASSERT(true == phy_pb.valid_pb_id(cur_phy_pb_id));

  /* Finish for primitive node */
  if (true == is_primitive_pb_type(pb_type)) {
    /* Deposite mode bits here */
    phy_pb.set_mode_bits(cur_phy_pb_id,
                         device_annotation.pb_type_mode_bits(pb_type));
    return;
  }

  /* Find the physical mode */
  VTR_ASSERT(nullptr != physical_mode);

  /* Go to the leaf nodes first. This aims to build all the primitive nodes
   * first and then we build the parents and create links
   */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb;
         ++jpb) {
      rec_alloc_physical_pb_from_pb_graph(
        phy_pb,
        &(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]),
        device_annotation);
    }
  }
}

/************************************************************************
 * Build all the relationships between parent and children
 * inside a physical pb graph
 * This function must be executed after rec_alloc_physical_pb_from_pb_graph()!!!
 ***********************************************************************/
static void rec_build_physical_pb_children_from_pb_graph(
  PhysicalPb& phy_pb, const t_pb_graph_node* pb_graph_node,
  const VprDeviceAnnotation& device_annotation) {
  t_pb_type* pb_type = pb_graph_node->pb_type;

  /* Finish for primitive node */
  if (true == is_primitive_pb_type(pb_type)) {
    return;
  }

  t_mode* physical_mode = device_annotation.physical_mode(pb_type);
  VTR_ASSERT(nullptr != physical_mode);

  /* Please use the openfpga naming function so that you can build the link to
   * module manager */
  PhysicalPbId parent_pb_id = phy_pb.find_pb(pb_graph_node);
  VTR_ASSERT(true == phy_pb.valid_pb_id(parent_pb_id));

  /* Add all the children */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb;
         ++jpb) {
      PhysicalPbId child_pb_id = phy_pb.find_pb(
        &(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]));
      VTR_ASSERT(true == phy_pb.valid_pb_id(child_pb_id));
      phy_pb.add_child(parent_pb_id, child_pb_id,
                       &(physical_mode->pb_type_children[ipb]));
    }
  }

  /* Go to the leaf nodes first. This aims to build all the primitive nodes
   * first and then we build the parents and create links
   */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb;
         ++jpb) {
      rec_build_physical_pb_children_from_pb_graph(
        phy_pb,
        &(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]),
        device_annotation);
    }
  }
}

/************************************************************************
 * Allocate an empty physical pb graph based on pb_graph
 * This function should start with an empty physical pb object!!!
 * Suggest to check this before executing this function
 *   VTR_ASSERT(true == phy_pb.empty());
 ***********************************************************************/
void alloc_physical_pb_from_pb_graph(
  PhysicalPb& phy_pb, const t_pb_graph_node* pb_graph_head,
  const VprDeviceAnnotation& device_annotation) {
  VTR_ASSERT(true == phy_pb.empty());

  rec_alloc_physical_pb_from_pb_graph(phy_pb, pb_graph_head, device_annotation);
  rec_build_physical_pb_children_from_pb_graph(phy_pb, pb_graph_head,
                                               device_annotation);
}

/************************************************************************
 * Update a mapping net from a pin of an operating primitive pb to a
 * physical pb data base
 ***********************************************************************/
static void update_primitive_physical_pb_pin_atom_net(
  PhysicalPb& phy_pb, const PhysicalPbId& primitive_pb,
  const t_pb_graph_pin* pb_graph_pin, const t_pb_routes& pb_route,
  const VprDeviceAnnotation& device_annotation, const AtomNetlist& atom_nlist,
  const bool& verbose) {
  int node_index = pb_graph_pin->pin_count_in_cluster;
  if (pb_route.count(node_index)) {
    /* The pin is mapped to a net, find the original pin in the atom netlist */
    AtomNetId atom_net = pb_route[node_index].atom_net_id;

    VTR_ASSERT(atom_net);

    /* Find the physical pb_graph_pin */
    t_pb_graph_pin* physical_pb_graph_pin =
      device_annotation.physical_pb_graph_pin(pb_graph_pin);
    VTR_ASSERT(nullptr != physical_pb_graph_pin);

    if (AtomNetId::INVALID() != atom_net) {
      VTR_LOGV(verbose, "Synchronize net '%s' to physical pb_graph_pin '%s'\n",
               atom_nlist.net_name(atom_net).c_str(),
               pb_graph_pin->to_string().c_str());
    }

    /* Check if the pin has been mapped to a net.
     * If yes, the atom net must be the same
     */
    if (AtomNetId::INVALID() ==
        phy_pb.pb_graph_pin_atom_net(primitive_pb, physical_pb_graph_pin)) {
      phy_pb.set_pb_graph_pin_atom_net(primitive_pb, physical_pb_graph_pin,
                                       atom_net);
    } else {
      VTR_ASSERT(atom_net == phy_pb.pb_graph_pin_atom_net(
                               primitive_pb, physical_pb_graph_pin));
    }
  } else {
    VTR_LOGV(verbose,
             "Skip as no valid routing traces if found on physical "
             "pb_graph_pin '%s'\n",
             pb_graph_pin->to_string().c_str());
  }
}

/************************************************************************
 * Synchronize mapping nets from an operating primitive pb to a physical pb
 ***********************************************************************/
static void synchronize_primitive_physical_pb_atom_nets(
  PhysicalPb& phy_pb, const PhysicalPbId& primitive_pb,
  const t_pb_graph_node* pb_graph_node, const t_pb_routes& pb_route,
  const AtomContext& atom_ctx, const AtomBlockId& atom_blk,
  const VprDeviceAnnotation& device_annotation, const bool& verbose) {
  /* Iterate over all the ports: input, output and clock */
  VTR_LOGV(verbose, "Synchronizing atom nets on pb_graph_node '%s'...\n",
           pb_graph_node->hierarchical_type_name().c_str());

  for (int iport = 0; iport < pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_input_pins[iport]; ++ipin) {
      /* Port exists (some LUTs may have no input and hence no port in the atom
       * netlist) */
      VTR_LOGV(verbose, "Synchronizing atom nets on pb_graph_pin '%s'...\n",
               pb_graph_node->input_pins[iport][ipin].to_string().c_str());
      t_model_ports* model_port =
        pb_graph_node->input_pins[iport][ipin].port->model_port;
      /* Special for LUTs, the model port is hidden under 1 level
       * Do NOT do this. Net mapping on LUT inputs may be swapped during
       * rerouting
       * if (LUT_CLASS == pb_graph_node->pb_type->class_type) {
       *   VTR_ASSERT(pb_graph_node->pb_type->num_modes == 2);
       *   model_port = pb_graph_node->child_pb_graph_nodes[1][0][0]
       *                  .input_pins[iport][ipin]
       *                  .port->model_port;
       * }
       */
      /* It seems that LUT port are no longer built with an internal model */
      if (nullptr == model_port) {
        VTR_LOGV(verbose, "Skip due to empty model port\n");
        continue;
      }

      AtomPortId atom_port =
        atom_ctx.nlist.find_atom_port(atom_blk, model_port);
      if (!atom_port) {
        VTR_LOGV(verbose, "Skip due to invalid port\n");
        continue;
      }
      /* Find the atom nets mapped to the pin
       * Note that some inputs may not be used, we set them to be open by
       * default
       */
      update_primitive_physical_pb_pin_atom_net(
        phy_pb, primitive_pb, &(pb_graph_node->input_pins[iport][ipin]),
        pb_route, device_annotation, atom_ctx.nlist, verbose);
    }
  }

  for (int iport = 0; iport < pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_output_pins[iport]; ++ipin) {
      /* Port exists (some LUTs may have no input and hence no port in the atom
       * netlist) */
      VTR_LOGV(verbose, "Synchronizing atom nets on pb_graph_pin '%s'...\n",
               pb_graph_node->output_pins[iport][ipin].to_string().c_str());
      t_model_ports* model_port =
        pb_graph_node->output_pins[iport][ipin].port->model_port;
      /* Special for LUTs, the model port is hidden under 1 level
       * Do NOT do this. Net mapping on LUT inputs may be swapped during
       * rerouting
       * if (LUT_CLASS == pb_graph_node->pb_type->class_type) {
       *  VTR_ASSERT(pb_graph_node->pb_type->num_modes == 2);
       *  model_port = pb_graph_node->child_pb_graph_nodes[1][0][0]
       *                 .output_pins[iport][ipin]
       *                 .port->model_port;
       * }
       */
      if (nullptr == model_port) {
        VTR_LOGV(verbose, "Skip due to empty model port\n");
        continue;
      }

      AtomPortId atom_port =
        atom_ctx.nlist.find_atom_port(atom_blk, model_port);
      if (!atom_port) {
        VTR_LOGV(verbose, "Skip due to invalid port\n");
        continue;
      }
      /* Find the atom nets mapped to the pin
       * Note that some inputs may not be used, we set them to be open by
       * default
       */
      update_primitive_physical_pb_pin_atom_net(
        phy_pb, primitive_pb, &(pb_graph_node->output_pins[iport][ipin]),
        pb_route, device_annotation, atom_ctx.nlist, verbose);
    }
  }

  for (int iport = 0; iport < pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_clock_pins[iport]; ++ipin) {
      /* Port exists (some LUTs may have no input and hence no port in the atom
       * netlist) */
      VTR_LOGV(verbose, "Synchronizing atom nets on pb_graph_pin '%s'...\n",
               pb_graph_node->clock_pins[iport][ipin].to_string().c_str());
      t_model_ports* model_port =
        pb_graph_node->clock_pins[iport][ipin].port->model_port;
      if (nullptr == model_port) {
        VTR_LOGV(verbose, "Skip due to empty model port\n");
        continue;
      }

      AtomPortId atom_port =
        atom_ctx.nlist.find_atom_port(atom_blk, model_port);
      if (!atom_port) {
        VTR_LOGV(verbose, "Skip due to invalid port\n");
        continue;
      }
      /* Find the atom nets mapped to the pin
       * Note that some inputs may not be used, we set them to be open by
       * default
       */
      update_primitive_physical_pb_pin_atom_net(
        phy_pb, primitive_pb, &(pb_graph_node->clock_pins[iport][ipin]),
        pb_route, device_annotation, atom_ctx.nlist, verbose);
    }
  }
}

/************************************************************************
 * Reach this function, the primitive pb should be
 * - linked to a LUT pb_type
 * - operating in the wire mode of a LUT
 *
 * Note: this function will not check the prequistics here
 *       Users must be responsible for this!!!
 *
 * This function will find the physical pb_graph_pin for each output
 * of the pb_graph node and mark in the physical_pb database
 * as driven by an wired LUT
 ***********************************************************************/
static void mark_physical_pb_wired_lut_outputs(
  PhysicalPb& phy_pb, const PhysicalPbId& primitive_pb,
  const t_pb_graph_node* pb_graph_node,
  const VprDeviceAnnotation& device_annotation, const bool& verbose) {
  for (int iport = 0; iport < pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_output_pins[iport]; ++ipin) {
      t_pb_graph_pin* pb_graph_pin = &(pb_graph_node->output_pins[iport][ipin]);

      /* Find the physical pb_graph_pin */
      t_pb_graph_pin* physical_pb_graph_pin =
        device_annotation.physical_pb_graph_pin(pb_graph_pin);
      VTR_ASSERT(nullptr != physical_pb_graph_pin);

      /* Print debug info */
      VTR_LOGV(verbose, "Mark physical pb_graph pin '%s' as wire LUT output\n",
               physical_pb_graph_pin->to_string().c_str());

      /* Label the pins in physical_pb as driven by wired LUT*/
      phy_pb.set_wire_lut_output(primitive_pb, physical_pb_graph_pin, true);
    }
  }
}

/************************************************************************
 * Synchronize mapping results from an operating pb to a physical pb
 ***********************************************************************/
void rec_update_physical_pb_from_operating_pb(
  PhysicalPb& phy_pb, const t_pb* op_pb, const t_pb_routes& pb_route,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprBitstreamAnnotation& bitstream_annotation, const bool& verbose) {
  t_pb_graph_node* pb_graph_node = op_pb->pb_graph_node;
  t_pb_type* pb_type = pb_graph_node->pb_type;

  if (true == is_primitive_pb_type(pb_type)) {
    t_pb_graph_node* physical_pb_graph_node =
      device_annotation.physical_pb_graph_node(pb_graph_node);
    VTR_ASSERT(nullptr != physical_pb_graph_node);
    /* Find the physical pb */
    const PhysicalPbId& physical_pb = phy_pb.find_pb(physical_pb_graph_node);
    VTR_ASSERT(true == phy_pb.valid_pb_id(physical_pb));

    /* Set the mode bits */
    phy_pb.set_mode_bits(physical_pb,
                         device_annotation.pb_type_mode_bits(pb_type));

    /* Find mapped atom block and add to this physical pb */
    AtomBlockId atom_blk = atom_ctx.nlist.find_block(op_pb->name);
    VTR_ASSERT(atom_blk);

    phy_pb.add_atom_block(physical_pb, atom_blk);
    VTR_LOGV(verbose, "Update physical pb '%s' using atom block '%s'\n",
             physical_pb_graph_node->hierarchical_type_name().c_str(),
             atom_ctx.nlist.block_name(atom_blk).c_str());

    /* if the operating pb type has bitstream annotation,
     * bind the bitstream value from atom block to the physical pb
     */
    if (VprBitstreamAnnotation::e_bitstream_source_type::
          BITSTREAM_SOURCE_EBLIF ==
        bitstream_annotation.pb_type_bitstream_source(pb_type)) {
      StringToken tokenizer =
        bitstream_annotation.pb_type_bitstream_content(pb_type);
      std::vector<std::string> tokens = tokenizer.split(" ");
      /* FIXME: The token-level check should be done much earlier!!! */
      VTR_ASSERT(2 == tokens.size());
      /* The token is typically organized as <.param|.attr> <identifier string>
       */
      if (std::string(".param") == tokens[0]) {
        for (const auto& param_search : atom_ctx.nlist.block_params(atom_blk)) {
          /* Bypass unmatched parameter identifier */
          if (param_search.first != tokens[1]) {
            continue;
          }
          phy_pb.set_fixed_bitstream(physical_pb, param_search.second);
          phy_pb.set_fixed_bitstream_offset(
            physical_pb,
            bitstream_annotation.pb_type_bitstream_offset(pb_type));
        }
      } else if (std::string(".attr") == tokens[0]) {
        for (const auto& attr_search : atom_ctx.nlist.block_attrs(atom_blk)) {
          /* Bypass unmatched parameter identifier */
          if (attr_search.first == tokens[1]) {
            continue;
          }
          phy_pb.set_fixed_bitstream(physical_pb, attr_search.second);
          phy_pb.set_fixed_bitstream_offset(
            physical_pb,
            bitstream_annotation.pb_type_bitstream_offset(pb_type));
        }
      }
    }

    /* if the operating pb type has mode-select bitstream annotation,
     * bind the bitstream value from atom block to the physical pb
     */
    if (VprBitstreamAnnotation::e_bitstream_source_type::
          BITSTREAM_SOURCE_EBLIF ==
        bitstream_annotation.pb_type_mode_select_bitstream_source(pb_type)) {
      StringToken tokenizer =
        bitstream_annotation.pb_type_mode_select_bitstream_content(pb_type);
      std::vector<std::string> tokens = tokenizer.split(" ");
      /* FIXME: The token-level check should be done much earlier!!! */
      VTR_ASSERT(2 == tokens.size());
      /* The token is typically organized as <.param|.attr> <identifier string>
       */
      if (std::string(".param") == tokens[0]) {
        for (const auto& param_search : atom_ctx.nlist.block_params(atom_blk)) {
          /* Bypass unmatched parameter identifier */
          if (param_search.first != tokens[1]) {
            continue;
          }
          phy_pb.set_fixed_mode_select_bitstream(physical_pb,
                                                 param_search.second);
          phy_pb.set_fixed_mode_select_bitstream_offset(
            physical_pb,
            bitstream_annotation.pb_type_mode_select_bitstream_offset(pb_type));
        }
      } else if (std::string(".attr") == tokens[0]) {
        for (const auto& attr_search : atom_ctx.nlist.block_attrs(atom_blk)) {
          /* Bypass unmatched parameter identifier */
          if (attr_search.first == tokens[1]) {
            continue;
          }
          phy_pb.set_fixed_mode_select_bitstream(physical_pb,
                                                 attr_search.second);
          phy_pb.set_fixed_mode_select_bitstream_offset(
            physical_pb,
            bitstream_annotation.pb_type_mode_select_bitstream_offset(pb_type));
        }
      }
    }

    /* Iterate over ports and annotate the atom pins */
    synchronize_primitive_physical_pb_atom_nets(
      phy_pb, physical_pb, pb_graph_node, pb_route, atom_ctx, atom_blk,
      device_annotation, verbose);
    return;
  }

  /* Walk through the pb recursively but only visit the mapped modes and child
   * pbs */
  t_mode* mapped_mode = &(pb_graph_node->pb_type->modes[op_pb->mode]);
  for (int ipb = 0; ipb < mapped_mode->num_pb_type_children; ++ipb) {
    /* Each child may exist multiple times in the hierarchy*/
    for (int jpb = 0; jpb < mapped_mode->pb_type_children[ipb].num_pb; ++jpb) {
      if ((nullptr != op_pb->child_pbs[ipb]) &&
          (nullptr != op_pb->child_pbs[ipb][jpb].name)) {
        rec_update_physical_pb_from_operating_pb(
          phy_pb, &(op_pb->child_pbs[ipb][jpb]), pb_route, atom_ctx,
          device_annotation, bitstream_annotation, verbose);
      } else {
        /* Some pb may be used just in routing purpose, find out the output nets
         */
        /* The following code is inspired by output_cluster.cpp */
        bool is_used = false;
        t_pb_type* child_pb_type = &(mapped_mode->pb_type_children[ipb]);

        /* Bypass non-primitive pb_type, we care only the LUT pb_type */
        if (false == is_primitive_pb_type(child_pb_type)) {
          continue;
        }

        int port_index = 0;
        t_pb_graph_node* child_pb_graph_node =
          &(pb_graph_node->child_pb_graph_nodes[op_pb->mode][ipb][jpb]);

        for (int k = 0; k < child_pb_type->num_ports && !is_used; k++) {
          if (OUT_PORT == child_pb_type->ports[k].type) {
            for (int m = 0; m < child_pb_type->ports[k].num_pins; m++) {
              int node_index = child_pb_graph_node->output_pins[port_index][m]
                                 .pin_count_in_cluster;
              if (pb_route.count(node_index) &&
                  pb_route[node_index].atom_net_id) {
                is_used = true;
                break;
              }
            }
            port_index++;
          }
        }
        /* Identify output pb_graph_pin that is driven by a wired LUT
         * Without this function, physical Look-Up Table build-up will cause
         * errors and bitstream will be incorrect!!!
         */
        if (true == is_used) {
          VTR_ASSERT(LUT_CLASS == child_pb_type->class_type);

          t_pb_graph_node* physical_pb_graph_node =
            device_annotation.physical_pb_graph_node(child_pb_graph_node);
          VTR_ASSERT(nullptr != physical_pb_graph_node);
          /* Find the physical pb */
          const PhysicalPbId& physical_pb =
            phy_pb.find_pb(physical_pb_graph_node);
          VTR_ASSERT(true == phy_pb.valid_pb_id(physical_pb));

          /* Set the mode bits */
          phy_pb.set_mode_bits(
            physical_pb, device_annotation.pb_type_mode_bits(child_pb_type));

          mark_physical_pb_wired_lut_outputs(phy_pb, physical_pb,
                                             child_pb_graph_node,
                                             device_annotation, verbose);
        }
      }
    }
  }
}

/***************************************************************************************
 * This function will identify all the wire LUTs that is created by repacker
 *only under a physical pb
 *
 * A practical example of wire LUT that is created by VPR packer:
 *
 *           LUT
 *           +------------+
 *           |            |
 * netA ---->+----+       |
 *           |    |-------+-----> netC
 * netB ---->+----+       |
 *           |            |
 * netC ---->+------------+-----> netC
 *           |            |
 *           +------------+
 *
 * A fracturable LUT may be mapped to two functions:
 *  - a function which involves netA, netB and netC
 *    the function is defined in an atom block atomA
 *    In this case, netC's driver block in atom context is atomA
 *  - a function which just wire netC through the LUT
 *    the function is NOT defined in any atom block
 *    Such wire LUT is created by VPR's packer
 *
 * THIS CASE IS WHAT THIS FUNCTION IS HANDLING
 * A practical example of wire LUT that is created by repacker:
 *
 *           LUT
 *           +------------+
 *           |            |
 * netA ---->+----+       |
 *           |    |-------+-----> netC
 * netB ---->+----+       |
 *           |            |
 * netD ---->+------------+-----> netD
 *           |            |
 *           +------------+
 *
 * A fracturable LUT may be mapped to two functions:
 *  - a function which involves netA, netB and netC
 *    the function is defined in an atom block atomA
 *    In this case, netC's driver block in atom context is atomA
 *  - a function which just wire netD through the LUT
 *    the function is NOT defined in any atom block
 *    netD is driven by another atom block atomB which is not mapped to the LUT
 *    Such wire LUT is created by repacker
 *
 * Return the number of wire LUTs that are found
 ***************************************************************************************/
int identify_one_physical_pb_wire_lut_created_by_repack(
  PhysicalPb& physical_pb, const PhysicalPbId& lut_pb_id,
  const VprDeviceAnnotation& device_annotation, const AtomContext& atom_ctx,
  const CircuitLibrary& circuit_lib, const bool& verbose) {
  int wire_lut_counter = 0;
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
      /* Exclude all the LUTs that
       * - have been used as wires
       * - the driver atom block of the output_net is part of the atom blocks
       *   If so, the driver atom block is already mapped to this pb
       *   and the LUT is not used for wiring
       */
      if (true == physical_pb.is_wire_lut_output(lut_pb_id, output_pin)) {
        continue;
      }

      std::vector<AtomBlockId> pb_atom_blocks =
        physical_pb.atom_blocks(lut_pb_id);

      if (pb_atom_blocks.end() !=
          std::find(pb_atom_blocks.begin(), pb_atom_blocks.end(),
                    atom_ctx.nlist.net_driver_block(output_net))) {
        continue;
      }

      /* Bypass the net is NOT routed through the LUT */
      if (false == is_wired_lut(input_nets, output_net)) {
        continue;
      }

      /* Print debug info */
      VTR_LOGV(verbose,
               "Identify physical pb_graph pin '%s.%s[%d]' as wire LUT output "
               "created by repacker\n",
               output_pin->parent_node->pb_type->name, output_pin->port->name,
               output_pin->pin_number);

      /* Label the pins in physical_pb as driven by wired LUT*/
      physical_pb.set_wire_lut_output(lut_pb_id, output_pin, true);
      wire_lut_counter++;
    }
  }

  return wire_lut_counter;
}

} /* end namespace openfpga */
