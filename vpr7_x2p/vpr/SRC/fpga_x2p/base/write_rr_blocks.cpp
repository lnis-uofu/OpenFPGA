/* 
 * Output internal structure of classes defined in rr_blocks.h to XML format 
 */
#include <fstream>
#include <string.h>
#include <assert.h>

#include "rr_blocks.h"
#include "write_rr_blocks.h"

#include "fpga_x2p_utils.h"

void write_rr_switch_block_to_xml(std::string fname_prefix, RRSwitchBlock& rr_sb) {
  /* Create a file handler*/
  std::fstream fp;
  std::string fname = fname_prefix;
  fname += rr_sb.gen_verilog_instance_name();
  fname += ".xml";

  printf("Output SB XML: %s\n", fname.c_str());

  /* Open a file */
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  /* Output location of the Switch Block */
  fp << "<rr_sb x=\"" << rr_sb.get_x() << "\" y=\"" << rr_sb.get_y() << "\""
     << " num_sides=\"" << rr_sb.get_num_sides() << "\">" << std::endl;

  /* Output each side */ 
  for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
    Side side_manager(side);
    /* Output chan nodes */
    for (size_t inode = 0; inode < rr_sb.get_chan_width(side_manager.get_side()); ++inode) {
      /* We only care OUT_PORT */
      if (OUT_PORT != rr_sb.get_chan_node_direction(side_manager.get_side(), inode)) {
        continue;
      }
      /* Output drivers */
      size_t num_drive_rr_nodes = 0;
      t_rr_node**  drive_rr_nodes = 0;
      t_rr_node* cur_rr_node = rr_sb.get_chan_node(side_manager.get_side(), inode);

      /* Output node information: location, index, side */
      fp << "\t<" << convert_chan_type_to_string(cur_rr_node->type) 
         << " side=\"" << side_manager.to_string() 
         << "\" index=\"" << inode << "\">" 
         << std::endl; 

      /* Check if this node is directly connected to the node on the opposite side */
      if (true == rr_sb.is_node_imply_short_connection(cur_rr_node)) {
        /* Double check if the interc lies inside a channel wire, that is interc between segments */
        assert(true == rr_sb.is_node_exist_opposite_side(cur_rr_node, side_manager.get_side()));
        num_drive_rr_nodes = 0;
        drive_rr_nodes = NULL;
      } else {
        num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
        drive_rr_nodes = cur_rr_node->drive_rr_nodes;
      }
      /* Direct connection: output the node on the opposite side */
      if (0 == num_drive_rr_nodes) {
        Side oppo_side =  side_manager.get_opposite();
        fp << "\t\t<drive_node type=\"" << convert_chan_type_to_string(cur_rr_node->type) 
           << "\" side=\"" << oppo_side.to_string() 
           << "\" index=\"" << rr_sb.get_node_index(cur_rr_node, oppo_side.get_side(), IN_PORT) << "\"/>" 
           << std::endl; 
      } else {
        for (size_t jnode = 0; jnode < num_drive_rr_nodes; ++jnode) {
          enum e_side drive_node_side = NUM_SIDES;
          int drive_node_index = -1;
          rr_sb.get_node_side_and_index(drive_rr_nodes[jnode], IN_PORT, &drive_node_side, &drive_node_index);
          Side drive_side(drive_node_side);
          std::string node_type_str;
          if (OPIN == drive_rr_nodes[jnode]->type) {
            node_type_str = "opin";
          } else {
            node_type_str = convert_chan_type_to_string(drive_rr_nodes[jnode]->type);
          }
          fp << "\t\t<drive_node type=\"" << node_type_str 
             << "\" side=\"" << drive_side.to_string() 
             << "\" index=\"" << drive_node_index << "\"/>" 
             << std::endl; 
        }  
      }
      fp << "\t</" << convert_chan_type_to_string(cur_rr_node->type) << ">" << std::endl; 
    }
  }

  fp << "</rr_sb>" << std::endl;

  /* close a file */
  fp.close();

  return;
}

/* Output each rr_switch_block to a XML file */
void write_device_rr_switch_block_to_xml(DeviceRRSwitchBlock& LL_device_rr_switch_block) {
  std::string fname_prefix("/var/tmp/xtang/sb_xml/");
  DeviceCoordinator sb_range = LL_device_rr_switch_block.get_switch_block_range();

  /* For each switch block, an XML file will be outputted */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      RRSwitchBlock rr_sb = LL_device_rr_switch_block.get_switch_block(ix, iy);
      write_rr_switch_block_to_xml(fname_prefix, rr_sb);
    }
  }

  return;
}

