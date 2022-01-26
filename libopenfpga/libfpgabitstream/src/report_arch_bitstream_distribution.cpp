/********************************************************************
 * This file includes functions that report distribution of bitstream by blocks
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

#include "bitstream_manager_utils.h"
#include "report_arch_bitstream_distribution.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information for an XML file of bitstream distribution
 *******************************************************************/
static 
void report_architecture_bitstream_distribution_xml_file_head(std::fstream& fp,
                                                              const bool& include_time_stamp) {
  valid_file_stream(fp);
 
  fp << "<!-- " << std::endl;
  fp << "\t- Report Architecture Bitstream Distribution" << std::endl;

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
 * Recursively report the bitstream distribution of a block to a file
 * This function will use a Depth-First Search in outputting bitstream
 * for each block 
 * For block with child blocks, we visit each child recursively
 * The reporting can be stopped at a given maximum hierarchy level 
 * which is used to limit the length of the report
 *******************************************************************/
static 
void rec_report_block_bitstream_distribution_to_xml_file(std::fstream& fp,
                                                         const BitstreamManager& bitstream_manager, 
                                                         const ConfigBlockId& block,
                                                         const size_t& max_hierarchy_level,
                                                         const size_t& hierarchy_level) {
  valid_file_stream(fp);

  if (hierarchy_level > max_hierarchy_level) {
    return;
  }

  /* Write the bitstream distribution of this block */
  write_tab_to_file(fp, hierarchy_level);
  fp << "<block";
  fp << " name=\"" << bitstream_manager.block_name(block)<< "\"";
  fp << " number_of_bits=\"" << rec_find_bitstream_manager_block_sum_of_bits(bitstream_manager, block) << "\"";
  fp << ">" << std::endl;

  /* Dive to child blocks if this block has any */
  for (const ConfigBlockId& child_block : bitstream_manager.block_children(block)) {
    rec_report_block_bitstream_distribution_to_xml_file(fp, bitstream_manager, child_block,
                                                        max_hierarchy_level, hierarchy_level + 1);
  }

  write_tab_to_file(fp, hierarchy_level);
  fp << "</block>" <<std::endl;
}

/********************************************************************
 * Report the distribution of bitstream by blocks, e.g., the number of
 * configuration bits per SB/CB/CLB
 * This function can generate a report to a file 
 *
 * Notes: 
 *   - The output format is a table whose format is compatible with RST files
 *******************************************************************/
int report_architecture_bitstream_distribution(const BitstreamManager& bitstream_manager,
                                               const std::string& fname,
                                               const bool& include_time_stamp,
                                               const size_t& max_hierarchy_level) {
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR("Received empty file name to report bitstream!\n\tPlease specify a valid file name.\n");
    return 1;
  }

  std::string timer_message = std::string("Report architecture bitstream distribution into XML file '") + fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(fname.c_str(), fp);

  /* Put down a brief introduction */
  report_architecture_bitstream_distribution_xml_file_head(fp, include_time_stamp);

  /* Find the top block, which has not parents */
  std::vector<ConfigBlockId> top_block = find_bitstream_manager_top_blocks(bitstream_manager);
  /* Make sure we have only 1 top block */
  VTR_ASSERT(1 == top_block.size());

  /* Write bitstream, block by block, in a recursive way */
  rec_report_block_bitstream_distribution_to_xml_file(fp, bitstream_manager, top_block[0], max_hierarchy_level, 0);

  /* Close file handler */
  fp.close();

  return 0;
}

} /* end namespace openfpga */
