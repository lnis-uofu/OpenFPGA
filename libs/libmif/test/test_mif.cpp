/********************************************************************
 * Unit test: read init.hex, aggregate per pb, write preload .mem
 *
 * Usage:
 *   test_mif <bitstream_setting.xml> <benchmark.v> <init.hex> <init1.hex>
 *******************************************************************/
#include <cstdint>
#include <map>
#include <string>

#include "aggregate_mif.h"
#include "aggregated_mif_storage.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_io_utils.h"
#include "mif_storage.h"
#include "mif_storage_fwd.h"
#include "mif_verilog_utils.h"
#include "read_mif.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_mif.h"

int main(int argc, const char** argv) {
  if (argc < 5) {
    VTR_LOG_ERROR(
      "Usage: %s <bitstream_setting.xml> <benchmark.v> <init.hex> "
      "<init1.hex>\n",
      argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  openfpga::BitstreamSetting bitstream_setting;
  int status =
    read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  const openfpga::MifAddressMap mif_address_map =
    openfpga::mif_address_map_from_bitstream_setting(bitstream_setting);

  openfpga::MifStorage logical_storage;
  status = openfpga::read_mif(argv[3], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::read_mif(argv[4], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  const std::string instance_0 = openfpga::find_verilog_instance_reading_mif(
    argv[2], openfpga::mif_file_basename(argv[3]));
  const std::string instance_1 = openfpga::find_verilog_instance_reading_mif(
    argv[2], openfpga::mif_file_basename(argv[4]));
  VTR_ASSERT(!instance_0.empty() && !instance_1.empty());

  std::map<std::string, std::string> inst_pb_type_path_map;
  inst_pb_type_path_map[instance_0] = "dpram8x32[0]{dual}.dpram8x16[0]";
  inst_pb_type_path_map[instance_1] = "dpram8x32[0]{dual}.dpram8x16[1]";

  openfpga::AggregatedMifStorage aggregated_storage;
  status = openfpga::aggregate_mif(logical_storage, argv[2],
                                   inst_pb_type_path_map, mif_address_map,
                                   aggregated_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  VTR_ASSERT(1 == aggregated_storage.num_segments());
  VTR_ASSERT(aggregated_storage.physical_pb(MifSegmentId(0)) ==
             "dpram8x32{dual}.dpram8x16");
  VTR_ASSERT(32 == aggregated_storage.data_width(MifSegmentId(0)));

  status = openfpga::write_mif("final_mif.mem", aggregated_storage);
  return status;
}
