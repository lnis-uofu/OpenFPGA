/********************************************************************
 * Unit test: read MIF settings and an init.hex file.
 *
 * Usage:
 *   test_aggregate_mif <bitstream_setting.xml> <init.hex>
 *******************************************************************/
#include <cstdint>
#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "read_mif.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_log.h"

int main(int argc, const char** argv) {
  if (argc != 3) {
    VTR_LOG_ERROR("Usage: %s <bitstream_setting.xml> <init.hex>\n", argv[0]);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  openfpga::BitstreamSetting bitstream_setting;
  const int setting_status =
    read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != setting_status) {
    return setting_status;
  }

  size_t num_mif_sources = 0;
  std::string pb_type;
  for (const MifSourceSettingId& source_id :
       bitstream_setting.mif_source_settings()) {
    if (pb_type.empty()) {
      pb_type = bitstream_setting.mif_source_pb_type(source_id);
    }
    ++num_mif_sources;
  }
  if (num_mif_sources == 0 || pb_type.empty()) {
    VTR_LOG_ERROR("No MIF source was read from bitstream settings\n");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  openfpga::MifStorage mif_storage;
  const int mif_status =
    openfpga::read_mif_from_init_hex(argv[2], mif_storage, pb_type);
  if (openfpga::CMD_EXEC_SUCCESS != mif_status) {
    return mif_status;
  }
  if (mif_storage.num_segments() != 1) {
    VTR_LOG_ERROR("Expected one MIF segment, found %zu\n",
                  mif_storage.num_segments());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  const MifSegmentId segment_id(0);
  if (mif_storage.physical_pb(segment_id) != pb_type) {
    VTR_LOG_ERROR("MIF segment pb_type does not match bitstream settings\n");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  size_t num_words = 0;
  uint64_t expected_address = 0;
  for (const MifMemoryLineId& line_id :
       mif_storage.segment_memory_lines(segment_id)) {
    if (mif_storage.memory_line_address(line_id) != expected_address ||
        mif_storage.memory_line_data(line_id).size() != 16) {
      VTR_LOG_ERROR("Unexpected MIF word at index %zu\n", num_words);
      return openfpga::CMD_EXEC_FATAL_ERROR;
    }
    ++num_words;
    ++expected_address;
  }
  if (num_words != 8) {
    VTR_LOG_ERROR("Expected 8 MIF words, found %zu\n", num_words);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  VTR_LOG("Bitstream-setting and read_mif tests passed.\n");
  return openfpga::CMD_EXEC_SUCCESS;
}
