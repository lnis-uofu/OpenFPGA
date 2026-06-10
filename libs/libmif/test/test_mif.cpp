/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "read_mif.h"
#include "vtr_log.h"

int main(int argc, const char** argv) {
  openfpga::MifStorage storage;
  const int read_status = read_mif(argv[1], storage);
  if (openfpga::CMD_EXEC_SUCCESS != read_status) {
    return read_status;
  }
  VTR_LOG("Read the MIF file: %s.\n", argv[1]);

  if (3 <= argc) {
    const int write_status = write_mif(argv[2], storage);
    if (openfpga::CMD_EXEC_SUCCESS != write_status) {
      return write_status;
    }
    VTR_LOG("Echo the MIF file to: %s.\n", argv[2]);
  }
  return openfpga::CMD_EXEC_SUCCESS;
}
