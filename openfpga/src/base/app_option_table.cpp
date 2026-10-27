/********************************************************************
 * The application options and their default values.
 *
 * This is not a 1:1 mirror of the options of VPR (t_options in
 * vtr-verilog-to-routing/vpr/src/base/read_options.h). The names are
 * the ones of the shell, and some options are left out on purpose:
 * - Input files that have their own command: ArchFile, CircuitName,
 *   PlaceFile, RouteFile, ActFile, CmosTechFile, SDCFile, ...
 * - Stage switches (do_packing, do_placement, do_routing, do_analysis,
 *   ...): the stage commands set them
 * - Run control: show_help, show_version, show_graphics, strict_checks,
 *   server mode, ... They do not apply to a flow driven by commands
 * - NoC options, which are not supported
 * - Verbosity options: they belong to the commands
 * Options of the OpenFPGA shell only: build_fabric.*
 *
 * The groups of options, with the prefix of their names:
 *   filename, general, atom, aplace, cluster, place, tplace, router,
 *   trouter, analysis, build_fabric
 *******************************************************************/
#include "app_option_selection_map.h"
#include "app_options.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

using AppOptionEntry = std::pair<std::string, AppOptionValue>;

/* An entry of a group, in the form {"name", value} */
#define STRING_APP_OPTION(name, value, help_message) \
  {#name, AppOptionValue::make_string(value, help_message)},
#define INT_APP_OPTION(name, value, help_message) \
  {#name, AppOptionValue::make_int(value, help_message)},
#define FLOAT_APP_OPTION(name, value, help_message) \
  {#name, AppOptionValue::make_float(value, help_message)},
#define BOOLEAN_APP_OPTION(name, value, help_message) \
  {#name, AppOptionValue::make_boolean(value, help_message)},
#define SELECTION_APP_OPTION(name, value, selection_values, help_message) \
  {#name,                                                                 \
   AppOptionValue::make_selection(value, selection_values, help_message)},

// clang-format off
#define FILE_NAMES_OPTIONS_FIELDS \
  STRING_APP_OPTION(arch_file, "", "Architecture file") \
  STRING_APP_OPTION(circuit_name, "", "Circuit name") \
  STRING_APP_OPTION(net_file, "", "Netlist file") \
  STRING_APP_OPTION(flat_place_file, "", "Flat placement file") \
  STRING_APP_OPTION(place_file, "", "Placement file") \
  STRING_APP_OPTION(route_file, "", "Routing file") \
  STRING_APP_OPTION(circuit_file, "", "Circuit file") \
  STRING_APP_OPTION(act_file, "", "Activity file") \
  STRING_APP_OPTION(power_file, "", "Power file") \
  STRING_APP_OPTION(cmos_tech_file, "", "CMOS technology file") \
  STRING_APP_OPTION(sdc_file, "", "SDC file") \
  STRING_APP_OPTION(out_file_prefix, "", "Output file prefix") \
  STRING_APP_OPTION(constraints_file, "", "Constraints file") \
  STRING_APP_OPTION(write_rr_graph_file, "", "RR graph output file") \
  STRING_APP_OPTION(read_rr_graph_file, "", "RR graph input file") \
  STRING_APP_OPTION(read_rr_edge_override_file, "", "RR edge override input file") \
  STRING_APP_OPTION(write_initial_place_file, "", "Initial placement output file") \
  STRING_APP_OPTION(read_initial_place_file, "", "Initial placement input file") \
  STRING_APP_OPTION(read_vpr_constraints_file, "", "VPR constraints input file") \
  STRING_APP_OPTION(write_vpr_constraints_file, "", "VPR constraints output file") \
  STRING_APP_OPTION(write_constraints_file, "", "Constraints output file") \
  STRING_APP_OPTION(read_flat_place_file, "", "Flat placement input file") \
  STRING_APP_OPTION(write_flat_place_file, "", "Flat placement output file") \
  STRING_APP_OPTION(write_legalized_flat_place_file, "", "Legalized flat placement output file") \
  STRING_APP_OPTION(write_placement_delay_lookup, "", "Placement delay lookup output file") \
  STRING_APP_OPTION(read_placement_delay_lookup, "", "Placement delay lookup input file") \
  STRING_APP_OPTION(write_router_lookahead, "", "Router lookahead output file") \
  STRING_APP_OPTION(read_router_lookahead, "", "Router lookahead input file") \
  STRING_APP_OPTION(write_intra_cluster_router_lookahead, "", "Intra-cluster router lookahead output file") \
  STRING_APP_OPTION(read_intra_cluster_router_lookahead, "", "Intra-cluster router lookahead input file") \
  STRING_APP_OPTION(write_block_usage, "", "Block usage output file")

#define DEFINE_GENERAL_OPTIONS_FIELDS \
  INT_APP_OPTION(num_workers, 1, "Number of worker threads") \
  BOOLEAN_APP_OPTION(timing_analysis, true, "Enable timing analysis") \
  SELECTION_APP_OPTION(timing_update_type, "auto", timing_update_type_map, "Timing update mode") \
  STRING_APP_OPTION(device_layout, "auto", "Device layout name or 'auto'") \
  FLOAT_APP_OPTION(target_device_utilization, 0.8, "Target device utilization") \
  SELECTION_APP_OPTION(constant_net_method, "global", constant_net_method_map, "Constant-net handling method") \
  SELECTION_APP_OPTION(clock_modeling, "ideal", clock_modeling_map, "Clock modeling mode") \
  BOOLEAN_APP_OPTION(two_stage_clock_routing, false, "Enable two-stage clock routing") \
  STRING_APP_OPTION(disable_errors, "", "Colon-separated list of errors to disable") \
  STRING_APP_OPTION(suppress_warnings, "", "Warnings to suppress") \
  BOOLEAN_APP_OPTION(allow_dangling_combinational_nodes, false, "Allow dangling combinational nodes") \
  BOOLEAN_APP_OPTION(terminate_if_timing_fails, false, "Terminate if timing analysis fails")

#define DEFINE_ATOM_NETLIST_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(absorb_buffer_luts, true, "Whether to absorb buffer LUTs into their fanout logic") \
  SELECTION_APP_OPTION(const_gen_inference, "comb_seq", const_gen_inference_map, "The level of constant generator inference to perform") \
  BOOLEAN_APP_OPTION(sweep_dangling_primary_ios, true, "Whether to sweep dangling primary inputs and outputs") \
  BOOLEAN_APP_OPTION(sweep_dangling_nets, true, "Whether to sweep dangling nets") \
  BOOLEAN_APP_OPTION(sweep_dangling_blocks, true, "Whether to sweep dangling blocks") \
  BOOLEAN_APP_OPTION(sweep_constant_primary_outputs, false, "Whether to sweep constant primary outputs")

#define DEFINE_ANALYTICAL_PLACEMENT_OPTIONS_FIELDS \
  SELECTION_APP_OPTION(ap_analytical_solver, "Identity", ap_analytical_solver_map, "Analytical solver to use") \
  SELECTION_APP_OPTION(ap_partial_legalizer, "Identity", ap_partial_legalizer_map, "Partial legalizer to use") \
  SELECTION_APP_OPTION(ap_full_legalizer, "Naive", ap_full_legalizer_map, "Full legalizer to use") \
  SELECTION_APP_OPTION(ap_detailed_placer, "Identity", ap_detailed_placer_map, "Detailed placer to use") \
  STRING_APP_OPTION(ap_partial_legalizer_target_density, "", "Target density for the partial legalizer") \
  STRING_APP_OPTION(appack_max_dist_th, "", "APPack max distance threshold") \
  STRING_APP_OPTION(appack_unrelated_clustering_args, "", "Arguments for APPack unrelated clustering") \
  FLOAT_APP_OPTION(ap_timing_tradeoff, 0, "Timing tradeoff value") \
  INT_APP_OPTION(ap_high_fanout_threshold, 0, "High-fanout threshold") \
  BOOLEAN_APP_OPTION(ap_generate_mass_report, false, "Generate analytical placement mass report")

#define DEFINE_CLUSTERING_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(connection_driven_clustering, true, "Enable connection-driven clustering") \
  SELECTION_APP_OPTION(allow_unrelated_clustering, "auto", clustering_allow_unrelated_clustering_map, "Allow unrelated clustering") \
  FLOAT_APP_OPTION(timing_gain_weight, 0.75, "Timing gain weight") \
  FLOAT_APP_OPTION(connection_gain_weight, 0.9, "Connection gain weight") \
  BOOLEAN_APP_OPTION(timing_driven_clustering, true, "Enable timing-driven clustering") \
  SELECTION_APP_OPTION(cluster_seed_type, "blend2", clustering_cluster_seed_type_map, "Seed type for clustering") \
  BOOLEAN_APP_OPTION(enable_clustering_pin_feasibility_filter, false, "Enable clustering pin feasibility filter") \
  SELECTION_APP_OPTION(balance_block_type_utilization, "auto", clustering_balance_block_type_utilization_map, "Balance block type utilization") \
  STRING_APP_OPTION(target_external_pin_util, "auto", "Target external input pin utilization") \
  BOOLEAN_APP_OPTION(pack_prioritize_transitive_connectivity, false, "Prioritize transitive connectivity while packing") \
  INT_APP_OPTION(pack_transitive_fanout_threshold, 0, "Transitive fanout threshold") \
  INT_APP_OPTION(pack_feasible_block_array_size, 0, "Feasible block array size") \
  INT_APP_OPTION(pack_high_fanout_threshold, 0, "Pack high-fanout threshold") \
  BOOLEAN_APP_OPTION(memoize_cluster_packings, false, "Memoize cluster packings")

#define DEFINE_PLACEMENT_OPTIONS_FIELDS \
  INT_APP_OPTION(seed, 1, "Placement random seed") \
  BOOLEAN_APP_OPTION(show_place_timing, false, "Show placement timing information") \
  FLOAT_APP_OPTION(place_inner_num, 0.5, "Placement inner loop count") \
  FLOAT_APP_OPTION(place_auto_init_t_scale, 0.5, "Auto initial temperature scale") \
  FLOAT_APP_OPTION(place_init_t, 100, "Initial placement temperature") \
  FLOAT_APP_OPTION(place_exit_t, 0.5, "Exit placement temperature") \
  FLOAT_APP_OPTION(place_alpha_t, 0.5, "Placement alpha temperature value") \
  SELECTION_APP_OPTION(place_init_t_estimator, "cost_variance", place_init_t_estimator_map, "Initial temperature estimator") \
  \
  SELECTION_APP_OPTION(anneal_sched_type, "auto", anneal_sched_type_map, "Annealing schedule type") \
  SELECTION_APP_OPTION(place_algorithm, "criticality_timing", place_algorithm_map, "Placement algorithm") \
  SELECTION_APP_OPTION(place_quench_algorithm, "criticality_timing", place_algorithm_map, "Placement quench algorithm") \
  SELECTION_APP_OPTION(pad_loc_type, "free", pad_loc_type_map, "Pad location strategy") \
  INT_APP_OPTION(place_chan_width, 100, "Channel width hint for placement") \
  FLOAT_APP_OPTION(place_rlim_escape_fraction, 0.0, "RLIM escape fraction") \
  STRING_APP_OPTION(place_move_stats_file, "", "Placement move statistics output file") \
  INT_APP_OPTION(placement_saves_per_temperature, 0, "Number of placement saves per temperature") \
  SELECTION_APP_OPTION(place_effort_scaling, "circuit", place_effort_scaling_map, "Placement effort scaling mode") \
  SELECTION_APP_OPTION(place_delta_delay_matrix_calculation_method, "astar", place_delta_delay_algorithm_map, "Delta delay calculation method") \
  STRING_APP_OPTION(place_static_move_prob, "0.0", "Static move probability") \
  INT_APP_OPTION(place_high_fanout_net, 0, "High-fanout net threshold") \
  SELECTION_APP_OPTION(place_placement_freq, "once", place_placement_freq_map, "Placement frequency") \
  \
  BOOLEAN_APP_OPTION(RL_agent_placement, false, "Enable RL agent placement") \
  BOOLEAN_APP_OPTION(place_agent_multistate, false, "Enable multistate RL agent") \
  BOOLEAN_APP_OPTION(place_checkpointing, false, "Enable placement checkpointing") \
  STRING_APP_OPTION(place_agent_epsilon, "0.0", "RL agent epsilon") \
  STRING_APP_OPTION(place_agent_gamma, "0.0", "RL agent gamma") \
  STRING_APP_OPTION(place_dm_rlim, "0.0", "RLIM value for dynamic moves") \
  SELECTION_APP_OPTION(place_agent_space, "move_type", place_agent_space_map, "RL agent action space") \
  SELECTION_APP_OPTION(place_agent_algorithm, "e_greedy", place_agent_algorithm_map, "RL agent algorithm") \
  STRING_APP_OPTION(place_reward_fun, "", "Placement reward function") \
  STRING_APP_OPTION(place_crit_limit, "0.0", "Criticality limit") \
  INT_APP_OPTION(place_constraint_expand, 0, "Constraint expansion value") \
  BOOLEAN_APP_OPTION(place_constraint_subtile, false, "Enable subtile placement constraint") \
  INT_APP_OPTION(floorplan_num_horizontal_partitions, 0, "Number of horizontal floorplan partitions") \
  INT_APP_OPTION(floorplan_num_vertical_partitions, 0, "Number of vertical floorplan partitions") \
  BOOLEAN_APP_OPTION(place_quench_only, false, "Run quench stage only") \
  \
  INT_APP_OPTION(placer_debug_block, 0, "Debug placer at the block level") \
  INT_APP_OPTION(placer_debug_net, 0, "Debug placer at the net level")

#define DEFINE_TIMING_PLACEMENT_OPTIONS_FIELDS \
  FLOAT_APP_OPTION(place_congestion_factor, 0, "Placement congestion factor") \
  FLOAT_APP_OPTION(place_congestion_rlim_trigger_ratio, 0, "Congestion RLIM trigger ratio") \
  FLOAT_APP_OPTION(place_congestion_chan_util_threshold, 0, "Congestion channel utilization threshold") \
  FLOAT_APP_OPTION(place_timing_tradeoff, 0.5, "Timing tradeoff for placement") \
  INT_APP_OPTION(recompute_crit_iter, 1, "Criticality recompute interval") \
  INT_APP_OPTION(inner_loop_recompute_divider, 0, "Inner-loop recompute divider") \
  INT_APP_OPTION(quench_recompute_divider, 0, "Quench recompute divider") \
  FLOAT_APP_OPTION(place_exp_first, 1, "Initial exponent for timing placement") \
  FLOAT_APP_OPTION(place_exp_last, 8, "Final exponent for timing placement") \
  FLOAT_APP_OPTION(place_delay_offset, 0, "Placement delay offset") \
  INT_APP_OPTION(place_delay_ramp_delta_threshold, -1, "Placement delay ramp delta threshold") \
  FLOAT_APP_OPTION(place_delay_ramp_slope, 0, "Placement delay ramp slope") \
  FLOAT_APP_OPTION(place_tsu_rel_margin, 1, "Relative TSU margin") \
  FLOAT_APP_OPTION(place_tsu_abs_margin, 0, "Absolute TSU margin") \
  STRING_APP_OPTION(post_place_timing_report_file, "", "Post-placement timing report output file") \
  SELECTION_APP_OPTION(place_delay_model, "simple", timing_place_delay_model_map, "Timing placement delay model") \
  SELECTION_APP_OPTION(place_delay_model_reducer, "min", timing_place_delay_model_reducer_map, "Delay model reducer") \
  STRING_APP_OPTION(allowed_tiles_for_delay_model, "", "Allowed tiles for the delay model")

#define DEFINE_ROUTER_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(check_rr_graph, true, "Check RR graph consistency") \
  INT_APP_OPTION(max_router_iterations, 50, "Maximum router iterations") \
  FLOAT_APP_OPTION(first_iter_pres_fac, 0, "First-iteration present factor") \
  FLOAT_APP_OPTION(initial_pres_fac, 0.5, "Initial present factor") \
  FLOAT_APP_OPTION(pres_fac_mult, 1.3, "Present factor multiplier") \
  FLOAT_APP_OPTION(max_pres_fac, 1000, "Maximum present factor") \
  FLOAT_APP_OPTION(acc_fac, 1, "Accumulated factor") \
  INT_APP_OPTION(bb_factor, 3, "Routing bounding box factor") \
  SELECTION_APP_OPTION(base_cost_type, "delay_normalized_length", router_base_cost_type_map, "Router base cost type") \
  FLOAT_APP_OPTION(bend_cost, 0, "Routing bend cost") \
  SELECTION_APP_OPTION(route_type, "detailed", router_route_type_map, "Route type") \
  INT_APP_OPTION(route_chan_width, 100, "Routing channel width") \
  INT_APP_OPTION(min_route_chan_width_hint, 0, "Minimum route channel width hint") \
  BOOLEAN_APP_OPTION(verify_binary_search, false, "Verify binary search on channel width") \
  SELECTION_APP_OPTION(router_algorithm, "timing_driven", router_algorithm_map, "Router algorithm") \
  INT_APP_OPTION(min_incremental_reroute_fanout, 16, "Minimum fanout for incremental reroute") \
  BOOLEAN_APP_OPTION(read_rr_edge_metadata, false, "Read RR edge metadata") \
  BOOLEAN_APP_OPTION(exit_after_first_routing_iteration, false, "Exit after first routing iteration") \
  SELECTION_APP_OPTION(check_route, "full", router_check_route_map, "Route checking level") \
  INT_APP_OPTION(max_logged_overused_rr_nodes, 0, "Maximum logged overused RR nodes") \
  BOOLEAN_APP_OPTION(generate_rr_node_overuse_report, false, "Generate RR-node overuse report") \
  SELECTION_APP_OPTION(reorder_rr_graph_nodes_algorithm, "none", router_reorder_rr_graph_nodes_algorithm_map, "RR-node reordering algorithm") \
  INT_APP_OPTION(reorder_rr_graph_nodes_threshold, 0, "RR-node reordering threshold") \
  INT_APP_OPTION(reorder_rr_graph_nodes_seed, 1, "RR-node reordering seed") \
  BOOLEAN_APP_OPTION(flat_routing, false, "Enable flat routing") \
  BOOLEAN_APP_OPTION(router_opt_choke_points, true, "Enable router choke-point optimization")

#define DEFINE_TIMING_ROUTER_OPTIONS_FIELDS \
  FLOAT_APP_OPTION(astar_fac, 1.2, "A* factor") \
  FLOAT_APP_OPTION(astar_offset, 0, "A* offset") \
  FLOAT_APP_OPTION(router_profiler_astar_fac, 1.2, "Router profiler A* factor") \
  BOOLEAN_APP_OPTION(enable_parallel_connection_router, false, "Enable parallel connection router") \
  FLOAT_APP_OPTION(post_target_prune_fac, 1.2, "Post-target prune factor") \
  FLOAT_APP_OPTION(post_target_prune_offset, 0, "Post-target prune offset") \
  INT_APP_OPTION(multi_queue_num_threads, 1, "Number of multi-queue routing threads") \
  INT_APP_OPTION(multi_queue_num_queues, 2, "Number of multi-queue routing queues") \
  BOOLEAN_APP_OPTION(multi_queue_direct_draining, false, "Enable direct draining for multi-queue router") \
  FLOAT_APP_OPTION(max_criticality, 0.99, "Maximum criticality") \
  FLOAT_APP_OPTION(criticality_exp, 1, "Criticality exponent") \
  FLOAT_APP_OPTION(router_init_wirelength_abort_threshold, 0.85, "Wirelength abort threshold for router initialization") \
  SELECTION_APP_OPTION(incr_reroute_delay_ripup, "auto", timing_router_incr_reroute_delay_ripup_map, "Incremental reroute delay ripup mode") \
  SELECTION_APP_OPTION(routing_failure_predictor, "safe", timing_router_routing_failure_predictor_map, "Routing failure predictor") \
  SELECTION_APP_OPTION(routing_budgets_algorithm, "disable", timing_router_routing_budgets_algorithm_map, "Routing budgets algorithm") \
  BOOLEAN_APP_OPTION(save_routing_per_iteration, false, "Save routing results for each iteration") \
  FLOAT_APP_OPTION(congested_routing_iteration_threshold_frac, 1.0, "Congested routing iteration threshold fraction") \
  SELECTION_APP_OPTION(route_bb_update, "dynamic", timing_router_route_bb_update_map, "Route bounding-box update mode") \
  INT_APP_OPTION(router_high_fanout_threshold, 64, "Router high-fanout threshold") \
  STRING_APP_OPTION(router_high_fanout_max_slope, "0.0", "Maximum high-fanout slope") \
  INT_APP_OPTION(router_debug_net, -2, "Router debug net id") \
  INT_APP_OPTION(router_debug_sink_rr, -2, "Router debug sink RR node") \
  INT_APP_OPTION(router_debug_iteration, -2, "Router debug iteration") \
  SELECTION_APP_OPTION(router_lookahead_type, "map", timing_router_lookahead_type_map, "Router lookahead type") \
  BOOLEAN_APP_OPTION(generate_router_lookahead_report, false, "Generate router lookahead report") \
  FLOAT_APP_OPTION(router_initial_acc_cost_chan_congestion_threshold, 0.5, "Initial accumulated cost congestion threshold") \
  FLOAT_APP_OPTION(router_initial_acc_cost_chan_congestion_weight, 0.5, "Initial accumulated cost congestion weight") \
  INT_APP_OPTION(router_max_convergence_count, 1, "Maximum router convergence count") \
  FLOAT_APP_OPTION(router_reconvergence_cpd_threshold, 0.99, "Router reconvergence CPD threshold") \
  BOOLEAN_APP_OPTION(router_update_lower_bound_delays, true, "Update lower-bound delays during routing") \
  STRING_APP_OPTION(router_first_iteration_timing_report_file, "", "First-iteration timing report output file") \
  SELECTION_APP_OPTION(router_initial_timing, "lookahead", timing_router_initial_timing_map, "Initial timing mode") \
  SELECTION_APP_OPTION(router_heap, "four_ary", timing_router_heap_map, "Router heap implementation")

#define DEFINE_ANALYSIS_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(full_stats, false, "Enable full analysis statistics") \
  BOOLEAN_APP_OPTION(generate_post_synthesis_netlist, false, "Generate the post-synthesis netlist") \
  BOOLEAN_APP_OPTION(generate_post_implementation_merged_netlist, false, "Generate the merged post-implementation netlist") \
  BOOLEAN_APP_OPTION(generate_post_implementation_sdc, false, "Generate the post-implementation SDC") \
  INT_APP_OPTION(timing_report_npaths, 100, "Number of timing paths to report") \
  SELECTION_APP_OPTION(timing_report_detail, "netlist", analysis_timing_report_detail_map, "Timing report detail level") \
  BOOLEAN_APP_OPTION(timing_report_skew, false, "Report timing skew") \
  STRING_APP_OPTION(echo_dot_timing_graph_node, "-1", "Timing graph node to echo in DOT format") \
  SELECTION_APP_OPTION(post_synth_netlist_unconn_input_handling, "unconnected", analysis_post_synth_netlist_unconn_handling_map, "Handling for unconnected post-synthesis inputs") \
  SELECTION_APP_OPTION(post_synth_netlist_unconn_output_handling, "unconnected", analysis_post_synth_netlist_unconn_handling_map, "Handling for unconnected post-synthesis outputs") \
  BOOLEAN_APP_OPTION(post_synth_netlist_module_parameters, true, "Write post-synthesis netlist module parameters") \
  STRING_APP_OPTION(write_timing_summary, "", "Timing summary output file") \
  BOOLEAN_APP_OPTION(skip_sync_clustering_and_routing_results, false, "Skip syncing clustering and routing results") \
  BOOLEAN_APP_OPTION(generate_net_timing_report, false, "Generate a per-net timing report")

#define DEFINE_BUILD_FABRIC_OPTIONS_FIELDS \
  INT_APP_OPTION(gsb_version, 0, "GSB version") \
  BOOLEAN_APP_OPTION(reorder_incoming_edges, true, "Reorder incoming edges") \
  BOOLEAN_APP_OPTION(sort_gsb_chan_node_in_edges, true, "Sort GSB channel-node incoming edges")

// clang-format on

/* Add the options of a group to a table, with the prefix of the group */
void add_group(std::map<std::string, AppOptionValue>& table,
               const std::string& prefix,
               const std::vector<AppOptionEntry>& group) {
  for (const AppOptionEntry& entry : group) {
    table[prefix + "." + entry.first] = entry.second;
  }
}

}  // End anonymous namespace

const std::map<std::string, AppOptionValue>& app_option_defaults() {
  static const std::map<std::string, AppOptionValue> defaults = [] {
    std::map<std::string, AppOptionValue> table;
    add_group(table, "filename", {FILE_NAMES_OPTIONS_FIELDS});
    add_group(table, "general", {DEFINE_GENERAL_OPTIONS_FIELDS});
    add_group(table, "atom", {DEFINE_ATOM_NETLIST_OPTIONS_FIELDS});
    add_group(table, "aplace", {DEFINE_ANALYTICAL_PLACEMENT_OPTIONS_FIELDS});
    add_group(table, "cluster", {DEFINE_CLUSTERING_OPTIONS_FIELDS});
    add_group(table, "place", {DEFINE_PLACEMENT_OPTIONS_FIELDS});
    add_group(table, "tplace", {DEFINE_TIMING_PLACEMENT_OPTIONS_FIELDS});
    add_group(table, "router", {DEFINE_ROUTER_OPTIONS_FIELDS});
    add_group(table, "trouter", {DEFINE_TIMING_ROUTER_OPTIONS_FIELDS});
    add_group(table, "analysis", {DEFINE_ANALYSIS_OPTIONS_FIELDS});
    add_group(table, "build_fabric", {DEFINE_BUILD_FABRIC_OPTIONS_FIELDS});
    return table;
  }();
  return defaults;
}

#undef STRING_APP_OPTION
#undef INT_APP_OPTION
#undef FLOAT_APP_OPTION
#undef BOOLEAN_APP_OPTION
#undef SELECTION_APP_OPTION

} /* End namespace openfpga */
