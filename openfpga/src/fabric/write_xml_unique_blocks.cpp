
/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- reads an XML file of unique blocks to the associated
 * data structures: device_rr_gsb
 * -- write device__rr_gsb's info about unique blocks to a xml file
 *******************************************************************/

#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "command_exit_codes.h"
#include "device_rr_gsb_utils.h"
#include "openfpga_digest.h"
#include "read_xml_util.h"
#include "rr_gsb.h"
#include "write_xml_unique_blocks.h"
#include "write_xml_utils.h"
/********************************************************************
 * Parse XML codes of a <instance> to an object of device_rr_gsb
 * instance is the mirror of unique module.
 *******************************************************************/
namespace openfpga {

int write_xml_block(
  std::map<int, vtr::Point<size_t>>& id_unique_block_map,
  std::map<int, std::vector<vtr::Point<size_t>>>& id_instance_map,
  std::fstream& fp, std::string type) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  for (const auto& pair : id_unique_block_map) {
    openfpga::write_tab_to_file(fp, 1);
    fp << "<block";
    write_xml_attribute(fp, "type", type.c_str());
    write_xml_attribute(fp, "x", pair.second.x());
    write_xml_attribute(fp, "y", pair.second.y());

    fp << ">"
       << "\n";

    for (const auto& instance_info : id_instance_map[pair.first]) {
      if (instance_info.x() == pair.second.x() &&
          instance_info.y() == pair.second.y()) {
        ;
      } else {
        openfpga::write_tab_to_file(fp, 2);
        fp << "<instance";
        write_xml_attribute(fp, "x", instance_info.x());
        write_xml_attribute(fp, "y", instance_info.y());

        fp << "/>"
           << "\n";
      }
    }
    openfpga::write_tab_to_file(fp, 1);
    fp << "</block>"
       << "\n";
  }

  return CMD_EXEC_SUCCESS;
}

void report_unique_module_status_write(const DeviceRRGSB& device_rr_gsb,
                                       bool verbose_output) {
  /* Report the stats */
  VTR_LOGV(
    verbose_output,
    "Write %lu unique X-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    device_rr_gsb.get_num_cb_unique_module(CHANX),
    find_device_rr_gsb_num_cb_modules(device_rr_gsb, CHANX),
    100. * ((float)find_device_rr_gsb_num_cb_modules(device_rr_gsb, CHANX) /
              (float)device_rr_gsb.get_num_cb_unique_module(CHANX) -
            1.));

  VTR_LOGV(
    verbose_output,
    "Write %lu unique Y-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    device_rr_gsb.get_num_cb_unique_module(CHANY),
    find_device_rr_gsb_num_cb_modules(device_rr_gsb, CHANY),
    100. * ((float)find_device_rr_gsb_num_cb_modules(device_rr_gsb, CHANY) /
              (float)device_rr_gsb.get_num_cb_unique_module(CHANY) -
            1.));

  VTR_LOGV(verbose_output,
           "Write %lu unique switch blocks from a total of %d (compression "
           "rate=%.2f%)\n",
           device_rr_gsb.get_num_sb_unique_module(),
           find_device_rr_gsb_num_sb_modules(device_rr_gsb,
                                             g_vpr_ctx.device().rr_graph),
           100. * ((float)find_device_rr_gsb_num_sb_modules(
                     device_rr_gsb, g_vpr_ctx.device().rr_graph) /
                     (float)device_rr_gsb.get_num_sb_unique_module() -
                   1.));

  VTR_LOG(
    "Write %lu unique general switch blocks from a total of %d "
    "(compression "
    "rate=%.2f%)\n",
    device_rr_gsb.get_num_gsb_unique_module(),
    find_device_rr_gsb_num_gsb_modules(device_rr_gsb,
                                       g_vpr_ctx.device().rr_graph),
    100. * ((float)find_device_rr_gsb_num_gsb_modules(
              device_rr_gsb, g_vpr_ctx.device().rr_graph) /
              (float)device_rr_gsb.get_num_gsb_unique_module() -
            1.));
}

int write_xml_unique_blocks(const DeviceRRGSB& device_rr_gsb, const char* fname,
                            bool verbose_output) {
  vtr::ScopedStartFinishTimer timer("Write unique blocks...");
  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "<unique_blocks>"
     << "\n";

  int err_code = 0;
  std::map<int, vtr::Point<size_t>> id_unique_block_map;
  std::map<int, std::vector<vtr::Point<size_t>>> id_instance_map;
  device_rr_gsb.get_id_unique_sb_block_map(id_unique_block_map);
  device_rr_gsb.get_id_sb_instance_map(id_instance_map);
  err_code += write_xml_block(id_unique_block_map, id_instance_map, fp, "sb");

  id_unique_block_map.clear();
  id_instance_map.clear();
  device_rr_gsb.get_id_unique_cbx_block_map(id_unique_block_map);
  device_rr_gsb.get_id_cbx_instance_map(id_instance_map);
  err_code += write_xml_block(id_unique_block_map, id_instance_map, fp, "cbx");

  id_unique_block_map.clear();
  id_instance_map.clear();
  device_rr_gsb.get_id_unique_cby_block_map(id_unique_block_map);
  device_rr_gsb.get_id_cby_instance_map(id_instance_map);
  err_code += write_xml_block(id_unique_block_map, id_instance_map, fp, "cby");

  /* Finish writing the root node */
  fp << "</unique_blocks>"
     << "\n";

  /* Close the file stream */
  fp.close();
  if (verbose_output) {
    report_unique_module_status_write(device_rr_gsb, true);
  }

  if (err_code >= 1) {
    return CMD_EXEC_FATAL_ERROR;
  } else {
    return CMD_EXEC_SUCCESS;
  }
}
}  // namespace openfpga
