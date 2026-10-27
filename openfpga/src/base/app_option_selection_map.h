#ifndef APP_OPTION_SELECTION_MAP_H
#define APP_OPTION_SELECTION_MAP_H

#include <map>
#include <string>

#include "vpr_types.h"

/********************************************************************
 * The values that the selection application options accept, with the
 * VPR enumeration they stand for.
 * Keep them in sync with the enumerations of VPR
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

inline const std::map<std::string, int> const_gen_inference_map = {
  {"none", static_cast<int>(e_const_gen_inference::NONE)},
  {"comb", static_cast<int>(e_const_gen_inference::COMB)},
  {"comb_seq", static_cast<int>(e_const_gen_inference::COMB_SEQ)},
};

inline const std::map<std::string, int> ap_analytical_solver_map = {
  {"Identity", static_cast<int>(e_ap_analytical_solver::Identity)},
  {"QP-Hybrid", static_cast<int>(e_ap_analytical_solver::QP_Hybrid)},
  {"LP-B2B", static_cast<int>(e_ap_analytical_solver::LP_B2B)},
};

inline const std::map<std::string, int> ap_partial_legalizer_map = {
  {"Identity", static_cast<int>(e_ap_partial_legalizer::Identity)},
  {"BiPartitioning", static_cast<int>(e_ap_partial_legalizer::BiPartitioning)},
  {"FlowBased", static_cast<int>(e_ap_partial_legalizer::FlowBased)},
};

inline const std::map<std::string, int> ap_full_legalizer_map = {
  {"Naive", static_cast<int>(e_ap_full_legalizer::Naive)},
  {"APPack", static_cast<int>(e_ap_full_legalizer::APPack)},
  {"FlatRecon", static_cast<int>(e_ap_full_legalizer::FlatRecon)},
};

inline const std::map<std::string, int> ap_detailed_placer_map = {
  {"Identity", static_cast<int>(e_ap_detailed_placer::Identity)},
  {"Annealer", static_cast<int>(e_ap_detailed_placer::Annealer)},
};

inline const std::map<std::string, int> place_init_t_estimator_map = {
  {"cost_variance", static_cast<int>(e_anneal_init_t_estimator::COST_VARIANCE)},
  {"equilibrium", static_cast<int>(e_anneal_init_t_estimator::EQUILIBRIUM)},
};

inline const std::map<std::string, int> anneal_sched_type_map = {
  {"auto", static_cast<int>(e_sched_type::AUTO_SCHED)},
  {"user", static_cast<int>(e_sched_type::USER_SCHED)},
};

inline const std::map<std::string, int> place_algorithm_map = {
  {"bounding_box", static_cast<int>(e_place_algorithm::BOUNDING_BOX_PLACE)},
  {"criticality_timing",
   static_cast<int>(e_place_algorithm::CRITICALITY_TIMING_PLACE)},
  {"slack_timing", static_cast<int>(e_place_algorithm::SLACK_TIMING_PLACE)},
};

inline const std::map<std::string, int> pad_loc_type_map = {
  {"free", static_cast<int>(e_pad_loc_type::FREE)},
  {"random", static_cast<int>(e_pad_loc_type::RANDOM)},
};

inline const std::map<std::string, int> place_effort_scaling_map = {
  {"circuit", static_cast<int>(CIRCUIT)},
  {"device_circuit", static_cast<int>(DEVICE_CIRCUIT)},
};

inline const std::map<std::string, int> place_delta_delay_algorithm_map = {
  {"astar", static_cast<int>(e_place_delta_delay_algorithm::ASTAR_ROUTE)},
  {"dijkstra",
   static_cast<int>(e_place_delta_delay_algorithm::DIJKSTRA_EXPANSION)},
};

inline const std::map<std::string, int> place_placement_freq_map = {
  {"once", static_cast<int>(e_place_freq::ONCE)},
  {"always", static_cast<int>(e_place_freq::ALWAYS)},
};

inline const std::map<std::string, int> place_agent_space_map = {
  {"move_type", static_cast<int>(e_agent_space::MOVE_TYPE)},
  {"move_block_type", static_cast<int>(e_agent_space::MOVE_BLOCK_TYPE)},
};

inline const std::map<std::string, int> place_agent_algorithm_map = {
  {"e_greedy", static_cast<int>(e_agent_algorithm::E_GREEDY)},
  {"softmax", static_cast<int>(e_agent_algorithm::SOFTMAX)},
};

inline const std::map<std::string, int> timing_place_delay_model_map = {
  {"simple", static_cast<int>(PlaceDelayModelType::SIMPLE)},
  {"delta", static_cast<int>(PlaceDelayModelType::DELTA)},
  {"delta_override", static_cast<int>(PlaceDelayModelType::DELTA_OVERRIDE)},
};

inline const std::map<std::string, int> timing_place_delay_model_reducer_map = {
  {"min", static_cast<int>(e_reducer::MIN)},
  {"max", static_cast<int>(e_reducer::MAX)},
  {"median", static_cast<int>(e_reducer::MEDIAN)},
  {"arithmean", static_cast<int>(e_reducer::ARITHMEAN)},
  {"geomean", static_cast<int>(e_reducer::GEOMEAN)},
};

inline const std::map<std::string, int> router_base_cost_type_map = {
  {"demand_only", static_cast<int>(DEMAND_ONLY)},
  {"demand_only_normalized_length",
   static_cast<int>(DEMAND_ONLY_NORMALIZED_LENGTH)},
  {"delay_normalized", static_cast<int>(DELAY_NORMALIZED)},
  {"delay_normalized_length", static_cast<int>(DELAY_NORMALIZED_LENGTH)},
  {"delay_normalized_length_bounded",
   static_cast<int>(DELAY_NORMALIZED_LENGTH_BOUNDED)},
  {"delay_normalized_frequency", static_cast<int>(DELAY_NORMALIZED_FREQUENCY)},
  {"delay_normalized_length_frequency",
   static_cast<int>(DELAY_NORMALIZED_LENGTH_FREQUENCY)},
};

inline const std::map<std::string, int> router_route_type_map = {
  {"global", static_cast<int>(e_route_type::GLOBAL)},
  {"detailed", static_cast<int>(e_route_type::DETAILED)},
};

inline const std::map<std::string, int> router_algorithm_map = {
  {"parallel", static_cast<int>(e_router_algorithm::PARALLEL)},
  {"timing_driven", static_cast<int>(e_router_algorithm::TIMING_DRIVEN)},
};

inline const std::map<std::string, int> router_check_route_map = {
  {"off", static_cast<int>(e_check_route_option::OFF)},
  {"quick", static_cast<int>(e_check_route_option::QUICK)},
  {"full", static_cast<int>(e_check_route_option::FULL)},
};

inline const std::map<std::string, int>
  router_reorder_rr_graph_nodes_algorithm_map = {
    {"none", static_cast<int>(e_rr_node_reorder_algorithm::DONT_REORDER)},
    {"degree_bfs", static_cast<int>(e_rr_node_reorder_algorithm::DEGREE_BFS)},
    {"random_shuffle",
     static_cast<int>(e_rr_node_reorder_algorithm::RANDOM_SHUFFLE)},
};

inline const std::map<std::string, int>
  timing_router_incr_reroute_delay_ripup_map = {
    {"on", static_cast<int>(e_incr_reroute_delay_ripup::ON)},
    {"off", static_cast<int>(e_incr_reroute_delay_ripup::OFF)},
    {"auto", static_cast<int>(e_incr_reroute_delay_ripup::AUTO)},
};

inline const std::map<std::string, int>
  timing_router_routing_failure_predictor_map = {
    {"safe", static_cast<int>(e_routing_failure_predictor::SAFE)},
    {"aggressive", static_cast<int>(e_routing_failure_predictor::AGGRESSIVE)},
    {"off", static_cast<int>(e_routing_failure_predictor::OFF)},
};

inline const std::map<std::string, int>
  timing_router_routing_budgets_algorithm_map = {
    {"minimax", static_cast<int>(e_routing_budgets_algorithm::MINIMAX)},
    {"yoyo", static_cast<int>(e_routing_budgets_algorithm::YOYO)},
    {"scale_delay", static_cast<int>(e_routing_budgets_algorithm::SCALE_DELAY)},
    {"disable", static_cast<int>(e_routing_budgets_algorithm::DISABLE)},
};

inline const std::map<std::string, int> timing_router_route_bb_update_map = {
  {"static", static_cast<int>(e_route_bb_update::STATIC)},
  {"dynamic", static_cast<int>(e_route_bb_update::DYNAMIC)},
};

inline const std::map<std::string, int> timing_router_lookahead_type_map = {
  {"classic", static_cast<int>(e_router_lookahead::CLASSIC)},
  {"map", static_cast<int>(e_router_lookahead::MAP)},
  {"compressed_map", static_cast<int>(e_router_lookahead::COMPRESSED_MAP)},
  {"extended_map", static_cast<int>(e_router_lookahead::EXTENDED_MAP)},
  {"simple", static_cast<int>(e_router_lookahead::SIMPLE)},
};

inline const std::map<std::string, int> timing_router_initial_timing_map = {
  {"all_critical", static_cast<int>(e_router_initial_timing::ALL_CRITICAL)},
  {"lookahead", static_cast<int>(e_router_initial_timing::LOOKAHEAD)},
};

inline const std::map<std::string, int> timing_router_heap_map = {
  {"binary", static_cast<int>(e_heap_type::BINARY_HEAP)},
  {"four_ary", static_cast<int>(e_heap_type::FOUR_ARY_HEAP)},
  {"bucket", static_cast<int>(e_heap_type::INVALID_HEAP)},
};

inline const std::map<std::string, int> timing_update_type_map = {
  {"auto", static_cast<int>(e_timing_update_type::AUTO)},
  {"full", static_cast<int>(e_timing_update_type::FULL)},
  {"incremental", static_cast<int>(e_timing_update_type::INCREMENTAL)},
};

inline const std::map<std::string, int> constant_net_method_map = {
  {"global", static_cast<int>(e_constant_net_method::CONSTANT_NET_GLOBAL)},
  {"route", static_cast<int>(e_constant_net_method::CONSTANT_NET_ROUTE)},
};

inline const std::map<std::string, int> clock_modeling_map = {
  {"ideal", static_cast<int>(e_clock_modeling::IDEAL_CLOCK)},
  {"route", static_cast<int>(e_clock_modeling::ROUTED_CLOCK)},
  {"dedicated_network", static_cast<int>(e_clock_modeling::DEDICATED_NETWORK)},
};

inline const std::map<std::string, int> analysis_timing_report_detail_map = {
  {"netlist", static_cast<int>(e_timing_report_detail::NETLIST)},
  {"aggregated", static_cast<int>(e_timing_report_detail::AGGREGATED)},
  {"detailed", static_cast<int>(e_timing_report_detail::DETAILED_ROUTING)},
  {"debug", static_cast<int>(e_timing_report_detail::DEBUG)},
};

const std::map<std::string, int>
  analysis_post_synth_netlist_unconn_handling_map = {
    {"unconnected",
     static_cast<int>(e_post_synth_netlist_unconn_handling::UNCONNECTED)},
    {"nets", static_cast<int>(e_post_synth_netlist_unconn_handling::NETS)},
    {"gnd", static_cast<int>(e_post_synth_netlist_unconn_handling::GND)},
    {"vcc", static_cast<int>(e_post_synth_netlist_unconn_handling::VCC)},
};

inline const std::map<std::string, int>
  clustering_allow_unrelated_clustering_map = {
    {"on", static_cast<int>(e_unrelated_clustering::ON)},
    {"off", static_cast<int>(e_unrelated_clustering::OFF)},
    {"auto", static_cast<int>(e_unrelated_clustering::AUTO)},
};

inline const std::map<std::string, int>
  clustering_balance_block_type_utilization_map = {
    {"on", static_cast<int>(e_balance_block_type_util::ON)},
    {"off", static_cast<int>(e_balance_block_type_util::OFF)},
    {"auto", static_cast<int>(e_balance_block_type_util::AUTO)},
};

inline const std::map<std::string, int> clustering_cluster_seed_type_map = {
  {"timing", static_cast<int>(e_cluster_seed::TIMING)},
  {"max_inputs", static_cast<int>(e_cluster_seed::MAX_INPUTS)},
  {"blend", static_cast<int>(e_cluster_seed::BLEND)},
  {"max_pins", static_cast<int>(e_cluster_seed::MAX_PINS)},
  {"max_input_pins", static_cast<int>(e_cluster_seed::MAX_INPUT_PINS)},
  {"blend2", static_cast<int>(e_cluster_seed::BLEND2)},
};

} /* End namespace openfpga */

#endif
