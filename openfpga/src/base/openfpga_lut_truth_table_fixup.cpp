/********************************************************************
 * This file includes functions to fix up the pb pin mapping results 
 * after routing optimization
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "pb_type_utils.h"
#include "lut_utils.h"
#include "openfpga_lut_truth_table_fixup.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Apply the fix-up to truth table of LUT according to its pin
 * rotation status by packer
 *
 * Note: 
 *   - pb must represents a LUT pb in the graph and it should be primitive
 *******************************************************************/
static 
void fix_up_lut_atom_block_truth_table(const AtomContext& atom_ctx,
                                       t_pb* pb,
                                       const t_pb_routes& pb_route,
                                       VprClusteringAnnotation& vpr_clustering_annotation,
                                       const bool& verbose) {
  t_pb_graph_node* pb_graph_node = pb->pb_graph_node;
  t_pb_type* pb_type = pb->pb_graph_node->pb_type;

  VTR_ASSERT(LUT_CLASS == pb_type->class_type);

  for (int iport = 0; iport < pb_type->num_ports; ++iport) {
    /* We only care about input ports whose pins are equivalent */
    if (IN_PORT != pb_type->ports[iport].type || true == pb_type->ports[iport].is_clock) {
      continue;
    }
    if (pb_type->ports[iport].equivalent == PortEquivalence::NONE) {
      continue;
    }
    /* Reach here, we have to apply a fix-up */
    AtomBlockId atom_blk = atom_ctx.nlist.find_block(pb->name);
    VTR_ASSERT(atom_blk);

    /* Port exists (some LUTs may have no input and hence no port in the atom netlist) */
    AtomPortId atom_port = atom_ctx.nlist.find_atom_port(atom_blk, pb_type->ports[iport].model_port);
    if (atom_port) { 
      continue;
    }

    /* Find the pin rotation status and record it ,
     * Note that some LUT inputs may not be used, we set them to be open by default
     */
    std::vector<int> rotated_pin_map(pb_type->ports[iport].num_pins, -1);
    for (int ipin = 0; ipin < pb_type->ports[iport].num_pins; ++ipin) {
      int node_index = pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
      if (pb_route.count(node_index)) {
        /* The pin is mapped to a net, find the original pin in the atom netlist */
        AtomNetId atom_net = pb_route[node_index].atom_net_id;

        VTR_ASSERT(atom_net);

        for (AtomPinId atom_pin : atom_ctx.nlist.port_pins(atom_port)) {

          AtomNetId atom_pin_net = atom_ctx.nlist.pin_net(atom_pin);

          if (atom_pin_net == atom_net) {
            rotated_pin_map[ipin] = atom_ctx.nlist.pin_port_bit(atom_pin);
            break;
          }
        }
      }
    }

    /* We can apply truth table adaption now
     * For unused inputs : insert dont care
     * For used inputs : find the bit in the truth table rows and move it by the given mapping
     */
    const AtomNetlist::TruthTable& orig_tt = atom_ctx.nlist.block_truth_table(atom_blk);
    const AtomNetlist::TruthTable& adapt_tt = lut_truth_table_adaption(orig_tt, rotated_pin_map); 
    vpr_clustering_annotation.adapt_truth_table(pb, adapt_tt);

    /* Print info is in the verbose mode */
    VTR_LOGV(verbose, "Original truth table\n");
    for (const std::string& tt_line : truth_table_to_string(orig_tt)) {
      VTR_LOGV(verbose, "\t%s\n", tt_line.c_str());
    }
    VTR_LOGV(verbose, "\n");
    VTR_LOGV(verbose, "Adapt truth table\n");
    VTR_LOGV(verbose, "-----------------\n");
    for (const std::string& tt_line : truth_table_to_string(adapt_tt)) {
      VTR_LOGV(verbose, "\t%s\n", tt_line.c_str());
    }
    VTR_LOGV(verbose, "\n");
  }
}

/********************************************************************
 * This function recursively visits the pb graph until we reach a 
 * LUT pb_type (primitive node in the pb_graph with a class type 
 * of LUT_CLASS
 * Once we find a LUT node, we will apply the fix-up
 *******************************************************************/
static 
void rec_adapt_lut_pb_tt(const AtomContext& atom_ctx,
                         t_pb* pb,
                         const t_pb_routes& pb_route,
                         VprClusteringAnnotation& vpr_clustering_annotation,
                         const bool& verbose) {
  t_pb_graph_node* pb_graph_node = pb->pb_graph_node; 

  /* If we reach a primitive pb_graph node, we return */
  if (true == is_primitive_pb_type(pb_graph_node->pb_type)) {
    if (LUT_CLASS == pb_graph_node->pb_type->class_type) {
      /* Do fix-up here */
      fix_up_lut_atom_block_truth_table(atom_ctx, pb, pb_route, vpr_clustering_annotation, verbose);
    }
    return;
  }

  /* Recursively visit all the used pbs in the graph */
  t_mode* mapped_mode = &(pb_graph_node->pb_type->modes[pb->mode]);
  for (int ipb = 0; ipb < mapped_mode->num_pb_type_children; ++ipb) {
    /* Each child may exist multiple times in the hierarchy*/
    for (int jpb = 0; jpb < mapped_mode->pb_type_children[ipb].num_pb; ++jpb) {
      /* See if we still have any pb children to walk through */
      if ((pb->child_pbs[ipb] != nullptr) && (pb->child_pbs[ipb][jpb].name != nullptr)) {
        rec_adapt_lut_pb_tt(atom_ctx, &(pb->child_pbs[ipb][jpb]), pb_route, vpr_clustering_annotation, verbose);
      }
    }
  }
}

/********************************************************************
 * Main function to fix up truth table for each LUT used in FPGA
 * This function will walk through each clustered block
 *******************************************************************/
static 
void update_lut_tt_with_post_packing_results(const AtomContext& atom_ctx,
                                             const ClusteringContext& clustering_ctx,
                                             VprClusteringAnnotation& vpr_clustering_annotation,
                                             const bool& verbose) {
  for (auto blk_id : clustering_ctx.clb_nlist.blocks()) {
    rec_adapt_lut_pb_tt(atom_ctx,
                        clustering_ctx.clb_nlist.block_pb(blk_id),
                        clustering_ctx.clb_nlist.block_pb(blk_id)->pb_route,
                        vpr_clustering_annotation, verbose);
  }
}


/********************************************************************
 * Top-level function to fix up the lut truth table results after packing is done
 * The problem comes from a mismatch between the packing results and 
 * original truth tables in users' BLIF file
 * As LUT inputs are equivalent in nature, the router of packer will try
 * to swap the net mapping among these pins so as to achieve best 
 * routing optimization.
 * However, it will cause the original truth table out-of-date when packing is done.
 * This function aims to fix the mess after packing so that the truth table
 * can be synchronized
 *******************************************************************/
void lut_truth_table_fixup(OpenfpgaContext& openfpga_context,
                           const Command& cmd, const CommandContext& cmd_context) { 

  vtr::ScopedStartFinishTimer timer("Fix up LUT truth tables after packing optimization");

  CommandOptionId opt_verbose = cmd.option("verbose");

  /* Apply fix-up to each packed block */
  update_lut_tt_with_post_packing_results(g_vpr_ctx.atom(), 
                                          g_vpr_ctx.clustering(),
                                          openfpga_context.mutable_vpr_clustering_annotation(),
                                          cmd_context.option_enable(cmd, opt_verbose));
} 

} /* end namespace openfpga */
