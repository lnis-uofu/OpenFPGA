/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
#include "mif_storage.h"
#include "read_mif.h"
#include "vtr_log.h"

int main(int argc, const char** argv) {
  openfpga::MifStorage storage;
  read_mif(argv[1], storage);
  VTR_LOG("Read the MIF file: %s.\n", argv[1]);

  if (3 <= argc) {
    write_mif(argv[2], storage);
    VTR_LOG("Echo the MIF file to: %s.\n", argv[2]);
  }
}
