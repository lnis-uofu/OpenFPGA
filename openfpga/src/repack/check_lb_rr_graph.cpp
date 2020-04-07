#include <map>

#include "vtr_log.h"

#include "lb_rr_graph_utils.h"
#include "check_lb_rr_graph.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************** 
 * This function aims at checking any duplicated edges (with same EdgeId) 
 * of a given node. 
 * We will walkthrough the input edges of a node and see if there is any duplication
 **********************************************************************/
static bool check_lb_rr_graph_node_duplicated_edges(const LbRRGraph& lb_rr_graph,
                         const LbRRNodeId& node) {
  bool no_duplication = true;

  /* Create a map for each input edge */
  std::map<const t_mode*, std::map<LbRREdgeId, size_t>> edge_counter;

  /* Check each input edges */
  for (const auto edge : lb_rr_graph.node_in_edges(node)) {
    if (nullptr == lb_rr_graph.edge_mode(edge)) {
      continue;
    }
    auto result = edge_counter[lb_rr_graph.edge_mode(edge)].insert(std::pair<LbRREdgeId, size_t>(edge, 1));
    if (false == result.second) {
      result.first->second++;
    }
  }

  for (auto& edge_mode : edge_counter) {
    for (auto& elem : edge_mode.second) {
      if (elem.second > 1) {
        /* Reach here it means we find some duplicated edges and report errors */
        /* Print a warning! */
        VTR_LOG_WARN("Node %d has duplicated input edges (id = %d)!\n",
               size_t(node), size_t(elem.first));
        print_lb_rr_node(lb_rr_graph, node);
        no_duplication = false;
      }
    }
  }

  return no_duplication;
}

/*********************************************************************** 
 * Check the whole Routing Resource Graph  
 * identify and report any duplicated edges between two nodes 
 **********************************************************************/
static bool check_lb_rr_graph_duplicated_edges(const LbRRGraph& lb_rr_graph) {
  bool no_duplication = true;
  /* For each node:
   * Search input edges, see there are two edges with same id or address 
   */
  for (const auto& node : lb_rr_graph.nodes()) {
    if (false == check_lb_rr_graph_node_duplicated_edges(lb_rr_graph, node)) {
      no_duplication = false;
    }
  }

  return no_duplication;
}

/*********************************************************************** 
 * Identify and report any dangling node (nodes without any fan-in or fan-out)
 * in the LbRRGraph
 **********************************************************************/
static bool check_lb_rr_graph_dangling_nodes(const LbRRGraph& lb_rr_graph) {
  bool no_dangling = true;
  /* For each node: 
   * check if the number of input edges and output edges are both 0
   * If so, this is a dangling nodes and report 
   */
  for (auto node : lb_rr_graph.nodes()) {
    if ((0 == lb_rr_graph.node_in_edges(node).size())
      && (0 == lb_rr_graph.node_out_edges(node).size())) {
      /* Print a warning! */
      VTR_LOG_WARN("Node %lu is dangling (zero fan-in and zero fan-out)!\n",
                   size_t(node));
      VTR_LOG_WARN("Node details for debugging:\n");
      print_lb_rr_node(lb_rr_graph, node);
      no_dangling = false;
    }
  }

  return no_dangling;
}

/*********************************************************************** 
 * check if all the source nodes are in the right condition:
 * 1. zero fan-in and non-zero fanout
 **********************************************************************/
static bool check_lb_rr_graph_source_nodes(const LbRRGraph& lb_rr_graph) {
  bool invalid_sources = false;
  /* For each node: 
   * check if the number of input edges and output edges are both 0
   * If so, this is a dangling nodes and report 
   */
  for (auto node : lb_rr_graph.nodes()) {
    /* Pass nodes whose types are not LB_SOURCE */
    if (LB_SOURCE != lb_rr_graph.node_type(node)) {
      continue;
    }
    if ((0 != lb_rr_graph.node_in_edges(node).size())
      || (0 == lb_rr_graph.node_out_edges(node).size())) {
      /* Print a warning! */
      VTR_LOG_WARN("Source node %d is invalid (should have zero fan-in and non-zero fan-out)!\n",
             size_t(node));
      VTR_LOG_WARN("Node details for debugging:\n");
      print_lb_rr_node(lb_rr_graph, node);
      invalid_sources = true;
    }
  }

  return !invalid_sources;
}

/*********************************************************************** 
 * check if all the sink nodes are in the right condition:
 * 1. non-zero fan-in and zero fanout
 **********************************************************************/
static bool check_lb_rr_graph_sink_nodes(const LbRRGraph& lb_rr_graph) {
  bool invalid_sinks = false;
  /* For each node: 
   * check if the number of input edges and output edges are both 0
   * If so, this is a dangling nodes and report 
   */
  for (auto node : lb_rr_graph.nodes()) {
    /* Pass nodes whose types are not LB_SINK */
    if (LB_SINK != lb_rr_graph.node_type(node)) {
      continue;
    }
    if ((0 == lb_rr_graph.node_in_edges(node).size())
      || (0 != lb_rr_graph.node_out_edges(node).size())) {
      /* Print a warning! */
      VTR_LOG_WARN("Sink node %s is invalid (should have non-zero fan-in and zero fan-out)!\n",
             node);
      VTR_LOG_WARN("Node details for debugging:\n");
      print_lb_rr_node(lb_rr_graph, node);
      invalid_sinks = true;
    }
  }

  return !invalid_sinks;
}

/*********************************************************************** 
 * This is an advanced checker for LbRRGraph object
 * Note that the checker try to report as many problems as it can. 
 * The problems may cause routing efficiency or even failures, depending
 * on routing algorithms.
 * It is strongly suggested to run this sanitizer before conducting
 * routing algorithms
 * 
 * For those who will develop customized lb_rr_graphs and routers:
 * On the other hand, it is suggested that developers to create their 
 * own checking function for the lb_rr_graph, to guarantee their routers
 * will work properly.
 **********************************************************************/
bool check_lb_rr_graph(const LbRRGraph& lb_rr_graph) {
  size_t num_err = 0;

  if (false == check_lb_rr_graph_duplicated_edges(lb_rr_graph)) {
    VTR_LOG_WARN("Fail in checking duplicated edges !\n");
    num_err++;
  }

  if (false == check_lb_rr_graph_dangling_nodes(lb_rr_graph)) {
    VTR_LOG_WARN("Fail in checking dangling nodes !\n");
    num_err++;
  }

  if (false == check_lb_rr_graph_source_nodes(lb_rr_graph)) {
    VTR_LOG_WARN("Fail in checking source nodes!\n");
    num_err++;
  }

  if (false == check_lb_rr_graph_sink_nodes(lb_rr_graph)) {
    VTR_LOG_WARN("Fail in checking sink nodes!\n");
    num_err++;
  }

  if (false == check_lb_rr_graph_source_nodes(lb_rr_graph)) {
    VTR_LOG_WARN("Fail in checking source nodes!\n");
    num_err++;
  }

  if (false == check_lb_rr_graph_sink_nodes(lb_rr_graph)) {
    VTR_LOG_WARN("Fail in checking sink nodes!\n");
    num_err++;
  }

  /* Error out if there is any fatal errors found */
  if (0 < num_err) {
    VTR_LOG_WARN("Checked Logical tile Routing Resource graph with %d errors !\n",
           num_err);
  }

  return (0 == num_err);
}

} /* end namespace openfpga */
