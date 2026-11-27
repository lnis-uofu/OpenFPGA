#ifndef APP_OPTION_STAGE_H
#define APP_OPTION_STAGE_H

#include <string>

#include "openfpga_session.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * The stage that an application option tunes. Changing the value of an
 * option makes the results of this stage, and of the stages that depend on
 * it, out of date.
 *
 * The stage comes from the group of the option, which is the one of its
 * argument group in VPR:
 *   filename, general, atom             DEVICE
 *   cluster                             PACKED
 *   aplace, place, tplace               PLACED
 *   crr, router, trouter                ROUTED
 *   analysis, power                     ANALYZED
 *   build_fabric                        REPACKED
 * with a few exceptions among the filename options, and the options of
 * the packer of the analytical placer (appack_*), which have the stage of the
 * files they read and write, or of the packer. When in doubt, the earlier
 * stage is taken: it only costs a re-run
 *
 * Return false, and leave the stage untouched, if the name is not the one of
 * an option that is tagged with a stage
 *******************************************************************/
bool app_option_stage(const std::string& name, OpenFpgaStage& stage);

} /* End namespace openfpga */

#endif
