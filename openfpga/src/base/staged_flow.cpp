#include "staged_flow.h"

#include <exception>
#include <fstream>
#include <functional>
#include <vector>

#include "draw.h"
#include "globals.h"
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
#include "vtr_util.h"
#include "vtr_time.h"

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
    drop_run(session);
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
    drop_run(session);
    return false;
  }
  run.last_stage = OpenFpgaStage::PLACED;
  return true;
}

bool do_route(OpenFpgaSession& session, VprRun& run,
              const std::string& file = std::string()) {
  if (run.setup.RouterOpts.flat_routing) {
    VTR_LOG_ERROR(
      "Flat routing is not supported by the stage-by-stage flow yet\n");
    drop_run(session);
    return false;
  }
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
  const bool ok = guarded([&run]() {
    const Netlist<>& net_list =
      (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
    run.route_status = vpr_route_flow(net_list, run.setup, run.arch, false);
  });
  run.setup.RouterOpts.doRouting = e_stage_action::DO;
  if (!ok || !run.route_status.success()) {
    VTR_LOG_ERROR("Routing failed\n");
    drop_run(session);
    return false;
  }
  run.last_stage = OpenFpgaStage::ROUTED;
  return true;
}

/* Analysis changes the data of VPR: it synchronizes the packing with the
 * routing results. It is run on the data of the routing, as VPR cannot run it
 * twice on the same data */
bool do_analysis(OpenFpgaSession& session, VprRun& run) {
  bool analyzed = false;
  const bool ok = guarded([&run, &analyzed]() {
    const Netlist<>& net_list =
      (const Netlist<>&)g_vpr_ctx.clustering().clb_nlist;
    analyzed =
      vpr_analysis_flow(net_list, run.setup, run.arch, run.route_status, false);
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
       {OpenFpgaInput::VPR_ARCH, OpenFpgaInput::OPENFPGA_ARCH,
        OpenFpgaInput::CIRCUIT}) {
    if (!session.has_input(input)) {
      VTR_LOG_ERROR("The input %s has not been read: use the command '%s'\n",
                    to_string(input).c_str(), input_command(input));
    }
  }
}

/* Make sure the device is created, from the inputs read and the current
 * options */
bool ensure_device(OpenFpgaSession& session) {
  if (!session.has_all_inputs()) {
    report_missing_inputs(session);
    return false;
  }
  std::vector<std::string> args;
  if (!flow_args(session.app_options(), true, args)) {
    return false;
  }
  /* The data of the run is good if it was made from the same options */
  VprRun* run = vpr::current_vpr_run();
  if (nullptr == run || !run->initialized || run->args != args) {
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
  if (nullptr != run && run->initialized && run->args == args &&
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
  if (!do_pack(session, *run, session.imported_file(OpenFpgaStage::PACKED))) {
    return false;
  }
  if (OpenFpgaStage::PACKED == target) {
    return true;
  }
  if (!do_place(session, *run,
               session.imported_file(OpenFpgaStage::PLACED))) {
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
  session.read_input(OpenFpgaInput::VPR_ARCH);

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
  session.read_input(OpenFpgaInput::CIRCUIT);

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

bool openfpga_arch_input_read(OpenFpgaSession& session) {
  sync_with_vpr(session);
  session.read_input(OpenFpgaInput::OPENFPGA_ARCH);
  if (session.has_all_inputs()) {
    return ensure_device(session);
  }
  return true;
}

void fail_openfpga_arch_input(OpenFpgaSession& session) {
  sync_with_vpr(session);
  session.fail_input(OpenFpgaInput::OPENFPGA_ARCH);
}

bool run_flow_stage(OpenFpgaSession& session, const OpenFpgaStage& stage) {
  sync_with_vpr(session);
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
    session.fail(stage);
    return false;
  }
  session.complete(stage);
  return true;
}

bool import_flow_stage(OpenFpgaSession& session, const OpenFpgaStage& stage,
                       const std::string& file) {
  sync_with_vpr(session);
  const OpenFpgaStage required = required_stage(stage);
  const char* command = import_command(stage);
  if (OpenFpgaStage::PACKED != stage &&
      OpenFpgaStage::PLACED != stage && OpenFpgaStage::ROUTED != stage) {
    VTR_LOG_ERROR("The results of the stage %s cannot be read from a file\n",
                  to_string(stage).c_str());
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
    VTR_LOG_ERROR("Cannot read the results of the stage %s: the file '%s' "
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
    session.fail(stage);
    return false;
  }
  session.complete(stage);
  session.set_imported(stage, file);
  return true;
}

bool stage_ready(const OpenFpgaSession& session, const OpenFpgaStage& stage,
                 const char* command) {
  if (!in_staged_flow(session) || session.reached(stage)) {
    return true;
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
