#ifndef READ_XML_UNIQUE_BLOCKS_H
#define READ_XML_UNIQUE_BLOCKS_H

/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of unique routing blocks to the associated
 * data structures device_rr_gsb
 *******************************************************************/

#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "device_rr_gsb_utils.h"
#include "openfpga_digest.h"
#include "read_xml_unique_blocks.h"
#include "read_xml_util.h"
#include "rr_gsb.h"
#include "write_xml_utils.h"

/********************************************************************
 * Parse XML codes of a <instance> to an object of device_rr_gsb
 * instance is the mirror module of unique module.
 *******************************************************************/
vtr::Point<size_t> read_xml_unique_instance_info(
  pugi::xml_node& xml_instance_info, const pugiutil::loc_data& loc_data) {
  int instance_x = get_attribute(xml_instance_info, "x", loc_data).as_int();
  int instance_y = get_attribute(xml_instance_info, "y", loc_data).as_int();
  vtr::Point<size_t> instance_coordinate(instance_x, instance_y);
  return instance_coordinate;
}

template <class T>
void report_unique_module_status(T& openfpga_ctx, bool verbose_output) {
  /* Report the stats */
  VTR_LOGV(
    verbose_output,
    "Detected %lu unique X-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANX),
    find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANX),
    100. *
      ((float)find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(),
                                                CHANX) /
         (float)openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANX) -
       1.));

  VTR_LOGV(
    verbose_output,
    "Detected %lu unique Y-direction connection blocks from a total of %d "
    "(compression rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANY),
    find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(), CHANY),
    100. *
      ((float)find_device_rr_gsb_num_cb_modules(openfpga_ctx.device_rr_gsb(),
                                                CHANY) /
         (float)openfpga_ctx.device_rr_gsb().get_num_cb_unique_module(CHANY) -
       1.));

  VTR_LOGV(
    verbose_output,
    "Detected %lu unique switch blocks from a total of %d (compression "
    "rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_sb_unique_module(),
    find_device_rr_gsb_num_sb_modules(openfpga_ctx.device_rr_gsb(),
                                      g_vpr_ctx.device().rr_graph),
    100. * ((float)find_device_rr_gsb_num_sb_modules(
              openfpga_ctx.device_rr_gsb(), g_vpr_ctx.device().rr_graph) /
              (float)openfpga_ctx.device_rr_gsb().get_num_sb_unique_module() -
            1.));

  VTR_LOG(
    "Detected %lu unique general switch blocks from a total of %d "
    "(compression "
    "rate=%.2f%)\n",
    openfpga_ctx.device_rr_gsb().get_num_gsb_unique_module(),
    find_device_rr_gsb_num_gsb_modules(openfpga_ctx.device_rr_gsb(),
                                       g_vpr_ctx.device().rr_graph),
    100. * ((float)find_device_rr_gsb_num_gsb_modules(
              openfpga_ctx.device_rr_gsb(), g_vpr_ctx.device().rr_graph) /
              (float)openfpga_ctx.device_rr_gsb().get_num_gsb_unique_module() -
            1.));
}
/********************************************************************
 * Parse XML codes about <unique_blocks> to an object of device_rr_gsb
 *******************************************************************/
template <class T>
int read_xml_unique_blocks(T& openfpga_ctx, const char* file_name,
                           const char* file_type, bool verbose_output) {
  vtr::ScopedStartFinishTimer timer("Read unique blocks xml file");
  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;
  VTR_ASSERT(strcmp(file_type, "xml") == 0);
  try {
    loc_data = pugiutil::load_xml(doc, file_name);

    pugi::xml_node xml_root = get_single_child(doc, "unique_blocks", loc_data);

    /* get device_rr_gsb data type and initialize it*/
    openfpga::DeviceRRGSB& device_rr_gsb = openfpga_ctx.mutable_device_rr_gsb();
    /* clear unique modules */
    device_rr_gsb.clear_unique_modules();
     vtr::Point<size_t> grid_coord(g_vpr_ctx.device().grid.width() - 1,
                               g_vpr_ctx.device().grid.height() - 1);
    device_rr_gsb.reserve_unique_modules(grid_coord);
    /* load unique blocks xml file and set up device_rr_gdb */
    for (pugi::xml_node xml_block_info : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_block_info.name() == std::string("block")) {
        std::string type =
          get_attribute(xml_block_info, "type", loc_data).as_string();
        int block_x = get_attribute(xml_block_info, "x", loc_data).as_int();
        int block_y = get_attribute(xml_block_info, "y", loc_data).as_int();
        vtr::Point<size_t> block_coordinate(block_x, block_y);
        std::vector<vtr::Point<size_t>> instance_coords;
        for (pugi::xml_node xml_instance_info : xml_block_info.children()) {
          if (xml_instance_info.name() == std::string("instance")) {
            auto instance_coordinate =
              read_xml_unique_instance_info(xml_instance_info, loc_data);
            instance_coords.push_back(instance_coordinate);
          }
        }
        /* get block coordinate and instance coordinate, try to setup device rr
         * gsb */
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
          VTR_LOG_ERROR("Unexpected type!");
        }
      } else {
        bad_tag(xml_block_info, loc_data, xml_root, {"block"});
        return 1;
      }
    }
    device_rr_gsb.build_gsb_unique_module();
    device_rr_gsb.print_txt();
    if (verbose_output) {
      report_unique_module_status(openfpga_ctx, true);
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(file_name, e.line(), "%s", e.what());
  }
  
  return 0;
}

int write_xml_block(
  std::map<int, vtr::Point<size_t>>& id_unique_block_map,
  std::map<int, std::vector<vtr::Point<size_t>>>& id_instance_map,
  std::fstream& fp, std::string type) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
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
      }else{
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

  return 0;
}

template <class T>
int write_xml_unique_blocks(const T& openfpga_ctx, const char* fname,
                            const char* file_type, bool verbose_output) {
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
  openfpga_ctx.device_rr_gsb().get_id_unique_sb_block_map(id_unique_block_map);
  openfpga_ctx.device_rr_gsb().get_id_sb_instance_map(id_instance_map);
  err_code += write_xml_block(id_unique_block_map, id_instance_map, fp, "sb");

  id_unique_block_map.clear();
  id_instance_map.clear();
  openfpga_ctx.device_rr_gsb().get_id_unique_cbx_block_map(id_unique_block_map);
  openfpga_ctx.device_rr_gsb().get_id_cbx_instance_map(id_instance_map);
  err_code += write_xml_block(id_unique_block_map, id_instance_map, fp, "cbx");

  id_unique_block_map.clear();
  id_instance_map.clear();
  openfpga_ctx.device_rr_gsb().get_id_unique_cby_block_map(id_unique_block_map);
  openfpga_ctx.device_rr_gsb().get_id_cby_instance_map(id_instance_map);
  err_code += write_xml_block(id_unique_block_map, id_instance_map, fp, "cby");

  /* Finish writing the root node */
  fp << "</unique_blocks>"
     << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}

#endif
