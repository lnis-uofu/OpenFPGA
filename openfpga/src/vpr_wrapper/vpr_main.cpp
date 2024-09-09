/**
 * This is a wrapper file for VPR API. Mirrored from the main.cpp of vpr
 */

#include "vpr_main.h"

#include <cstdio>
#include <cstring>
#include <ctime>

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
 * 4.  Clean up
 */
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
