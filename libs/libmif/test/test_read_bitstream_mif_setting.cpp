/********************************************************************
 * Unit test: read MIF-related entries from openfpga_bitstream_setting
 *
 * Usage:
 *   test_read_bitstream_mif_setting <bitstream_setting.xml> [out.xml]
 *******************************************************************/
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_xml_openfpga_arch.h"

static void validate_mif_bitstream_setting(
  const openfpga::BitstreamSetting& bitstream_setting) {
  VTR_ASSERT(2 == bitstream_setting.mif_source_settings().size());
  VTR_ASSERT(4 == bitstream_setting.mif_address_map_settings().size());

  const MifSourceSettingId src0 =
    *bitstream_setting.mif_source_settings().begin();
  VTR_ASSERT(bitstream_setting.mif_source_pb_type(src0) ==
             "dpram8x32[dual].dpram8x16");
  VTR_ASSERT(bitstream_setting.mif_source_source(src0) == "eblif");
  VTR_ASSERT(bitstream_setting.mif_source_content(src0) == ".param INIT");

  const MifAddressMapSettingId dualA1_id =
    bitstream_setting.find_mif_address_map_by_pb_type(
      "dpram8x32[0]{dualA}.dpram4x32[1]");
  VTR_ASSERT(dualA1_id.is_valid());
  VTR_ASSERT(4 == bitstream_setting.mif_address_map_address_offset(dualA1_id));
  VTR_ASSERT(0 == bitstream_setting.mif_address_map_data_offset(dualA1_id));

  VTR_LOG("Validated %lu mif_source and %lu mif_address_map entries.\n",
          static_cast<unsigned long>(
            bitstream_setting.mif_source_settings().size()),
          static_cast<unsigned long>(
            bitstream_setting.mif_address_map_settings().size()));
}

int main(int argc, const char** argv) {
  VTR_ASSERT((2 == argc) || (3 == argc));

  openfpga::BitstreamSetting bitstream_setting;
  const int status =
    read_xml_openfpga_bitstream_settings(argv[1], bitstream_setting);
  if (openfpga::CMD_EXEC_SUCCESS != status) {
    return status;
  }
  VTR_LOG("Parsed bitstream settings from XML %s.\n", argv[1]);

  validate_mif_bitstream_setting(bitstream_setting);

  if (3 <= argc) {
    write_xml_openfpga_bitstream_settings(argv[2], bitstream_setting);
    VTR_LOG("Echo the OpenFPGA bitstream settings to an XML file: %s.\n",
            argv[2]);
  }

  return openfpga::CMD_EXEC_SUCCESS;
}
