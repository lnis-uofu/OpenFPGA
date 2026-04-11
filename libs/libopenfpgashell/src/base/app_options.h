#pragma once

#include <array>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>

#include "app_option_selection_map.h"
#include "vpr_types.h"

/* Keep these option declarations in sync with
 * OpenfpgaShell::setup_default_app_options() in `openfpga_shell.cpp`.
 */

template <typename T>
using app_option_storage_t =
  std::conditional_t<std::is_convertible_v<std::decay_t<T>, const char*>,
                     std::string, std::decay_t<T>>;

// template <typename T>
// struct AppOptionValue {
//   T value{};
//   std::string option_name_;
//   std::string help_message_;
//   std::map<std::string, int> selection_values;

//   AppOptionValue(T value, std::string option_name,
//                  std::string help_message = "")
//     : value(value), option_name_(option_name), help_message_(help_message) {}

//   AppOptionValue(std::string value, std::map<std::string, int>
//   selection_values,
//                  std::string option_name, std::string help_message = "")
//     : value(value),
//       option_name_(option_name),
//       help_message_(help_message),
//       selection_values(selection_values) {}
// };

struct AppOptionValue {
  enum e_type {
    EMPTY,
    STRING,
    INT,
    FLOAT,
    BOOLEAN,
    SELECTION,
  };

  e_type type = EMPTY;
  std::string help_message;
  std::string string_value;
  std::map<std::string, int> selection_values;
  int int_value = 0;
  float float_value = 0.f;
  bool bool_value = false;

  static AppOptionValue make_empty() {
    AppOptionValue value;
    value.type = EMPTY;
    return value;
  }

  static AppOptionValue make_string(const std::string& string_value,
                                    std::string help_message = "") {
    AppOptionValue value;
    value.type = STRING;
    value.string_value = string_value;
    value.help_message = help_message;
    return value;
  }

  static AppOptionValue make_int(const int int_value,
                                 std::string help_message = "") {
    AppOptionValue value;
    value.type = INT;
    value.int_value = int_value;
    value.help_message = help_message;
    return value;
  }

  static AppOptionValue make_boolean(const bool bool_value,
                                     std::string help_message = "") {
    AppOptionValue value;
    value.type = BOOLEAN;
    value.bool_value = bool_value;
    value.help_message = help_message;
    return value;
  }

  static AppOptionValue make_float(const float float_value,
                                   std::string help_message = "") {
    AppOptionValue value;
    value.type = FLOAT;
    value.float_value = float_value;
    value.help_message = help_message;
    return value;
  }

  static AppOptionValue make_selection(
    const std::string& selected_value,
    const std::map<std::string, int>& selection_enums,
    std::string help_message = "") {
    AppOptionValue value;
    value.type = SELECTION;
    value.string_value = selected_value;
    value.selection_values = selection_enums;
    value.help_message = help_message;
    const auto selected_it = value.selection_values.find(selected_value);
    if (selected_it != value.selection_values.end()) {
      value.int_value = selected_it->second;
    }
    return value;
  }

  int to_enum() const {
    if (SELECTION != type) {
      return 0;
    }

    const auto selected_it = selection_values.find(string_value);
    if (selected_it == selection_values.end()) {
      return 0;
    }

    return selected_it->second;
  }

  std::string to_string() const {
    switch (type) {
      case STRING:
        return string_value;
      case INT:
        return std::to_string(int_value);
      case BOOLEAN:
        return bool_value ? std::string("true") : std::string("false");
      case FLOAT:
        return std::to_string(float_value);
      case SELECTION:
        return string_value;
      default:
        return std::string();
    }
  }

  void update(const std::string& new_value) {
    if (type == SELECTION) {
      const auto it = selection_values.find(new_value);
      if (it == selection_values.end()) {
        return; /* invalid selection — caller should validate first */
      }
      string_value = new_value;
      int_value = it->second;
    } else {
      string_value = new_value;
    }
  }

  void update(const int new_value) { int_value = new_value; }

  void update(const float new_value) { float_value = new_value; }

  void update(const bool new_value) { bool_value = new_value; }
};

#define STRING_APP_OPTION(name, value, help_message) \
  AppOptionValue name = AppOptionValue::make_string(value, help_message);

#define INT_APP_OPTION(name, value, help_message) \
  AppOptionValue name = AppOptionValue::make_int(value, help_message);

#define FLOAT_APP_OPTION(name, value, help_message) \
  AppOptionValue name = AppOptionValue::make_float(value, help_message);

#define BOOLEAN_APP_OPTION(name, value, help_message) \
  AppOptionValue name = AppOptionValue::make_boolean(value, help_message);

#define SELECTION_APP_OPTION(name, value, selection_values, help_message) \
  AppOptionValue name =                                                   \
    AppOptionValue::make_selection(value, selection_values, help_message);

// clang-format off
#define DEFINE_ATOM_NETLIST_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(absorb_buffer_luts, false, "Whether to absorb buffer LUTs into their fanout logic") \
  SELECTION_APP_OPTION(const_gen_inference, "none", const_gen_inference_map, "The level of constant generator inference to perform") \
  BOOLEAN_APP_OPTION(sweep_dangling_primary_ios, false, "Whether to sweep dangling primary inputs and outputs") \
  BOOLEAN_APP_OPTION(sweep_dangling_nets, false, "Whether to sweep dangling nets") \
  BOOLEAN_APP_OPTION(sweep_dangling_blocks, false, "Whether to sweep dangling blocks") \
  BOOLEAN_APP_OPTION(sweep_constant_primary_outputs, false, "Whether to sweep constant primary outputs")

#define DEFINE_ANALYTICAL_PLACEMENT_OPTIONS_FIELDS \
  SELECTION_APP_OPTION(ap_analytical_solver, "Identity", ap_analytical_solver_map, "Analytical solver to use") \
  SELECTION_APP_OPTION(ap_partial_legalizer, "Identity", ap_partial_legalizer_map, "Partial legalizer to use") \
  SELECTION_APP_OPTION(ap_full_legalizer, "Naive", ap_full_legalizer_map, "Full legalizer to use") \
  SELECTION_APP_OPTION(ap_detailed_placer, "Identity", ap_detailed_placer_map, "Detailed placer to use") \
  STRING_APP_OPTION(ap_partial_legalizer_target_density, "", "Target density for the partial legalizer") \
  STRING_APP_OPTION(appack_max_dist_th, "", "APPack max distance threshold") \
  STRING_APP_OPTION(appack_unrelated_clustering_args, "", "Arguments for APPack unrelated clustering") \
  STRING_APP_OPTION(ap_timing_tradeoff, "0.0", "Timing tradeoff value") \
  INT_APP_OPTION(ap_high_fanout_threshold, 0, "High-fanout threshold") \
  BOOLEAN_APP_OPTION(ap_generate_mass_report, false, "Generate analytical placement mass report")

#define DEFINE_CLUSTERING_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(connection_driven_clustering, false, "Enable connection-driven clustering") \
  SELECTION_APP_OPTION(allow_unrelated_clustering, "auto", clustering_allow_unrelated_clustering_map, "Allow unrelated clustering") \
  STRING_APP_OPTION(timing_gain_weight, "0.0", "Timing gain weight") \
  STRING_APP_OPTION(connection_gain_weight, "0.0", "Connection gain weight") \
  BOOLEAN_APP_OPTION(timing_driven_clustering, false, "Enable timing-driven clustering") \
  SELECTION_APP_OPTION(cluster_seed_type, "timing", clustering_cluster_seed_type_map, "Seed type for clustering") \
  BOOLEAN_APP_OPTION(enable_clustering_pin_feasibility_filter, false, "Enable clustering pin feasibility filter") \
  SELECTION_APP_OPTION(balance_block_type_utilization, "auto", clustering_balance_block_type_utilization_map, "Balance block type utilization") \
  STRING_APP_OPTION(target_external_pin_util, "auto", "Target external input pin utilization") \
  BOOLEAN_APP_OPTION(pack_prioritize_transitive_connectivity, false, "Prioritize transitive connectivity while packing") \
  STRING_APP_OPTION(pack_transitive_fanout_threshold, "0", "Transitive fanout threshold") \
  STRING_APP_OPTION(pack_feasible_block_array_size, "0", "Feasible block array size") \
  STRING_APP_OPTION(pack_high_fanout_threshold, "0", "Pack high-fanout threshold") \
  BOOLEAN_APP_OPTION(memoize_cluster_packings, false, "Memoize cluster packings") \
  STRING_APP_OPTION(net_file, "$DESIGN_.net", "Output net file") \
  STRING_APP_OPTION(circuit_file, "", "Input circuit file") \
  BOOLEAN_APP_OPTION(pack_verbosity, false, "Enable verbose output during packing") \

#define DEFINE_PLACEMENT_OPTIONS_FIELDS \
  INT_APP_OPTION(seed, 0, "Placement random seed") \
  BOOLEAN_APP_OPTION(show_place_timing, false, "Show placement timing information") \
  STRING_APP_OPTION(place_inner_num, "0.0", "Placement inner loop count") \
  STRING_APP_OPTION(place_auto_init_t_scale, "0.0", "Auto initial temperature scale") \
  STRING_APP_OPTION(place_init_t, "0.0", "Initial placement temperature") \
  STRING_APP_OPTION(place_exit_t, "0.0", "Exit placement temperature") \
  STRING_APP_OPTION(place_alpha_t, "0.0", "Placement alpha temperature value") \
  SELECTION_APP_OPTION(place_init_t_estimator, "cost_variance", place_init_t_estimator_map, "Initial temperature estimator") \
  SELECTION_APP_OPTION(anneal_sched_type, "auto", anneal_sched_type_map, "Annealing schedule type") \
  SELECTION_APP_OPTION(place_algorithm, "criticality_timing", place_algorithm_map, "Placement algorithm") \
  SELECTION_APP_OPTION(place_quench_algorithm, "criticality_timing", place_algorithm_map, "Placement quench algorithm") \
  SELECTION_APP_OPTION(pad_loc_type, "free", pad_loc_type_map, "Pad location strategy") \
  INT_APP_OPTION(place_chan_width, 0, "Channel width hint for placement") \
  STRING_APP_OPTION(place_rlim_escape_fraction, "0.0", "RLIM escape fraction") \
  STRING_APP_OPTION(place_move_stats_file, "", "Placement move statistics output file") \
  INT_APP_OPTION(placement_saves_per_temperature, 0, "Number of placement saves per temperature") \
  SELECTION_APP_OPTION(place_effort_scaling, "circuit", place_effort_scaling_map, "Placement effort scaling mode") \
  SELECTION_APP_OPTION(place_delta_delay_matrix_calculation_method, "astar", place_delta_delay_algorithm_map, "Delta delay calculation method") \
  STRING_APP_OPTION(place_static_move_prob, "0.0", "Static move probability") \
  INT_APP_OPTION(place_high_fanout_net, 0, "High-fanout net threshold") \
  SELECTION_APP_OPTION(place_bounding_box_mode, "auto_bb", place_bounding_box_mode_map, "Bounding box mode") \
  SELECTION_APP_OPTION(place_placement_freq, "once", place_placement_freq_map, "Placement frequency") \
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
  BOOLEAN_APP_OPTION(place_quench_only, false, "Run quench stage only")

#define DEFINE_TIMING_PLACEMENT_OPTIONS_FIELDS \
  STRING_APP_OPTION(place_congestion_factor, "0.0", "Placement congestion factor") \
  STRING_APP_OPTION(place_congestion_rlim_trigger_ratio, "0.0", "Congestion RLIM trigger ratio") \
  STRING_APP_OPTION(place_congestion_chan_util_threshold, "0.0", "Congestion channel utilization threshold") \
  STRING_APP_OPTION(place_timing_tradeoff, "0.0", "Timing tradeoff for placement") \
  INT_APP_OPTION(recompute_crit_iter, 0, "Criticality recompute interval") \
  INT_APP_OPTION(inner_loop_recompute_divider, 0, "Inner-loop recompute divider") \
  INT_APP_OPTION(quench_recompute_divider, 0, "Quench recompute divider") \
  STRING_APP_OPTION(place_exp_first, "0.0", "Initial exponent for timing placement") \
  STRING_APP_OPTION(place_exp_last, "0.0", "Final exponent for timing placement") \
  STRING_APP_OPTION(place_delay_offset, "0.0", "Placement delay offset") \
  INT_APP_OPTION(place_delay_ramp_delta_threshold, 0, "Placement delay ramp delta threshold") \
  STRING_APP_OPTION(place_delay_ramp_slope, "0.0", "Placement delay ramp slope") \
  STRING_APP_OPTION(place_tsu_rel_margin, "0.0", "Relative TSU margin") \
  STRING_APP_OPTION(place_tsu_abs_margin, "0.0", "Absolute TSU margin") \
  STRING_APP_OPTION(post_place_timing_report_file, "", "Post-placement timing report output file") \
  SELECTION_APP_OPTION(place_delay_model, "delta", timing_place_delay_model_map, "Timing placement delay model") \
  SELECTION_APP_OPTION(place_delay_model_reducer, "min", timing_place_delay_model_reducer_map, "Delay model reducer") \
  STRING_APP_OPTION(allowed_tiles_for_delay_model, "", "Allowed tiles for the delay model")

#define DEFINE_ROUTER_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(check_rr_graph, false, "Check RR graph consistency") \
  INT_APP_OPTION(max_router_iterations, 0, "Maximum router iterations") \
  STRING_APP_OPTION(first_iter_pres_fac, "0.0", "First-iteration present factor") \
  STRING_APP_OPTION(initial_pres_fac, "0.0", "Initial present factor") \
  STRING_APP_OPTION(pres_fac_mult, "0.0", "Present factor multiplier") \
  STRING_APP_OPTION(max_pres_fac, "0.0", "Maximum present factor") \
  STRING_APP_OPTION(acc_fac, "0.0", "Accumulated factor") \
  INT_APP_OPTION(bb_factor, 0, "Routing bounding box factor") \
  SELECTION_APP_OPTION(base_cost_type, "delay_normalized", router_base_cost_type_map, "Router base cost type") \
  STRING_APP_OPTION(bend_cost, "0.0", "Routing bend cost") \
  SELECTION_APP_OPTION(route_type, "detailed", router_route_type_map, "Route type") \
  INT_APP_OPTION(route_chan_width, 0, "Routing channel width") \
  INT_APP_OPTION(min_route_chan_width_hint, 0, "Minimum route channel width hint") \
  BOOLEAN_APP_OPTION(verify_binary_search, false, "Verify binary search on channel width") \
  SELECTION_APP_OPTION(router_algorithm, "timing_driven", router_algorithm_map, "Router algorithm") \
  INT_APP_OPTION(min_incremental_reroute_fanout, 0, "Minimum fanout for incremental reroute") \
  BOOLEAN_APP_OPTION(read_rr_edge_metadata, false, "Read RR edge metadata") \
  BOOLEAN_APP_OPTION(exit_after_first_routing_iteration, false, "Exit after first routing iteration") \
  SELECTION_APP_OPTION(check_route, "off", router_check_route_map, "Route checking level") \
  INT_APP_OPTION(max_logged_overused_rr_nodes, 0, "Maximum logged overused RR nodes") \
  BOOLEAN_APP_OPTION(generate_rr_node_overuse_report, false, "Generate RR-node overuse report") \
  SELECTION_APP_OPTION(reorder_rr_graph_nodes_algorithm, "none", router_reorder_rr_graph_nodes_algorithm_map, "RR-node reordering algorithm") \
  INT_APP_OPTION(reorder_rr_graph_nodes_threshold, 0, "RR-node reordering threshold") \
  INT_APP_OPTION(reorder_rr_graph_nodes_seed, 0, "RR-node reordering seed") \
  BOOLEAN_APP_OPTION(flat_routing, false, "Enable flat routing") \
  BOOLEAN_APP_OPTION(router_opt_choke_points, false, "Enable router choke-point optimization")

#define DEFINE_TIMING_ROUTER_OPTIONS_FIELDS \
  STRING_APP_OPTION(astar_fac, "0.0", "A* factor") \
  STRING_APP_OPTION(astar_offset, "0.0", "A* offset") \
  STRING_APP_OPTION(router_profiler_astar_fac, "0.0", "Router profiler A* factor") \
  BOOLEAN_APP_OPTION(enable_parallel_connection_router, false, "Enable parallel connection router") \
  STRING_APP_OPTION(post_target_prune_fac, "0.0", "Post-target prune factor") \
  STRING_APP_OPTION(post_target_prune_offset, "0.0", "Post-target prune offset") \
  INT_APP_OPTION(multi_queue_num_threads, 0, "Number of multi-queue routing threads") \
  INT_APP_OPTION(multi_queue_num_queues, 0, "Number of multi-queue routing queues") \
  BOOLEAN_APP_OPTION(multi_queue_direct_draining, false, "Enable direct draining for multi-queue router") \
  STRING_APP_OPTION(max_criticality, "0.0", "Maximum criticality") \
  STRING_APP_OPTION(criticality_exp, "0.0", "Criticality exponent") \
  STRING_APP_OPTION(router_init_wirelength_abort_threshold, "0.0", "Wirelength abort threshold for router initialization") \
  SELECTION_APP_OPTION(incr_reroute_delay_ripup, "auto", timing_router_incr_reroute_delay_ripup_map, "Incremental reroute delay ripup mode") \
  SELECTION_APP_OPTION(routing_failure_predictor, "safe", timing_router_routing_failure_predictor_map, "Routing failure predictor") \
  SELECTION_APP_OPTION(routing_budgets_algorithm, "disable", timing_router_routing_budgets_algorithm_map, "Routing budgets algorithm") \
  BOOLEAN_APP_OPTION(save_routing_per_iteration, false, "Save routing results for each iteration") \
  STRING_APP_OPTION(congested_routing_iteration_threshold_frac, "0.0", "Congested routing iteration threshold fraction") \
  SELECTION_APP_OPTION(route_bb_update, "static", timing_router_route_bb_update_map, "Route bounding-box update mode") \
  INT_APP_OPTION(router_high_fanout_threshold, 0, "Router high-fanout threshold") \
  STRING_APP_OPTION(router_high_fanout_max_slope, "0.0", "Maximum high-fanout slope") \
  INT_APP_OPTION(router_debug_net, 0, "Router debug net id") \
  INT_APP_OPTION(router_debug_sink_rr, 0, "Router debug sink RR node") \
  INT_APP_OPTION(router_debug_iteration, 0, "Router debug iteration") \
  SELECTION_APP_OPTION(router_lookahead_type, "classic", timing_router_lookahead_type_map, "Router lookahead type") \
  BOOLEAN_APP_OPTION(generate_router_lookahead_report, false, "Generate router lookahead report") \
  STRING_APP_OPTION(router_initial_acc_cost_chan_congestion_threshold, "0.0", "Initial accumulated cost congestion threshold") \
  STRING_APP_OPTION(router_initial_acc_cost_chan_congestion_weight, "0.0", "Initial accumulated cost congestion weight") \
  INT_APP_OPTION(router_max_convergence_count, 0, "Maximum router convergence count") \
  STRING_APP_OPTION(router_reconvergence_cpd_threshold, "0.0", "Router reconvergence CPD threshold") \
  BOOLEAN_APP_OPTION(router_update_lower_bound_delays, false, "Update lower-bound delays during routing") \
  STRING_APP_OPTION(router_first_iteration_timing_report_file, "", "First-iteration timing report output file") \
  SELECTION_APP_OPTION(router_initial_timing, "lookahead", timing_router_initial_timing_map, "Initial timing mode") \
  SELECTION_APP_OPTION(router_heap, "binary", timing_router_heap_map, "Router heap implementation")

#define DEFINE_ANALYSIS_OPTIONS_FIELDS \
  BOOLEAN_APP_OPTION(full_stats, false, "Enable full analysis statistics") \
  BOOLEAN_APP_OPTION(generate_post_synthesis_netlist, false, "Generate the post-synthesis netlist") \
  BOOLEAN_APP_OPTION(generate_post_implementation_merged_netlist, false, "Generate the merged post-implementation netlist") \
  BOOLEAN_APP_OPTION(generate_post_implementation_sdc, false, "Generate the post-implementation SDC") \
  INT_APP_OPTION(timing_report_npaths, 0, "Number of timing paths to report") \
  SELECTION_APP_OPTION(timing_report_detail, "netlist", analysis_timing_report_detail_map, "Timing report detail level") \
  BOOLEAN_APP_OPTION(timing_report_skew, false, "Report timing skew") \
  STRING_APP_OPTION(echo_dot_timing_graph_node, "", "Timing graph node to echo in DOT format") \
  SELECTION_APP_OPTION(post_synth_netlist_unconn_input_handling, "unconnected", analysis_post_synth_netlist_unconn_handling_map, "Handling for unconnected post-synthesis inputs") \
  SELECTION_APP_OPTION(post_synth_netlist_unconn_output_handling, "unconnected", analysis_post_synth_netlist_unconn_handling_map, "Handling for unconnected post-synthesis outputs") \
  BOOLEAN_APP_OPTION(post_synth_netlist_module_parameters, false, "Write post-synthesis netlist module parameters") \
  STRING_APP_OPTION(write_timing_summary, "", "Timing summary output file") \
  BOOLEAN_APP_OPTION(skip_sync_clustering_and_routing_results, false, "Skip syncing clustering and routing results") \
  BOOLEAN_APP_OPTION(generate_net_timing_report, false, "Generate a per-net timing report")

#define DEFINE_GENERAL_OPTIONS_FIELDS \
  INT_APP_OPTION(num_workers, 1, "Number of worker threads") \
  BOOLEAN_APP_OPTION(timing_analysis, false, "Enable timing analysis") \
  STRING_APP_OPTION(device_layout, "auto", "Device layout name or 'auto'") \
  SELECTION_APP_OPTION(timing_update_type, "auto", timing_update_type_map, "Timing update mode") \
  STRING_APP_OPTION(target_device_utilization, "0.0", "Target device utilization") \
  SELECTION_APP_OPTION(constant_net_method, "global", constant_net_method_map, "Constant-net handling method") \
  SELECTION_APP_OPTION(clock_modeling, "ideal", clock_modeling_map, "Clock modeling mode") \
  BOOLEAN_APP_OPTION(two_stage_clock_routing, false, "Enable two-stage clock routing") \
  STRING_APP_OPTION(disable_errors, "", "Colon-separated list of errors to disable") \
  STRING_APP_OPTION(suppress_warnings, "", "Warnings to suppress") \
  BOOLEAN_APP_OPTION(allow_dangling_combinational_nodes, false, "Allow dangling combinational nodes") \
  BOOLEAN_APP_OPTION(terminate_if_timing_fails, false, "Terminate if timing analysis fails")

#define DEFINE_BUILD_FABRIC_OPTIONS_FIELDS \
  INT_APP_OPTION(gsb_version, 0, "GSB version") \
  BOOLEAN_APP_OPTION(reorder_incoming_edges, true, "Reorder incoming edges") \
  BOOLEAN_APP_OPTION(sort_gsb_chan_node_in_edges, true, "Sort GSB channel-node incoming edges")
// clang-format on

struct atom_netlist_opts {
  DEFINE_ATOM_NETLIST_OPTIONS_FIELDS;
};

struct analytical_placement_opts {
  DEFINE_ANALYTICAL_PLACEMENT_OPTIONS_FIELDS;
};

struct clustering_opts {
  DEFINE_CLUSTERING_OPTIONS_FIELDS;
};

struct placement_opts {
  DEFINE_PLACEMENT_OPTIONS_FIELDS;
};

struct timing_placement_opts {
  DEFINE_TIMING_PLACEMENT_OPTIONS_FIELDS;
};

struct router_opts {
  DEFINE_ROUTER_OPTIONS_FIELDS;
};

struct timing_router_opts {
  DEFINE_TIMING_ROUTER_OPTIONS_FIELDS;
};

struct analysis_opts {
  DEFINE_ANALYSIS_OPTIONS_FIELDS;
};

struct general_opts {
  DEFINE_GENERAL_OPTIONS_FIELDS;
};

struct build_fabric_opts {
  DEFINE_BUILD_FABRIC_OPTIONS_FIELDS;
};

struct application_options {
  atom_netlist_opts atom_netlist;
  analytical_placement_opts analytical_placement;
  clustering_opts clustering;
  placement_opts placement;
  timing_placement_opts timing_placement;
  router_opts router;
  timing_router_opts timing_router;
  analysis_opts analysis;
  general_opts general;
  build_fabric_opts build_fabric;
};

#undef APP_OPTION
#undef STRING_APP_OPTION
#undef INT_APP_OPTION
#undef FLOAT_APP_OPTION
#undef BOOLEAN_APP_OPTION
#undef SELECTION_APP_OPTION

// Keep the field declaration of application options in sync with the
// implementation in `openfpga_shell.cpp` and the option definitions above.

// #define APP_OPTION(name, value, help_message) #name,
// #define STRING_APP_OPTION(name, value, help_message) #name,
// #define INT_APP_OPTION(name, value, help_message) #name,
// #define FLOAT_APP_OPTION(name, value, help_message) #name,
// #define BOOLEAN_APP_OPTION(name, value, help_message) #name,
// #define SELECTION_APP_OPTION(name, value, selection_values, help_message)
// #name,

// constexpr size_t atom_netlist_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_ATOM_NETLIST_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, atom_netlist_field_count>
//   atom_netlist_fields = {DEFINE_ATOM_NETLIST_OPTIONS_FIELDS};

// constexpr size_t analytical_placement_field_count =
//   std::tuple_size_v<decltype(std::array{
//     DEFINE_ANALYTICAL_PLACEMENT_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, analytical_placement_field_count>
//   analytical_placement_fields = {DEFINE_ANALYTICAL_PLACEMENT_OPTIONS_FIELDS};

// constexpr size_t clustering_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_CLUSTERING_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, clustering_field_count>
//   clustering_fields = {DEFINE_CLUSTERING_OPTIONS_FIELDS};

// constexpr size_t placement_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_PLACEMENT_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, placement_field_count>
// placement_fields =
//   {DEFINE_PLACEMENT_OPTIONS_FIELDS};

// constexpr size_t timing_placement_field_count =
//   std::tuple_size_v<decltype(std::array{
//     DEFINE_TIMING_PLACEMENT_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, timing_placement_field_count>
//   timing_placement_fields = {DEFINE_TIMING_PLACEMENT_OPTIONS_FIELDS};

// constexpr size_t router_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_ROUTER_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, router_field_count> router_fields = {
//   DEFINE_ROUTER_OPTIONS_FIELDS};

// constexpr size_t timing_router_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_TIMING_ROUTER_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, timing_router_field_count>
//   timing_router_fields = {DEFINE_TIMING_ROUTER_OPTIONS_FIELDS};

// constexpr size_t analysis_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_ANALYSIS_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, analysis_field_count> analysis_fields
// = {
//   DEFINE_ANALYSIS_OPTIONS_FIELDS};

// constexpr size_t general_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_GENERAL_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, general_field_count> general_fields =
// {
//   DEFINE_GENERAL_OPTIONS_FIELDS};

// constexpr size_t build_fabric_field_count =
//   std::tuple_size_v<decltype(std::array{DEFINE_BUILD_FABRIC_OPTIONS_FIELDS})>;

// constexpr std::array<std::string_view, build_fabric_field_count>
//   build_fabric_fields = {DEFINE_BUILD_FABRIC_OPTIONS_FIELDS};

// #undef APP_OPTION
// #undef STRING_APP_OPTION
// #undef INT_APP_OPTION
// #undef FLOAT_APP_OPTION
// #undef BOOLEAN_APP_OPTION
// #undef SELECTION_APP_OPTION