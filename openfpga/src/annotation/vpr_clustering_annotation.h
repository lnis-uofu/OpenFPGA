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
    /* Xifan Tang: I created two functions for each data query in purpose!
     * As this is an annotation, some net/block may be changed to invalid id
     * In this case, return an invalid value does not mean that a net is not renamed
     */
    bool is_net_renamed(const ClusterBlockId& block_id, const int& pin_index) const;
    ClusterNetId net(const ClusterBlockId& block_id, const int& pin_index) const;
    bool is_truth_table_adapted(t_pb* pb) const;
    AtomNetlist::TruthTable truth_table(t_pb* pb) const;
  public:  /* Public mutators */
    void rename_net(const ClusterBlockId& block_id, const int& pin_index,
                    const ClusterNetId& net_id);
    void adapt_truth_table(t_pb* pb, const AtomNetlist::TruthTable& tt);
  private: /* Internal data */
    /* Pair a regular pb_type to its physical pb_type */
    std::map<ClusterBlockId, std::map<int, ClusterNetId>> net_names_;
    std::map<t_pb*, AtomNetlist::TruthTable> block_truth_tables_;
};

} /* End namespace openfpga*/

#endif
