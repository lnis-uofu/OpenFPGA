#include "staged_flow.h"

#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <vector>

#include "analytical_placement_flow.h"
#include "draw.h"
#include "echo_files.h"
#include "globals.h"
#include "read_place.h"
#include "setup_grid.h"
#include "stats.h"
#include "tatum/error.hpp"
#include "vpr_api.h"
#include "vpr_app_options.h"
#include "vpr_error.h"
#include "vpr_run.h"
#include "vpr_tatum_error.h"
#include "vtr_error.h"
#include "vtr_log.h"
#include "vtr_time.h"
#include "vtr_util.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

using vpr::VprRun;

/* The name of the command to read an input */
const char* input_command(const OpenFpgaInput& input) {
  switch (input) {
    case OpenFpgaInput::VPR_ARCH:
      return "read_vpr_arch";
    case OpenFpgaInput::OPENFPGA_ARCH:
      return "read_openfpga_arch";
    case OpenFpgaInput::CIRCUIT:
      return "read_circuit";
    default:
      break;
  }
  return "";
}

/* The name of the command that runs a stage */
const char* stage_command(const OpenFpgaStage& stage) {
  switch (stage) {
    case OpenFpgaStage::PACKED:
      return "pack";
    case OpenFpgaStage::PLACED:
      return "place";
    case OpenFpgaStage::ROUTED:
      return "route";
    case OpenFpgaStage::ANALYZED:
      return "analysis";
    case OpenFpgaStage::REPACKED:
      return "repack";
    case OpenFpgaStage::BITSTREAM:
      return "build_architecture_bitstream";
    default:
      break;
  }
  return "";
}

/* The name of the command that reads the results of a stage from a file */
const char* import_command(const OpenFpgaStage& stage) {
  switch (stage) {
    case OpenFpgaStage::PACKED:
      return "read_packing";
    case OpenFpgaStage::PLACED:
      return "read_placement";
    case OpenFpgaStage::ROUTED:
      return "read_routing";
    default:
      break;
  }
  return "";
}

/* Run a function that may throw the errors of VPR. Log the error and return
 * false if it does */
bool guarded(const std::function<void()>& func) {
  try {
    func();
    return true;
  } catch (const tatum::Error& tatum_error) {
    VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());
  } catch (const VprError& vpr_error) {
    vpr_print_error(vpr_error);
  } catch (const vtr::VtrError& vtr_error) {
    VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(),
                  vtr_error.what());
  } catch (const std::exception& exc) {
    VTR_LOG_ERROR("%s\n", exc.what());
  }
  return false;
}

/* The path as a whole, from the current directory, so that it still names the
 * file when the directory changes. A path that cannot be made one is kept */
std::string whole_path(const std::string& file) {
  std::error_code error;
  const std::filesystem::path path = std::filesystem::absolute(file, error);
  return error ? file : path.lexically_normal().string();
}

/* The session forgets what it knows if another command replaced the data of
 * VPR */
void sync_with_vpr(OpenFpgaSession& session) {
  if (session.vpr_generation() != vpr::vpr_run_generation()) {
    session.reset();
    session.set_vpr_generation(vpr::vpr_run_generation());
  }
}

/* Return true if the flow is run stage by stage, with the data of VPR that the
 * session works with */
bool in_staged_flow(const OpenFpgaSession& session) {
  return session.has_input(OpenFpgaInput::VPR_ARCH) &&
         session.vpr_generation() == vpr::vpr_run_generation();
}

/* Start a new run of VPR, which is the one the session works with */
VprRun& start_run(OpenFpgaSession& session) {
  VprRun& run = vpr::begin_vpr_run();
  session.set_vpr_generation(vpr::vpr_run_generation());
  return run;
}

void drop_run(OpenFpgaSession& session) {
  vpr::release_vpr_run();
  session.set_vpr_generation(vpr::vpr_run_generation());
}

/* A stage failed. If VPR ended the stage without an error (the circuit does
 * not fit, the router found no legal routing), it left what it had made,
 * e.g., the routing with the nodes that nets share: keep it, so that it can be
 * read to tell why, and mark the run as failed. If it stopped with an error,
 * VPR may have been interrupted anywhere: the packing and the placement are
 * not read after that, but the device is made before them and they do not
 * change it, so it is kept for them. A routing is not */
void keep_failed_run(OpenFpgaSession& session, VprRun& run,
                     const OpenFpgaStage& stage, const bool vpr_ended) {
  if (vpr_ended || OpenFpgaStage::PACKED == stage ||
      OpenFpgaStage::PLACED == stage) {
    run.failed = true;
  } else {
    drop_run(session);
  }
}

/* Why a stage failed, for the session to tell */
std::string failure_message(const OpenFpgaStage& stage, const VprRun& run) {
  switch (stage) {
    case OpenFpgaStage::PACKED:
      return "Packing failed: the circuit could not be packed on the device";
    case OpenFpgaStage::PLACED:
      return "Placement failed: the blocks could not be placed on the device";
    case OpenFpgaStage::ROUTED:
      if (NO_FIXED_CHANNEL_WIDTH != run.setup.RouterOpts.fixed_channel_width) {
        return "Routing failed: the router found no legal routing with the "
               "channel width " +
               std::to_string(run.setup.RouterOpts.fixed_channel_width);
      }
      return "Routing failed: the router found no legal routing";
    default:
      break;
  }
  return to_string(stage) + " failed";
}

/* The command line of VPR for the flow. The stages are run by the commands,
 * so that all of them are asked of VPR. If the circuit is not known yet, it
 * has a placeholder name */
bool flow_args(const AppOptions& options, const bool with_circuit,
               std::vector<std::string>& args) {
  AppOptions copy = options;
  std::string error;
  if (!with_circuit && !copy.set("filename.circuit_name", "circuit", error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return false;
  }
  if (!vpr_args_from_app_options(copy, args, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return false;
  }
  args.push_back("--pack");
  args.push_back("--place");
  args.push_back("--route");
  args.push_back("--analysis");
  return true;
}

/* Read the architecture of VPR and set up VPR for it, as 'vpr' does before it
 * reads the circuit */
void read_arch_only(VprRun& run) {
  t_vpr_setup& setup = run.setup;
  const t_options& options = run.options;
  setup.TimingEnabled = options.timing_analysis;
  setup.device_layout = options.device_layout;
  setup.device_width = options.device_width;
  setup.constant_net_method = options.constant_net_method;
  setup.clock_modeling = options.clock_modeling;
  setup.two_stage_clock_routing = options.two_stage_clock_routing;
  setup.exit_before_pack = options.exit_before_pack;
  setup.num_workers = 1;
  /* As 'vpr' does first: the device is made from this alone, before a circuit
   * is read, and it looks up the echo files */
  init_echo_files_from_options(run.options.echo_file.value());
  vpr_setup_vpr(
    &run.options, setup.TimingEnabled, true, &setup.FileNameOpts, &run.arch,
    &setup.NetlistOpts, &setup.PackerOpts, &setup.PlacerOpts, &setup.APOpts,
    &setup.RouterOpts, &setup.AnalysisOpts, &setup.CRROpts, &setup.NocOpts,
    &setup.ServerOpts, setup.RoutingArch, &setup.PackerRRGraph, setup.Segments,
    &setup.Timing, &setup.ShowGraphics, &setup.GraphPause, &setup.SaveGraphics,
    &setup.GraphicsCommands, &setup.RendererType, &setup.PowerOpts, &setup);
  vpr_check_arch(run.arch);
}

/* Read the architecture and the circuit, as 'vpr' does. Start from fresh
 * data */
bool init_run(OpenFpgaSession& session, const std::vector<std::string>& args) {
  VprRun& run = start_run(session);
  run.args = args;
  const bool ok = guarded([&run, &args]() {
    run.options = read_vpr_options(args);
    vpr_init_with_options(&run.options, &run.setup, &run.arch);
  });
  if (!ok) {
    drop_run(session);
    return false;
  }
  run.initialized = true;
  return true;
}

/* Create the device, once the run is initialized */
bool create_device(OpenFpgaSession& session, VprRun& run) {
  if (!has_fixed_device_size(run.setup)) {
    VTR_LOG_ERROR(
      "The layout of the device is '%s', which is sized from the result of "
      "packing. The stage-by-stage flow needs a fixed layout: give the "
      "name of one with 'read_vpr_arch --layout', or set the option "
      "general.device_layout\n",
      run.setup.device_layout.c_str());
    drop_run(session);
    return false;
  }
  const bool ok = guarded([&run]() {
    vpr_create_device(run.setup, run.arch, false);
    init_graphics(run.setup, run.arch);
  });
  if (!ok) {
    drop_run(session);
    return false;
  }
  run.last_stage = OpenFpgaStage::DEVICE;
  return true;
}

/* The stages are run, or, if a file is given, their results are read from it,
 * as VPR does for the stages before the last one it is asked for */
bool do_pack(OpenFpgaSession& session, VprRun& run,
             const std::string& file = std::string()) {
  bool packed = false;
  if (!file.empty()) {
    run.setup.FileNameOpts.NetFile = file;
    run.setup.PackerOpts.doPacking = e_stage_action::LOAD;
  }
  const bool ok = guarded([&run, &packed]() {
    packed = vpr_pack_flow(run.setup, run.arch);
    if (packed) {
      print_resource_usage();
      print_device_utilization(run.setup.PackerOpts.target_device_utilization);
    }
  });
  run.setup.PackerOpts.doPacking = e_stage_action::DO;
  if (!ok || !packed) {
    VTR_LOG_ERROR("Packing failed\n");
    keep_failed_run(session, run, OpenFpgaStage::PACKED, ok);
    return false;
  }
  run.last_stage = OpenFpgaStage::PACKED;
  return true;
}

bool do_place(OpenFpgaSession& session, VprRun& run,
              const std::string& file = std::string()) {
  bool placed = false;
  if (!file.empty()) {
    run.setup.FileNameOpts.PlaceFile = file;
    run.setup.PlacerOpts.do_placement = e_stage_action::LOAD;
  }
  const bool ok = guarded([&run, &placed]() {
    const Netlist<>& net_list =
      (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
    placed = vpr_place_flow(net_list, run.setup, run.arch);
  });
  run.setup.PlacerOpts.do_placement = e_stage_action::DO;
  if (!ok || !placed) {
    VTR_LOG_ERROR("Placement failed\n");
    keep_failed_run(session, run, OpenFpgaStage::PLACED, ok);
    return false;
  }
  run.last_stage = OpenFpgaStage::PLACED;
  return true;
}

/* Pack and place at once with the analytical placement of VPR, as
 * 'vpr --analytical_place' does, on the device that is made already */
bool do_analytical_place(OpenFpgaSession& session, VprRun& run) {
  const bool ok = guarded([&run]() {
    run_analytical_placement_flow(run.setup);
    const auto& filename_opts = run.setup.FileNameOpts;
    const auto& cluster_ctx = g_vpr_ctx.clustering();
    g_vpr_ctx.mutable_placement().placement_id = print_place(
      filename_opts.NetFile.c_str(), cluster_ctx.clb_nlist.netlist_id().c_str(),
      filename_opts.PlaceFile.c_str(), g_vpr_ctx.placement().block_locs());
  });
  if (!ok) {
    VTR_LOG_ERROR("Analytical placement failed\n");
    keep_failed_run(session, run, OpenFpgaStage::PLACED, ok);
    return false;
  }
  run.last_stage = OpenFpgaStage::PLACED;
  return true;
}

/* The netlist that the router works on: the atoms when the routing is flat,
 * the clusters otherwise */
const Netlist<>& router_net_list(const VprRun& run) {
  if (run.setup.RouterOpts.flat_routing) {
    return (const Netlist<>&)g_vpr_ctx.atom().netlist();
  }
  return (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
}

/* Port equivalence does not hold when the routing goes inside the clusters:
 * turn it off in the device, as VPR does before a flat routing. The device is
 * that of a fresh run, which is made again for each routing */
void unset_port_equivalences() {
  DeviceContext& device_ctx = g_vpr_ctx.mutable_device();
  for (auto& physical_type : device_ctx.physical_tile_types) {
    for (auto& sub_tile : physical_type.sub_tiles) {
      for (auto& port : sub_tile.ports) {
        port.equivalent = PortEquivalence::NONE;
      }
    }
  }
  for (auto& logical_type : device_ctx.logical_block_types) {
    if (!logical_type.pb_type) {
      continue;
    }
    for (int i = 0; i < logical_type.pb_type->num_ports; i++) {
      logical_type.pb_type->ports[i].equivalent = PortEquivalence::NONE;
    }
  }
}

bool do_route(OpenFpgaSession& session, VprRun& run,
              const std::string& file = std::string()) {
  if (!file.empty()) {
    if (NO_FIXED_CHANNEL_WIDTH == run.setup.RouterOpts.fixed_channel_width) {
      VTR_LOG_ERROR(
        "The routing is read against a channel width, which is not fixed: "
        "set the option router.fixed_channel_width\n");
      drop_run(session);
      return false;
    }
    run.setup.FileNameOpts.RouteFile = file;
    run.setup.RouterOpts.doRouting = e_stage_action::LOAD;
  }
  const bool flat = run.setup.RouterOpts.flat_routing;
  const bool ok = guarded([&run, flat]() {
    if (flat) {
      VTR_LOG_WARN(
        "Disabling port equivalence in the architecture since flat routing "
        "is enabled.\n");
      unset_port_equivalences();
    }
    run.route_status =
      vpr_route_flow(router_net_list(run), run.setup, run.arch, flat);
  });
  run.setup.RouterOpts.doRouting = e_stage_action::DO;
  if (!ok || !run.route_status.success()) {
    VTR_LOG_ERROR("Routing failed\n");
    keep_failed_run(session, run, OpenFpgaStage::ROUTED, ok);
    return false;
  }
  run.last_stage = OpenFpgaStage::ROUTED;
  return true;
}

/* Analysis changes the data of VPR: it synchronizes the packing with the
 * routing results (with the routing inside the clusters, when it is flat).
 * It is run on the data of the routing, as VPR cannot run it twice on the
 * same data */
bool do_analysis(OpenFpgaSession& session, VprRun& run) {
  bool analyzed = false;
  const bool ok = guarded([&run, &analyzed]() {
    analyzed =
      vpr_analysis_flow(router_net_list(run), run.setup, run.arch,
                        run.route_status, run.setup.RouterOpts.flat_routing);
  });
  if (!ok || !analyzed) {
    VTR_LOG_ERROR("Analysis failed\n");
    drop_run(session);
    return false;
  }
  run.last_stage = OpenFpgaStage::ANALYZED;
  return true;
}

/* Tell which inputs are missing, and how to read them */
void report_missing_inputs(const OpenFpgaSession& session) {
  for (const OpenFpgaInput& input :
       {OpenFpgaInput::VPR_ARCH, OpenFpgaInput::OPENFPGA_ARCH}) {
    if (!session.has_input(input)) {
      VTR_LOG_ERROR("The input %s has not been read: use the command '%s'\n",
                    to_string(input).c_str(), input_command(input));
    }
  }
}

/* A stage that packs the circuit (PACKED, and PLACED with the analytical
 * placer) needs it read: the device alone is made without it */
bool circuit_ready(const OpenFpgaSession& session, const OpenFpgaStage& stage,
                   const char* command) {
  const bool packs =
    OpenFpgaStage::PACKED == stage ||
    (OpenFpgaStage::PLACED == stage && session.analytical_placer());
  /* Without the architectures, making the device says what is missing */
  if (!packs || session.has_input(OpenFpgaInput::CIRCUIT) ||
      !session.has_architectures()) {
    return true;
  }
  VTR_LOG_ERROR(
    "The input %s has not been read: use the command '%s' (the device is made "
    "from the architectures, and '%s' packs the circuit onto it)\n",
    to_string(OpenFpgaInput::CIRCUIT).c_str(),
    input_command(OpenFpgaInput::CIRCUIT), command);
  return false;
}

/* Make the device from the architectures alone: the circuit is not read yet */
bool ensure_device_without_circuit(OpenFpgaSession& session,
                                   const std::vector<std::string>& args) {
  VprRun* current = vpr::current_vpr_run();
  const bool same = nullptr != current && !current->failed &&
                    !current->initialized && current->args == args;
  /* The architecture of VPR was just read for these options: the device is
   * made from it */
  if (same && OpenFpgaStage::UNINIT == current->last_stage) {
    if (!create_device(session, *current)) {
      return false;
    }
  } else if (!same || OpenFpgaStage::DEVICE != current->last_stage) {
    VprRun& run = start_run(session);
    run.args = args;
    const bool ok = guarded([&run, &args]() {
      run.options = read_vpr_options(args);
      read_arch_only(run);
    });
    if (!ok) {
      drop_run(session);
      return false;
    }
    if (!create_device(session, run)) {
      return false;
    }
  }
  session.complete(OpenFpgaStage::DEVICE);
  return true;
}

/* Make sure the device is created, from the inputs read and the current
 * options: from the architectures, and the circuit once it is read */
bool ensure_device(OpenFpgaSession& session) {
  if (!session.has_architectures()) {
    report_missing_inputs(session);
    return false;
  }
  const bool with_circuit = session.has_input(OpenFpgaInput::CIRCUIT);
  std::vector<std::string> args;
  if (!flow_args(session.app_options(), with_circuit, args)) {
    return false;
  }
  if (!with_circuit) {
    return ensure_device_without_circuit(session, args);
  }
  /* The data of the run is good if it was made from the same options */
  VprRun* run = vpr::current_vpr_run();
  if (nullptr == run || !run->initialized || run->failed || run->args != args) {
    if (!init_run(session, args)) {
      return false;
    }
    run = vpr::current_vpr_run();
  }
  if (OpenFpgaStage::UNINIT == run->last_stage &&
      !create_device(session, *run)) {
    return false;
  }
  session.complete(OpenFpgaStage::DEVICE);
  return true;
}

/* Bring the data of VPR to the results of a stage, by running the stages
 * again from the device if need be */
bool prepare_vpr(OpenFpgaSession& session, const OpenFpgaStage& target) {
  std::vector<std::string> args;
  if (!flow_args(session.app_options(), true, args)) {
    return false;
  }
  VprRun* run = vpr::current_vpr_run();
  if (nullptr != run && run->initialized && !run->failed && run->args == args &&
      run->last_stage == target) {
    return true;
  }
  if (OpenFpgaStage::DEVICE != target) {
    VTR_LOG("Run the stages up to %s again on fresh data\n",
            to_string(target).c_str());
  }
  if (!init_run(session, args)) {
    return false;
  }
  run = vpr::current_vpr_run();
  if (!create_device(session, *run)) {
    return false;
  }
  if (OpenFpgaStage::DEVICE == target) {
    return true;
  }
  /* The analytical placer made the packing and the placement at once: it
   * makes them again, unless they were read from files */
  const std::string packing = session.imported_file(OpenFpgaStage::PACKED);
  if (session.analytical_placer() && packing.empty()) {
    if (OpenFpgaStage::PACKED == target) {
      VTR_LOG_ERROR(
        "The packing of the analytical placer (option place.placer) is made "
        "with its placement, and the placement cannot be made again apart: "
        "use the command 'place' (or 'pack') to make both again\n");
      return false;
    }
    if (!do_analytical_place(session, *run)) {
      return false;
    }
    if (OpenFpgaStage::PLACED == target) {
      return true;
    }
    return do_route(session, *run,
                    session.imported_file(OpenFpgaStage::ROUTED));
  }
  if (!do_pack(session, *run, packing)) {
    return false;
  }
  if (OpenFpgaStage::PACKED == target) {
    return true;
  }
  if (!do_place(session, *run, session.imported_file(OpenFpgaStage::PLACED))) {
    return false;
  }
  if (OpenFpgaStage::PLACED == target) {
    return true;
  }
  return do_route(session, *run, session.imported_file(OpenFpgaStage::ROUTED));
}

}  // End anonymous namespace

bool read_vpr_arch_input(OpenFpgaSession& session, const std::string& file,
                         const std::string& layout) {
  sync_with_vpr(session);
  std::string error;
  if (!session.set_app_option("filename.arch_file", file, error) ||
      !session.set_app_option("general.device_layout", layout, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return false;
  }
  session.read_input(OpenFpgaInput::VPR_ARCH, whole_path(file));

  VTR_LOG("Reading VPR architecture '%s' with layout '%s'...\n", file.c_str(),
          layout.c_str());
  std::vector<std::string> args;
  if (!flow_args(session.app_options(), false, args)) {
    session.fail_input(OpenFpgaInput::VPR_ARCH);
    return false;
  }
  VprRun& run = start_run(session);
  run.args = args;
  const bool ok = guarded([&run, &args]() {
    run.options = read_vpr_options(args);
    read_arch_only(run);
  });
  if (!ok) {
    drop_run(session);
    session.fail_input(OpenFpgaInput::VPR_ARCH);
    return false;
  }
  /* The device is made once both architectures are read */
  if (session.has_architectures()) {
    return ensure_device(session);
  }
  return true;
}

bool read_circuit_input(OpenFpgaSession& session, const std::string& file) {
  sync_with_vpr(session);
  if (!session.can_read(OpenFpgaInput::CIRCUIT)) {
    VTR_LOG_ERROR(
      "The circuit is read against the VPR architecture, which has not been "
      "read: use the command 'read_vpr_arch' first\n");
    return false;
  }
  std::string error;
  if (!session.set_app_option("filename.circuit_file", file, error)) {
    VTR_LOG_ERROR("%s\n", error.c_str());
    return false;
  }
  session.read_input(OpenFpgaInput::CIRCUIT, whole_path(file));

  VTR_LOG("Reading circuit '%s'...\n", file.c_str());
  std::vector<std::string> args;
  if (!flow_args(session.app_options(), true, args) ||
      !init_run(session, args)) {
    session.fail_input(OpenFpgaInput::CIRCUIT);
    return false;
  }
  /* The device is created if the last input has been read */
  if (session.has_all_inputs()) {
    return ensure_device(session);
  }
  return true;
}

bool make_flow_device(OpenFpgaSession& session) {
  sync_with_vpr(session);
  return session.reached(OpenFpgaStage::DEVICE) || ensure_device(session);
}

bool openfpga_arch_input_read(OpenFpgaSession& session,
                              const std::string& file) {
  sync_with_vpr(session);
  session.read_input(OpenFpgaInput::OPENFPGA_ARCH, whole_path(file));
  if (session.has_architectures()) {
    return ensure_device(session);
  }
  return true;
}

void fail_openfpga_arch_input(OpenFpgaSession& session) {
  sync_with_vpr(session);
  session.fail_input(OpenFpgaInput::OPENFPGA_ARCH);
}

namespace {

/* Pack and place at once, with the analytical placer: the command 'pack' or
 * 'place' when the option place.placer is analytical */
bool run_analytical_stages(OpenFpgaSession& session) {
  if (!session.reached(OpenFpgaStage::DEVICE) && !ensure_device(session)) {
    return false;
  }
  vtr::ScopedStartFinishTimer timer("Stages PACKED and PLACED (analytical)");
  if (!prepare_vpr(session, OpenFpgaStage::DEVICE)) {
    session.fail(OpenFpgaStage::DEVICE);
    return false;
  }
  VprRun& run = *vpr::current_vpr_run();
  const auto started = std::chrono::steady_clock::now();
  if (!do_analytical_place(session, run)) {
    session.fail(OpenFpgaStage::PACKED,
                 "Analytical placement failed: the circuit could not be "
                 "packed and placed on the device");
    return false;
  }
  session.complete(OpenFpgaStage::PACKED);
  session.complete(OpenFpgaStage::PLACED);
  /* The time is the one of the placement, which made the packing too */
  session.set_stage_seconds(
    OpenFpgaStage::PLACED,
    std::chrono::duration<double>(std::chrono::steady_clock::now() - started)
      .count());
  return true;
}

}  // End anonymous namespace

bool run_flow_stage(OpenFpgaSession& session, const OpenFpgaStage& stage) {
  sync_with_vpr(session);
  if (!circuit_ready(session, stage, stage_command(stage))) {
    return false;
  }
  if (session.analytical_placer() &&
      (OpenFpgaStage::PACKED == stage || OpenFpgaStage::PLACED == stage)) {
    return run_analytical_stages(session);
  }
  const OpenFpgaStage required = required_stage(stage);

  /* The device is made from the inputs: it can be created by the stage */
  if (!session.reached(required) && OpenFpgaStage::DEVICE == required) {
    if (!ensure_device(session)) {
      return false;
    }
  }
  if (!session.reached(required)) {
    VTR_LOG_ERROR(
      "Cannot run the stage %s: the stage %s has not been reached, use the "
      "command '%s' first\n",
      to_string(stage).c_str(), to_string(required).c_str(),
      stage_command(required));
    return false;
  }

  vtr::ScopedStartFinishTimer timer(std::string("Stage ") + to_string(stage));
  if (!prepare_vpr(session, required)) {
    session.fail(required);
    return false;
  }
  VprRun& run = *vpr::current_vpr_run();
  const auto started = std::chrono::steady_clock::now();
  /* The run may be released by a stage that fails */
  const std::string failed_message = failure_message(stage, run);
  bool ok = false;
  switch (stage) {
    case OpenFpgaStage::PACKED:
      ok = do_pack(session, run);
      break;
    case OpenFpgaStage::PLACED:
      ok = do_place(session, run);
      break;
    case OpenFpgaStage::ROUTED:
      ok = do_route(session, run);
      break;
    case OpenFpgaStage::ANALYZED:
      ok = do_analysis(session, run);
      break;
    default:
      VTR_LOG_ERROR("The stage %s cannot be run by a command yet\n",
                    to_string(stage).c_str());
      return false;
  }
  if (!ok) {
    session.fail(stage, failed_message);
    return false;
  }
  session.complete(stage);
  session.set_stage_seconds(stage, std::chrono::duration<double>(
                                     std::chrono::steady_clock::now() - started)
                                     .count());
  return true;
}

bool import_flow_stage(OpenFpgaSession& session, const OpenFpgaStage& stage,
                       const std::string& file) {
  sync_with_vpr(session);
  const OpenFpgaStage required = required_stage(stage);
  const char* command = import_command(stage);
  if (OpenFpgaStage::PACKED != stage && OpenFpgaStage::PLACED != stage &&
      OpenFpgaStage::ROUTED != stage) {
    VTR_LOG_ERROR("The results of the stage %s cannot be read from a file\n",
                  to_string(stage).c_str());
    return false;
  }
  if (!circuit_ready(session, stage, command)) {
    return false;
  }
  if (!session.reached(required) && OpenFpgaStage::DEVICE == required) {
    if (!ensure_device(session)) {
      return false;
    }
  }
  if (!session.reached(required)) {
    VTR_LOG_ERROR(
      "Cannot read the results of the stage %s with '%s': the stage %s has "
      "not been reached, use the command '%s' first\n",
      to_string(stage).c_str(), command, to_string(required).c_str(),
      stage_command(required));
    return false;
  }
  if (!std::ifstream(file).good()) {
    VTR_LOG_ERROR(
      "Cannot read the results of the stage %s: the file '%s' "
      "does not exist\n",
      to_string(stage).c_str(), file.c_str());
    return false;
  }

  vtr::ScopedStartFinishTimer timer(std::string("Read stage ") +
                                    to_string(stage));
  if (!prepare_vpr(session, required)) {
    session.fail(required);
    return false;
  }
  VprRun& run = *vpr::current_vpr_run();
  /* The run may be released by a stage that fails */
  const std::string failed_message = failure_message(stage, run);
  bool ok = false;
  switch (stage) {
    case OpenFpgaStage::PACKED:
      ok = do_pack(session, run, file);
      break;
    case OpenFpgaStage::PLACED:
      ok = do_place(session, run, file);
      break;
    default:
      ok = do_route(session, run, file);
      break;
  }
  if (!ok) {
    session.fail(stage, failed_message);
    return false;
  }
  session.complete(stage);
  session.set_imported(stage, file);
  return true;
}

bool staged_flow_current(const OpenFpgaSession& session) {
  return in_staged_flow(session);
}

bool stage_ready(const OpenFpgaSession& session, const OpenFpgaStage& stage,
                 const char* command) {
  if (!in_staged_flow(session) || session.reached(stage)) {
    return true;
  }
  if (OpenFpgaStage::DEVICE == stage) {
    VTR_LOG_ERROR(
      "Cannot run '%s': the stage DEVICE has not been reached, because the "
      "architectures are not read (or one was read again). Use the commands "
      "'read_vpr_arch' and 'read_openfpga_arch' first\n",
      command);
    return false;
  }
  if (const std::string* failure = session.failure(stage)) {
    VTR_LOG_ERROR(
      "Cannot run '%s': the stage %s has not been reached, because it failed "
      "(%s). Change what made it fail, and use the command '%s' again\n",
      command, to_string(stage).c_str(), failure->c_str(),
      stage_command(stage));
    return false;
  }
  VTR_LOG_ERROR(
    "Cannot run '%s': the stage %s has not been reached, or it was dropped "
    "because an earlier stage or an option changed. Use the command '%s' "
    "first\n",
    command, to_string(stage).c_str(), stage_command(stage));
  return false;
}

void stage_linked(OpenFpgaSession& session) {
  if (in_staged_flow(session)) {
    session.set_linked_generation(session.vpr_generation());
  }
}

bool linked_ready(const OpenFpgaSession& session, const char* command) {
  if (!in_staged_flow(session) ||
      session.linked_generation() == session.vpr_generation()) {
    return true;
  }
  VTR_LOG_ERROR(
    "Cannot run '%s': the architecture was linked to the data of an earlier "
    "run of VPR, which a stage that ran again has replaced. Use the commands "
    "'link_openfpga_arch' and 'build_fabric' again first\n",
    command);
  return false;
}

void stage_done(OpenFpgaSession& session, const OpenFpgaStage& stage) {
  if (in_staged_flow(session)) {
    session.complete(stage);
  }
}

void stage_failed(OpenFpgaSession& session, const OpenFpgaStage& stage) {
  if (in_staged_flow(session)) {
    session.fail(stage);
  }
}

} /* End namespace openfpga */
