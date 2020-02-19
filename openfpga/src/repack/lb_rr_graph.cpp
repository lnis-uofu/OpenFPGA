/************************************************************************
 * Member Functions of LbRRGraph
 * include mutators, accessors and utility functions 
 ***********************************************************************/
#include "vtr_assert.h"
#include "vtr_log.h"
#include "lb_rr_graph.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
LbRRGraph::LbRRGraph() {
  ext_source_node_ = LbRRNodeId::INVALID();
  ext_sink_node_ = LbRRNodeId::INVALID();
}

/**************************************************
 * Public Accessors: Aggregates 
 *************************************************/
LbRRGraph::node_range LbRRGraph::nodes() const {
  return vtr::make_range(node_ids_.begin(), node_ids_.end());
}

LbRRGraph::edge_range LbRRGraph::edges() const {
  return vtr::make_range(edge_ids_.begin(), edge_ids_.end());
}

/**************************************************
 * Public Accessors node-level attributes 
 *************************************************/
e_lb_rr_type LbRRGraph::node_type(const LbRRNodeId& node) const {
  VTR_ASSERT(true == valid_node_id(node));
  return node_types_[node];
}

short LbRRGraph::node_capacity(const LbRRNodeId& node) const {
  VTR_ASSERT(true == valid_node_id(node));
  return node_capacities_[node];
}

t_pb_graph_pin* LbRRGraph::node_pb_graph_pin(const LbRRNodeId& node) const {
  VTR_ASSERT(true == valid_node_id(node));
  return node_pb_graph_pins_[node];
}

float LbRRGraph::node_intrinsic_cost(const LbRRNodeId& node) const {
  VTR_ASSERT(true == valid_node_id(node));
  return node_intrinsic_costs_[node];
}

std::vector<LbRREdgeId> LbRRGraph::node_in_edges(const LbRRNodeId& node) const {
  VTR_ASSERT(true == valid_node_id(node));
  return node_in_edges_[node];
}

std::vector<LbRREdgeId> LbRRGraph::node_in_edges(const LbRRNodeId& node, t_mode* mode) const {
  std::vector<LbRREdgeId> in_edges;

  VTR_ASSERT(true == valid_node_id(node));
  for (const LbRREdgeId& edge : node_in_edges_[node]) {
    if (mode == edge_mode(edge)) {
      in_edges.push_back(edge);
    }
  }

  return in_edges;
}

std::vector<LbRREdgeId> LbRRGraph::node_out_edges(const LbRRNodeId& node) const {
  VTR_ASSERT(true == valid_node_id(node));
  return node_out_edges_[node];
}

std::vector<LbRREdgeId> LbRRGraph::node_out_edges(const LbRRNodeId& node, t_mode* mode) const {
  std::vector<LbRREdgeId> out_edges;

  VTR_ASSERT(true == valid_node_id(node));
  for (const LbRREdgeId& edge : node_out_edges_[node]) {
    if (mode == edge_mode(edge)) {
      out_edges.push_back(edge);
    }
  }

  return out_edges;
}

LbRRNodeId LbRRGraph::find_node(const e_lb_rr_type& type, const t_pb_graph_pin* pb_graph_pin) const {
  if (size_t(type) >= node_lookup_.size()) {
    return LbRRNodeId::INVALID();
  }

  if (0 == node_lookup_[size_t(type)].count(pb_graph_pin)) {
    return LbRRNodeId::INVALID();
  }
  
  return node_lookup_[size_t(type)].at(pb_graph_pin);
}

LbRRNodeId LbRRGraph::ext_source_node() const {
  return ext_source_node_;
}

LbRRNodeId LbRRGraph::ext_sink_node() const {
  return ext_sink_node_;
}

std::vector<LbRREdgeId> LbRRGraph::find_edge(const LbRRNodeId& src_node, const LbRRNodeId& sink_node) const {
  std::vector<LbRREdgeId> edges;
  for (const LbRREdgeId& edge : node_out_edges_[src_node]) {
    if (sink_node == edge_sink_node(edge)) {
      edges.push_back(edge);
    }
  }
  return edges;
}

LbRRNodeId LbRRGraph::edge_src_node(const LbRREdgeId& edge) const {
  VTR_ASSERT(true == valid_edge_id(edge));
  return edge_src_nodes_[edge];
}

LbRRNodeId LbRRGraph::edge_sink_node(const LbRREdgeId& edge) const {
  VTR_ASSERT(true == valid_edge_id(edge));
  return edge_sink_nodes_[edge];
}

float LbRRGraph::edge_intrinsic_cost(const LbRREdgeId& edge) const {
  VTR_ASSERT(true == valid_edge_id(edge));
  return edge_intrinsic_costs_[edge];
}

t_mode* LbRRGraph::edge_mode(const LbRREdgeId& edge) const {
  VTR_ASSERT(true == valid_edge_id(edge));
  return edge_modes_[edge];
}

/******************************************************************************
 * Public Mutators
 ******************************************************************************/
void LbRRGraph::reserve_nodes(const unsigned long& num_nodes) {
  node_ids_.reserve(num_nodes);
  node_types_.reserve(num_nodes);
  node_capacities_.reserve(num_nodes);
  node_pb_graph_pins_.reserve(num_nodes);
  node_intrinsic_costs_.reserve(num_nodes);

  node_in_edges_.reserve(num_nodes);
  node_out_edges_.reserve(num_nodes);
}

void LbRRGraph::reserve_edges(const unsigned long& num_edges) {
  edge_ids_.reserve(num_edges);
  edge_intrinsic_costs_.reserve(num_edges);
  edge_modes_.reserve(num_edges);

  edge_src_nodes_.reserve(num_edges);
  edge_sink_nodes_.reserve(num_edges);
}

LbRRNodeId LbRRGraph::create_node(const e_lb_rr_type& type) {
  /* Create an new id */
  LbRRNodeId node = LbRRNodeId(node_ids_.size());
  node_ids_.push_back(node);

  /* Allocate other attributes */
  node_types_.push_back(type);
  node_capacities_.push_back(-1);
  node_pb_graph_pins_.push_back(nullptr);
  node_intrinsic_costs_.push_back(0.);

  node_in_edges_.emplace_back();
  node_out_edges_.emplace_back();

  return node;
}

LbRRNodeId LbRRGraph::create_ext_source_node(const e_lb_rr_type& type) {
  LbRRNodeId ext_source_node = create_node(type); 
  ext_source_node_ = ext_source_node;

  return ext_source_node;
}

LbRRNodeId LbRRGraph::create_ext_sink_node(const e_lb_rr_type& type) {
  LbRRNodeId ext_sink_node = create_node(type); 
  ext_sink_node_ = ext_sink_node;

  return ext_sink_node;
}

void LbRRGraph::set_node_type(const LbRRNodeId& node, const e_lb_rr_type& type) {
  VTR_ASSERT(true == valid_node_id(node));
  node_types_[node] = type;
}

void LbRRGraph::set_node_capacity(const LbRRNodeId& node, const short& capacity) {
  VTR_ASSERT(true == valid_node_id(node));
  node_capacities_[node] = capacity;
}

void LbRRGraph::set_node_pb_graph_pin(const LbRRNodeId& node, t_pb_graph_pin* pb_graph_pin) {
  VTR_ASSERT(true == valid_node_id(node));
  node_pb_graph_pins_[node] = pb_graph_pin;

  /* Register in fast node look-up */
  if (node_type(node) >= node_lookup_.size()) {
    node_lookup_.resize(node_type(node) + 1);
  }

  if (0 < node_lookup_[node_type(node)].count(pb_graph_pin)) {
    VTR_LOG_WARN("Detect pb_graph_pin '%s[%lu]' is mapped to LbRRGraph nodes (exist: %lu) and (to be mapped: %lu). Overwrite is done\n",
                 pb_graph_pin->port->name, pb_graph_pin->pin_number,
                 size_t(node_lookup_[node_type(node)].at(pb_graph_pin)),
                 size_t(node));
  }
  node_lookup_[node_type(node)][pb_graph_pin] = node;
}

void LbRRGraph::set_node_intrinsic_cost(const LbRRNodeId& node, const float& cost) {
  VTR_ASSERT(true == valid_node_id(node));
  node_intrinsic_costs_[node] = cost;
}

LbRREdgeId LbRRGraph::create_edge(const LbRRNodeId& source,
                                  const LbRRNodeId& sink,
                                  t_mode* mode) {
  VTR_ASSERT(true == valid_node_id(source));
  VTR_ASSERT(true == valid_node_id(sink));

  /* Create an new id */
  LbRREdgeId edge = LbRREdgeId(edge_ids_.size());
  edge_ids_.push_back(edge);

  /* Allocate other attributes */
  edge_src_nodes_.push_back(source);
  edge_sink_nodes_.push_back(sink);
  edge_intrinsic_costs_.push_back(0.);
  edge_modes_.push_back(mode);

  node_out_edges_[source].push_back(edge);
  node_in_edges_[sink].push_back(edge);

  return edge;
}

void LbRRGraph::set_edge_intrinsic_cost(const LbRREdgeId& edge, const float& cost) {
  VTR_ASSERT(true == valid_edge_id(edge));
  edge_intrinsic_costs_[edge] = cost;
}

/******************************************************************************
 * Public validators/invalidators
 ******************************************************************************/
bool LbRRGraph::valid_node_id(const LbRRNodeId& node_id) const {
  return ( size_t(node_id) < node_ids_.size() ) && ( node_id == node_ids_[node_id] ); 
}

bool LbRRGraph::valid_edge_id(const LbRREdgeId& edge_id) const {
  return ( size_t(edge_id) < edge_ids_.size() ) && ( edge_id == edge_ids_[edge_id] ); 
}

} /* end namespace openfpga */
