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

static std::map<uint64_t, uint64_t> collect_lines(
  const openfpga::MifStorage& storage) {
  std::map<uint64_t, uint64_t> out;
  VTR_ASSERT(1 == storage.num_segments());
  const MifSegmentId seg(0);
  for (const MifMemoryLineId& line_id : storage.segment_memory_lines(seg)) {
    out[storage.memory_line_address(line_id)] =
      storage.memory_line_data(line_id);
  }
  return out;
}

static int check_aggregated(const openfpga::MifStorage& aggregated,
                            const std::string& expect_pb, int expect_width,
                            size_t expect_addr_lsb, size_t expect_addr_msb,
                            const std::map<uint64_t, uint64_t>& expect_lines) {
  VTR_ASSERT(1 == aggregated.num_segments());
  const MifSegmentId seg(0);
  VTR_ASSERT(aggregated.physical_pb(seg) == expect_pb);
  VTR_ASSERT(expect_width == aggregated.data_width(seg));
  VTR_ASSERT(aggregated.addr_range(seg).is_valid());
  VTR_ASSERT(expect_addr_lsb == aggregated.addr_range(seg).get_lsb());
  VTR_ASSERT(expect_addr_msb == aggregated.addr_range(seg).get_msb());

  const auto got = collect_lines(aggregated);
  VTR_ASSERT(expect_lines.size() == got.size());
  for (const auto& kv : expect_lines) {
    VTR_ASSERT(got.count(kv.first) == 1);
    VTR_ASSERT(got.at(kv.first) == kv.second);
  }
  return openfpga::CMD_EXEC_SUCCESS;
}

static int run_remap_case(const char* setting_path, const char* hex_path,
                          const std::string& expect_pb, int expect_width,
                          size_t expect_addr_lsb, size_t expect_addr_msb,
                          const std::map<uint64_t, uint64_t>& expect_lines) {
  openfpga::BitstreamSetting bitstream_setting;
  int status =
    read_xml_openfpga_bitstream_settings(setting_path, bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  openfpga::MifStorage logical_storage;
  status = openfpga::read_mif(hex_path, logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  openfpga::MifStorage aggregated;
  status =
    openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  return check_aggregated(aggregated, expect_pb, expect_width, expect_addr_lsb,
                          expect_addr_msb, expect_lines);
}

static int run_sibling_case(const char* setting_path) {
  openfpga::BitstreamSetting bitstream_setting;
  int status =
    read_xml_openfpga_bitstream_settings(setting_path, bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  openfpga::MifStorage logical_storage;
  const openfpga::BasicPort addr_range("address", 0, 7);

  {
    const MifSegmentId seg0 = logical_storage.create_segment();
    logical_storage.set_segment_physical_pb(
      seg0, "memory[dpram8x32].dpram8x16[0]");
    logical_storage.set_segment_addr_range(seg0, addr_range);
    logical_storage.create_memory_line(seg0, 0, 0x138F);
    logical_storage.create_memory_line(seg0, 1, 0x0020);
    logical_storage.create_memory_line(seg0, 2, 0x37EA);
    logical_storage.create_memory_line(seg0, 7, 0x42FB);

    openfpga::MifStorage aggregated;
    status =
      openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
    const std::map<uint64_t, uint64_t> expect = {
      {0, 0x138F0000ull},
      {1, 0x00200000ull},
      {2, 0x37EA0000ull},
      {7, 0x42FB0000ull},
    };
    status = check_aggregated(aggregated, "memory[dpram8x32].dpram8x32", 32, 0,
                              7, expect);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
  }

  {
    const MifSegmentId seg1 = logical_storage.create_segment();
    logical_storage.set_segment_physical_pb(
      seg1, "memory[dpram8x32].dpram8x16[1]");
    logical_storage.set_segment_addr_range(seg1, addr_range);
    logical_storage.create_memory_line(seg1, 0, 0xABCD);

    openfpga::MifStorage aggregated;
    status =
      openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
    const std::map<uint64_t, uint64_t> expect = {
      {0, 0x138FABCDull},
      {1, 0x00200000ull},
      {2, 0x37EA0000ull},
      {7, 0x42FB0000ull},
    };
    status = check_aggregated(aggregated, "memory[dpram8x32].dpram8x32", 32, 0,
                              7, expect);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
  }

  return openfpga::CMD_EXEC_SUCCESS;
}

/* Mode catalog like test_bitstream_mif_setting_out.xml: 3 maps, one des. */
static int run_catalog_case(const char* setting_path, const char* hex_8x32) {
  openfpga::BitstreamSetting bitstream_setting;
  int status =
    read_xml_openfpga_bitstream_settings(setting_path, bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  size_t map_count = 0;
  for (const auto& id : bitstream_setting.mif_address_map_settings()) {
    (void)id;
    ++map_count;
  }
  VTR_ASSERT(3 == map_count);

  openfpga::MifStorage logical_storage;
  status = openfpga::read_mif(hex_8x32, logical_storage);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  logical_storage.set_segment_physical_pb(
    MifSegmentId(0), "memory[mem_8x32_dp].mem_8x32_dp");

  openfpga::MifStorage aggregated;
  status =
    openfpga::aggregate_mif(logical_storage, bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  const std::map<uint64_t, uint64_t> expect = {
    {0, 0xBEEFull},
    {8, 0xDEADull},
  };
  return check_aggregated(aggregated, "memory[mem_16x16_phy].mem_16x16_dp_phy",
                          16, 0, 15, expect);
}

int main(int argc, const char** argv) {
  if (argc < 7) {
    VTR_LOG_ERROR(
      "Usage: %s <mif_map_8x32_only.xml> <init_8x32_remap.hex> "
      "<mif_map_32x8_only.xml> <init_32x8_remap.hex> "
      "<mif_map_sibling_8x16.xml> <mif_bitstream_setting.xml>\n",
      argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  {
    const std::map<uint64_t, uint64_t> expect = {
      {0, 0xBEEFull},
      {8, 0xDEADull},
    };
    const int status = run_remap_case(
      argv[1], argv[2], "memory[mem_16x16_phy].mem_16x16_dp_phy", 16, 0, 15,
      expect);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
  }

  {
    const std::map<uint64_t, uint64_t> expect = {
      {0, 0xA53Cull},
    };
    const int status = run_remap_case(
      argv[3], argv[4], "memory[mem_16x16_phy].mem_16x16_dp_phy", 16, 0, 15,
      expect);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
  }

  {
    const int status = run_sibling_case(argv[5]);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
  }

  {
    const int status = run_catalog_case(argv[6], argv[2]);
    if (openfpga::CMD_EXEC_SUCCESS != status) {
      return status;
    }
  }

  VTR_LOG("aggregate_mif bit/addr remapping tests passed.\n");
  return openfpga::CMD_EXEC_SUCCESS;
}
