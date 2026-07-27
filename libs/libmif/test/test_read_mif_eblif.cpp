/********************************************************************
 * Unit test: eblif -> read_mif -> aggregate_mif -> write_mif
 *
 * Usage:
 *   test_read_mif_eblif <mif_eblif_8x16_setting.xml>
 *                       <dual_port_ram_8x16_mif_yosys_out.eblif>
 *                       <out_aggregated.mem>
 *******************************************************************/
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

#include "aggregate_mif.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "mif_storage_fwd.h"
#include "read_mif.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_mif.h"

static std::map<uint64_t, uint64_t> collect_segment_lines(
  const openfpga::MifStorage& storage, const MifSegmentId& seg) {
  std::map<uint64_t, uint64_t> out;
  for (const MifMemoryLineId& line_id : storage.segment_memory_lines(seg)) {
    out[storage.memory_line_address(line_id)] =
      storage.memory_line_data(line_id);
  }
  return out;
}

static bool mem_file_contains(const std::string& path,
                              const std::string& needle) {
  std::ifstream ifs(path.c_str());
  if (!ifs.is_open()) {
    return false;
  }
  std::ostringstream oss;
  oss << ifs.rdbuf();
  return oss.str().find(needle) != std::string::npos;
}

int main(int argc, const char** argv) {
  if (argc < 4) {
    VTR_LOG_ERROR(
      "Usage: %s <mif_eblif_8x16_setting.xml> "
      "<dual_port_ram_8x16_mif_yosys_out.eblif> <out_aggregated.mem>\n",
      argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  openfpga::BitstreamSetting bitstream_setting;
  int status = read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  /* ---- read_mif (eblif) ---- */
  openfpga::MifStorage logical_storage;
  status = openfpga::read_mif(argv[2], logical_storage, bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  /* Two .param INIT -> two logical segments (memory_0, memory_1). */
  VTR_ASSERT(2 == logical_storage.num_segments());

  const MifSegmentId seg0(0);
  const MifSegmentId seg1(1);
  VTR_ASSERT(logical_storage.physical_pb(seg0) ==
             "memory[mem_8x16_dp].mem_8x16_dp[0]");
  VTR_ASSERT(logical_storage.physical_pb(seg1) ==
             "memory[mem_8x16_dp].mem_8x16_dp[1]");

  const auto mem0 = collect_segment_lines(logical_storage, seg0);
  const auto mem1 = collect_segment_lines(logical_storage, seg1);

  /* Matches libs/libmif/example/init.hex (memory_0) */
  VTR_ASSERT(mem0.at(0) == 0x138Full);
  VTR_ASSERT(mem0.at(1) == 0x0020ull);
  VTR_ASSERT(mem0.at(2) == 0x37EAull);
  VTR_ASSERT(mem0.at(7) == 0x42FBull);

  /* Matches libs/libmif/example/init1.hex (memory_1) */
  VTR_ASSERT(mem1.at(0) == 0xABCDull);
  VTR_ASSERT(mem1.at(1) == 0x1111ull);
  VTR_ASSERT(mem1.at(2) == 0x2222ull);
  VTR_ASSERT(mem1.at(7) == 0xDEADull);

  /* ---- aggregate_mif reads the Yosys eblif, then packs into 32-bit des ---- */
  openfpga::MifStorage aggregated;
  logical_storage.clear();
  status = openfpga::aggregate_mif(argv[2], logical_storage,
                                   bitstream_setting, aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  VTR_ASSERT(1 == aggregated.num_segments());
  const MifSegmentId out_seg(0);
  VTR_ASSERT(aggregated.physical_pb(out_seg) ==
             "memory[dpram8x32].dpram8x32");
  VTR_ASSERT(32 == aggregated.data_width(out_seg));
  VTR_ASSERT(aggregated.addr_range(out_seg).is_valid());
  VTR_ASSERT(0 == aggregated.addr_range(out_seg).get_lsb());
  VTR_ASSERT(7 == aggregated.addr_range(out_seg).get_msb());

  const auto phys = collect_segment_lines(aggregated, out_seg);
  /* [0] -> bits[16:31], [1] -> bits[0:15], same address */
  VTR_ASSERT(phys.at(0) == 0x138FABCDull);
  VTR_ASSERT(phys.at(1) == 0x00201111ull);
  VTR_ASSERT(phys.at(2) == 0x37EA2222ull);
  VTR_ASSERT(phys.at(7) == 0x42FBDEADull);

  /* ---- write_mif ---- */
  status = openfpga::write_mif(argv[3], aggregated);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  VTR_ASSERT(mem_file_contains(argv[3], "PB_TYPE memory[dpram8x32].dpram8x32"));
  VTR_ASSERT(mem_file_contains(argv[3], "Data width: 32"));
  VTR_ASSERT(mem_file_contains(argv[3], "0 0x138fabcd"));
  VTR_ASSERT(mem_file_contains(argv[3], "7 0x42fbdead"));

  VTR_LOG(
    "eblif -> read_mif -> aggregate_mif -> write_mif passed (wrote '%s').\n",
    argv[3]);
  return openfpga::CMD_EXEC_SUCCESS;
}
