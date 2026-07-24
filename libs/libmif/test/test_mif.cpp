/********************************************************************
 * Unit test: read init.hex formats and parse new bitstream MIF syntax
 *
 * Usage:
 *   test_mif <bitstream_setting.xml> <benchmark.v> <init.hex> <init1.hex>
 *            <init_addr_data.hex>
 *******************************************************************/
#include <cstdint>
#include <map>
#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_io_utils.h"
#include "mif_storage.h"
#include "mif_storage_fwd.h"
#include "read_mif.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"

int main(int argc, const char** argv) {
  if (argc < 6) {
    VTR_LOG_ERROR(
      "Usage: %s <bitstream_setting.xml> <benchmark.v> <init.hex> "
      "<init1.hex> <init_addr_data.hex>\n",
      argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  openfpga::BitstreamSetting bitstream_setting;
  int status = read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  size_t num_sources = 0;
  for (const auto& id : bitstream_setting.mif_source_settings()) {
    (void)id;
    ++num_sources;
  }
  VTR_ASSERT(0 < num_sources);

  size_t num_maps = 0;
  for (const auto& id : bitstream_setting.mif_address_map_settings()) {
    (void)id;
    ++num_maps;
  }
  VTR_ASSERT(0 < num_maps);

  const MifAddressMapSettingId map_8x32 =
    bitstream_setting.find_mif_address_map_by_src_pb_type(
      "memory[mem_8x32_dp].mem_8x32_dp");
  VTR_ASSERT(map_8x32.is_valid());
  VTR_ASSERT(bitstream_setting.mif_address_map_des_pb_type(map_8x32) ==
             "memory[mem_16x16_phy].mem_16x16_dp_phy");

  openfpga::MifStorage addr_data_storage;
  status = openfpga::read_mif(argv[5], addr_data_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_ASSERT(1 == addr_data_storage.num_segments());
  VTR_ASSERT(addr_data_storage.has_addr_range(MifSegmentId(0)));
  VTR_ASSERT(0 == addr_data_storage.min_addr(MifSegmentId(0)));
  VTR_ASSERT(7 == addr_data_storage.max_addr(MifSegmentId(0)));

  openfpga::MifStorage logical_storage;
  status = openfpga::read_mif(argv[3], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = openfpga::read_mif(argv[4], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_ASSERT(2 == logical_storage.num_segments());

  const std::string instance_0 = openfpga::find_verilog_instance_reading_mif(
    argv[2], openfpga::mif_file_basename(argv[3]));
  const std::string instance_1 = openfpga::find_verilog_instance_reading_mif(
    argv[2], openfpga::mif_file_basename(argv[4]));
  VTR_ASSERT(!instance_0.empty() && !instance_1.empty());

  VTR_LOG(
    "Parsed new bitstream MIF syntax, init.hex formats, and verilog "
    "instances '%s' / '%s'.\n",
    instance_0.c_str(), instance_1.c_str());
  return openfpga::CMD_EXEC_SUCCESS;
}
