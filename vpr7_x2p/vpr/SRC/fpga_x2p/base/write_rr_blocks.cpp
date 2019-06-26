/* 
 * Output internal structure of classes defined in rr_blocks.h to XML format 
 */
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>

#include "rr_blocks.h"
#include "rr_blocks_naming.h"

#include "write_rr_blocks.h"

void write_rr_switch_block_to_xml(std::string fname_prefix, RRGSB& rr_gsb) {
  /* Prepare file name */
  std::string fname(fname_prefix);
  fname += rr_gsb.gen_gsb_verilog_module_name();
  fname += ".xml";

  vpr_printf(TIO_MESSAGE_INFO, "Output SB XML: %s\r", fname.c_str());

  /* Create a file handler*/
  std::fstream fp;
  /* Open a file */
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  /* Output location of the Switch Block */
  fp << "<rr_gsb x=\"" << rr_gsb.get_x() << "\" y=\"" << rr_gsb.get_y() << "\""
     << " num_sides=\"" << rr_gsb.get_num_sides() << "\">" << std::endl;

  /* Output each side */ 
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side gsb_side_manager(side);
    enum e_side gsb_side = gsb_side_manager.get_side();
   
    /* Output IPIN nodes */ 
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(gsb_side); ++inode) {
      t_rr_node* cur_rr_node = rr_gsb.get_ipin_node(gsb_side, inode);
      /* General information of this IPIN */
      fp << "\t<" << rr_node_typename[cur_rr_node->type]
         << " side=\"" << gsb_side_manager.to_string() 
         << "\" index=\"" << inode 
         << "\" mux_size=\"" << cur_rr_node->num_drive_rr_nodes
         << "\">" 
         << std::endl; 
      /* General information of each driving nodes */
      for (int jnode = 0; jnode < cur_rr_node->num_drive_rr_nodes; ++jnode) {
        enum e_side chan_side = rr_gsb.get_cb_chan_side(gsb_side);
        Side chan_side_manager(chan_side);

         /* For channel node, we do not know the node direction
         * But we are pretty sure it is either IN_PORT or OUT_PORT
         * So we just try and find what is valid
         */
        int drive_node_index = rr_gsb.get_chan_node_index(chan_side, cur_rr_node->drive_rr_nodes[jnode]);
        /* We must have a valide node index */
        assert (-1 != drive_node_index);

        size_t des_segment_id = rr_gsb.get_chan_node_segment(chan_side, drive_node_index);

        fp << "\t\t<driver_node type=\"" << rr_node_typename[cur_rr_node->drive_rr_nodes[jnode]->type]
           << "\" side=\"" << chan_side_manager.to_string() 
           << "\" index=\"" << drive_node_index 
           << "\" segment_id=\"" << des_segment_id 
           << "\"/>" 
           << std::endl; 
      }
      fp << "\t</" << rr_node_typename[cur_rr_node->type] 
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
      size_t num_drive_rr_nodes = 0;
      t_rr_node**  drive_rr_nodes = 0;
      t_rr_node* cur_rr_node = rr_gsb.get_chan_node(gsb_side, inode);

      /* Output node information: location, index, side */
      size_t src_segment_id = rr_gsb.get_chan_node_segment(gsb_side, inode);

      /* Check if this node is directly connected to the node on the opposite side */
      if (true == rr_gsb.is_sb_node_imply_short_connection(cur_rr_node)) {
        /* Double check if the interc lies inside a channel wire, that is interc between segments */
        assert(true == rr_gsb.is_sb_node_exist_opposite_side(cur_rr_node, gsb_side));
        num_drive_rr_nodes = 0;
        drive_rr_nodes = NULL;
      } else {
        num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
        drive_rr_nodes = cur_rr_node->drive_rr_nodes;
      }

      fp << "\t<" << rr_node_typename[cur_rr_node->type]
         << " side=\"" << gsb_side_manager.to_string() 
         << "\" index=\"" << inode 
         << "\" segment_id=\"" << src_segment_id 
         << "\" mux_size=\"" << num_drive_rr_nodes
         << "\">" 
         << std::endl; 

      /* Direct connection: output the node on the opposite side */
      if (0 == num_drive_rr_nodes) {
        Side oppo_side =  gsb_side_manager.get_opposite();
        fp << "\t\t<driver_node type=\"" << rr_node_typename[cur_rr_node->type]
           << "\" side=\"" << oppo_side.to_string() 
           << "\" index=\"" << rr_gsb.get_node_index(cur_rr_node, oppo_side.get_side(), IN_PORT) 
           << "\" segment_id=\"" << src_segment_id 
           << "\"/>" 
           << std::endl; 
      } else {
        for (size_t jnode = 0; jnode < num_drive_rr_nodes; ++jnode) {
          enum e_side drive_node_side = NUM_SIDES;
          int drive_node_index = -1;
          rr_gsb.get_node_side_and_index(drive_rr_nodes[jnode], IN_PORT, &drive_node_side, &drive_node_index);
          if (-1 == drive_node_index) 
          assert(-1 != drive_node_index);
          Side drive_side(drive_node_side);

          if (OPIN == drive_rr_nodes[jnode]->type) {
            Side grid_side(rr_gsb.get_opin_node_grid_side(drive_node_side, drive_node_index));
            fp << "\t\t<driver_node type=\"" << rr_node_typename[OPIN]
               << "\" side=\"" << drive_side.to_string() 
               << "\" index=\"" << drive_node_index  
               << "\" grid_side=\"" <<  grid_side.to_string() 
               <<"\"/>" 
               << std::endl; 
          } else {
            size_t des_segment_id = rr_gsb.get_chan_node_segment(drive_node_side, drive_node_index);
            fp << "\t\t<driver_node type=\"" << rr_node_typename[drive_rr_nodes[jnode]->type]
               << "\" side=\"" << drive_side.to_string() 
               << "\" index=\"" << drive_node_index 
               << "\" segment_id=\"" << des_segment_id 
               << "\"/>" 
               << std::endl; 
          }
        }  
      }
      fp << "\t</" << convert_chan_type_to_string(cur_rr_node->type) 
         << ">" 
         << std::endl; 
    }
  }

  fp << "</rr_gsb>" 
     << std::endl;

  /* close a file */
  fp.close();

  return;
}

/* Output each rr_switch_block to a XML file */
void write_device_rr_gsb_to_xml(char* sb_xml_dir, 
                                DeviceRRGSB& LL_device_rr_gsb) {
  std::string fname_prefix(sb_xml_dir);
  /* Add slash if needed */
  if ('/' != fname_prefix.back()) {
    fname_prefix += '/';
  }

  DeviceCoordinator sb_range = LL_device_rr_gsb.get_gsb_range();

  /* For each switch block, an XML file will be outputted */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      RRGSB rr_gsb = LL_device_rr_gsb.get_gsb(ix, iy);
      write_rr_switch_block_to_xml(fname_prefix, rr_gsb);
    }
  }

  return;
}

