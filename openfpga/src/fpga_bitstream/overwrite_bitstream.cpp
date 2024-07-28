/********************************************************************
 * This file includes functions to build bitstream from a mapped
 * FPGA fabric.
 * We decode the bitstream from configuration of routing multiplexers
 * and Look-Up Tables (LUTs) which locate in CLBs and global routing
 *architecture
 *******************************************************************/

/* Headers from vtrutil library */
#include "overwrite_bitstream.h"

#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Overwrite bitstream retrieve from bitstream annotation XML which stored in
 *BitstreamSetting
 *******************************************************************/
void overwrite_bitstream(openfpga::BitstreamManager& bitstream_manager,
                         const openfpga::BitstreamSetting& bitstream_setting,
                         const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("\nOverwrite Bitstream\n");

  /* Apply overwrite_bitstream bit's path and value */
  for (auto& id : bitstream_setting.overwrite_bitstreams()) {
    std::string path = bitstream_setting.overwrite_bitstream_path(id);
    bool value = bitstream_setting.overwrite_bitstream_value(id);
    VTR_LOGV(verbose, "Overwrite bitstream path='%s' to value='%d'\n",
             path.c_str(), value);
    bitstream_manager.overwrite_bitstream(path, value);
  }
}

} /* end namespace openfpga */
