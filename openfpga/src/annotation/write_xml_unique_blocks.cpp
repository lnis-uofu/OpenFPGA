
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
 * This file includes the top-level functions of this library
 * which includes:
 *  -- write the unique blocks' information in the associated data structures:
 *device_rr_gsb to a XML file
 *******************************************************************/
namespace openfpga {

/*Write unique blocks and their corresponding instances' information from
 *device_rr_gsb to a XML file*/
int write_xml_atom_block(std::fstream& fp,
                         const std::vector<vtr::Point<size_t>>& instance_map,
                         const vtr::Point<size_t>& unique_block_coord,
                         std::string type) {
  if (false == openfpga::valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<block";
  write_xml_attribute(fp, "type", type.c_str());
  write_xml_attribute(fp, "x", unique_block_coord.x());
  write_xml_attribute(fp, "y", unique_block_coord.y());

  fp << ">"
     << "\n";

  for (const auto& instance_info : instance_map) {
    if (instance_info.x() == unique_block_coord.x() &&
        instance_info.y() == unique_block_coord.y()) {
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
  return openfpga::CMD_EXEC_SUCCESS;
}

/* Report information about written unique blocks */
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

/*Top level function to write the xml file of unique blocks*/
int write_xml_unique_blocks(const DeviceRRGSB& device_rr_gsb, const char* fname,
                            bool verbose_output) {
  vtr::ScopedStartFinishTimer timer("Write unique blocks...");
  if (device_rr_gsb.is_compressed() == false) {
    VTR_LOG_ERROR("unique_blocks are empty!");
    return CMD_EXEC_FATAL_ERROR;
  }
  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "<unique_blocks>"
     << "\n";

  for (size_t id = 0; id < device_rr_gsb.get_num_sb_unique_module(); ++id) {
    const auto unique_block_coord = device_rr_gsb.get_sb_unique_block_coord(id);
    const std::vector<vtr::Point<size_t>> instance_map =
      device_rr_gsb.get_sb_unique_block_instance_coord(unique_block_coord);
    int status_code =
      write_xml_atom_block(fp, instance_map, unique_block_coord, "sb");
    if (status_code != 0) {
      VTR_LOG_ERROR("write sb unique blocks into xml file failed!");
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  for (size_t id = 0; id < device_rr_gsb.get_num_cb_unique_module(CHANX);
       ++id) {
    const auto unique_block_coord =
      device_rr_gsb.get_cbx_unique_block_coord(id);
    const std::vector<vtr::Point<size_t>> instance_map =
      device_rr_gsb.get_cbx_unique_block_instance_coord(unique_block_coord);
    int status_code =
      write_xml_atom_block(fp, instance_map, unique_block_coord, "cbx");
    if (status_code != 0) {
      VTR_LOG_ERROR("write cbx unique blocks into xml file failed!");
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  for (size_t id = 0; id < device_rr_gsb.get_num_cb_unique_module(CHANY);
       ++id) {
    const auto unique_block_coord =
      device_rr_gsb.get_cby_unique_block_coord(id);
    const std::vector<vtr::Point<size_t>> instance_map =
      device_rr_gsb.get_cby_unique_block_instance_coord(unique_block_coord);
    int status_code =
      write_xml_atom_block(fp, instance_map, unique_block_coord, "cby");
    if (status_code != 0) {
      VTR_LOG_ERROR("write cby unique blocks into xml file failed!");
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  /* Finish writing the root node */
  fp << "</unique_blocks>"
     << "\n";

  /* Close the file stream */
  fp.close();
  if (verbose_output) {
    report_unique_module_status_write(device_rr_gsb, true);
  }

  return CMD_EXEC_SUCCESS;
}
}  // namespace openfpga
