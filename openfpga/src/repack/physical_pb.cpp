/******************************************************************************
 * Memember functions for data structure PhysicalPb
 ******************************************************************************/
#include "vtr_assert.h"
#include "vtr_log.h"

#include "physical_pb.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors 
 *************************************************/
PhysicalPb::physical_pb_range PhysicalPb::pbs() const {
  return vtr::make_range(pb_ids_.begin(), pb_ids_.end());
}

std::vector<PhysicalPbId> PhysicalPb::primitive_pbs() const {
  std::vector<PhysicalPbId> results; 
  /* The primitive pbs are those without any children */
  for (auto pb : pbs()) {
    if (true == child_pbs_[pb].empty()) {
      results.push_back(pb);
    } 
  }
  return results;
}

std::string PhysicalPb::name(const PhysicalPbId& pb) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  return names_[pb];
}

const t_pb_graph_node* PhysicalPb::pb_graph_node(const PhysicalPbId& pb) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  return pb_graph_nodes_[pb];
}

/* Find the module id by a given name, return invalid if not found */
PhysicalPbId PhysicalPb::find_pb(const t_pb_graph_node* pb_graph_node) const {
  if (type2id_map_.find(pb_graph_node) != type2id_map_.end()) {
    /* Find it, return the id */
    return type2id_map_.at(pb_graph_node); 
  }
  /* Not found, return an invalid id */
  return PhysicalPbId::INVALID();
}

PhysicalPbId PhysicalPb::parent(const PhysicalPbId& pb) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  return parent_pbs_[pb];
}

PhysicalPbId PhysicalPb::child(const PhysicalPbId& pb,
                               const t_pb_type* pb_type, 
                               const size_t& index) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  if (0 < child_pbs_[pb].count(pb_type)) {
    if (index < child_pbs_[pb].at(pb_type).size()) {
      return child_pbs_[pb].at(pb_type)[index]; 
    }
  }
  return PhysicalPbId::INVALID();
}

std::vector<AtomBlockId> PhysicalPb::atom_blocks(const PhysicalPbId& pb) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  
  return atom_blocks_[pb];
}

AtomNetId PhysicalPb::pb_graph_pin_atom_net(const PhysicalPbId& pb,
                                            const t_pb_graph_pin* pb_graph_pin) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  if (pin_atom_nets_[pb].find(pb_graph_pin) != pin_atom_nets_[pb].end()) {
    /* Find it, return the id */
    return pin_atom_nets_[pb].at(pb_graph_pin); 
  }
  /* Not found, return an invalid id */
  return AtomNetId::INVALID();
}

bool PhysicalPb::is_wire_lut_output(const PhysicalPbId& pb,
                                    const t_pb_graph_pin* pb_graph_pin) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  if (wire_lut_outputs_[pb].find(pb_graph_pin) != wire_lut_outputs_[pb].end()) {
    /* Find it, return the status */
    return wire_lut_outputs_[pb].at(pb_graph_pin); 
  }
  /* Not found, return false */
  return false;
}

std::map<const t_pb_graph_pin*, AtomNetlist::TruthTable> PhysicalPb::truth_tables(const PhysicalPbId& pb) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  return truth_tables_[pb];
}

std::vector<size_t> PhysicalPb::mode_bits(const PhysicalPbId& pb) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  return mode_bits_[pb];
}

std::string PhysicalPb::fixed_bitstream(const PhysicalPbId& pb) const {
  VTR_ASSERT(true == valid_pb_id(pb));
  return fixed_bitstreams_[pb];
}

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
PhysicalPbId PhysicalPb::create_pb(const t_pb_graph_node* pb_graph_node) {
  /* Find if the name has been used. If used, return an invalid Id and report error! */
  std::map<const t_pb_graph_node*, PhysicalPbId>::iterator it = type2id_map_.find(pb_graph_node);
  if (it != type2id_map_.end()) {
    return PhysicalPbId::INVALID();
  }

  /* Create an new id */
  PhysicalPbId pb = PhysicalPbId(pb_ids_.size());
  pb_ids_.push_back(pb);

  /* Allocate other attributes */
  names_.emplace_back();
  pb_graph_nodes_.push_back(pb_graph_node);
  atom_blocks_.emplace_back();
  pin_atom_nets_.emplace_back();
  wire_lut_outputs_.emplace_back();

  child_pbs_.emplace_back();
  parent_pbs_.emplace_back();

  truth_tables_.emplace_back();
  mode_bits_.emplace_back();
  fixed_bitstreams_.emplace_back();

  /* Register in the name2id map */
  type2id_map_[pb_graph_node] = pb;

  return pb;
}

void PhysicalPb::add_child(const PhysicalPbId& parent,
                           const PhysicalPbId& child,
                           const t_pb_type* child_type) {
  VTR_ASSERT(true == valid_pb_id(parent)); 
  VTR_ASSERT(true == valid_pb_id(child)); 

  child_pbs_[parent][child_type].push_back(child);

  if (PhysicalPbId::INVALID() != parent_pbs_[child]) {
    VTR_LOGF_WARN(__FILE__, __LINE__,
                  "Overwrite parent '%s' for physical pb '%s' with a new one '%s'!\n",
                  pb_graph_nodes_[parent_pbs_[child]]->hierarchical_type_name().c_str(),
                  pb_graph_nodes_[child]->hierarchical_type_name().c_str(),
                  pb_graph_nodes_[parent]->hierarchical_type_name().c_str());
  }
  parent_pbs_[child] = parent;
}

void PhysicalPb::set_truth_table(const PhysicalPbId& pb,
                                 const t_pb_graph_pin* pb_graph_pin,
                                 const AtomNetlist::TruthTable& truth_table) {
  VTR_ASSERT(true == valid_pb_id(pb)); 

  if (0 < truth_tables_[pb].count(pb_graph_pin)) {
    VTR_LOG_WARN("Overwrite truth tables mapped to pb_graph_pin '%s[%ld]!\n",
                  pb_graph_pin->port->name, pb_graph_pin->pin_number);
  }
   
  truth_tables_[pb][pb_graph_pin] = truth_table;
}

void PhysicalPb::set_mode_bits(const PhysicalPbId& pb,
                               const std::vector<size_t>& mode_bits) {
  VTR_ASSERT(true == valid_pb_id(pb)); 
   
  mode_bits_[pb] = mode_bits;
}

void PhysicalPb::add_atom_block(const PhysicalPbId& pb,
                                const AtomBlockId& atom_block) {
  VTR_ASSERT(true == valid_pb_id(pb)); 
  
  atom_blocks_[pb].push_back(atom_block);
}

void PhysicalPb::set_pb_graph_pin_atom_net(const PhysicalPbId& pb,
                                           const t_pb_graph_pin* pb_graph_pin,
                                           const AtomNetId& atom_net) {
  VTR_ASSERT(true == valid_pb_id(pb)); 
  if (pin_atom_nets_[pb].end() != pin_atom_nets_[pb].find(pb_graph_pin)) {
    VTR_LOG_WARN("Overwrite pb_graph_pin '%s[%d]' atom net '%lu' with '%lu'\n",
                 pb_graph_pin->port->name, pb_graph_pin->pin_number,
                 size_t(pin_atom_nets_[pb][pb_graph_pin]),
                 size_t(atom_net));
  }

  pin_atom_nets_[pb][pb_graph_pin] = atom_net;
}

void PhysicalPb::set_wire_lut_output(const PhysicalPbId& pb,
                                     const t_pb_graph_pin* pb_graph_pin,
                                     const bool& wire_lut_output) {
  VTR_ASSERT(true == valid_pb_id(pb)); 
  if (wire_lut_outputs_[pb].end() != wire_lut_outputs_[pb].find(pb_graph_pin)) {
    VTR_LOG_WARN("Overwrite pb_graph_pin '%s[%d]' status on wire LUT output\n",
                 pb_graph_pin->port->name, pb_graph_pin->pin_number);
  }

  wire_lut_outputs_[pb][pb_graph_pin] = wire_lut_output;
}

void PhysicalPb::set_fixed_bitstream(const PhysicalPbId& pb,
                                     const std::string& fixed_bitstream) {
  VTR_ASSERT(true == valid_pb_id(pb)); 
  fixed_bitstreams_[pb] = fixed_bitstream;
}

/******************************************************************************
 * Private validators/invalidators
 ******************************************************************************/
bool PhysicalPb::valid_pb_id(const PhysicalPbId& pb_id) const {
  return ( size_t(pb_id) < pb_ids_.size() ) && ( pb_id == pb_ids_[pb_id] ); 
}

bool PhysicalPb::empty() const {
  return 0 == pb_ids_.size();
}

} /* end namespace openfpga */
