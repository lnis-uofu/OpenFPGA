/********************************************************************
 * This file includes functions that report distribution of bitstream by regions
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_tokenizer.h"
#include "openfpga_version.h"

#include "openfpga_reserved_words.h"

#include "report_arch_bitstream_distribution.h"
#include "report_fabric_bitstream_distribution.h"
#include "report_bitstream_distribution.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information for an XML file of bitstream distribution
 *******************************************************************/
static 
void report_bitstream_distribution_xml_file_head(std::fstream& fp,
                                                 const bool& include_time_stamp) {
  valid_file_stream(fp);
 
  fp << "<!-- " << std::endl;
  fp << "\t- Report Bitstream Distribution" << std::endl;

  if (include_time_stamp) {
    auto end = std::chrono::system_clock::now(); 
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    /* Note that version is also a type of time stamp */
    fp << "\t- Version: " << openfpga::VERSION << std::endl;
    fp << "\t- Date: " << std::ctime(&end_time) ;
  }

  fp << "--> " << std::endl;
  fp << std::endl;
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
    VTR_LOG_ERROR("Received empty file name to report bitstream!\n\tPlease specify a valid file name.\n");
    return 1;
  }

  std::string timer_message = std::string("Report bitstream distribution into XML file '") + fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(fname.c_str(), fp);

  /* Put down a brief introduction */
  report_bitstream_distribution_xml_file_head(fp, include_time_stamp);

  int curr_level = 0;
  write_tab_to_file(fp, curr_level);
  fp << "<bitstream_distribution>" <<std::endl;

  int status = 0;
  status = report_fabric_bitstream_distribution(fp, fabric_bitstream, curr_level + 1);
  if (status == 1) {
    return status;
  }
  status = report_architecture_bitstream_distribution(fp, bitstream_manager, max_hierarchy_level, curr_level + 1);

  fp << "</bitstream_distribution>" <<std::endl;

  /* Close file handler */
  fp.close();

  return status;
}

} /* end namespace openfpga */
