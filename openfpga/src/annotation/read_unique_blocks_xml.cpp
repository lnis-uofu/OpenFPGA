
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
#include "globals.h"
#include "mmap_file.h"
#include "openfpga_digest.h"
#include "read_unique_blocks_xml.h"
#include "read_xml_util.h"
#include "rr_gsb.h"
#include "write_xml_utils.h"

/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- reads an XML file of unique blocks to the associated
 * data structures: device_rr_gsb
 *******************************************************************/
namespace openfpga {
/*read the instances' coordinate of a unique block from a xml file*/
std::vector<vtr::Point<size_t>> read_xml_unique_instance_coords(
  const pugi::xml_node& xml_block_info, const pugiutil::loc_data& loc_data) {
  std::vector<vtr::Point<size_t>> instance_coords;
  for (pugi::xml_node xml_instance_info : xml_block_info.children()) {
    if (xml_instance_info.name() == std::string("instance")) {
      int instance_x = get_attribute(xml_instance_info, "x", loc_data).as_int();
      int instance_y = get_attribute(xml_instance_info, "y", loc_data).as_int();
      vtr::Point<size_t> instance_coordinate(instance_x, instance_y);
      instance_coords.push_back(instance_coordinate);
    }
  }
  return instance_coords;
}

/*read the unique block coordinate from a xml file */
vtr::Point<size_t> read_xml_unique_block_coord(
  const pugi::xml_node& xml_block_info, const pugiutil::loc_data& loc_data) {
  int block_x = get_attribute(xml_block_info, "x", loc_data).as_int();
  int block_y = get_attribute(xml_block_info, "y", loc_data).as_int();
  vtr::Point<size_t> block_coordinate(block_x, block_y);
  return block_coordinate;
}

/*report information of read unique blocks*/
void report_unique_module_status_read(const DeviceRRGSB& device_rr_gsb,
                                      bool verbose_output) {
  /* Report the stats */
  VTR_LOGV(
    verbose_output,
    "Read %lu unique X-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANX),
    find_device_rr_gsb_num_cb_modules(device_rr_gsb, e_rr_type::CHANX),
    100. * ((float)find_device_rr_gsb_num_cb_modules(device_rr_gsb,
                                                     e_rr_type::CHANX) /
              (float)device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANX) -
            1.));
  VTR_LOGV(
    verbose_output,
    "Read %lu unique Y-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANY),
    find_device_rr_gsb_num_cb_modules(device_rr_gsb, e_rr_type::CHANY),
    100. * ((float)find_device_rr_gsb_num_cb_modules(device_rr_gsb,
                                                     e_rr_type::CHANY) /
              (float)device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANY) -
            1.));

  VTR_LOGV(verbose_output,
           "Read %lu unique switch blocks from a total of %d (compression "
           "rate=%.2f%)\n",
           device_rr_gsb.get_num_sb_unique_module(),
           find_device_rr_gsb_num_sb_modules(device_rr_gsb,
                                             g_vpr_ctx.device().rr_graph),
           100. * ((float)find_device_rr_gsb_num_sb_modules(
                     device_rr_gsb, g_vpr_ctx.device().rr_graph) /
                     (float)device_rr_gsb.get_num_sb_unique_module() -
                   1.));

  VTR_LOG(
    "Read %lu unique general switch blocks from a total of %d "
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

/*Parse XML codes about <unique_blocks> to an object of device_rr_gsb*/
int read_xml_unique_blocks(DeviceRRGSB& device_rr_gsb, const char* file_name,
                           bool verbose_output) {
  vtr::ScopedStartFinishTimer timer("Read unique blocks xml file");
  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;
  try {
    loc_data = pugiutil::load_xml(doc, file_name);

    pugi::xml_node xml_root = get_single_child(doc, "unique_blocks", loc_data);
    /* clear unique modules & reserve memory to relavant vectors */
    device_rr_gsb.clear_unique_modules();
    device_rr_gsb.reserve_unique_modules();

    /* load unique blocks xml file and set up device_rr_gdb */
    for (pugi::xml_node xml_block_info : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_block_info.name() == std::string("block")) {
        std::string type =
          get_attribute(xml_block_info, "type", loc_data).as_string();
        vtr::Point<size_t> block_coordinate =
          read_xml_unique_block_coord(xml_block_info, loc_data);
        std::vector<vtr::Point<size_t>> instance_coords =
          read_xml_unique_instance_coords(xml_block_info, loc_data);

        /* get block coordinate and instance coordinate, try to setup
         * device_rr_gsb */
        if (type == "sb") {
          device_rr_gsb.preload_unique_sb_module(block_coordinate,
                                                 instance_coords);
        } else if (type == "cby") {
          device_rr_gsb.preload_unique_cby_module(block_coordinate,
                                                  instance_coords);
        } else if (type == "cbx") {
          device_rr_gsb.preload_unique_cbx_module(block_coordinate,
                                                  instance_coords);
        } else {
          archfpga_throw(loc_data.filename_c_str(),
                         loc_data.line(xml_block_info),
                         "Invalid block type '%s'\n", type.c_str());
        }
      } else {
        bad_tag(xml_block_info, loc_data, xml_root, {"block"});
      }
    }
    /* As preloading gsb hasn't been developed, we should build gsb using the
     * preloaded cbs and sbs*/
    device_rr_gsb.build_gsb_unique_module();
    if (verbose_output) {
      report_unique_module_status_read(device_rr_gsb, true);
    }
    return CMD_EXEC_SUCCESS;
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(file_name, e.line(), "%s", e.what());
  }
}
}  // namespace openfpga
