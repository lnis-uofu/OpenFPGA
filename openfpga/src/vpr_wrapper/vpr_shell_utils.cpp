#include "vpr_shell_utils.h"

#include <algorithm>

#include "ShowSetup.h"
#include "command_exit_codes.h"
#include "echo_files.h"
#include "openfpga_context.h"
#include "shell.h"
#include "vpr_types.h"
#include "vtr_log.h"

namespace vpr {

void sync_vpr_setup_to_app_options(t_vpr_setup& vpr_setup,
                                   openfpga::Shell<OpenfpgaContext>& shell) {
  vpr_setup.TimingEnabled =
    shell.app_options_.general.timing_analysis.bool_value;
  vpr_setup.device_layout =
    shell.app_options_.general.device_layout.string_value;
  vpr_setup.constant_net_method = static_cast<e_constant_net_method>(
    shell.app_options_.general.constant_net_method.to_enum());
  vpr_setup.clock_modeling = static_cast<e_clock_modeling>(
    shell.app_options_.general.clock_modeling.to_enum());
  vpr_setup.two_stage_clock_routing =
    shell.app_options_.general.two_stage_clock_routing.bool_value;
  vpr_setup.exit_before_pack = false;
  vpr_setup.num_workers = shell.app_options_.general.num_workers.int_value;

  setEchoEnabled(false);

  /* = = = = = = = = = = = = = = = = = =
   * TODO: Separate supress warning and errors to different options, and support
   * more flexible configuration (e.g. specify a log file to output the
   * supressed warnings/errors)
   * = = = = = = = = = = = = = = = = = = */
  /*
   * Initialize the functions names for which VPR_ERRORs
   * are demoted to VTR_LOG_WARNs
   */
  for (const std::string& func_name :
       vtr::StringToken(shell.app_options_.general.disable_errors.string_value)
         .split(":")) {
    map_error_activation_status(func_name);
  }

  /*
   * Initialize the functions names for which
   * warnings are being suppressed
   */
  std::vector<std::string> split_warning_option =
    vtr::StringToken(shell.app_options_.general.suppress_warnings.string_value)
      .split(",");
  std::string warn_log_file;
  std::string warn_functions;
  // If no log file name is provided, the specified warning
  // to suppress are not output anywhere.
  if (split_warning_option.size() == 1) {
    warn_functions = split_warning_option[0];
  } else if (split_warning_option.size() == 2) {
    warn_log_file = split_warning_option[0];
    warn_functions = split_warning_option[1];
  }
  /* = = = = = = = = = = = = = = = = = = */
  shell_setup_netlist_opts(vpr_setup, shell);
  shell_setup_placer_opts(vpr_setup, shell);
  shell_setup_anneal_sched(vpr_setup, shell);
  shell_setup_packer_opts(vpr_setup, shell);
  shell_setup_router_opts(vpr_setup, shell);
  shell_setup_analysis_opts(vpr_setup, shell);
  shell_setup_crr_opts(vpr_setup, shell);
  shell_setup_power_opts(vpr_setup, shell);
  shell_setup_noc_opts(vpr_setup, shell);
  shell_setup_server_opts(vpr_setup, shell);
}

void shell_setup_netlist_opts(t_vpr_setup& vpr_setup,
                              openfpga::Shell<OpenfpgaContext>& shell) {

  auto NetlistOpts = &vpr_setup.NetlistOpts;
  auto& ShellAtomNetlistOpts = shell.app_options_.atom_netlist;

  NetlistOpts->const_gen_inference =
    static_cast<e_const_gen_inference>(ShellAtomNetlistOpts.const_gen_inference.to_enum());
  NetlistOpts->absorb_buffer_luts =
    ShellAtomNetlistOpts.absorb_buffer_luts.bool_value;
      NetlistOpts->sweep_dangling_primary_ios =
      ShellAtomNetlistOpts.sweep_dangling_primary_ios.bool_value;
  NetlistOpts->sweep_dangling_nets = ShellAtomNetlistOpts.sweep_dangling_nets.bool_value;
  NetlistOpts->sweep_dangling_blocks = ShellAtomNetlistOpts.sweep_dangling_blocks.bool_value;
  NetlistOpts->sweep_constant_primary_outputs =
    ShellAtomNetlistOpts.sweep_constant_primary_outputs.bool_value;
}

void shell_setup_timing(t_vpr_setup& vpr_setup,
                        openfpga::Shell<OpenfpgaContext>& shell) {

  auto Timing = &vpr_setup.Timing;
  auto Options = shell.app_options_;
  bool TimingEnabled = Options.general.timing_analysis.bool_value;

  if (!TimingEnabled) {
    Timing->timing_analysis_enabled = false;
    return;
  }

  Timing->timing_analysis_enabled = TimingEnabled;
  Timing->SDCFile = Options.filename.sdc_file.string_value;
}

void shell_setup_placer_opts(
  t_vpr_setup& vpr_setup, openfpga::Shell<OpenfpgaContext>& shell) {
  // Sync the options in VPR setup to the app options in the shell

  // Setup placer options
  t_placer_opts* PlacerOpts = &vpr_setup.PlacerOpts;

  auto ShellPlacerOpts = shell.app_options_.placement;
  auto ShellTimingPlacementOpts = shell.app_options_.timing_placement;
  auto ShellGeneralOpts = shell.app_options_.general;
  auto ShellFilenameOpts = shell.app_options_.filename;

  PlacerOpts->do_placement = e_stage_action::DO;

  PlacerOpts->inner_loop_recompute_divider =
    ShellTimingPlacementOpts.inner_loop_recompute_divider.int_value;
  PlacerOpts->quench_recompute_divider =
    ShellTimingPlacementOpts.quench_recompute_divider.int_value;

  PlacerOpts->place_algorithm =
    static_cast<e_place_algorithm>(ShellPlacerOpts.place_algorithm.to_enum());
  PlacerOpts->place_quench_algorithm = static_cast<e_place_algorithm>(ShellPlacerOpts.place_quench_algorithm.to_enum());
  PlacerOpts->pad_loc_type = static_cast<e_pad_loc_type>(ShellPlacerOpts.pad_loc_type.to_enum());
  PlacerOpts->place_chan_width = ShellPlacerOpts.place_chan_width.int_value;
  PlacerOpts->inner_loop_recompute_divider = ShellTimingPlacementOpts.inner_loop_recompute_divider.int_value;
  PlacerOpts->quench_recompute_divider = ShellTimingPlacementOpts.quench_recompute_divider.int_value;
  PlacerOpts->td_place_exp_first = ShellTimingPlacementOpts.place_exp_first.float_value;
  PlacerOpts->td_place_exp_last = ShellTimingPlacementOpts.place_exp_last.float_value;
  PlacerOpts->constraints_file = ShellFilenameOpts.constraints_file.string_value;
  PlacerOpts->write_initial_place_file =
    ShellFilenameOpts.write_initial_place_file.string_value;
  PlacerOpts->read_initial_place_file =
    ShellFilenameOpts.read_initial_place_file.string_value;
  PlacerOpts->recompute_crit_iter = ShellTimingPlacementOpts.recompute_crit_iter.int_value;
  PlacerOpts->timing_tradeoff =
    ShellTimingPlacementOpts.place_timing_tradeoff.float_value;
  PlacerOpts->congestion_factor = ShellTimingPlacementOpts.place_congestion_factor.float_value;
  PlacerOpts->congestion_rlim_trigger_ratio = ShellTimingPlacementOpts.place_congestion_rlim_trigger_ratio.float_value;
  PlacerOpts->congestion_chan_util_threshold = ShellTimingPlacementOpts.place_congestion_chan_util_threshold.float_value;
  PlacerOpts->delay_offset = ShellTimingPlacementOpts.place_delay_offset.float_value;
  PlacerOpts->delay_ramp_delta_threshold = ShellTimingPlacementOpts.place_delay_ramp_delta_threshold.int_value;
  PlacerOpts->delay_ramp_slope = ShellTimingPlacementOpts.place_delay_ramp_slope.float_value;
  PlacerOpts->tsu_rel_margin = ShellTimingPlacementOpts.place_tsu_rel_margin.float_value;
  PlacerOpts->tsu_abs_margin = ShellTimingPlacementOpts.place_tsu_abs_margin.float_value;
  PlacerOpts->delay_model_type = static_cast<PlaceDelayModelType>(
    ShellTimingPlacementOpts.place_delay_model.int_value);
  PlacerOpts->delay_model_reducer =
    static_cast<e_reducer>(ShellTimingPlacementOpts.place_delay_model_reducer.int_value);
  PlacerOpts->place_freq =
    static_cast<e_place_freq>(ShellPlacerOpts.place_placement_freq.int_value);
  PlacerOpts->post_place_timing_report_file = ShellTimingPlacementOpts.post_place_timing_report_file.string_value;
  PlacerOpts->rlim_escape_fraction = ShellPlacerOpts.place_rlim_escape_fraction.float_value;
  PlacerOpts->move_stats_file =
    ShellPlacerOpts.place_move_stats_file.string_value;
  PlacerOpts->placement_saves_per_temperature = ShellPlacerOpts.placement_saves_per_temperature.int_value;
  PlacerOpts->placer_debug_block = ShellPlacerOpts.placer_debug_block.int_value;
  PlacerOpts->placer_debug_net = ShellPlacerOpts.placer_debug_net.int_value;
  PlacerOpts->seed = ShellPlacerOpts.seed.int_value;
}

void shell_setup_anneal_sched(t_vpr_setup& vpr_setup,
                              openfpga::Shell<OpenfpgaContext>& shell) {
  t_annealing_sched* AnnealSched = &vpr_setup.PlacerOpts.anneal_sched;
  auto& ShellPlacerOpts = shell.app_options_.placement;

  AnnealSched->alpha_t = ShellPlacerOpts.place_alpha_t.float_value;
  if (AnnealSched->alpha_t >= 1 || AnnealSched->alpha_t <= 0) {
    VPR_FATAL_ERROR(VPR_ERROR_OTHER, "alpha_t must be between 0 and 1 exclusive.\n");
  }

  AnnealSched->exit_t = ShellPlacerOpts.place_exit_t.float_value;
  if (AnnealSched->exit_t <= 0) {
    VPR_FATAL_ERROR(VPR_ERROR_OTHER, "exit_t must be greater than 0.\n");
  }

  AnnealSched->init_t = ShellPlacerOpts.place_init_t.float_value;
  if (AnnealSched->init_t <= 0) {
    VPR_FATAL_ERROR(VPR_ERROR_OTHER, "init_t must be greater than 0.\n");
  }

  if (AnnealSched->init_t < AnnealSched->exit_t) {
    VPR_FATAL_ERROR(VPR_ERROR_OTHER,
                    "init_t must be greater or equal to than exit_t.\n");
  }

  AnnealSched->inner_num = ShellPlacerOpts.place_inner_num.float_value;
  if (AnnealSched->inner_num <= 0) {
    VPR_FATAL_ERROR(VPR_ERROR_OTHER, "inner_num must be greater than 0.\n");
  }

  AnnealSched->type =
    static_cast<e_sched_type>(ShellPlacerOpts.anneal_sched_type.to_enum());
}

void shell_setup_packer_opts(t_vpr_setup& vpr_setup,
                             openfpga::Shell<OpenfpgaContext>& shell) {
  // Sync the options in VPR setup to the app options in the shell

  // Setup netlist options
  t_netlist_opts* NetlistOpts = &vpr_setup.NetlistOpts;
  t_packer_opts* PackerOpts = &vpr_setup.PackerOpts;
  // Shell app_options
  auto ShellNetlistOpts = shell.app_options_.atom_netlist;
  auto ShellPackerOpts = shell.app_options_.clustering;
  auto ShellGeneralOpts = shell.app_options_.general;
  auto ShellFilenameOpts = shell.app_options_.filename;

  // init global variables

  NetlistOpts->const_gen_inference = static_cast<e_const_gen_inference>(
    ShellNetlistOpts.const_gen_inference.to_enum());
  NetlistOpts->absorb_buffer_luts =
    ShellNetlistOpts.absorb_buffer_luts.bool_value;
  NetlistOpts->sweep_dangling_primary_ios =
    ShellNetlistOpts.sweep_dangling_primary_ios.bool_value;
  NetlistOpts->sweep_dangling_nets =
    ShellNetlistOpts.sweep_dangling_nets.bool_value;
  NetlistOpts->sweep_dangling_blocks =
    ShellNetlistOpts.sweep_dangling_blocks.bool_value;
  NetlistOpts->sweep_constant_primary_outputs =
    ShellNetlistOpts.sweep_constant_primary_outputs.bool_value;

  // Setup packer options
  PackerOpts->output_file = ShellFilenameOpts.net_file.string_value;
  PackerOpts->circuit_file_name = ShellFilenameOpts.circuit_file.string_value;
  PackerOpts->doPacking = e_stage_action::DO;

  PackerOpts->allow_unrelated_clustering = static_cast<e_unrelated_clustering>(
    ShellPackerOpts.allow_unrelated_clustering.to_enum());
  PackerOpts->connection_driven =
    ShellPackerOpts.connection_driven_clustering.bool_value;
  PackerOpts->timing_driven =
    ShellPackerOpts.timing_driven_clustering.bool_value;
  PackerOpts->cluster_seed_type =
    static_cast<e_cluster_seed>(ShellPackerOpts.cluster_seed_type.to_enum());
  PackerOpts->timing_gain_weight =
    ShellPackerOpts.timing_gain_weight.float_value;
  PackerOpts->connection_gain_weight =
    ShellPackerOpts.connection_gain_weight.float_value;
  PackerOpts->pack_verbosity = 0;
  PackerOpts->memoize_cluster_packings =
    ShellPackerOpts.memoize_cluster_packings.bool_value;
  PackerOpts->enable_pin_feasibility_filter =
    ShellPackerOpts.enable_clustering_pin_feasibility_filter.bool_value;
  PackerOpts->balance_block_type_utilization =
    static_cast<e_balance_block_type_util>(
      ShellPackerOpts.balance_block_type_utilization.to_enum());
  PackerOpts->target_external_pin_util =
    vtr::StringToken(ShellPackerOpts.target_external_pin_util.string_value)
      .split(" ");
  PackerOpts->target_device_utilization =
    ShellGeneralOpts.target_device_utilization.float_value;
  PackerOpts->prioritize_transitive_connectivity =
    ShellPackerOpts.pack_prioritize_transitive_connectivity.bool_value;
  PackerOpts->high_fanout_threshold =
    vtr::StringToken(ShellPackerOpts.pack_high_fanout_threshold.string_value)
      .split(" ");
  PackerOpts->transitive_fanout_threshold =
    ShellPackerOpts.pack_transitive_fanout_threshold.int_value;
  PackerOpts->feasible_block_array_size =
    ShellPackerOpts.pack_feasible_block_array_size.int_value;

  PackerOpts->device_layout = ShellGeneralOpts.device_layout.string_value;

  PackerOpts->timing_update_type = static_cast<e_timing_update_type>(
    ShellGeneralOpts.timing_update_type.to_enum());
}

void shell_setup_router_opts(t_vpr_setup& vpr_setup,
                             openfpga::Shell<OpenfpgaContext>& shell) {
  t_router_opts* RouterOpts = &vpr_setup.RouterOpts;
  auto& ShellRouterOpts = shell.app_options_.router;
  auto& ShellTimingRouterOpts = shell.app_options_.timing_router;
  auto& ShellGeneralOpts = shell.app_options_.general;
  auto& ShellFilenameOpts = shell.app_options_.filename;

  RouterOpts->doRouting = e_stage_action::DO;

  RouterOpts->do_check_rr_graph = ShellRouterOpts.check_rr_graph.bool_value;
  RouterOpts->astar_fac = ShellTimingRouterOpts.astar_fac.float_value;
  RouterOpts->astar_offset = ShellTimingRouterOpts.astar_offset.float_value;
  RouterOpts->router_profiler_astar_fac =
    ShellTimingRouterOpts.router_profiler_astar_fac.float_value;
  RouterOpts->enable_parallel_connection_router =
    ShellTimingRouterOpts.enable_parallel_connection_router.bool_value;
  RouterOpts->post_target_prune_fac =
    ShellTimingRouterOpts.post_target_prune_fac.float_value;
  RouterOpts->post_target_prune_offset =
    ShellTimingRouterOpts.post_target_prune_offset.float_value;
  RouterOpts->multi_queue_num_threads =
    ShellTimingRouterOpts.multi_queue_num_threads.int_value;
  RouterOpts->multi_queue_num_queues =
    ShellTimingRouterOpts.multi_queue_num_queues.int_value;
  RouterOpts->multi_queue_direct_draining =
    ShellTimingRouterOpts.multi_queue_direct_draining.bool_value;
  RouterOpts->bb_factor = ShellRouterOpts.bb_factor.int_value;
  RouterOpts->criticality_exp =
    ShellTimingRouterOpts.criticality_exp.float_value;
  RouterOpts->max_criticality =
    ShellTimingRouterOpts.max_criticality.float_value;
  RouterOpts->max_router_iterations =
    ShellRouterOpts.max_router_iterations.int_value;
  RouterOpts->init_wirelength_abort_threshold = ShellTimingRouterOpts.router_init_wirelength_abort_threshold.float_value;
  RouterOpts->min_incremental_reroute_fanout =
    ShellRouterOpts.min_incremental_reroute_fanout.int_value;
  RouterOpts->incr_reroute_delay_ripup =
    static_cast<e_incr_reroute_delay_ripup>(
      ShellTimingRouterOpts.incr_reroute_delay_ripup.to_enum());
  RouterOpts->pres_fac_mult = ShellRouterOpts.pres_fac_mult.float_value;
  RouterOpts->max_pres_fac = ShellRouterOpts.max_pres_fac.float_value;
  RouterOpts->route_type =
    static_cast<e_route_type>(ShellRouterOpts.route_type.to_enum());

  RouterOpts->full_stats = shell.app_options_.analysis.full_stats.bool_value;
  RouterOpts->congestion_analysis =
    shell.app_options_.analysis.full_stats.bool_value;
  RouterOpts->fanout_analysis =
    shell.app_options_.analysis.full_stats.bool_value;
  RouterOpts->switch_usage_analysis =
    shell.app_options_.analysis.full_stats.bool_value;

  RouterOpts->verify_binary_search =
    ShellRouterOpts.verify_binary_search.bool_value;
  RouterOpts->router_algorithm = static_cast<e_router_algorithm>(
    ShellRouterOpts.router_algorithm.to_enum());
  RouterOpts->fixed_channel_width = ShellRouterOpts.route_chan_width.int_value;
  RouterOpts->min_channel_width_hint =
    ShellRouterOpts.min_route_chan_width_hint.int_value;
  RouterOpts->read_rr_edge_metadata =
    ShellRouterOpts.read_rr_edge_metadata.bool_value;
  RouterOpts->reorder_rr_graph_nodes_algorithm =
    static_cast<e_rr_node_reorder_algorithm>(
      ShellRouterOpts.reorder_rr_graph_nodes_algorithm.to_enum());
  RouterOpts->reorder_rr_graph_nodes_threshold =
    ShellRouterOpts.reorder_rr_graph_nodes_threshold.int_value;
  RouterOpts->reorder_rr_graph_nodes_seed =
    ShellRouterOpts.reorder_rr_graph_nodes_seed.int_value;
  RouterOpts->initial_pres_fac = ShellRouterOpts.initial_pres_fac.float_value;
  RouterOpts->base_cost_type =
    static_cast<e_base_cost_type>(ShellRouterOpts.base_cost_type.to_enum());
  RouterOpts->first_iter_pres_fac =
    ShellRouterOpts.first_iter_pres_fac.float_value;
  RouterOpts->acc_fac = ShellRouterOpts.acc_fac.float_value;
  RouterOpts->bend_cost = ShellRouterOpts.bend_cost.float_value;

  RouterOpts->routing_failure_predictor =
    static_cast<e_routing_failure_predictor>(
      ShellTimingRouterOpts.routing_failure_predictor.to_enum());
  RouterOpts->routing_budgets_algorithm =
    static_cast<e_routing_budgets_algorithm>(
      ShellTimingRouterOpts.routing_budgets_algorithm.to_enum());
  RouterOpts->save_routing_per_iteration =
    ShellTimingRouterOpts.save_routing_per_iteration.bool_value;
  RouterOpts->congested_routing_iteration_threshold_frac =
    ShellTimingRouterOpts.congested_routing_iteration_threshold_frac.float_value;
  RouterOpts->route_bb_update = static_cast<e_route_bb_update>(
    ShellTimingRouterOpts.route_bb_update.to_enum());
  RouterOpts->clock_modeling =
    static_cast<e_clock_modeling>(ShellGeneralOpts.clock_modeling.to_enum());
  RouterOpts->two_stage_clock_routing =
    ShellGeneralOpts.two_stage_clock_routing.bool_value;
  RouterOpts->high_fanout_threshold =
    ShellTimingRouterOpts.router_high_fanout_threshold.int_value;
  RouterOpts->high_fanout_max_slope = std::stof(
    ShellTimingRouterOpts.router_high_fanout_max_slope.string_value);
  RouterOpts->router_debug_net =
    ShellTimingRouterOpts.router_debug_net.int_value;
  RouterOpts->router_debug_sink_rr =
    ShellTimingRouterOpts.router_debug_sink_rr.int_value;
  RouterOpts->router_debug_iteration =
    ShellTimingRouterOpts.router_debug_iteration.int_value;
  RouterOpts->lookahead_type = static_cast<e_router_lookahead>(
    ShellTimingRouterOpts.router_lookahead_type.to_enum());
  RouterOpts->initial_acc_cost_chan_congestion_threshold = ShellTimingRouterOpts.router_initial_acc_cost_chan_congestion_threshold
      .float_value;
  RouterOpts->initial_acc_cost_chan_congestion_weight = ShellTimingRouterOpts.router_initial_acc_cost_chan_congestion_weight
      .float_value;
  RouterOpts->max_convergence_count =
    ShellTimingRouterOpts.router_max_convergence_count.int_value;
  RouterOpts->reconvergence_cpd_threshold =
    ShellTimingRouterOpts.router_reconvergence_cpd_threshold.float_value;
  RouterOpts->initial_timing = static_cast<e_router_initial_timing>(
    ShellTimingRouterOpts.router_initial_timing.to_enum());
  RouterOpts->update_lower_bound_delays =
    ShellTimingRouterOpts.router_update_lower_bound_delays.bool_value;
  RouterOpts->first_iteration_timing_report_file =
    ShellTimingRouterOpts.router_first_iteration_timing_report_file
      .string_value;

  RouterOpts->write_router_lookahead =
    ShellFilenameOpts.write_router_lookahead.string_value;
  RouterOpts->read_router_lookahead =
    ShellFilenameOpts.read_router_lookahead.string_value;
  RouterOpts->write_intra_cluster_router_lookahead =
    ShellFilenameOpts.write_intra_cluster_router_lookahead.string_value;
  RouterOpts->read_intra_cluster_router_lookahead =
    ShellFilenameOpts.read_intra_cluster_router_lookahead.string_value;

  RouterOpts->router_heap =
    static_cast<e_heap_type>(ShellTimingRouterOpts.router_heap.to_enum());
  RouterOpts->exit_after_first_routing_iteration =
    ShellRouterOpts.exit_after_first_routing_iteration.bool_value;
  RouterOpts->check_route = static_cast<e_check_route_option>(
    ShellRouterOpts.check_route.to_enum());
  RouterOpts->timing_update_type = static_cast<e_timing_update_type>(
    ShellGeneralOpts.timing_update_type.to_enum());
  RouterOpts->max_logged_overused_rr_nodes =
    ShellRouterOpts.max_logged_overused_rr_nodes.int_value;
  RouterOpts->generate_rr_node_overuse_report =
    ShellRouterOpts.generate_rr_node_overuse_report.bool_value;
  RouterOpts->flat_routing = ShellRouterOpts.flat_routing.bool_value;
  RouterOpts->has_choke_point =
    ShellRouterOpts.router_opt_choke_points.bool_value;
  RouterOpts->with_timing_analysis =
    ShellGeneralOpts.timing_analysis.bool_value;
  RouterOpts->generate_router_lookahead_report =
    ShellTimingRouterOpts.generate_router_lookahead_report.bool_value;
}

void shell_setup_analysis_opts(t_vpr_setup& vpr_setup,
                               openfpga::Shell<OpenfpgaContext>& shell) {
  t_analysis_opts* AnalysisOpts = &vpr_setup.AnalysisOpts;
  auto& ShellAnalysisOpts = shell.app_options_.analysis;
  auto& ShellGeneralOpts = shell.app_options_.general;

  AnalysisOpts->doAnalysis = e_stage_action::DO;

  AnalysisOpts->gen_post_synthesis_netlist =
    ShellAnalysisOpts.generate_post_synthesis_netlist.bool_value;
  AnalysisOpts->gen_post_implementation_merged_netlist =
    ShellAnalysisOpts.generate_post_implementation_merged_netlist.bool_value;
  AnalysisOpts->gen_post_implementation_sdc =
    ShellAnalysisOpts.generate_post_implementation_sdc.bool_value;
  AnalysisOpts->timing_report_npaths =
    ShellAnalysisOpts.timing_report_npaths.int_value;
  AnalysisOpts->timing_report_detail = static_cast<e_timing_report_detail>(
    ShellAnalysisOpts.timing_report_detail.to_enum());
  AnalysisOpts->timing_report_skew =
    ShellAnalysisOpts.timing_report_skew.bool_value;
  AnalysisOpts->echo_dot_timing_graph_node =
    ShellAnalysisOpts.echo_dot_timing_graph_node.string_value;
  AnalysisOpts->post_synth_netlist_unconn_input_handling =
    static_cast<e_post_synth_netlist_unconn_handling>(
      ShellAnalysisOpts.post_synth_netlist_unconn_input_handling.to_enum());
  AnalysisOpts->post_synth_netlist_unconn_output_handling =
    static_cast<e_post_synth_netlist_unconn_handling>(
      ShellAnalysisOpts.post_synth_netlist_unconn_output_handling.to_enum());
  AnalysisOpts->post_synth_netlist_module_parameters =
    ShellAnalysisOpts.post_synth_netlist_module_parameters.bool_value;
  AnalysisOpts->timing_update_type = static_cast<e_timing_update_type>(
    ShellGeneralOpts.timing_update_type.to_enum());
  AnalysisOpts->write_timing_summary =
    ShellAnalysisOpts.write_timing_summary.string_value;
  AnalysisOpts->skip_sync_clustering_and_routing_results =
    ShellAnalysisOpts.skip_sync_clustering_and_routing_results.bool_value;
  AnalysisOpts->generate_net_timing_report =
    ShellAnalysisOpts.generate_net_timing_report.bool_value;
}

void shell_setup_crr_opts(t_vpr_setup& vpr_setup,
                          openfpga::Shell<OpenfpgaContext>& shell) {
  /* CRR fields (sb_maps, sb_templates, preserve_input_pin_connections,
   * preserve_output_pin_connections, annotated_rr_graph,
   * remove_dangling_nodes, sb_count_dir) are intentionally not exposed as
   * app options. No mapping is performed here. */
  (void)vpr_setup;
  (void)shell;
}

void shell_setup_power_opts(t_vpr_setup& vpr_setup,
                            openfpga::Shell<OpenfpgaContext>& shell) {
  /* do_power and related Arch power/clock fields are intentionally not
   * exposed as app options. No mapping is performed here. */
  (void)vpr_setup;
  (void)shell;
}

void shell_setup_noc_opts(t_vpr_setup& vpr_setup,
                          openfpga::Shell<OpenfpgaContext>& shell) {
  /* NoC options are intentionally not exposed as app options.
   * No mapping is performed here. */
  (void)vpr_setup;
  (void)shell;
}

void shell_setup_server_opts(t_vpr_setup& vpr_setup,
                             openfpga::Shell<OpenfpgaContext>& shell) {
  /* Server options (is_server_mode_enabled, port_num) are intentionally not
   * exposed as app options. No mapping is performed here. */
  (void)vpr_setup;
  (void)shell;
}

int validate_vpr_arch_types(
  const std::string& arch_file_name,
  const std::vector<t_physical_tile_type>& physical_tile_types,
  const std::vector<t_logical_block_type>& logical_block_types) {
  int num_inputs = 0;
  int num_outputs = 0;
  bool has_empty_physical_tile = false;
  for (const t_physical_tile_type& type : physical_tile_types) {
    if (type.is_empty()) {
      has_empty_physical_tile = true;
    }

    if (type.is_input_type) {
      ++num_inputs;
    }

    if (type.is_output_type) {
      ++num_outputs;
    }
  }

  bool has_empty_logical_block = false;
  int max_equivalent_tiles = 0;
  for (const t_logical_block_type& type : logical_block_types) {
    if (type.is_empty()) {
      has_empty_logical_block = true;
    }

    max_equivalent_tiles = std::max(
      max_equivalent_tiles, static_cast<int>(type.equivalent_tiles.size()));
  }

  if (!has_empty_physical_tile || !has_empty_logical_block) {
    VTR_LOG_ERROR(
      "Invalid VPR architecture '%s': missing empty physical/logical block "
      "type.\n",
      arch_file_name.c_str());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  if (max_equivalent_tiles <= 0) {
    VTR_LOG_ERROR("Invalid VPR architecture '%s': no equivalent tiles found.\n",
                  arch_file_name.c_str());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  if (num_inputs == 0) {
    VTR_LOG_ERROR(
      "Invalid VPR architecture '%s': no top-level block type contains "
      "'.input' models.\n",
      arch_file_name.c_str());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  if (num_outputs == 0) {
    VTR_LOG_ERROR(
      "Invalid VPR architecture '%s': no top-level block type contains "
      "'.output' models.\n",
      arch_file_name.c_str());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  return openfpga::CMD_EXEC_SUCCESS;
}

}  // namespace vpr
