/********************************************************************
 * Unit test: aggregate_mif bit/addr remapping via <map> rules
 *
 * Usage:
 *   test_aggregate_mif <mif_map_8x32_only.xml> <init_8x32_remap.hex>
 *                      <mif_map_32x8_only.xml> <init_32x8_remap.hex>
 *                      <mif_map_sibling_8x16.xml>
 *                      <mif_bitstream_setting.xml>
 *******************************************************************/
#include <cstdint>
#include <map>
#include <string>

#include "aggregate_mif.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "mif_storage_fwd.h"
#include "openfpga_port.h"
#include "read_mif.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"

int main(int argc, const char** argv) {
  if (argc < 7) {
    VTR_LOG_ERROR(
      "Usage: %s <mif_map_8x32_only.xml> <init_8x32_remap.hex> "
      "<mif_map_32x8_only.xml> <init_32x8_remap.hex> "
      "<mif_map_sibling_8x16.xml> <mif_bitstream_setting.xml>\n",
      argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  int status = openfpga::CMD_EXEC_SUCCESS;
  openfpga::BitstreamSetting bitstream_setting;
  openfpga::MifStorage logical_storage;
  openfpga::MifStorage aggregated;
  std::map<uint64_t, uint64_t> got;
  const MifSegmentId seg0(0);

  /* ---- 8x32 wide word -> phys low@0, high@8 ---- */
  
  status = read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  status = openfpga::read_mif(argv[2], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  
  status =
    openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_ASSERT(1 == aggregated.num_segments());
  VTR_ASSERT(aggregated.physical_pb(seg0) ==
             "memory[mem_16x16_phy].mem_16x16_dp_phy");
  VTR_ASSERT(16 == aggregated.data_width(seg0));
  VTR_ASSERT(aggregated.addr_range(seg0).is_valid());
  VTR_ASSERT(0 == aggregated.addr_range(seg0).get_lsb());
  VTR_ASSERT(15 == aggregated.addr_range(seg0).get_msb());
  got.clear();
  for (const MifMemoryLineId& line_id : aggregated.segment_memory_lines(seg0)) {
    got[aggregated.memory_line_address(line_id)] =
      aggregated.memory_line_data(line_id);
  }
  VTR_ASSERT(2 == got.size());
  VTR_ASSERT(got.at(0) == 0xBEEFull);
  VTR_ASSERT(got.at(8) == 0xDEADull);

  /* ---- 32x8 narrow words -> pack addr0 + addr0x10 into phys addr0 ---- */
  bitstream_setting.clear();
  status = read_xml_openfpga_bitstream_settings(argv[3], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  logical_storage.clear();
  status = openfpga::read_mif(argv[4], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  aggregated.clear();
  status =
    openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_ASSERT(1 == aggregated.num_segments());
  VTR_ASSERT(aggregated.physical_pb(seg0) ==
             "memory[mem_16x16_phy].mem_16x16_dp_phy");
  VTR_ASSERT(16 == aggregated.data_width(seg0));
  VTR_ASSERT(0 == aggregated.addr_range(seg0).get_lsb());
  VTR_ASSERT(15 == aggregated.addr_range(seg0).get_msb());
  got.clear();
  for (const MifMemoryLineId& line_id : aggregated.segment_memory_lines(seg0)) {
    got[aggregated.memory_line_address(line_id)] =
      aggregated.memory_line_data(line_id);
  }
  VTR_ASSERT(1 == got.size());
  VTR_ASSERT(got.at(0) == 0xA53Cull);

  /* ---- sibling 8x16[0] only: high half, low half stays 0 ---- */
  bitstream_setting.clear();
  status = read_xml_openfpga_bitstream_settings(argv[5], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  logical_storage.clear();
  {
    const openfpga::BasicPort addr_range("address", 0, 7);
    const MifSegmentId s0 = logical_storage.create_segment();
    logical_storage.set_segment_physical_pb(
      s0, "memory[dpram8x32].dpram8x16[0]");
    logical_storage.set_segment_addr_range(s0, addr_range);
    logical_storage.create_memory_line(s0, 0, 0x138F);
    logical_storage.create_memory_line(s0, 1, 0x0020);
    logical_storage.create_memory_line(s0, 2, 0x37EA);
    logical_storage.create_memory_line(s0, 7, 0x42FB);
  }
  aggregated.clear();
  status =
    openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_ASSERT(1 == aggregated.num_segments());
  VTR_ASSERT(aggregated.physical_pb(seg0) == "memory[dpram8x32].dpram8x32");
  VTR_ASSERT(32 == aggregated.data_width(seg0));
  VTR_ASSERT(0 == aggregated.addr_range(seg0).get_lsb());
  VTR_ASSERT(7 == aggregated.addr_range(seg0).get_msb());
  got.clear();
  for (const MifMemoryLineId& line_id : aggregated.segment_memory_lines(seg0)) {
    got[aggregated.memory_line_address(line_id)] =
      aggregated.memory_line_data(line_id);
  }
  VTR_ASSERT(4 == got.size());
  VTR_ASSERT(got.at(0) == 0x138F0000ull);
  VTR_ASSERT(got.at(1) == 0x00200000ull);
  VTR_ASSERT(got.at(2) == 0x37EA0000ull);
  VTR_ASSERT(got.at(7) == 0x42FB0000ull);

  /* ---- sibling + 8x16[1] at addr0: OR into low half ---- */
  {
    const openfpga::BasicPort addr_range("address", 0, 7);
    const MifSegmentId s1 = logical_storage.create_segment();
    logical_storage.set_segment_physical_pb(
      s1, "memory[dpram8x32].dpram8x16[1]");
    logical_storage.set_segment_addr_range(s1, addr_range);
    logical_storage.create_memory_line(s1, 0, 0xABCD);
  }
  aggregated.clear();
  status =
    openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  got.clear();
  for (const MifMemoryLineId& line_id : aggregated.segment_memory_lines(seg0)) {
    got[aggregated.memory_line_address(line_id)] =
      aggregated.memory_line_data(line_id);
  }
  VTR_ASSERT(4 == got.size());
  VTR_ASSERT(got.at(0) == 0x138FABCDull);
  VTR_ASSERT(got.at(1) == 0x00200000ull);
  VTR_ASSERT(got.at(2) == 0x37EA0000ull);
  VTR_ASSERT(got.at(7) == 0x42FB0000ull);

  /* ---- mode catalog (3 maps, one des): only matching 8x32 map applies ---- */
  bitstream_setting.clear();
  status = read_xml_openfpga_bitstream_settings(argv[6], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  size_t map_count = 0;
  for (const auto& id : bitstream_setting.mif_address_map_settings()) {
    (void)id;
    ++map_count;
  }
  VTR_ASSERT(3 == map_count);
  logical_storage.clear();
  status = openfpga::read_mif(argv[2], logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  logical_storage.set_segment_physical_pb(
    MifSegmentId(0), "memory[mem_8x32_dp].mem_8x32_dp");
  aggregated.clear();
  status =
    openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_ASSERT(1 == aggregated.num_segments());
  VTR_ASSERT(aggregated.physical_pb(seg0) ==
             "memory[mem_16x16_phy].mem_16x16_dp_phy");
  VTR_ASSERT(16 == aggregated.data_width(seg0));
  got.clear();
  for (const MifMemoryLineId& line_id : aggregated.segment_memory_lines(seg0)) {
    got[aggregated.memory_line_address(line_id)] =
      aggregated.memory_line_data(line_id);
  }
  VTR_ASSERT(2 == got.size());
  VTR_ASSERT(got.at(0) == 0xBEEFull);
  VTR_ASSERT(got.at(8) == 0xDEADull);

  VTR_LOG("aggregate_mif bit/addr remapping tests passed.\n");
  return openfpga::CMD_EXEC_SUCCESS;
}
