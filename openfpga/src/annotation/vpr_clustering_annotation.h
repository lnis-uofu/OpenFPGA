#ifndef VPR_CLUSTERING_ANNOTATION_H
#define VPR_CLUSTERING_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map> 

/* Header from vpr library */
#include "clustered_netlist.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This is the critical data structure to link the pb_type in VPR
 * to openfpga annotations
 * With a given pb_type pointer, it aims to identify:
 * 1. if the pb_type is a physical pb_type or a operating pb_type
 * 2. what is the circuit model id linked to a physical pb_type
 * 3. what is the physical pb_type for an operating pb_type
 * 4. what is the mode pointer that represents the physical mode for a pb_type
 *******************************************************************/
class VprClusteringAnnotation {
  public:  /* Constructor */
    VprClusteringAnnotation();
  public:  /* Public accessors */
    bool is_net_renamed(const ClusterBlockId& block_id, const int& pin_index) const;
    ClusterNetId net(const ClusterBlockId& block_id, const int& pin_index) const;
  public:  /* Public mutators */
    void rename_net(const ClusterBlockId& block_id, const int& pin_index,
                    const ClusterNetId& net_id);
  private: /* Internal data */
    /* Pair a regular pb_type to its physical pb_type */
    std::map<ClusterBlockId, std::map<int, ClusterNetId>> net_names_;
};

} /* End namespace openfpga*/

#endif
