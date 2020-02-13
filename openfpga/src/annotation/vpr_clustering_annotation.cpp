/************************************************************************
 * Member functions for class VprClusteringAnnotation
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vpr_clustering_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprClusteringAnnotation::VprClusteringAnnotation() {
  return;
}

/************************************************************************
 * Public accessors
 ***********************************************************************/
bool VprClusteringAnnotation::is_net_renamed(const ClusterBlockId& block_id, const int& pin_index) const {
  /* Ensure that the block_id is in the list */
  if (net_names_.end() == net_names_.find(block_id)) {
    return false;
  }
  return (net_names_.at(block_id).end() != net_names_.at(block_id).find(pin_index));
}

ClusterNetId VprClusteringAnnotation::net(const ClusterBlockId& block_id, const int& pin_index) const {
  VTR_ASSERT(true == is_net_renamed(block_id, pin_index));
  return net_names_.at(block_id).at(pin_index);
}

bool VprClusteringAnnotation::is_truth_table_adapted(t_pb* pb) const {
  /* Ensure that the block_id is in the list */
  return (block_truth_tables_.end() != block_truth_tables_.find(pb));
}

AtomNetlist::TruthTable VprClusteringAnnotation::truth_table(t_pb* pb) const {
  VTR_ASSERT(true == is_truth_table_adapted(pb));
  return block_truth_tables_.at(pb);
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprClusteringAnnotation::rename_net(const ClusterBlockId& block_id, const int& pin_index,
                                                const ClusterNetId& net_id) {
  /* Warn any override attempt */
  if ( (net_names_.end() != net_names_.find(block_id))
    && (net_names_.at(block_id).end() != net_names_.at(block_id).find(pin_index)) ) {
    VTR_LOG_WARN("Override the net '%ld' for block '%ld' pin '%d' with in clustering context annotation!\n",
                 size_t(net_id), size_t(block_id), pin_index);
  }

  net_names_[block_id][pin_index] = net_id;
}

void VprClusteringAnnotation::adapt_truth_table(t_pb* pb,
                                                const AtomNetlist::TruthTable& tt) {
  /* Warn any override attempt */
  if (block_truth_tables_.end() != block_truth_tables_.find(pb)) {
    VTR_LOG_WARN("Override the truth table for pb '%s' in clustering context annotation!\n",
                 pb->name);
  }

  block_truth_tables_[pb] = tt;
}

} /* End namespace openfpga*/
