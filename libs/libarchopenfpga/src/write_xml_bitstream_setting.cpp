/********************************************************************
 * This file includes functions that outputs a bitstream setting to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <string>

/* Headers from vtr util library */
#include "openfpga_digest.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga library */
#include "bitstream_setting_xml_constants.h"
#include "write_xml_bitstream_setting.h"
#include "write_xml_utils.h"

/********************************************************************
 * Generate the full hierarchy name for a pb_type in bitstream setting
 *******************************************************************/
static std::string generate_bitstream_setting_pb_type_hierarchy_name(
  const openfpga::BitstreamSetting& bitstream_setting,
  const BitstreamPbTypeSettingId& bitstream_pb_type_setting_id) {
  /* Iterate over the parent_pb_type and modes names, they should well match */
  VTR_ASSERT_SAFE(
    bitstream_setting.parent_pb_type_names(bitstream_pb_type_setting_id)
      .size() ==
    bitstream_setting.parent_mode_names(bitstream_pb_type_setting_id).size());

  std::string hie_name;

  for (size_t i = 0;
       i < bitstream_setting.parent_pb_type_names(bitstream_pb_type_setting_id)
             .size();
       ++i) {
    hie_name +=
      bitstream_setting.parent_pb_type_names(bitstream_pb_type_setting_id)[i];
    hie_name += std::string("[");
    hie_name +=
      bitstream_setting.parent_mode_names(bitstream_pb_type_setting_id)[i];
    hie_name += std::string("]");
    hie_name += std::string(".");
  }

  /* Add the leaf pb_type */
  hie_name += bitstream_setting.pb_type_name(bitstream_pb_type_setting_id);

  return hie_name;
}

/********************************************************************
 * Generate the full hierarchy name for a pb_type in bitstream setting
 *******************************************************************/
static std::string generate_bitstream_setting_pb_type_hierarchy_name(
  const openfpga::BitstreamSetting& bitstream_setting,
  const BitstreamDefaultModeSettingId& bitstream_pb_type_setting_id) {
  /* Iterate over the parent_pb_type and modes names, they should well match */
  VTR_ASSERT_SAFE(
    bitstream_setting
      .default_mode_parent_pb_type_names(bitstream_pb_type_setting_id)
      .size() == bitstream_setting
                   .default_mode_parent_mode_names(bitstream_pb_type_setting_id)
                   .size());

  std::string hie_name;

  for (size_t i = 0;
       i < bitstream_setting
             .default_mode_parent_pb_type_names(bitstream_pb_type_setting_id)
             .size();
       ++i) {
    hie_name += bitstream_setting.default_mode_parent_pb_type_names(
      bitstream_pb_type_setting_id)[i];
    hie_name += std::string("[");
    hie_name += bitstream_setting.default_mode_parent_mode_names(
      bitstream_pb_type_setting_id)[i];
    hie_name += std::string("]");
    hie_name += std::string(".");
  }

  /* Add the leaf pb_type */
  hie_name +=
    bitstream_setting.default_mode_pb_type_name(bitstream_pb_type_setting_id);

  return hie_name;
}

/********************************************************************
 * Generate the full hierarchy name for an interconnect in bitstream setting
 *******************************************************************/
static std::string generate_bitstream_setting_interconnect_hierarchy_name(
  const openfpga::BitstreamSetting& bitstream_setting,
  const BitstreamInterconnectSettingId& bitstream_interc_setting_id) {
  /* Iterate over the parent_pb_type and modes names, they should well match */
  VTR_ASSERT_SAFE(
    bitstream_setting.parent_pb_type_names(bitstream_interc_setting_id)
      .size() ==
    bitstream_setting.parent_mode_names(bitstream_interc_setting_id).size());

  std::string hie_name;

  for (size_t i = 0;
       i < bitstream_setting.parent_pb_type_names(bitstream_interc_setting_id)
             .size();
       ++i) {
    hie_name +=
      bitstream_setting.parent_pb_type_names(bitstream_interc_setting_id)[i];
    hie_name += std::string("[");
    hie_name +=
      bitstream_setting.parent_mode_names(bitstream_interc_setting_id)[i];
    hie_name += std::string("]");
    hie_name += std::string(".");
  }

  /* Add the leaf pb_type */
  hie_name += bitstream_setting.interconnect_name(bitstream_interc_setting_id);

  return hie_name;
}

/********************************************************************
 * A writer to output a bitstream pb_type setting to XML format
 *******************************************************************/
static void write_xml_bitstream_pb_type_setting(
  std::fstream& fp, const char* fname,
  const openfpga::BitstreamSetting& bitstream_setting,
  const BitstreamPbTypeSettingId& bitstream_pb_type_setting_id) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t"
     << "<" << XML_PB_TYPE_NODE_NAME;

  /* Generate the full hierarchy name of the pb_type */
  write_xml_attribute(fp, XML_PB_TYPE_ATTRIBUTE_NAME,
                      generate_bitstream_setting_pb_type_hierarchy_name(
                        bitstream_setting, bitstream_pb_type_setting_id)
                        .c_str());

  write_xml_attribute(
    fp, XML_PB_TYPE_ATTRIBUTE_SOURCE,
    bitstream_setting.pb_type_bitstream_source(bitstream_pb_type_setting_id)
      .c_str());
  write_xml_attribute(
    fp, XML_PB_TYPE_ATTRIBUTE_CONTENT,
    bitstream_setting.pb_type_bitstream_content(bitstream_pb_type_setting_id)
      .c_str());
  write_xml_attribute(
    fp, XML_PB_TYPE_ATTRIBUTE_IS_MODE_SELECT_BITSTREAM,
    bitstream_setting.is_mode_select_bitstream(bitstream_pb_type_setting_id));
  write_xml_attribute(
    fp, XML_PB_TYPE_ATTRIBUTE_BITSTREAM_OFFSET,
    bitstream_setting.bitstream_offset(bitstream_pb_type_setting_id));

  fp << "/>"
     << "\n";
}

/********************************************************************
 * A writer to output a bitstream pb_type setting to XML format
 *******************************************************************/
static void write_xml_bitstream_default_mode_setting(
  std::fstream& fp, const char* fname,
  const openfpga::BitstreamSetting& bitstream_setting,
  const BitstreamDefaultModeSettingId& bitstream_default_mode_setting_id) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t"
     << "<" << XML_DEFAULT_MODE_BITS_NODE_NAME;

  /* Generate the full hierarchy name of the pb_type */
  write_xml_attribute(fp, XML_DEFAULT_MODE_BITS_ATTRIBUTE_NAME,
                      generate_bitstream_setting_pb_type_hierarchy_name(
                        bitstream_setting, bitstream_default_mode_setting_id)
                        .c_str());

  write_xml_attribute(
    fp, XML_DEFAULT_MODE_BITS_ATTRIBUTE_MODE_BITS,
    bitstream_setting
      .default_mode_bits_to_string(bitstream_default_mode_setting_id)
      .c_str());
  fp << "/>"
     << "\n";
}

/********************************************************************
 * A writer to output a bitstream clock_routing setting to XML format
 *******************************************************************/
static void write_xml_bitstream_clock_routing_setting(
  std::fstream& fp, const char* fname,
  const openfpga::BitstreamSetting& bitstream_setting,
  const BitstreamClockRoutingSettingId& bitstream_clock_routing_setting_id) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t"
     << "<" << XML_CLOCK_ROUTING_NODE_NAME;

  /* Generate the full hierarchy name of the pb_type */
  write_xml_attribute(
    fp, XML_CLOCK_ROUTING_ATTRIBUTE_NETWORK,
    bitstream_setting.clock_routing_network(bitstream_clock_routing_setting_id)
      .c_str());

  write_xml_attribute(
    fp, XML_CLOCK_ROUTING_ATTRIBUTE_PIN,
    bitstream_setting.clock_routing_pin(bitstream_clock_routing_setting_id)
      .to_verilog_string()
      .c_str());
  fp << "/>"
     << "\n";
}

/********************************************************************
 * A writer to output a bitstream interconnect setting to XML format
 *******************************************************************/
static void write_xml_bitstream_interconnect_setting(
  std::fstream& fp, const char* fname,
  const openfpga::BitstreamSetting& bitstream_setting,
  const BitstreamInterconnectSettingId& bitstream_interc_setting_id) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t"
     << "<" << XML_INTERCONNECT_NODE_NAME;

  /* Generate the full hierarchy name of the pb_type */
  write_xml_attribute(fp, XML_INTERCONNECT_ATTRIBUTE_NAME,
                      generate_bitstream_setting_interconnect_hierarchy_name(
                        bitstream_setting, bitstream_interc_setting_id)
                        .c_str());

  write_xml_attribute(
    fp, XML_INTERCONNECT_ATTRIBUTE_DEFAULT_PATH,
    bitstream_setting.default_path(bitstream_interc_setting_id).c_str());

  fp << "/>"
     << "\n";
}

/********************************************************************
 * A writer to output a bitstream setting to XML format
 *******************************************************************/
void write_xml_bitstream_setting(
  std::fstream& fp, const char* fname,
  const openfpga::BitstreamSetting& bitstream_setting) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node <openfpga_bitstream_setting>
   */
  fp << "<" << XML_BITSTREAM_SETTING_ROOT_NAME << ">"
     << "\n";

  /* Write pb_type -related settings */
  for (const auto& bitstream_pb_type_setting_id :
       bitstream_setting.pb_type_settings()) {
    write_xml_bitstream_pb_type_setting(fp, fname, bitstream_setting,
                                        bitstream_pb_type_setting_id);
  }

  /* Write default_mode -related settings */
  for (const auto& bitstream_default_mode_setting_id :
       bitstream_setting.default_mode_settings()) {
    write_xml_bitstream_default_mode_setting(fp, fname, bitstream_setting,
                                             bitstream_default_mode_setting_id);
  }

  /* Write clock_routing -related settings */
  for (const auto& bitstream_clock_routing_setting_id :
       bitstream_setting.clock_routing_settings()) {
    write_xml_bitstream_clock_routing_setting(
      fp, fname, bitstream_setting, bitstream_clock_routing_setting_id);
  }

  /* Write interconnect -related settings */
  for (const auto& bitstream_interc_setting_id :
       bitstream_setting.interconnect_settings()) {
    write_xml_bitstream_interconnect_setting(fp, fname, bitstream_setting,
                                             bitstream_interc_setting_id);
  }

  /* Write the root node <openfpga_bitstream_setting> */
  fp << "</" << XML_BITSTREAM_SETTING_ROOT_NAME << ">"
     << "\n";
}
