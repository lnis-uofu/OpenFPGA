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

} /* End namespace openfpga*/
