/**
 * This is a wrapper file for VPR API. Mirrored from the main.cpp of vpr
 */

#include "vpr_main.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <vector>

#include "CheckArch.h"
#include "CheckSetup.h"
#include "show_setup.h"
#include "arch_util.h"
#include "atom_netlist_utils.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "lb_type_rr_graph.h"
#include "pb_type_graph.h"
#include "read_sdc.h"
#include "read_xml_arch_file.h"
#include "setup_vib_utils.h"
#include "setup_vpr.cpp"
#include "tatum/error.hpp"
#include "timing_fail_error.h"
#include "timing_graph_builder.h"
#include "vpr_api.h"
#include "vpr_error.h"
#include "vpr_exit_codes.h"
#include "vpr_report_utils.h"
#include "vpr_shell_utils.h"
#include "vpr_signal_handler.h"
#include "vpr_tatum_error.h"
#include "vtr_error.h"
#include "vtr_log.h"
#include "vtr_memory.h"
#include "vtr_path.h"
#include "vtr_time.h"

namespace vpr {
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Read and validate a VPR architecture XML file = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = = = = = = = = = = = =
int read_vpr_arch_template(openfpga::Shell<OpenfpgaContext>* shell,
                           OpenfpgaContext& openfpga_ctx,
                           const openfpga::Command& cmd,
                           const openfpga::CommandContext& cmd_context) {
  openfpga::CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  openfpga::CommandOptionId opt_layout = cmd.option("layout");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_layout));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_layout).empty());

  std::string layout = cmd_context.option_value(cmd, opt_layout);

  VTR_LOG("Reading VPR XML architecture '%s' with layout '%s'...\n",
          arch_file_name.c_str(), layout.c_str());

  DeviceContext& device_ctx = g_vpr_ctx.mutable_device();
  t_vpr_setup& vpr_setup = openfpga_ctx.mutable_vpr_setup();

  // Check that no architecture has already been loaded -- only one arch can
  // be loaded at a time
  if (nullptr != device_ctx.arch) {
    VTR_LOG_ERROR(
      "Cannot read VPR architecture: an architecture has already been "
      "loaded. Only one architecture can be loaded at a time.\n");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  static t_arch arch = t_arch();
  device_ctx.arch = &arch;

  // Set device layout from the mandatory argument
  vpr_setup.device_layout = layout;
  arch.device_layout = layout;
  shell->app_options_.general.device_layout.update(layout);
  vpr_setup.PackerOpts.device_layout = layout;

  device_ctx.physical_tile_types.clear();
  device_ctx.logical_block_types.clear();

  try {
    // Separate this method into reading XML file and processing XML data,
    // It will allow read encrypted or compressed file in the future without
    // changing the processing flow
    xml_read_arch(arch_file_name, vpr_setup.TimingEnabled, &arch,
                  device_ctx.physical_tile_types,
                  device_ctx.logical_block_types, FALSE);

    const int status =
      validate_vpr_arch_types(arch_file_name, device_ctx.physical_tile_types,
                              device_ctx.logical_block_types);
    if (status != openfpga::CMD_EXEC_SUCCESS) {
      free_arch(&arch);
      return status;
    }
  } catch (const std::exception& e) {
    free_arch(&arch);
    VTR_LOG_ERROR("Failed to read VPR XML architecture '%s': %s\n",
                  arch_file_name.c_str(), e.what());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  // Find the empty physical tile types from the arch
  device_ctx.EMPTY_PHYSICAL_TILE_TYPE = nullptr;
  for (t_physical_tile_type& type : device_ctx.physical_tile_types) {
    if (type.is_empty()) {
      device_ctx.EMPTY_PHYSICAL_TILE_TYPE = &type;
      break;
    }
  }

  // Find the empty logical block type from the arch
  device_ctx.EMPTY_LOGICAL_BLOCK_TYPE = nullptr;
  int max_equivalent_tiles = 0;
  for (const t_logical_block_type& type : device_ctx.logical_block_types) {
    if (type.is_empty()) {
      device_ctx.EMPTY_LOGICAL_BLOCK_TYPE = &type;
    }

    max_equivalent_tiles =
      std::max(max_equivalent_tiles, (int)type.equivalent_tiles.size());
  }
  device_ctx.has_multiple_equivalent_tiles = max_equivalent_tiles > 1;

  // Setup the routing architecture from the arch
  vpr_setup.Segments = arch.Segments;
  setup_switches(arch, vpr_setup.RoutingArch, arch.switches);
  setup_routing_arch(arch, vpr_setup.RoutingArch);

  // Setup the VIB (Virtual Interconnect Block) information from the arch
  if (!arch.vib_infs.empty()) {
    setup_vib_inf(device_ctx.physical_tile_types, arch.switches, arch.Segments,
                  arch.vib_infs);
  }

  for (bool has_global_routing : arch.layer_global_routing) {
    device_ctx.inter_cluster_prog_routing_resources.emplace_back(
      has_global_routing);
  }

  {
    vtr::ScopedStartFinishTimer t("Building complex block graph");
    alloc_and_load_all_pb_graphs(vpr_setup.PowerOpts.do_power,
                                 vpr_setup.RouterOpts.flat_routing);
    vpr_setup.PackerRRGraph = alloc_and_load_all_lb_type_rr_graph();
  }

  if ((vpr_setup.clock_modeling == ROUTED_CLOCK) ||
      (vpr_setup.clock_modeling == DEDICATED_NETWORK)) {
    ClockModeling::treat_clock_pins_as_non_globals();
  }

  // Skipped graphics related commands from here

  /* Check inputs are reasonable */
  // Skipped for now
  // CheckArch(arch);

  VTR_LOG("Read VPR XML architecture '%s' successfully.\n",
          arch_file_name.c_str());
  return openfpga::CMD_EXEC_SUCCESS;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Show the VPR setup = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = =
int show_vpr_setup_template(openfpga::Shell<OpenfpgaContext>* shell,
                            OpenfpgaContext& openfpga_ctx,
                            const openfpga::Command& cmd,
                            const openfpga::CommandContext& cmd_context) {
  (void)cmd;
  (void)cmd_context;

  vpr::sync_vpr_setup_to_app_options(openfpga_ctx.mutable_vpr_setup(), *shell);
  show_setup(openfpga_ctx.vpr_setup());
  return openfpga::CMD_EXEC_SUCCESS;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Read and set circuit file = = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = =
int read_circuit_template(openfpga::Shell<OpenfpgaContext>* shell,
                          OpenfpgaContext& openfpga_ctx,
                          const openfpga::Command& cmd,
                          const openfpga::CommandContext& cmd_context) {
  openfpga::CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  DeviceContext& device_ctx = g_vpr_ctx.mutable_device();
  std::string circuit_file = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Reading circuit file '%s'...\n", circuit_file.c_str());

  // Set the circuit file in vpr_setup
  t_vpr_setup& vpr_setup = openfpga_ctx.mutable_vpr_setup();
  vpr_setup.FileNameOpts.CircuitFile = circuit_file;
  vpr_setup.PackerOpts.circuit_file_name = circuit_file;
  std::string circuit_name = vtr::split_ext(vtr::basename(circuit_file))[0];
  vpr_setup.FileNameOpts.CircuitName = circuit_name;

  // Check that an architecture has been loaded before trying to read the
  // circuit
  if (nullptr == device_ctx.arch) {
    VTR_LOG_ERROR(
      "Cannot run packing: no architecture is loaded. Run 'read_vpr_arch' "
      "first.\n");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  /* Read blif file and sweep unused components */
  t_arch& arch = *const_cast<t_arch*>(device_ctx.arch);
  auto& atom_ctx = g_vpr_ctx.mutable_atom();
  atom_ctx.mutable_netlist() =
    read_and_process_circuit(e_circuit_format::BLIF, vpr_setup, arch);

  /* Keep shell app option net_file in sync with the loaded circuit name. */
  std::string net_file = circuit_file;
  const size_t ext_pos = net_file.find_last_of('.');
  if (std::string::npos != ext_pos) {
    net_file = net_file.substr(0, ext_pos);
  }
  VTR_LOG(
    "Updating shell app option net_file to '%s' to match the loaded "
    "circuit name.\n",
    (net_file + ".net").c_str());
  vpr_setup.FileNameOpts.NetFile = net_file + ".net";

  // Initialize timing graph and constraints
  if (vpr_setup.TimingEnabled) {
    auto& timing_ctx = g_vpr_ctx.mutable_timing();
    {
      vtr::ScopedStartFinishTimer t("Build Timing Graph");
      auto& atom_ctx = g_vpr_ctx.mutable_atom();
      timing_ctx.graph =
        TimingGraphBuilder(atom_ctx.netlist(), atom_ctx.mutable_lookup(),
                           arch.models)
          .timing_graph(shell->app_options_.general
                          .allow_dangling_combinational_nodes.bool_value);
      VTR_LOG("  Timing Graph Nodes: %zu\n", timing_ctx.graph->nodes().size());
      VTR_LOG("  Timing Graph Edges: %zu\n", timing_ctx.graph->edges().size());
      VTR_LOG("  Timing Graph Levels: %zu\n",
              timing_ctx.graph->levels().size());
    }
    {
      print_netlist_clock_info(atom_ctx.netlist(), arch.models);
    }
    {
      vtr::ScopedStartFinishTimer t("Load Timing Constraints");
      timing_ctx.constraints =
        read_sdc(vpr_setup.Timing, atom_ctx.netlist(), atom_ctx.lookup(),
                 arch.models, *timing_ctx.graph);
    }
    {
      set_terminate_if_timing_fails(
        shell->app_options_.general.terminate_if_timing_fails.bool_value);
    }
  }

  VTR_LOG("Circuit file '%s' read successfully.\n", circuit_file.c_str());
  return openfpga::CMD_EXEC_SUCCESS;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Run VPR packing flow only = = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = =
int pack_template(openfpga::Shell<OpenfpgaContext>* shell,
                  OpenfpgaContext& openfpga_ctx, const openfpga::Command& cmd,
                  const openfpga::CommandContext& cmd_context) {
  t_vpr_setup& vpr_setup = openfpga_ctx.mutable_vpr_setup();

  // Check if architecture file is loaded
  const DeviceContext& device_ctx = g_vpr_ctx.device();
  if (nullptr == device_ctx.arch) {
    VTR_LOG_ERROR(
      "Cannot run packing: no architecture is loaded. Run 'read_vpr_arch' "
      "first.\n");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Running VPR pack flow...\n");

  // Check if circuit is loaded, check vpr_setup.FileNameOpts.CircuitFile
  // exist
  if (vpr_setup.FileNameOpts.CircuitFile.empty()) {
    VTR_LOG_ERROR(
      "Cannot run packing: no circuit netlist has been read. Run "
      "'read_circuit' first.\n");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  // Update vpr_setup from the openfpga shell options
  // vpr::sync_vpr_setup_to_app_options(vpr_setup, *shell);
  vpr::sync_vpr_setup_to_app_options(openfpga_ctx.mutable_vpr_setup(), *shell);

  // Get the device layout to use for packing from the shell options (if
  // specified)
  std::string device_layout = vpr_setup.PackerOpts.device_layout;
  openfpga::CommandOptionId opt_device = cmd.option("device");
  if (cmd_context.option_enable(cmd, opt_device)) {
    VTR_ASSERT(false == cmd_context.option_value(cmd, opt_device).empty());
    device_layout = cmd_context.option_value(cmd, opt_device);
  }

  // Handle optional output_file argument
  std::string pack_output_file = vpr_setup.FileNameOpts.NetFile;
  openfpga::CommandOptionId opt_output_file = cmd.option("output_file");
  if (cmd_context.option_enable(cmd, opt_output_file)) {
    if (!cmd_context.option_value(cmd, opt_output_file).empty()) {
      pack_output_file = cmd_context.option_value(cmd, opt_output_file);
    }
  }

  // Handle optional packing verbosity argument
  int pack_verbosity = vpr_setup.PackerOpts.pack_verbosity;
  openfpga::CommandOptionId opt_packing_verbose = cmd.option("verbose");
  if (cmd_context.option_enable(cmd, opt_packing_verbose)) {
    // pack_verbosity =
    //   std::stoi(cmd_context.option_value(cmd, opt_packing_verbose));
    pack_verbosity = 10;
  }

  // Force packing to be run and update vpr_setup
  vpr_setup.PackerOpts.doPacking = e_stage_action::DO;
  vpr_setup.PackerOpts.device_layout = device_layout;
  // vpr_setup.PackerOpts.output_file = pack_output_file;
  // vpr_setup.PackerOpts.pack_verbosity = pack_verbosity;

  // Run the VPR packing flow
  show_setup(openfpga_ctx.vpr_setup());
  CheckSetup(vpr_setup.PackerOpts, vpr_setup.PlacerOpts, vpr_setup.APOpts,
             vpr_setup.RouterOpts, vpr_setup.ServerOpts, vpr_setup.RoutingArch,
             vpr_setup.Segments, vpr_setup.Timing, device_ctx.arch->Chans);
  bool pack_success = vpr_pack_flow(vpr_setup, *device_ctx.arch);
  if (!pack_success) {
    VTR_LOG_ERROR("VPR packing failed.\n");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  VTR_LOG("VPR packing completed successfully.\n");
  return openfpga::CMD_EXEC_SUCCESS;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Report cluster template = = = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = =
int report_cluster_template(openfpga::Shell<OpenfpgaContext>* shell,
                            OpenfpgaContext& openfpga_ctx,
                            const openfpga::Command& cmd,
                            const openfpga::CommandContext& cmd_context) {
  (void)shell;
  (void)openfpga_ctx;
  (void)cmd;
  (void)cmd_context;

  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  std::string query = "*";
  openfpga::CommandOptionId opt_query = cmd.option("query");
  if (cmd_context.option_enable(cmd, opt_query)) {
    query = cmd_context.option_value(cmd, opt_query);
  }

  cluster_netlist_report clb_rptr(clb_nlist);

  const auto filtered_blocks = clb_rptr.filter_cluster_netlist(query);
  for (const ClusterBlockId blk_id : filtered_blocks) {
    VTR_LOG("Cluster: %s [%s]\n", clb_nlist.block_name(blk_id).c_str(),
            clb_nlist.block_type(blk_id)->pb_type->name);
  }
  VTR_LOG("Total clusters: %zu\n", clb_nlist.blocks().size());

  return openfpga::CMD_EXEC_SUCCESS;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Run VPR placement flow only = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = = =
int place_template(openfpga::Shell<OpenfpgaContext>* shell,
                   OpenfpgaContext& openfpga_ctx, const openfpga::Command& cmd,
                   const openfpga::CommandContext& cmd_context) {
  t_vpr_setup& vpr_setup = openfpga_ctx.mutable_vpr_setup();
  const DeviceContext& device_ctx = g_vpr_ctx.device();

  // Handle optional verbose argument
  bool verbose = false;
  openfpga::CommandOptionId opt_verbose = cmd.option("verbose");
  if (cmd_context.option_enable(cmd, opt_verbose)) {
    verbose = true;
  }

  vpr_setup.PlacerOpts.do_placement = e_stage_action::DO;
  vpr_setup.PlacerOpts.place_chan_width =
    shell->app_options_.placement.place_chan_width.int_value;
  vpr_setup.FileNameOpts.PlaceFile =
    vpr_setup.FileNameOpts.CircuitName + ".place";

  VTR_LOG("Running VPR place flow...\n");
  show_setup(openfpga_ctx.vpr_setup());
  const auto& placement_net_list =
    (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
  bool place_success =
    vpr_place_flow(placement_net_list, vpr_setup, *device_ctx.arch);

  if (!place_success) {
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  // TODO: Implement place flow logic here, using verbose if needed
  return openfpga::CMD_EXEC_SUCCESS;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Run VPR route flow only = = = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = =
int route_template(openfpga::Shell<OpenfpgaContext>* shell,
                   OpenfpgaContext& openfpga_ctx, const openfpga::Command& cmd,
                   const openfpga::CommandContext& cmd_context) {
  t_vpr_setup& vpr_setup = openfpga_ctx.mutable_vpr_setup();
  const DeviceContext& device_ctx = g_vpr_ctx.device();

  // Handle optional verbose argument
  bool verbose = false;
  openfpga::CommandOptionId opt_verbose = cmd.option("verbose");
  if (cmd_context.option_enable(cmd, opt_verbose)) {
    verbose = true;
  }

  vpr_setup.FileNameOpts.RouteFile =
    vpr_setup.FileNameOpts.CircuitName + ".route";

  show_setup(openfpga_ctx.vpr_setup());
  VTR_LOG("Running VPR route flow...\n");

  bool is_flat = vpr_setup.RouterOpts.flat_routing;
  const Netlist<>& router_net_list =
    is_flat ? (const Netlist<>&)g_vpr_ctx.atom().netlist()
            : (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
  if (is_flat) {
    VTR_LOG_WARN(
      "Disabling port equivalence in the architecture since flat routing is "
      "enabled.\n");
    // TODO:Fix this call to unset port equivalences from the device context
    // unset_port_equivalences(g_vpr_ctx.mutable_device());
  }
  RouteStatus route_status;
  route_status =
    vpr_route_flow(router_net_list, vpr_setup, *device_ctx.arch, is_flat);

  return openfpga::CMD_EXEC_SUCCESS;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// = Run VPR analysis flow only = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = = =
int analysis_template(openfpga::Shell<OpenfpgaContext>* shell,
                      OpenfpgaContext& openfpga_ctx,
                      const openfpga::Command& cmd,
                      const openfpga::CommandContext& cmd_context) {
  t_vpr_setup& vpr_setup = openfpga_ctx.mutable_vpr_setup();
  const DeviceContext& device_ctx = g_vpr_ctx.device();
  bool is_flat = vpr_setup.RouterOpts.flat_routing;
  const Netlist<>& router_net_list =
    is_flat ? (const Netlist<>&)g_vpr_ctx.atom().netlist()
            : (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
  const t_arch& arch = *device_ctx.arch;

  // Handle optional verbose argument
  bool verbose = false;
  openfpga::CommandOptionId opt_verbose = cmd.option("verbose");
  if (cmd_context.option_enable(cmd, opt_verbose)) {
    verbose = true;
  }

  VTR_LOG("Running VPR analysis flow...\n");
  // TODO: Instead of assuming success here, we should pass the actual routing
  // status from the route flow to the analysis flow and use it to determine
  // the exit code for this command
  auto chan_width = vpr_setup.RouterOpts.fixed_channel_width;
  RouteStatus route_status =
    RouteStatus(true, chan_width);  // Assume success for now since we don't
                                    // have the actual routing status here
  vpr_analysis_flow(router_net_list, vpr_setup, arch, route_status, is_flat);

  return openfpga::CMD_EXEC_SUCCESS;
}

static int vpr(int argc, char** argv) {
  vtr::ScopedFinishTimer t("The entire flow of VPR");

  t_options Options = t_options();
  /* Arch should NOT be freed once this function is done */
  t_arch* Arch = new t_arch;
  t_vpr_setup vpr_setup = t_vpr_setup();

  try {
    vpr_install_signal_handler();

    /* Read options, architecture, and circuit netlist */
    vpr_init(argc, const_cast<const char**>(argv), &Options, &vpr_setup, Arch);

    if (Options.show_version) {
      return SUCCESS_EXIT_CODE;
    }

    bool flow_succeeded = vpr_flow(vpr_setup, *Arch);
    if (!flow_succeeded) {
      VTR_LOG("VPR failed to implement circuit\n");
      return UNIMPLEMENTABLE_EXIT_CODE;
    }

    auto& timing_ctx = g_vpr_ctx.timing();
    print_timing_stats("Flow", timing_ctx.stats);

    /* TODO: move this to the end of flow
     * free data structures
     */
    /* vpr_free_all(Arch, vpr_setup); */

    VTR_LOG("VPR succeeded\n");

  } catch (const tatum::Error& tatum_error) {
    VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());

    return ERROR_EXIT_CODE;

  } catch (const VprError& vpr_error) {
    vpr_print_error(vpr_error);

    if (vpr_error.type() == VPR_ERROR_INTERRUPTED) {
      return INTERRUPTED_EXIT_CODE;
    } else {
      return ERROR_EXIT_CODE;
    }

  } catch (const vtr::VtrError& vtr_error) {
    VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(),
                  vtr_error.what());

    return ERROR_EXIT_CODE;
  }

  /* Signal success to scripts */
  return SUCCESS_EXIT_CODE;
}

/* A wrapper to return proper codes for openfpga shell */
int vpr_wrapper(int argc, char** argv) {
  if (SUCCESS_EXIT_CODE != vpr(argc, argv)) {
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  return openfpga::CMD_EXEC_SUCCESS;
}

/**
 * VPR program with clean up
 */
static int vpr_standalone(int argc, char** argv) {
  vtr::ScopedFinishTimer t("The entire flow of VPR");

  t_options Options = t_options();
  t_arch Arch = t_arch();
  t_vpr_setup vpr_setup = t_vpr_setup();

  try {
    vpr_install_signal_handler();

    /* Read options, architecture, and circuit netlist */
    vpr_init(argc, const_cast<const char**>(argv), &Options, &vpr_setup, &Arch);
    if (Options.show_version) {
      vpr_free_all(Arch, vpr_setup);
      return SUCCESS_EXIT_CODE;
    }

    bool flow_succeeded = vpr_flow(vpr_setup, Arch);
    if (!flow_succeeded) {
      VTR_LOG("VPR failed to implement circuit\n");
      vpr_free_all(Arch, vpr_setup);
      return UNIMPLEMENTABLE_EXIT_CODE;
    }

    auto& timing_ctx = g_vpr_ctx.timing();
    print_timing_stats("Flow", timing_ctx.stats);

    /* free data structures */
    vpr_free_all(Arch, vpr_setup);

    VTR_LOG("VPR succeeded\n");

  } catch (const tatum::Error& tatum_error) {
    VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());
    vpr_free_all(Arch, vpr_setup);

    return ERROR_EXIT_CODE;

  } catch (const VprError& vpr_error) {
    vpr_print_error(vpr_error);
    if (vpr_error.type() == VPR_ERROR_INTERRUPTED) {
      vpr_free_all(Arch, vpr_setup);
      return INTERRUPTED_EXIT_CODE;
    } else {
      vpr_free_all(Arch, vpr_setup);
      return ERROR_EXIT_CODE;
    }

  } catch (const vtr::VtrError& vtr_error) {
    VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(),
                  vtr_error.what());
    vpr_free_all(Arch, vpr_setup);

    return ERROR_EXIT_CODE;
  }

  /* Signal success to scripts */
  return SUCCESS_EXIT_CODE;
}

/* A wrapper to return proper codes for openfpga shell */
int vpr_standalone_wrapper(int argc, char** argv) {
  if (SUCCESS_EXIT_CODE != vpr_standalone(argc, argv)) {
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  return openfpga::CMD_EXEC_SUCCESS;
}

} /* End namespace vpr */
