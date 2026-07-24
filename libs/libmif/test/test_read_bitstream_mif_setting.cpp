/********************************************************************
 * Unit test: read MIF-related entries from openfpga_bitstream_setting
 *
 * Usage:
 *   test_read_bitstream_mif_setting <bitstream_setting.xml> [out.xml]
 *******************************************************************/
#include <string>
#include <vector>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "openfpga_port.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_xml_openfpga_arch.h"

namespace {

bool range_eq(const openfpga::BasicPort& port, size_t lsb, size_t msb) {
  return port.is_valid() && port.get_lsb() == lsb && port.get_msb() == msb;
}

} /* namespace */

int main(int argc, const char** argv) {
  VTR_ASSERT((2 == argc) || (3 == argc));

  openfpga::BitstreamSetting bitstream_setting;
  const int status =
    read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("Parsed bitstream settings from XML %s.\n", argv[1]);

  /* Sanity-check the new mif_source / mif_address_map syntax. */
  size_t num_sources = 0;
  for (const auto& id : bitstream_setting.mif_source_settings()) {
    (void)id;
    ++num_sources;
  }
  VTR_ASSERT(4 == num_sources);

  const MifSourceSettingId phy_source =
    bitstream_setting.find_mif_source_by_pb_type(
      "memory[mem_16x16_phy].mem_16x16_dp_phy");
  VTR_ASSERT(phy_source.is_valid());
  VTR_ASSERT(bitstream_setting.mif_source_source(phy_source) == "eblif");
  VTR_ASSERT(bitstream_setting.mif_source_content(phy_source) ==
             ".param INIT");
  VTR_ASSERT(range_eq(bitstream_setting.mif_source_address_range(phy_source),
                      0, 15));
  VTR_ASSERT(
    range_eq(bitstream_setting.mif_source_data_range(phy_source), 0, 15));

  const MifSourceSettingId src_8x32 =
    bitstream_setting.find_mif_source_by_pb_type(
      "memory[mem_8x32_dp].mem_8x32_dp");
  VTR_ASSERT(src_8x32.is_valid());
  VTR_ASSERT(
    range_eq(bitstream_setting.mif_source_address_range(src_8x32), 0, 7));
  VTR_ASSERT(
    range_eq(bitstream_setting.mif_source_data_range(src_8x32), 0, 31));

  size_t num_maps = 0;
  for (const auto& id : bitstream_setting.mif_address_map_settings()) {
    (void)id;
    ++num_maps;
  }
  VTR_ASSERT(3 == num_maps);

  const MifAddressMapSettingId map_8x32 =
    bitstream_setting.find_mif_address_map_by_src_pb_type(
      "memory[mem_8x32_dp].mem_8x32_dp");
  VTR_ASSERT(map_8x32.is_valid());
  VTR_ASSERT(bitstream_setting.mif_address_map_des_pb_type(map_8x32) ==
             "memory[mem_16x16_phy].mem_16x16_dp_phy");

  std::vector<MifAddressMapRuleId> rules_8x32;
  for (const MifAddressMapRuleId& rule_id :
       bitstream_setting.mif_address_map_rules(map_8x32)) {
    rules_8x32.push_back(rule_id);
  }
  VTR_ASSERT(2 == rules_8x32.size());
  VTR_ASSERT(range_eq(
    bitstream_setting.mif_address_map_rule_src_addr_range(rules_8x32[0]), 0,
    7));
  VTR_ASSERT(0 == bitstream_setting.mif_address_map_rule_des_addr_offset(
                    rules_8x32[0]));
  VTR_ASSERT(range_eq(
    bitstream_setting.mif_address_map_rule_src_mif_bits(rules_8x32[0]), 0,
    15));
  VTR_ASSERT(range_eq(
    bitstream_setting.mif_address_map_rule_des_mif_bits(rules_8x32[0]), 0,
    15));
  VTR_ASSERT(8 == bitstream_setting.mif_address_map_rule_des_addr_offset(
                    rules_8x32[1]));
  VTR_ASSERT(range_eq(
    bitstream_setting.mif_address_map_rule_src_mif_bits(rules_8x32[1]), 16,
    31));

  const MifAddressMapSettingId map_32x8 =
    bitstream_setting.find_mif_address_map_by_src_pb_type(
      "memory[mem_32x8_dp].mem_32x8_dp");
  VTR_ASSERT(map_32x8.is_valid());
  std::vector<MifAddressMapRuleId> rules_32x8;
  for (const MifAddressMapRuleId& rule_id :
       bitstream_setting.mif_address_map_rules(map_32x8)) {
    rules_32x8.push_back(rule_id);
  }
  VTR_ASSERT(2 == rules_32x8.size());
  VTR_ASSERT(range_eq(
    bitstream_setting.mif_address_map_rule_src_addr_range(rules_32x8[1]), 16,
    31));
  VTR_ASSERT(-16 == bitstream_setting.mif_address_map_rule_des_addr_offset(
                      rules_32x8[1]));
  VTR_ASSERT(range_eq(
    bitstream_setting.mif_address_map_rule_des_mif_bits(rules_32x8[1]), 8,
    15));

  if (3 <= argc) {
    write_xml_openfpga_bitstream_settings(argv[2], bitstream_setting);
    VTR_LOG("Echo the OpenFPGA bitstream settings to an XML file: %s.\n",
            argv[2]);
  }

  return openfpga::CMD_EXEC_SUCCESS;
}
