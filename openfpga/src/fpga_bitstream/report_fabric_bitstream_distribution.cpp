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

#include "bitstream_manager_utils.h"
#include "report_fabric_bitstream_distribution.h"

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
void report_region_bitstream_distribution_to_xml_file(std::fstream& fp,
                                                      const FabricBitstream& fabric_bitstream, 
                                                      const FabricBitRegionId& region,
                                                      const int& hierarchy_level) {
  valid_file_stream(fp);

  /* Write the bitstream distribution of this block */
  write_tab_to_file(fp, hierarchy_level);
  fp << "<region";
  fp << " id=\"" << size_t(region) << "\"";
  fp << " number_of_bits=\"" << fabric_bitstream.region_bits(region).size() << "\"";
  fp << ">" << std::endl;

  write_tab_to_file(fp, hierarchy_level);
  fp << "</region>" <<std::endl;
}

/********************************************************************
 * Report the distribution of bitstream by regions
 * This function can generate a report to a file 
 *******************************************************************/
int report_fabric_bitstream_distribution(std::fstream& fp,
                                         const FabricBitstream& fabric_bitstream,
                                         const int& hierarchy_level) {
  std::string timer_message = std::string("Report fabric bitstream distribution");
  vtr::ScopedStartFinishTimer timer(timer_message);

  valid_file_stream(fp);

  /* Write bitstream, region by region, in a recursive way */
  int curr_level = hierarchy_level;
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    write_tab_to_file(fp, curr_level);
    fp << "<regions>" <<std::endl;
    report_region_bitstream_distribution_to_xml_file(fp, fabric_bitstream, region, curr_level + 1);
    write_tab_to_file(fp, curr_level);
    fp << "</regions>" <<std::endl;

  }

  return 0;
}

} /* end namespace openfpga */
