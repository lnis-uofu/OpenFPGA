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
int report_architecture_bitstream_distribution(std::fstream& fp,
                                               const BitstreamManager& bitstream_manager,
                                               const size_t& max_hierarchy_level,
                                               const size_t& hierarchy_level) {
  std::string timer_message = std::string("Report architecture bitstream distribution");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Check the file stream */
  valid_file_stream(fp);

  int curr_level = hierarchy_level;
  write_tab_to_file(fp, curr_level);
  fp << "<blocks>" <<std::endl;

  /* Find the top block, which has not parents */
  std::vector<ConfigBlockId> top_block = find_bitstream_manager_top_blocks(bitstream_manager);
  /* Make sure we have only 1 top block */
  VTR_ASSERT(1 == top_block.size());

  /* Write bitstream, block by block, in a recursive way */
  rec_report_block_bitstream_distribution_to_xml_file(fp, bitstream_manager, top_block[0], max_hierarchy_level + 2, curr_level + 1);

  write_tab_to_file(fp, curr_level);
  fp << "</blocks>" <<std::endl;

  return 0;
}

} /* end namespace openfpga */
