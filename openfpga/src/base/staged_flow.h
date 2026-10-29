#ifndef STAGED_FLOW_H
#define STAGED_FLOW_H

#include <string>

#include "openfpga_session.h"

/********************************************************************
 * The flow that is run stage by stage: the inputs are read first, then the
 * stages are run one at a time, with the application options as the
 * settings of the engines.
 *
 * The data is in the live run of VPR (see vpr_run.h), and the session tracks
 * which inputs have been read and which stages are valid.
 *
 * The inputs:
 * - The VPR architecture is read, and checked, at once. The OpenFPGA
 *   architecture needs it
 * - The circuit is read against the VPR architecture. VPR derives the names of
 *   its files from the circuit and reads it together with the architecture, so
 *   the architecture is read again
 * - The device, i.e., the grid and, if the channel width is fixed, the routing
 *   resource graph, is created once the three inputs are read. It needs a
 *   fixed layout: an automatic layout is sized from the result of packing,
 *   which is not known yet
 *
 * The stages, which need the stage before them in the tree of stages:
 * - Running a stage again, or after an application option changed, runs the
 *   stages before it again on fresh VPR data, as VPR cannot run a stage twice
 *   on the same data. The results are the same, since VPR is deterministic
 * - Analysis changes the data of VPR, as it synchronizes the packing with the
 *   routing. Running it again, or routing again, starts from fresh data. It
 *   leaves the stages on the other branches of the tree alone
 * - Another command that replaces the VPR data, e.g., 'vpr', makes the session
 *   forget everything it knows
 *
 * All the functions log the reason of a failure and return false
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* Read the VPR architecture with the layout of the device. It starts the flow
 * over: the results of the stages and the circuit are dropped */
bool read_vpr_arch_input(OpenFpgaSession& session, const std::string& file,
                         const std::string& layout);

/* Read the circuit. The VPR architecture must have been read */
bool read_circuit_input(OpenFpgaSession& session, const std::string& file);

/* Record that the OpenFPGA architecture has been read, and create the device
 * if all the inputs are there now. Call fail_openfpga_arch_input() instead if
 * the architecture could not be read */
bool openfpga_arch_input_read(OpenFpgaSession& session);
void fail_openfpga_arch_input(OpenFpgaSession& session);

/* Run a stage: PACKED, PLACED, ROUTED or ANALYZED. The device is
 * created first if it is not there yet */
bool run_flow_stage(OpenFpgaSession& session, const OpenFpgaStage& stage);

} /* End namespace openfpga */

#endif
