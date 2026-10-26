/**
 * This is a wrapper file for VPR API. Mirrored from the main.cpp of vpr
 */

#include "vpr_main.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <exception>
#include <memory>

#include "command_exit_codes.h"
#include "globals.h"
#include "tatum/error.hpp"
#include "vpr_api.h"
#include "vpr_error.h"
#include "vpr_exit_codes.h"
#include "vpr_signal_handler.h"
#include "vpr_tatum_error.h"
#include "vtr_error.h"
#include "vtr_log.h"
#include "vtr_memory.h"
#include "vtr_time.h"

namespace vpr {

namespace {

/* Data of a VPR run, which has to outlive the function that ran VPR:
 * later OpenFPGA commands read it through the global VPR contexts (g_vpr_ctx),
 * which point to the architecture. */
struct VprRun {
  t_options options = t_options();
  t_arch arch = t_arch();
  t_vpr_setup setup = t_vpr_setup();
};

/* The last successful run of command 'vpr'. Like g_vpr_ctx, it is global to
 * the process */
std::unique_ptr<VprRun> last_vpr_run;

/* Free the data of a run, from the global VPR contexts as well */
void free_vpr_run(VprRun& run) {
  try {
    vpr_free_all(run.arch, run.setup);
  } catch (const std::exception& exc) {
    VTR_LOG_ERROR("Fail to free VPR data: %s\n", exc.what());
  }
}

/* VPR keeps its data in global contexts, and loading a new design on top of
 * the data of a previous run crashes. Therefore, release the last run before
 * starting a new one */
void release_last_vpr_run() {
  if (last_vpr_run) {
    VTR_LOG("Free the data of the previous VPR run\n");
    free_vpr_run(*last_vpr_run);
    last_vpr_run.reset();
  }
}

} /* End anonymous namespace */

/**
 * VPR program without clean up
 * VPR program
 * Generate FPGA architecture given architecture description
 * Pack, place, and route circuit into FPGA architecture
 * Electrical timing analysis on results
 *
 * Overall steps
 * 1.  Initialization
 * 2.  Pack
 * 3.  Place-and-route and timing analysis
 * 4.  Clean up: skipped, as the data is needed by the commands that follow.
 *     It is freed when VPR is run again, or fails
 */
static int vpr(int argc, char** argv) {
  vtr::ScopedFinishTimer t("The entire flow of VPR");

  release_last_vpr_run();

  /* The run is kept (see last_vpr_run) only when VPR succeeds. When it fails,
   * its data is freed, so that the next run starts from a clean state */
  std::unique_ptr<VprRun> run = std::make_unique<VprRun>();
  int exit_code = SUCCESS_EXIT_CODE;

  try {
    vpr_install_signal_handler();

    /* Read options, architecture, and circuit netlist */
    vpr_init(argc, const_cast<const char**>(argv), &run->options, &run->setup,
             &run->arch);

    if (run->options.show_version) {
      free_vpr_run(*run);
      return SUCCESS_EXIT_CODE;
    }

    bool flow_succeeded = vpr_flow(run->setup, run->arch);
    if (!flow_succeeded) {
      VTR_LOG("VPR failed to implement circuit\n");
      exit_code = UNIMPLEMENTABLE_EXIT_CODE;
    } else {
      auto& timing_ctx = g_vpr_ctx.timing();
      print_timing_stats("Flow", timing_ctx.stats);

      VTR_LOG("VPR succeeded\n");
    }

  } catch (const tatum::Error& tatum_error) {
    VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());
    exit_code = ERROR_EXIT_CODE;

  } catch (const VprError& vpr_error) {
    vpr_print_error(vpr_error);

    if (vpr_error.type() == VPR_ERROR_INTERRUPTED) {
      exit_code = INTERRUPTED_EXIT_CODE;
    } else {
      exit_code = ERROR_EXIT_CODE;
    }

  } catch (const vtr::VtrError& vtr_error) {
    VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(),
                  vtr_error.what());
    exit_code = ERROR_EXIT_CODE;
  }

  if (SUCCESS_EXIT_CODE == exit_code) {
    last_vpr_run = std::move(run);
  } else {
    free_vpr_run(*run);
  }

  return exit_code;
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

  release_last_vpr_run();

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
