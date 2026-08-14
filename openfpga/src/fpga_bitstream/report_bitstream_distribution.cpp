/********************************************************************
 * This file includes functions that report distribution of bitstream by regions
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>

/* Headers from vtrutil library */
#include "pugixml.hpp"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "command_exit_codes.h"
#include "openfpga_digest.h"
#include "openfpga_gz_xml_writer.h"
#include "openfpga_reserved_words.h"
#include "openfpga_tokenizer.h"
#include "openfpga_version.h"
#include "report_arch_bitstream_distribution.h"
#include "report_bitstream_distribution.h"
#include "report_fabric_bitstream_distribution.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information for an XML file of bitstream
 *distribution
 *******************************************************************/
static void report_bitstream_distribution_xml_file_head(
  pugi::xml_node& root_node, const bool& include_time_stamp) {
  std::stringstream ss;
  ss << "\t- Report Bitstream Distribution" << std::endl;

  if (include_time_stamp) {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    /* Note that version is also a type of time stamp */
    ss << "\t- Version: " << openfpga::VERSION << std::endl;
    ss << "\t- Date: " << std::ctime(&end_time);
  }

  pugi::xml_node cmt_node = root_node.append_child(pugi::node_comment);
  cmt_node.set_value(ss.str().c_str());
}

/********************************************************************
 * Report the distribution of bitstream at architecture-level and fabric-level
 * This function can generate a report to a file
 *******************************************************************/
int report_bitstream_distribution(const std::string& fname,
                                  const BitstreamManager& bitstream_manager,
                                  const FabricBitstream& fabric_bitstream,
                                  const bool& include_time_stamp,
                                  const size_t& max_hierarchy_level) {
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR(
      "Received empty file name to report bitstream!\n\tPlease specify a valid "
      "file name.\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  std::string timer_message =
    std::string("Report bitstream distribution into XML file '") + fname +
    std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  pugi::xml_document doc;
  pugi::xml_node root_node = doc.append_child("bitstream_distribution");

  /* Put down a brief introduction */
  report_bitstream_distribution_xml_file_head(root_node, include_time_stamp);

  int status = CMD_EXEC_FATAL_ERROR;
  status = report_fabric_bitstream_distribution(root_node, fabric_bitstream);
  if (status == CMD_EXEC_FATAL_ERROR) {
    return status;
  }
  int curr_level = 0;
  status = report_architecture_bitstream_distribution(
    root_node, bitstream_manager, max_hierarchy_level, curr_level + 1);

  /* Output to xml file */
  bool output_success = false;
  if (file_require_gz(fname)) {
    GzXmlWriter writer(fname.c_str());
    if (writer.isValid()) {
      doc.save(writer);
      output_success = true;
    }
  } else {
    output_success = doc.save_file(fname.c_str());
  }
  if (output_success) {
    VTR_LOGV("Succeed to output XML file: %s\n", fname.c_str());
  } else {
    VTR_LOG_ERROR("Failed to output XML file: %s\n", fname.c_str());
  }
  return output_success ? CMD_EXEC_SUCCESS : CMD_EXEC_FATAL_ERROR;
}

} /* end namespace openfpga */
