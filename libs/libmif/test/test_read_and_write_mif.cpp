/********************************************************************
 * Unit test: read init.hex formats and write_mif
 *
 * Usage:
 *   test_read_and_write_mif <out.mem> <init.hex> <init1.hex>
 *                           <init_addr_data.hex>
 *******************************************************************/
#include <cstdint>
#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "mif_storage_fwd.h"
#include "read_mif.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_mif.h"

int main(int argc, const char** argv) {
  if (argc < 5) {
    VTR_LOG_ERROR(
      "Usage: %s <out.mem> <init.hex> <init1.hex> <init_addr_data.hex>\n",
      argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  openfpga::MifStorage logical_storage;
  int status = openfpga::read_mif(argv[4], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  status = openfpga::read_mif(argv[2], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::read_mif(argv[3], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::write_mif(argv[1], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  return openfpga::CMD_EXEC_SUCCESS;
}
