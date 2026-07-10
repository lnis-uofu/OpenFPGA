/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. OpenFPGA .mif reader/writer
 * 2. Verilog init.hex reader (via read_mif) and echo as .mif
 * 3. memory_address_map.xml reader
 *
 * Usage:
 *   test_mif <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> \
 *            <memory_address_map.xml>
 *******************************************************************/
#include "command_exit_codes.h"
#include "memory_address_map.h"
#include "mif_storage.h"
#include "read_mif.h"
#include "read_xml_memory_address_map.h"
#include "vtr_log.h"
#include "write_mif.h"

int main(int argc, const char** argv) {
  if (argc < 6) {
    VTR_LOG_ERROR(
      "Usage: %s <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> "
      "<memory_address_map.xml>\n",
      argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  /* 1) OpenFPGA .mif format */
  openfpga::MifStorage mif_storage;
  int status = openfpga::read_mif(argv[1], mif_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("Read the MIF file: %s.\n", argv[1]);

  status = openfpga::write_mif(argv[2], mif_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("Echo the MIF file to: %s.\n", argv[2]);

  /* 2) Verilog init.hex format through read_mif */
  openfpga::MifStorage hex_storage;
  status = openfpga::read_mif(argv[3], hex_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("Read the MIF file: %s.\n", argv[3]);

  status = openfpga::write_mif(argv[4], hex_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("Echo the MIF file to: %s.\n", argv[4]);

  /* 3) memory_address_map.xml */
  openfpga::MemoryAddressMap memory_address_map;
  status = openfpga::read_xml_memory_address_map(argv[5], memory_address_map);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  VTR_LOG("Read %lu memories from %s.\n",
          static_cast<unsigned long>(memory_address_map.num_memories()),
          argv[5]);
  for (const MemoryAddressId& memory_id : memory_address_map.memories()) {
    VTR_LOG("  memory x=%d y=%d id=%d addr_width=%d data_width=%d\n",
            memory_address_map.coord_x(memory_id),
            memory_address_map.coord_y(memory_id),
            memory_address_map.ram_id(memory_id),
            memory_address_map.addr_width(memory_id),
            memory_address_map.data_width(memory_id));
  }

  return openfpga::CMD_EXEC_SUCCESS;
}
