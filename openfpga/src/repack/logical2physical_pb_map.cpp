/******************************************************************************
 * Memember functions for data structure PhysicalPb
 ******************************************************************************/
#include "logical2physical_pb_map.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors
 *************************************************/
const t_pb_type* Logical2PhysicalPbMap::pb_type(const t_pb_type* lgk_pb_type) const {
  if (empty()) {
    return lgk_pb_type;
  }
  auto result = pb_type_map_.find(lgk_pb_type);
  if (result == pb_type_map_.end()) {
    return nullptr;
  }
  return result->second;
}

const t_pb_graph_node* Logical2PhysicalPbMap::pb_graph_node(const t_pb_graph_node* lgk_pb_graph_node) const {
  if (empty()) {
    return lgk_pb_graph_node;
  }
  auto result = pb_graph_node_map_.find(lgk_pb_graph_node);
  if (result == pb_graph_node_map_.end()) {
    return nullptr;
  }
  return result->second;
}

const t_pb_graph_pin* Logical2PhysicalPbMap::pb_graph_pin(const t_pb_graph_pin* lgk_pb_graph_pin) const {
  if (empty()) {
    return lgk_pb_graph_pin;
  }
  auto result = pb_graph_pin_map_.find(lgk_pb_graph_pin);
  if (result == pb_graph_pin_map_.end()) {
    return nullptr;
  }
  return result->second;
}

bool Logical2PhysicalPbMap::init(t_logical_block_type_ptr lgk_lb_type, 
                                 t_logical_block_type_ptr phy_lb_type,
                                 const bool& verbose) {
  if (lgk_lb_type == phy_pb_type) {
    return true;
  }
}

bool Logical2PhysicalPbMap::empty() const { 
  return pb_type_map_.empty() && pb_graph_node_map_.empty() && pb_graph_pin_map_.empty();
}

} /* end namespace openfpga */
