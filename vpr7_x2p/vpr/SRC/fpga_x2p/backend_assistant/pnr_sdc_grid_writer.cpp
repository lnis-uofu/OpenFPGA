/********************************************************************
 * This file includes functions that print SDC (Synopsys Design Constraint) 
 * files in physical design tools, i.e., Place & Route (PnR) tools
 * The SDC files are used to constrain the physical design for each grid
 * (CLBs, heterogeneous blocks etc.)
 *
 * Note that this is different from the SDC to constrain VPR Place&Route
 * engine! These SDCs are designed for PnR to generate FPGA layouts!!!
 *******************************************************************/
#include <ctime>
#include <fstream>

#include "vtr_assert.h"
#include "device_port.h"

#include "util.h"
#include "mux_utils.h"

#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "pnr_sdc_grid_writer.h"

#include "globals.h"

/********************************************************************
 * Print pin-to-pin timing constraints for a given interconnection
 * at an output port of a pb_graph node
 *******************************************************************/
static 
void print_pnr_sdc_constrain_pb_pin_interc_timing(std::fstream& fp,
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& parent_module,
                                                  const e_side& border_side,
                                                  t_pb_graph_pin* des_pb_graph_pin,
                                                  t_mode* physical_mode) {

  /* Validate file stream */ 
  check_file_handler(fp);

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Print SDC timing constraints
   */
  int fan_in = 0;
  t_interconnect* cur_interc = NULL;
  find_interc_fan_in_des_pb_graph_pin(des_pb_graph_pin, physical_mode, &cur_interc, &fan_in);
  if ((NULL == cur_interc) || (0 == fan_in)) { 
    /* No interconnection matched */
    return;
  }

  /* Print pin-to-pin SDC contraint here */ 
  /* For more than one mode defined, the direct interc has more than one input_edge ,
   * We need to find which edge is connected the pin we want
   */
  for (int iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
    if (cur_interc != des_pb_graph_pin->input_edges[iedge]->interconnect) {
      continue;
    }

    /* Source pin, node, pb_type*/
    t_pb_graph_pin* src_pb_graph_pin = des_pb_graph_pin->input_edges[iedge]->input_pins[0];
    t_pb_graph_node* src_pb_graph_node = src_pb_graph_pin->parent_node;
    /* Des pin, node, pb_type */
    t_pb_graph_node* des_pb_graph_node  = des_pb_graph_pin->parent_node;

    /* Find the src module in module manager */
    std::string src_module_name_prefix = generate_grid_block_prefix(std::string(GRID_MODULE_NAME_PREFIX), border_side);
    std::string src_module_name = generate_physical_block_module_name(src_module_name_prefix, src_pb_graph_pin->parent_node->pb_type);
    ModuleId src_module = module_manager.find_module(src_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(src_module));

    ModulePortId src_module_port_id = module_manager.find_module_port(src_module, generate_pb_type_port_name(src_pb_graph_pin->port));
    VTR_ASSERT(true == module_manager.valid_module_port_id(src_module, src_module_port_id));

    /* Generate the name of the des instance name 
     * If des module is not the parent module, it is a child module.
     * We should find the instance id 
     */
    std::string src_instance_name = src_module_name; 
    if (parent_module != src_module) {
      src_instance_name = module_manager.module_name(parent_module) + std::string("/"); 
      /* Instance id is actually the placement index */
      size_t instance_id = src_pb_graph_node->placement_index;
      if (true == module_manager.instance_name(parent_module, src_module, instance_id).empty()) {  
        src_instance_name += src_module_name; 
        src_instance_name += "_";
        src_instance_name += std::to_string(instance_id);
        src_instance_name += "_";
      } else {
        src_instance_name += module_manager.instance_name(parent_module, src_module, instance_id);  
      }
    }

    /* Generate src port information */
    BasicPort src_port = module_manager.module_port(src_module, src_module_port_id);
    src_port.set_width(src_pb_graph_pin->pin_number, src_pb_graph_pin->pin_number);

    /* Find the des module in module manager */
    std::string des_module_name_prefix = generate_grid_block_prefix(std::string(GRID_MODULE_NAME_PREFIX), border_side);
    std::string des_module_name = generate_physical_block_module_name(des_module_name_prefix, des_pb_graph_pin->parent_node->pb_type);
    ModuleId des_module = module_manager.find_module(des_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(des_module));
    ModulePortId des_module_port_id = module_manager.find_module_port(des_module, generate_pb_type_port_name(des_pb_graph_pin->port));
    VTR_ASSERT(true == module_manager.valid_module_port_id(des_module, des_module_port_id));

    /* Generate the name of the des instance name 
     * If des module is not the parent module, it is a child module.
     * We should find the instance id 
     */
    std::string des_instance_name = des_module_name; 
    if (parent_module != des_module) {
      des_instance_name = module_manager.module_name(parent_module) + std::string("/"); 
      /* Instance id is actually the placement index */
      size_t instance_id = des_pb_graph_node->placement_index;
      if (true == module_manager.instance_name(parent_module, des_module, instance_id).empty()) {  
        des_instance_name += des_module_name; 
        des_instance_name += "_";
        des_instance_name += std::to_string(instance_id);
        des_instance_name += "_";
      } else {
        des_instance_name += module_manager.instance_name(parent_module, des_module, instance_id);  
      }
    }

    /* Generate des port information */
    BasicPort des_port = module_manager.module_port(des_module, des_module_port_id);
    des_port.set_width(des_pb_graph_pin->pin_number, des_pb_graph_pin->pin_number);

    /* Print a SDC timing constraint */
    print_pnr_sdc_constrain_max_delay(fp, 
                                      src_instance_name, 
                                      generate_sdc_port(src_port),
                                      des_instance_name, 
                                      generate_sdc_port(des_port),
                                      des_pb_graph_pin->input_edges[iedge]->delay_max);
  }
}

/********************************************************************
 * Print port-to-port timing constraints which source from
 * an output port of a pb_graph node
 *******************************************************************/
static 
void print_pnr_sdc_constrain_pb_interc_timing(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& parent_module,
                                              const e_side& border_side,
                                              t_pb_graph_node* des_pb_graph_node,
                                              const e_spice_pb_port_type& pb_port_type,
                                              t_mode* physical_mode) {
  /* Validate file stream */ 
  check_file_handler(fp);

  switch (pb_port_type) {
  case SPICE_PB_PORT_INPUT: {
    for (int iport = 0; iport < des_pb_graph_node->num_input_ports; ++iport) {
      for (int ipin = 0; ipin < des_pb_graph_node->num_input_pins[iport]; ++ipin) {
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        print_pnr_sdc_constrain_pb_pin_interc_timing(fp, module_manager, parent_module, border_side, 
                                     &(des_pb_graph_node->input_pins[iport][ipin]),
                                     physical_mode);
      }
    }
    break;
  }
  case SPICE_PB_PORT_OUTPUT: {
    for (int iport = 0; iport < des_pb_graph_node->num_output_ports; ++iport) {
      for (int ipin = 0; ipin < des_pb_graph_node->num_output_pins[iport]; ++ipin) {
        print_pnr_sdc_constrain_pb_pin_interc_timing(fp, module_manager, parent_module, border_side, 
                                     &(des_pb_graph_node->output_pins[iport][ipin]),
                                     physical_mode);
      }
    }
    break;
  }
  case SPICE_PB_PORT_CLOCK: {
    /* Do NOT constrain clock here, it should be handled by Clock Tree Synthesis */
    break;
  }
  default:
   vpr_printf(TIO_MESSAGE_ERROR,
              "(File:%s, [LINE%d]) Invalid pb port type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/********************************************************************
 * This function will generate a SDC file for each pb_type,
 * constraining the pin-to-pin timing between 
 * 1. input port of parent_pb_graph_node and input port of child_pb_graph_nodes
 * 2. output port of parent_pb_graph_node and output port of child_pb_graph_nodes
 * 3. output port of child_pb_graph_node and input port of child_pb_graph_nodes
 *******************************************************************/
static 
void print_pnr_sdc_constrain_pb_graph_node_timing(const std::string& sdc_dir,
                                                  const ModuleManager& module_manager,
                                                  t_pb_graph_node* parent_pb_graph_node,
                                                  const int& physical_mode_index,
                                                  const e_side& border_side) {

  /* Get the pb_type definition related to the node */
  t_pb_type* physical_pb_type = parent_pb_graph_node->pb_type; 
  std::string pb_module_name_prefix = generate_grid_block_prefix(std::string(GRID_MODULE_NAME_PREFIX), border_side);
  std::string pb_module_name = generate_physical_block_module_name(pb_module_name_prefix, physical_pb_type);

  /* Find the pb module in module manager */
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Create the file name for SDC */
  std::string sdc_fname(sdc_dir + pb_module_name + std::string(SDC_FILE_NAME_POSTFIX));

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Timing constraints for Grid " + pb_module_name + " in PnR"));

  t_mode* physical_mode = &(parent_pb_graph_node->pb_type->modes[physical_mode_index]);

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  print_pnr_sdc_constrain_pb_interc_timing(fp, module_manager, pb_module, border_side,
                                           parent_pb_graph_node, 
                                           SPICE_PB_PORT_OUTPUT,
                                           physical_mode);
  
  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
      t_pb_graph_node* child_pb_graph_node = &(parent_pb_graph_node->child_pb_graph_nodes[physical_mode_index][ipb][jpb]);
      /* For each child_pb_graph_node input pins*/
      print_pnr_sdc_constrain_pb_interc_timing(fp, module_manager, pb_module, border_side,
                                               child_pb_graph_node, 
                                               SPICE_PB_PORT_INPUT,
                                               physical_mode);
      /* Do NOT constrain clock here, it should be handled by Clock Tree Synthesis */
    }
  }
  
  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Recursively print SDC timing constraints for a pb_type
 * This function will generate a SDC file for each pb_type,
 * constraining the pin-to-pin timing
 *******************************************************************/
static 
void rec_print_pnr_sdc_constrain_pb_graph_timing(const std::string& sdc_dir,
                                                 const ModuleManager& module_manager,
                                                 t_pb_graph_node* parent_pb_graph_node,
                                                 const e_side& border_side) {
  /* Validate pb_graph node */
  if (NULL == parent_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, [LINE%d]) Invalid parent_pb_graph_node.\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Get the pb_type */
  t_pb_type* parent_pb_type = parent_pb_graph_node->pb_type;

  /* No need to constrain the primitive node */
  if (TRUE == is_primitive_pb_type(parent_pb_type)) {
    return;
  }

  /* Note we only go through the graph through the physical modes. 
   * which we build the modules 
   */
  int physical_mode_index = find_pb_type_physical_mode_index((*parent_pb_type));  

  /* Write a SDC file for this pb_type */
  print_pnr_sdc_constrain_pb_graph_node_timing(sdc_dir, module_manager,
                                               parent_pb_graph_node, physical_mode_index,
                                               border_side);

  /* Go recursively to the lower level in the pb_graph
   * Note that we assume a full hierarchical P&R, we will only visit pb_graph_node of unique pb_type 
   */
  for (int ipb = 0; ipb < parent_pb_type->modes[physical_mode_index].num_pb_type_children; ++ipb) {
    rec_print_pnr_sdc_constrain_pb_graph_timing(sdc_dir, module_manager, 
                                                &(parent_pb_graph_node->child_pb_graph_nodes[physical_mode_index][ipb][0]),
                                                border_side);
  }
}

/********************************************************************
 * Top-level function to print timing constraints for pb_types
 *******************************************************************/
void print_pnr_sdc_constrain_grid_timing(const std::string& sdc_dir,
                                         const ModuleManager& module_manager) {

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining grid timing for P&R flow...");

  /* Start time count */
  clock_t t_start = clock();

  for (int itype = 0; itype < num_types; itype++) {
    /* Bypass EMPTY types */
    if (EMPTY_TYPE == &type_descriptors[itype]) {
      continue;
    }

    /* For IO_TYPE, we have four types of I/Os */
    if (IO_TYPE == &type_descriptors[itype]) {
      /* Special for I/O block, generate one module for each border side */
      for (int iside = 0; iside < NUM_SIDES; iside++) {
        Side side_manager(iside);
        rec_print_pnr_sdc_constrain_pb_graph_timing(sdc_dir, module_manager, 
                                                    type_descriptors[itype].pb_graph_head,
                                                    side_manager.get_side());
      }
    } else if (FILL_TYPE == &type_descriptors[itype]) {
      /* For CLB */
      rec_print_pnr_sdc_constrain_pb_graph_timing(sdc_dir, module_manager,
                                                  type_descriptors[itype].pb_graph_head,
                                                  NUM_SIDES);
    } else {
      /* For heterogenenous blocks */
      rec_print_pnr_sdc_constrain_pb_graph_timing(sdc_dir, module_manager,
                                                  type_descriptors[itype].pb_graph_head,
                                                  NUM_SIDES);
    }
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

