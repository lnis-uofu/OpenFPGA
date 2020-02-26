#ifndef PHYSICAL_PB_H
#define PHYSICAL_PB_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* Headers from readarch library */
#include "physical_types.h"

/* Headers from vpr library */
#include "atom_netlist.h"

#include "physical_pb_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * PhysicalPb object aims to store the mapped result for a programmable 
 * logical block like the VPR data structure t_pb does.
 * Differently, it is tailored for the physical implementation of a
 * programmable block. 
 *  - It does not contain multi-mode for each child physical_pb while
 *    VPR t_pb does have multi-mode. This is because that the hardware 
 *    implementation is unique
 *  - It contains mode-selection bits for each primitive physical_pb
 *    This is used to help bitstream generator to configure a primitive
 *    circuit in the correct mode
 *  - A primitive LUT can be mapped to various truth tables. 
 *    This is true for any fracturable LUTs.
 *******************************************************************/
class PhysicalPb {
  public: /* Types and ranges */
    typedef vtr::vector<PhysicalPbId, PhysicalPbId>::const_iterator physical_pb_iterator;
    typedef vtr::Range<physical_pb_iterator> physical_pb_range;
  public: /* Public aggregators */
    physical_pb_range pbs() const;
    std::vector<PhysicalPbId> primitive_pbs() const;
    std::string name(const PhysicalPbId& pb) const;
    const t_pb_graph_node* pb_graph_node(const PhysicalPbId& pb) const;
    PhysicalPbId find_pb(const t_pb_graph_node* name) const;
    PhysicalPbId parent(const PhysicalPbId& pb) const;
    PhysicalPbId child(const PhysicalPbId& pb,
                       const t_pb_type* pb_type, 
                       const size_t& index) const;
    std::vector<AtomBlockId> atom_blocks(const PhysicalPbId& pb) const;
    AtomNetId pb_graph_pin_atom_net(const PhysicalPbId& pb,
                                    const t_pb_graph_pin* pb_graph_pin) const;
    std::map<const t_pb_graph_pin*, AtomNetlist::TruthTable> truth_tables(const PhysicalPbId& pb) const;
    std::vector<size_t> mode_bits(const PhysicalPbId& pb) const;
  public: /* Public mutators */
    PhysicalPbId create_pb(const t_pb_graph_node* pb_graph_node);
    void add_child(const PhysicalPbId& parent,
                   const PhysicalPbId& child,
                   const t_pb_type* child_type);
    void add_atom_block(const PhysicalPbId& pb,
                        const AtomBlockId& atom_block);
    void set_truth_table(const PhysicalPbId& pb,
                         const t_pb_graph_pin* pb_graph_pin,
                         const AtomNetlist::TruthTable& truth_table);
    void set_mode_bits(const PhysicalPbId& pb,
                       const std::vector<size_t>& mode_bits);
    void set_pb_graph_pin_atom_net(const PhysicalPbId& pb,
                                   const t_pb_graph_pin* pb_graph_pin,
                                   const AtomNetId& atom_net);
  public: /* Public validators/invalidators */
    bool valid_pb_id(const PhysicalPbId& pb_id) const;
    bool empty() const;
  private: /* Internal Data */
    vtr::vector<PhysicalPbId, PhysicalPbId> pb_ids_;
    vtr::vector<PhysicalPbId, const t_pb_graph_node*> pb_graph_nodes_;
    vtr::vector<PhysicalPbId, std::string> names_;
    vtr::vector<PhysicalPbId, std::vector<AtomBlockId>> atom_blocks_;
    vtr::vector<PhysicalPbId, std::map<const t_pb_graph_pin*, AtomNetId>> pin_atom_nets_;

    /* Child pbs are organized as [0..num_child_pb_types-1][0..child_pb_type->num_pb-1] */
    vtr::vector<PhysicalPbId, std::map<const t_pb_type*, std::vector<PhysicalPbId>>> child_pbs_;
    vtr::vector<PhysicalPbId, PhysicalPbId> parent_pbs_;

    /* configuration bits 
     * Truth tables and mode selection
     */
    vtr::vector<PhysicalPbId, std::map<const t_pb_graph_pin*, AtomNetlist::TruthTable>> truth_tables_;

    vtr::vector<PhysicalPbId, std::vector<size_t>> mode_bits_;

    /* Fast lookup */
    std::map<const t_pb_graph_node*, PhysicalPbId> type2id_map_;
};

} /* End namespace openfpga*/

#endif
