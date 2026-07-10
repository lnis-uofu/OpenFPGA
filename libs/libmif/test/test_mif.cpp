/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. OpenFPGA .mif reader/writer
 * 2. Verilog init.hex reader (via read_mif) and echo as .mif
 * 3. memory_address_map.xml reader
 * 4. find Verilog instance (memory_0) that $readmemh the init.hex
 *
 * Usage:
 *   test_mif <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> \
 *            <memory_address_map.xml> <benchmark.v> <init.hex>
 *******************************************************************/
#include <map>
#include <string>
#include <utility>

#include "bind_bram_to_mif_storage.h"
#include "command_exit_codes.h"
#include "memory_address_map.h"
#include "mif_storage.h"
#include "read_mif.h"
#include "read_xml_memory_address_map.h"
#include "vtr_log.h"
#include "write_mif.h"

int main(int argc, const char** argv) {
  if (argc < 8) {
    VTR_LOG_ERROR(
      "Usage: %s <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> "
      "<memory_address_map.xml> <benchmark.v> <init.hex>\n",
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

  /* 4) Find instance that reads init.hex, then bind. */
  const std::string instance_name =
    openfpga::find_verilog_instance_reading_mif(argv[6], "init.hex");
  if (instance_name.empty()) {
    VTR_LOG_ERROR("Failed to find instance reading init.hex in %s\n", argv[6]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Found instance reading init.hex: %s\n", instance_name.c_str());

  openfpga::MifStorage bind_storage;
  status = openfpga::read_mif(argv[7], bind_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  /* Mock placement: memory_0 at (2,2) matches memory_address_map.xml */
  std::map<std::string, std::pair<int, int>> inst_coord_map;
  inst_coord_map[instance_name] = std::make_pair(2, 2);
  status = openfpga::bind_bram_to_mif_storage(
    bind_storage, argv[6], inst_coord_map, memory_address_map);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  return openfpga::CMD_EXEC_SUCCESS;
}
