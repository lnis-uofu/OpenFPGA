/***************************************************************************************
 * Output internal structure of DeviceRRGSB to XML format 
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"
#include "openfpga_digest.h"

#include "openfpga_naming.h"
#include "openfpga_rr_graph_utils.h"

#include "write_xml_device_rr_gsb.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Output internal structure (only the switch block part) of a RRGSB to XML format 
 ***************************************************************************************/
static 
void write_rr_switch_block_to_xml(const std::string fname_prefix,
                                  const RRGraph& rr_graph,
                                  const RRGSB& rr_gsb,
                                  const bool& verbose) {
  /* Prepare file name */
  std::string fname(fname_prefix);
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  fname += generate_switch_block_module_name(gsb_coordinate);
  fname += ".xml";

  VTR_LOGV(verbose,
           "Output internal structure of Switch Block to '%s'\n",
           fname.c_str());

  /* Create a file handler*/
  std::fstream fp;
  /* Open a file */
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(fname.c_str(), fp);

  /* Output location of the Switch Block */
  fp << "<rr_gsb x=\"" << rr_gsb.get_x() << "\" y=\"" << rr_gsb.get_y() << "\""
     << " num_sides=\"" << rr_gsb.get_num_sides() << "\">" << std::endl;

  /* Output each side */ 
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    SideManager gsb_side_manager(side);
    enum e_side gsb_side = gsb_side_manager.get_side();
   
    /* Output IPIN nodes */ 
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(gsb_side); ++inode) {
      const RRNodeId& cur_rr_node = rr_gsb.get_ipin_node(gsb_side, inode);
      /* General information of this IPIN */
      fp << "\t<" << rr_node_typename[rr_graph.node_type(cur_rr_node)]
         << " side=\"" << gsb_side_manager.to_string() 
         << "\" index=\"" << inode 
         << "\" mux_size=\"" << get_rr_graph_configurable_driver_nodes(rr_graph, cur_rr_node).size()
         << "\">" 
         << std::endl; 
      /* General information of each driving nodes */
      for (const RRNodeId& driver_node : get_rr_graph_configurable_driver_nodes(rr_graph, cur_rr_node)) {
        /* Skip OPINs: they should be in direct connections */
        if (OPIN == rr_graph.node_type(driver_node)) {
          continue;
        }

        enum e_side chan_side = rr_gsb.get_cb_chan_side(gsb_side);
        SideManager chan_side_manager(chan_side);

         /* For channel node, we do not know the node direction
         * But we are pretty sure it is either IN_PORT or OUT_PORT
         * So we just try and find what is valid
         */
        int driver_node_index = rr_gsb.get_chan_node_index(chan_side, driver_node);
        /* We must have a valide node index */
        VTR_ASSERT(-1 != driver_node_index);

        const RRSegmentId& des_segment_id = rr_gsb.get_chan_node_segment(chan_side, driver_node_index);

        fp << "\t\t<driver_node type=\"" << rr_node_typename[rr_graph.node_type(driver_node)]
           << "\" side=\"" << chan_side_manager.to_string() 
           << "\" index=\"" << driver_node_index 
           << "\" segment_id=\"" << size_t(des_segment_id)
           << "\"/>" 
           << std::endl; 
      }
      fp << "\t</" << rr_node_typename[rr_graph.node_type(cur_rr_node)] 
         << ">" 
         << std::endl; 
    }

    /* Output chan nodes */
    for (size_t inode = 0; inode < rr_gsb.get_chan_width(gsb_side); ++inode) {
      /* We only care OUT_PORT */
      if (OUT_PORT != rr_gsb.get_chan_node_direction(gsb_side, inode)) {
        continue;
      }
      /* Output drivers */
      const RRNodeId& cur_rr_node = rr_gsb.get_chan_node(gsb_side, inode);
      std::vector<RREdgeId> driver_rr_edges = rr_gsb.get_chan_node_in_edges(rr_graph, gsb_side, inode);

      /* Output node information: location, index, side */
      const RRSegmentId& src_segment_id = rr_gsb.get_chan_node_segment(gsb_side, inode);

      /* Check if this node is directly connected to the node on the opposite side */
      if (true == rr_gsb.is_sb_node_passing_wire(rr_graph, gsb_side, inode)) {
        driver_rr_edges.clear();
      }

      fp << "\t<" << rr_node_typename[rr_graph.node_type(cur_rr_node)]
         << " side=\"" << gsb_side_manager.to_string() 
         << "\" index=\"" << inode 
         << "\" segment_id=\"" << size_t(src_segment_id)
         << "\" mux_size=\"" << driver_rr_edges.size()
         << "\">" 
         << std::endl; 

      /* Direct connection: output the node on the opposite side */
      if (0 == driver_rr_edges.size()) {
        SideManager oppo_side =  gsb_side_manager.get_opposite();
        fp << "\t\t<driver_node type=\"" << rr_node_typename[rr_graph.node_type(cur_rr_node)]
           << "\" side=\"" << oppo_side.to_string() 
           << "\" index=\"" << rr_gsb.get_node_index(rr_graph, cur_rr_node, oppo_side.get_side(), IN_PORT) 
           << "\" segment_id=\"" << size_t(src_segment_id)
           << "\"/>" 
           << std::endl; 
      } else {
        for (const RREdgeId& driver_rr_edge : driver_rr_edges) {
          const RRNodeId& driver_rr_node = rr_graph.edge_src_node(driver_rr_edge);
          e_side driver_node_side = NUM_SIDES;
          int driver_node_index = -1;
          rr_gsb.get_node_side_and_index(rr_graph, driver_rr_node, IN_PORT, driver_node_side, driver_node_index);
          VTR_ASSERT(-1 != driver_node_index);
          SideManager driver_side(driver_node_side);

          if (OPIN == rr_graph.node_type(driver_rr_node)) {
            SideManager grid_side(rr_graph.node_side(driver_rr_node));
            fp << "\t\t<driver_node type=\"" << rr_node_typename[OPIN]
               << "\" side=\"" << driver_side.to_string() 
               << "\" index=\"" << driver_node_index  
               << "\" grid_side=\"" <<  grid_side.to_string() 
               <<"\"/>" 
               << std::endl; 
          } else {
            const RRSegmentId& des_segment_id = rr_gsb.get_chan_node_segment(driver_node_side, driver_node_index);
            fp << "\t\t<driver_node type=\"" << rr_node_typename[rr_graph.node_type(driver_rr_node)]
               << "\" side=\"" << driver_side.to_string() 
               << "\" index=\"" << driver_node_index 
               << "\" segment_id=\"" << size_t(des_segment_id)
               << "\"/>" 
               << std::endl; 
          }
        }  
      }
      fp << "\t</" << rr_node_typename[rr_graph.node_type(cur_rr_node)]
         << ">" 
         << std::endl; 
    }
  }

  fp << "</rr_gsb>" 
     << std::endl;

  /* close a file */
  fp.close();
}

/***************************************************************************************
 * Output internal structure (only the switch block part) of all the RRGSBs
 * in a DeviceRRGSB  to XML format 
 ***************************************************************************************/
void write_device_rr_gsb_to_xml(const char* sb_xml_dir, 
                                const RRGraph& rr_graph,
                                const DeviceRRGSB& device_rr_gsb,
                                const bool& verbose) {
  std::string xml_dir_name = format_dir_path(std::string(sb_xml_dir));

  /* Create directories */
  create_directory(xml_dir_name);

  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();

  size_t gsb_counter = 0;

  /* For each switch block, an XML file will be outputted */
  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      write_rr_switch_block_to_xml(xml_dir_name, rr_graph, rr_gsb, verbose);
      gsb_counter++;
    }
  }

  VTR_LOG("Output %lu XML files to directory '%s'\n",
          gsb_counter,
          xml_dir_name.c_str());
}

} /* end namespace openfpga */
