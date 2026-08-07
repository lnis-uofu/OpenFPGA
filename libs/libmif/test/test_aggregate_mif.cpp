/********************************************************************
 * Unit test: Yosys eblif .param INIT -> MifPipeline
 *
 * Usage:
 *   test_aggregate_mif <bitstream_setting.xml> <yosys_output.eblif>
 *******************************************************************/
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_pipeline.h"
#include "mif_storage_fwd.h"
#include "read_xml_openfpga_arch.h"
#include "vpr_context.h"
#include "vtr_log.h"
#include "write_mif.h"

namespace {

/* Stub resolver: unit test has no packed VPR netlist. */
const std::vector<std::string> k_stub_pb_types = {
  "memory[mem_8x16_dp].mem_8x16_dp[0]", "memory[mem_8x16_dp].mem_8x16_dp[1]"};
size_t g_next_stub_pb_type = 0;

std::string stub_mif_pb_type_from_vpr(
  const AtomContext&, const DeviceContext&, const std::string&,
  const openfpga::MifEblifPortConnections&) {
  return k_stub_pb_types.at(g_next_stub_pb_type++);
}

} /* namespace */

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

  openfpga::MifPipeline pipeline;
  AtomContext atom_ctx;
  DeviceContext device_ctx;
  status = pipeline.load_eblif(argv[2], bitstream_setting, atom_ctx, device_ctx,
                               stub_mif_pb_type_from_vpr);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = pipeline.merge_to_logical(bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = pipeline.decode_logical(bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  status = pipeline.aggregate_to_physical(bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  const MifSegmentId out_segment(0);

  std::map<uint64_t, std::string> words;
  for (const MifMemoryLineId& line_id :
       pipeline.storage(openfpga::MifPipeline::Stage::PHYSICAL)
         .segment_memory_lines(out_segment)) {
    words[pipeline.storage(openfpga::MifPipeline::Stage::PHYSICAL)
            .memory_line_address(line_id)] =
      pipeline.storage(openfpga::MifPipeline::Stage::PHYSICAL)
        .memory_line_data(line_id);
  }

  status = pipeline.write_stage("aggregated_preload.mem",
                                openfpga::MifPipeline::Stage::PHYSICAL);

  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("MifPipeline eblif-source test passed.\n");
  return openfpga::CMD_EXEC_SUCCESS;
}
