/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. OpenFPGA .mif reader/writer
 * 2. Verilog init.hex reader (via read_mif) and echo as .mif
 * 3. memory_address_map.xml reader (MifAddressMap)
 * 4. find Verilog instances and bind two init hex files
 *
 * Usage:
 *   test_mif <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> \
 *            <memory_address_map.xml> <benchmark.v> <init.hex> <init1.hex>
 *******************************************************************/
#include <map>
#include <string>
#include <utility>

#include "bind_bram_to_mif_storage.h"
#include "command_exit_codes.h"
#include "mif_address_map.h"
#include "mif_io_utils.h"
#include "mif_storage.h"
#include "read_mif.h"
#include "read_xml_mif_address_map.h"
#include "vtr_log.h"
#include "write_mif.h"

int main(int argc, const char** argv) {
  if (argc < 9) {
    VTR_LOG_ERROR(
      "Usage: %s <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> "
      "<memory_address_map.xml> <benchmark.v> <init.hex> <init1.hex>\n",
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

  /* 3) memory_address_map.xml -> MifAddressMap */
  openfpga::MifAddressMap mif_address_map;
  status = openfpga::read_xml_mif_address_map(argv[5], mif_address_map);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  VTR_LOG("Read %lu address maps from %s.\n",
          static_cast<unsigned long>(mif_address_map.num_address_maps()),
          argv[5]);
  for (const MifAddressMapId& map_id : mif_address_map.address_maps()) {
    VTR_LOG("  address_map pb_type='%s' address_offset=%d data_offset=%d\n",
            mif_address_map.pb_type(map_id).c_str(),
            mif_address_map.address_offset(map_id),
            mif_address_map.data_offset(map_id));
  }

  /* 4) Read two init hex files and bind placement coords by instance */
  openfpga::MifStorage bind_storage;
  status = openfpga::read_mif(argv[7], bind_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::read_mif(argv[8], bind_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  const std::string init0_name = openfpga::mif_file_basename(argv[7]);
  const std::string init1_name = openfpga::mif_file_basename(argv[8]);
  const std::string instance_0 =
    openfpga::find_verilog_instance_reading_mif(argv[6], init0_name);
  const std::string instance_1 =
    openfpga::find_verilog_instance_reading_mif(argv[6], init1_name);
  if (instance_0.empty() || instance_1.empty()) {
    VTR_LOG_ERROR(
      "Failed to find instances for '%s' / '%s' in %s\n", init0_name.c_str(),
      init1_name.c_str(), argv[6]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Found instance reading %s: %s\n", init0_name.c_str(),
          instance_0.c_str());
  VTR_LOG("Found instance reading %s: %s\n", init1_name.c_str(),
          instance_1.c_str());

  /* Mock placement: memory_0 -> (2,2), memory_1 -> (2,1) */
  std::map<std::string, std::pair<int, int>> inst_coord_map;
  inst_coord_map[instance_0] = std::make_pair(2, 2);
  inst_coord_map[instance_1] = std::make_pair(2, 1);
  status = openfpga::bind_bram_to_mif_storage(bind_storage, argv[6],
                                              inst_coord_map);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::write_mif("final_mif.mif", bind_storage);
  return status;
}
