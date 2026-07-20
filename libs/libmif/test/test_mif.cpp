/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. OpenFPGA .mif reader/writer
 * 2. Verilog init.hex reader (via read_mif) and echo as .mif
 * 3. find Verilog instances and aggregate two init hex files per pb
 *
 * Usage:
 *   test_mif <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> \
 *            <benchmark.v> <init.hex> <init1.hex>
 *******************************************************************/
#include <map>
#include <string>

#include "aggregate_mif.h"
#include "bind_bram_to_mif_storage.h"
#include "command_exit_codes.h"
#include "mif_address_map.h"
#include "mif_io_utils.h"
#include "mif_storage.h"
#include "read_mif.h"
#include "vtr_log.h"
#include "write_mif.h"

static openfpga::MifAddressMap build_test_mif_address_map() {
  openfpga::MifAddressMap mif_address_map;
  mif_address_map.create_address_map("dpram8x32[0]{dual}.dpram8x16[0]", 0, 0);
  mif_address_map.create_address_map("dpram8x32[0]{dual}.dpram8x16[1]", 0, 16);
  return mif_address_map;
}

int main(int argc, const char** argv) {
  if (argc < 8) {
    VTR_LOG_ERROR(
      "Usage: %s <test.mif> <mif_out.mif> <test_init.hex> <hex_out.mif> "
      "<benchmark.v> <init.hex> <init1.hex>\n",
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

  /* 3) Read two init hex files and aggregate per physical pb */
  openfpga::MifStorage logical_storage;
  status = openfpga::read_mif(argv[6], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::read_mif(argv[7], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  const std::string init0_name = openfpga::mif_file_basename(argv[6]);
  const std::string init1_name = openfpga::mif_file_basename(argv[7]);
  const std::string instance_0 =
    openfpga::find_verilog_instance_reading_mif(argv[5], init0_name);
  const std::string instance_1 =
    openfpga::find_verilog_instance_reading_mif(argv[5], init1_name);
  if (instance_0.empty() || instance_1.empty()) {
    VTR_LOG_ERROR(
      "Failed to find instances for '%s' / '%s' in %s\n", init0_name.c_str(),
      init1_name.c_str(), argv[5]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Found instance reading %s: %s\n", init0_name.c_str(),
          instance_0.c_str());
  VTR_LOG("Found instance reading %s: %s\n", init1_name.c_str(),
          instance_1.c_str());

  std::map<std::string, std::string> inst_pb_type_path_map;
  inst_pb_type_path_map[instance_0] = "dpram8x32[0]{dual}.dpram8x16[0]";
  inst_pb_type_path_map[instance_1] = "dpram8x32[0]{dual}.dpram8x16[1]";

  const openfpga::MifAddressMap mif_address_map = build_test_mif_address_map();
  openfpga::MifStorage aggregated_storage;
  status = openfpga::aggregate_mif(logical_storage, argv[5],
                                   inst_pb_type_path_map, mif_address_map,
                                   aggregated_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::write_mif("final_mif.mif", aggregated_storage);
  return status;
}
