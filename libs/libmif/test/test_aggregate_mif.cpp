/********************************************************************
 * Unit test: Yosys eblif .param INIT -> aggregate_mif
 *
 * Usage:
 *   test_aggregate_mif <bitstream_setting.xml> <yosys_output.eblif>
 *******************************************************************/
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "aggregate_mif.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "mif_storage_fwd.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_log.h"
#include "write_mif.h"

int main(int argc, const char** argv) {
  if (argc != 3) {
    VTR_LOG_ERROR("Usage: %s <bitstream_setting.xml> <yosys_output.eblif>\n",
                  argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  openfpga::BitstreamSetting bitstream_setting;
  int status = read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }

  openfpga::MifStorage logical_storage;
  openfpga::MifStorage aggregated_storage;
  const std::vector<std::string> input_pb_types = {
    "memory[mem_8x16_dp].mem_8x16_dp[0]", "memory[mem_8x16_dp].mem_8x16_dp[1]"};
  size_t next_pb_type = 0;
  status = openfpga::aggregate_mif(
    logical_storage, bitstream_setting, aggregated_storage,
    [&input_pb_types, &next_pb_type](const std::string&,
                                     const openfpga::MifEblifPortConnections&) {
      return input_pb_types.at(next_pb_type++);
    },
    argv[2]);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  const MifSegmentId out_segment(0);

  std::map<uint64_t, uint64_t> words;
  for (const MifMemoryLineId& line_id :
       aggregated_storage.segment_memory_lines(out_segment)) {
    words[aggregated_storage.memory_line_address(line_id)] =
      aggregated_storage.memory_line_data(line_id);
  }

  status = openfpga::write_mif("aggregated_preload.mem", aggregated_storage);

  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("aggregate_mif eblif-source test passed.\n");
  return openfpga::CMD_EXEC_SUCCESS;
}
