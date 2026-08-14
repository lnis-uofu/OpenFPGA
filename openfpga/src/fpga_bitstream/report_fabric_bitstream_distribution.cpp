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
#include "bitstream_manager_utils.h"
#include "openfpga_digest.h"
#include "openfpga_reserved_words.h"
#include "openfpga_tokenizer.h"
#include "openfpga_version.h"
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
static void report_region_bitstream_distribution_to_xml_file(
  pugi::xml_node& parent_node, const FabricBitstream& fabric_bitstream,
  const FabricBitRegionId& region) {
  /* Write the bitstream distribution of this block */
  pugi::xml_node region_node = parent_node.append_child("region");
  region_node.append_attribute("id").set_value(static_cast<unsigned long long>(size_t(region)));
  region_node.append_attribute("number_of_bits").set_value(static_cast<unsigned long long>(fabric_bitstream.region_bits(region).size()));
}

/********************************************************************
 * Report the distribution of bitstream by regions
 * This function can generate a report to a file
 *******************************************************************/
int report_fabric_bitstream_distribution(
  pugi::xml_node& parent_node, const FabricBitstream& fabric_bitstream) {
  std::string timer_message =
    std::string("Report fabric bitstream distribution");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Write bitstream, region by region, in a recursive way */
  int curr_level = hierarchy_level;
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    pugi::xml_node regions_node = parent_node.append_child("regions");
    report_region_bitstream_distribution_to_xml_file(regions_node, fabric_bitstream,
                                                     region);
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
