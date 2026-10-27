#ifndef VPR_RUN_H
#define VPR_RUN_H

#include <cstdint>
#include <string>
#include <vector>

#include "openfpga_session.h"
#include "read_options.h"
#include "vpr_api.h"
#include "vpr_types.h"

/********************************************************************
 * The data of the VPR run that is alive in the process.
 *
 * VPR keeps its data in global contexts (g_vpr_ctx) that point to the
 * architecture, so the data of a run has to outlive the function that ran
 * VPR, and a new run cannot start before the previous one is released.
 * Both the command 'vpr' and the commands of the stage-by-stage flow use
 * this holder, so that they release the data of each other
 *******************************************************************/
/* Begin namespace vpr */
namespace vpr {

struct VprRun {
  t_options options = t_options();
  t_arch arch = t_arch();
  t_vpr_setup setup = t_vpr_setup();
  RouteStatus route_status = RouteStatus();
  /* Stage-by-stage flow only: the command line that VPR was started with,
   * and, once it is initialized, the last stage of the main path (DEVICE,
   * PACKED, PLACED, ROUTED) whose results are in the VPR data. UNINIT while the
   * device is not created */
  std::vector<std::string> args;
  /* True once VPR has read the architecture and the circuit, and nothing
   * failed since */
  bool initialized = false;
  openfpga::OpenFpgaStage last_stage = openfpga::OpenFpgaStage::UNINIT;
};

/* Release the data of the live run, if any, and start a new one */
VprRun& begin_vpr_run();

/* The live run, nullptr if there is none */
VprRun* current_vpr_run();

/* Release the data of the live run, if any. Tell it in the log if announce is
 * set */
void release_vpr_run(bool announce = false);

/* Number of times the live run has changed, i.e., started or released. A
 * holder of data that refers to a run can tell it is stale if the number
 * changed */
uint64_t vpr_run_generation();

} /* End namespace vpr */

#endif
