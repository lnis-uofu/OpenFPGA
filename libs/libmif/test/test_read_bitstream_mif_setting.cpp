/********************************************************************
 * Unit test: read MIF-related entries from openfpga_bitstream_setting
 *
 * Usage:
 *   test_read_bitstream_mif_setting <bitstream_setting.xml> [out.xml]
 *******************************************************************/
#include <string>
#include <vector>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "openfpga_port.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_xml_openfpga_arch.h"

int main(int argc, const char** argv) {
  VTR_ASSERT((2 == argc) || (3 == argc));

  openfpga::BitstreamSetting bitstream_setting;
  const int status =
    read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("Parsed bitstream settings from XML %s.\n", argv[1]);

  return openfpga::CMD_EXEC_SUCCESS;
}
