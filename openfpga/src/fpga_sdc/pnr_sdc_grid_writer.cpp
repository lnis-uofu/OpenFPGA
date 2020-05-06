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

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_scale.h"
#include "openfpga_port.h"
#include "openfpga_digest.h"
#include "openfpga_side_manager.h"

#include "openfpga_interconnect_types.h"
#include "vpr_utils.h"
#include "mux_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "pb_type_utils.h"
#include "pb_graph_utils.h"
#include "openfpga_physical_tile_utils.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "pnr_sdc_grid_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print pin-to-pin timing constraints for a given interconnection
 * at an output port of a pb_graph node
 *******************************************************************/
static 
void print_pnr_sdc_constrain_pb_pin_interc_timing(std::fstream& fp,
                                                  const float& time_unit,
                                                  const bool& hierarchical,
                                                  const std::string& module_path,
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& parent_module,
                                                  t_pb_graph_pin* des_pb_graph_pin,
                                                  t_mode* physical_mode,
                                                  const bool& constrain_zero_delay_paths) {

  /* Validate file stream */ 
  valid_file_stream(fp);

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Print SDC timing constraints
   */
  t_interconnect* cur_interc = pb_graph_pin_interc(des_pb_graph_pin, physical_mode);
  size_t fan_in = pb_graph_pin_inputs(des_pb_graph_pin, cur_interc).size();
  if ((nullptr == cur_interc) || (0 == fan_in)) { 
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
    std::string src_module_name = generate_physical_block_module_name(src_pb_graph_pin->parent_node->pb_type);
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
    std::string des_module_name = generate_physical_block_module_name(des_pb_graph_pin->parent_node->pb_type);
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

    /* If we have a zero-delay path to contrain, we will skip unless users want so */
    if ( (false == constrain_zero_delay_paths)
      && (0. == des_pb_graph_pin->input_edges[iedge]->delay_max) ) {
      continue;
    }

    /* Give full path if hierarchical is not enabled */
    std::string src_module_path = src_instance_name;
    if (false == hierarchical) {
      src_module_path = module_path + src_instance_name;
    }

    std::string des_module_path = des_instance_name;
    if (false == hierarchical) {
      src_module_path = module_path + des_instance_name;
    }

    /* Print a SDC timing constraint */
    print_pnr_sdc_constrain_max_delay(fp, 
                                      src_module_path, 
                                      generate_sdc_port(src_port),
                                      des_module_path, 
                                      generate_sdc_port(des_port),
                                      des_pb_graph_pin->input_edges[iedge]->delay_max / time_unit);
  }
}

/********************************************************************
 * Print port-to-port timing constraints which source from
 * an output port of a pb_graph node
 *******************************************************************/
static 
void print_pnr_sdc_constrain_pb_interc_timing(std::fstream& fp,
                                              const float& time_unit,
                                              const bool& hierarchical,
                                              const std::string& module_path,
                                              const ModuleManager& module_manager,
                                              const ModuleId& parent_module,
                                              t_pb_graph_node* des_pb_graph_node,
                                              const e_circuit_pb_port_type& pb_port_type,
                                              t_mode* physical_mode,
                                              const bool& constrain_zero_delay_paths) {
  /* Validate file stream */ 
  valid_file_stream(fp);

  switch (pb_port_type) {
  case CIRCUIT_PB_PORT_INPUT: {
    for (int iport = 0; iport < des_pb_graph_node->num_input_ports; ++iport) {
      for (int ipin = 0; ipin < des_pb_graph_node->num_input_pins[iport]; ++ipin) {
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        print_pnr_sdc_constrain_pb_pin_interc_timing(fp,
                                                     time_unit, 
                                                     hierarchical, 
                                                     module_path, 
                                                     module_manager, parent_module, 
                                                     &(des_pb_graph_node->input_pins[iport][ipin]),
                                                     physical_mode,
                                                     constrain_zero_delay_paths);
      }
    }
    break;
  }
  case CIRCUIT_PB_PORT_OUTPUT: {
    for (int iport = 0; iport < des_pb_graph_node->num_output_ports; ++iport) {
      for (int ipin = 0; ipin < des_pb_graph_node->num_output_pins[iport]; ++ipin) {
        print_pnr_sdc_constrain_pb_pin_interc_timing(fp,
                                                     time_unit, 
                                                     hierarchical, 
                                                     module_path, 
                                                     module_manager, parent_module, 
                                                     &(des_pb_graph_node->output_pins[iport][ipin]),
                                                     physical_mode,
                                                     constrain_zero_delay_paths);
      }
    }
    break;
  }
  case CIRCUIT_PB_PORT_CLOCK: {
    /* Do NOT constrain clock here, it should be handled by Clock Tree Synthesis */
    break;
  }
  default:
   VTR_LOGF_ERROR(__FILE__, __LINE__,
                  "Invalid pb port type!\n");
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
                                                  const float& time_unit,
                                                  const bool& hierarchical,
                                                  const std::string& module_path,
                                                  const ModuleManager& module_manager,
                                                  t_pb_graph_node* parent_pb_graph_node,
                                                  t_mode* physical_mode,
                                                  const bool& constrain_zero_delay_paths) {

  /* Get the pb_type definition related to the node */
  t_pb_type* physical_pb_type = parent_pb_graph_node->pb_type; 
  std::string pb_module_name = generate_physical_block_module_name(physical_pb_type);

  /* Find the pb module in module manager */
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Create the file name for SDC */
  std::string sdc_fname(sdc_dir + pb_module_name + std::string(SDC_FILE_NAME_POSTFIX));

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Timing constraints for Grid " + pb_module_name + " in PnR"));

  /* Print time unit for the SDC file */
  print_sdc_timescale(fp, time_unit_to_string(time_unit));

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  print_pnr_sdc_constrain_pb_interc_timing(fp,
                                           time_unit,
                                           hierarchical,
                                           module_path,
                                           module_manager, pb_module,
                                           parent_pb_graph_node, 
                                           CIRCUIT_PB_PORT_OUTPUT,
                                           physical_mode,
                                           constrain_zero_delay_paths);
  
  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
      t_pb_graph_node* child_pb_graph_node = &(parent_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]);
      /* For each child_pb_graph_node input pins*/
      print_pnr_sdc_constrain_pb_interc_timing(fp,
                                               time_unit,
                                               hierarchical,
                                               module_path,
                                               module_manager, pb_module,
                                               child_pb_graph_node, 
                                               CIRCUIT_PB_PORT_INPUT,
                                               physical_mode,
                                               constrain_zero_delay_paths);
      /* Do NOT constrain clock here, it should be handled by Clock Tree Synthesis */
    }
  }
  
  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Print SDC timing constraints for a primitive pb_type
 * This function will generate SDC to constrain pin-to-pin timing
 * if it is defined in XML
 *
 * This is designed for LUT, adder or other hard IPs
 * When PnR the modules, we want to minimize the interconnect delay
 *******************************************************************/
static 
void print_pnr_sdc_constrain_primitive_pb_graph_node(const std::string& sdc_dir,
                                                     const float& time_unit,
                                                     const bool& hierarchical,
                                                     const std::string& module_path,
                                                     const ModuleManager& module_manager,
                                                     t_pb_graph_node* primitive_pb_graph_node,
                                                     const bool& constrain_zero_delay_paths) {
  /* Validate pb_graph node */
  if (nullptr == primitive_pb_graph_node) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid primitive_pb_graph_node.\n");
    exit(1);
  }

  t_pb_graph_node* logical_primitive_pb_graph_node = primitive_pb_graph_node;

  /* Get the pb_type where the timing annotations are stored
   * Note that some primitive pb_type has child modes
   *   - Look-Up Table
   *   - Memory
   * For those pb_type, timing annotations are stored in the child pb_type
   */
  t_pb_type* primitive_pb_type = primitive_pb_graph_node->pb_type;
  if (LUT_CLASS == primitive_pb_type->class_type) {
    VTR_ASSERT(VPR_PB_TYPE_LUT_MODE < primitive_pb_type->num_modes);
    VTR_ASSERT(1 == primitive_pb_type->modes[VPR_PB_TYPE_LUT_MODE].num_pb_type_children);
    primitive_pb_type = &(primitive_pb_type->modes[VPR_PB_TYPE_LUT_MODE].pb_type_children[0]);
    logical_primitive_pb_graph_node = primitive_pb_graph_node->child_pb_graph_nodes[VPR_PB_TYPE_LUT_MODE][0];
    VTR_ASSERT(nullptr != logical_primitive_pb_graph_node);
  } else if (MEMORY_CLASS == primitive_pb_type->class_type) {
    VTR_ASSERT(1 == primitive_pb_type->num_modes);
    VTR_ASSERT(1 == primitive_pb_type->modes[0].num_pb_type_children);
    primitive_pb_type = &(primitive_pb_type->modes[0].pb_type_children[0]);
    logical_primitive_pb_graph_node = primitive_pb_graph_node->child_pb_graph_nodes[0][0];
  }
  VTR_ASSERT(nullptr != primitive_pb_type);
  VTR_ASSERT(nullptr != logical_primitive_pb_graph_node);

  /* We can directly return if there is no timing annotation defined */
  if (0 == primitive_pb_type->num_annotations) {
    return;
  } 

  /* Get the pb_type definition related to the node */
  t_pb_type* physical_pb_type = primitive_pb_graph_node->pb_type; 
  std::string pb_module_name = generate_physical_block_module_name(physical_pb_type);

  /* Find the pb module in module manager */
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Create the file name for SDC */
  std::string sdc_fname(sdc_dir + pb_module_name + std::string(SDC_FILE_NAME_POSTFIX));

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Timing constraints for Grid " + pb_module_name + " in PnR"));

  /* Print time unit for the SDC file */
  print_sdc_timescale(fp, time_unit_to_string(time_unit));

  /* We traverse the pb_graph pins where we can find pin-to-pin timing annotation
   * We walk through output pins here, build timing constraints by pair each output to input
   * Clock pins are not walked through because they will be handled by clock tree synthesis
   */
  for (int iport = 0; iport < logical_primitive_pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < logical_primitive_pb_graph_node->num_output_pins[iport]; ++ipin) {
       t_pb_graph_pin* sink_pin = &(logical_primitive_pb_graph_node->output_pins[iport][ipin]);

       /* Port must exist in the module graph */
       ModulePortId sink_module_port_id = module_manager.find_module_port(pb_module, generate_pb_type_port_name(physical_pb_type, sink_pin->port));
       VTR_ASSERT(true == module_manager.valid_module_port_id(pb_module, sink_module_port_id));
       BasicPort sink_port = module_manager.module_port(pb_module, sink_module_port_id);
       /* Set the correct pin number of the port */
       sink_port.set_width(sink_pin->pin_number, sink_pin->pin_number);

       /* Find all the sink pin from this source pb_graph_pin */
       for (int iedge = 0; iedge < sink_pin->num_input_edges; ++iedge) {
         VTR_ASSERT(1 == sink_pin->input_edges[iedge]->num_input_pins);
         t_pb_graph_pin* src_pin = sink_pin->input_edges[iedge]->input_pins[0];

         /* Port must exist in the module graph */
         ModulePortId src_module_port_id = module_manager.find_module_port(pb_module, generate_pb_type_port_name(physical_pb_type, src_pin->port));
         VTR_ASSERT(true == module_manager.valid_module_port_id(pb_module, src_module_port_id));
         BasicPort src_port = module_manager.module_port(pb_module, src_module_port_id);
         /* Set the correct pin number of the port */
         src_port.set_width(src_pin->pin_number, src_pin->pin_number);

         /* Find max delay between src and sink pin */
         float tmax = sink_pin->input_edges[iedge]->delay_max;

         /* Generate module path in hierarchy depending if the hierarchical is enabled */
         std::string module_hie_path = pb_module_name;
         if (false == hierarchical) {
           module_hie_path = module_path + pb_module_name;
         }

         /* If the delay is zero, constrain only when user wants it */
         if ( (true == constrain_zero_delay_paths)
           || (0. == tmax) ) {
           print_pnr_sdc_constrain_max_delay(fp, 
                                             module_hie_path, 
                                             generate_sdc_port(src_port),
                                             module_hie_path, 
                                             generate_sdc_port(sink_port),
                                             tmax / time_unit);
         }

         /* Find min delay between src and sink pin */
         float tmin = sink_pin->input_edges[iedge]->delay_min;
         /* If the delay is zero, constrain only when user wants it */
         if ( (true == constrain_zero_delay_paths)
           || (0. == tmin) ) {
           print_pnr_sdc_constrain_min_delay(fp, 
                                             module_hie_path, 
                                             generate_sdc_port(src_port),
                                             module_hie_path, 
                                             generate_sdc_port(sink_port),
                                             tmin / time_unit);
        } 
      }
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
                                                 const float& time_unit,
                                                 const bool& hierarchical,
                                                 const std::string& module_path,
                                                 const ModuleManager& module_manager,
                                                 const VprDeviceAnnotation& device_annotation,
                                                 t_pb_graph_node* parent_pb_graph_node,
                                                 const bool& constrain_zero_delay_paths) {
  /* Validate pb_graph node */
  if (nullptr == parent_pb_graph_node) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid parent_pb_graph_node.\n");
    exit(1);
  }

  /* Get the pb_type */
  t_pb_type* parent_pb_type = parent_pb_graph_node->pb_type;

  /* Constrain the primitive node if a timing matrix is defined */
  if (true == is_primitive_pb_type(parent_pb_type)) {
    print_pnr_sdc_constrain_primitive_pb_graph_node(sdc_dir,
                                                    time_unit,
                                                    hierarchical,
                                                    module_path,
                                                    module_manager,
                                                    parent_pb_graph_node,
                                                    constrain_zero_delay_paths);
    return;
  }

  /* Note we only go through the graph through the physical modes. 
   * which we build the modules 
   */
  t_mode* physical_mode = device_annotation.physical_mode(parent_pb_type);  

  /* Write a SDC file for this pb_type */
  print_pnr_sdc_constrain_pb_graph_node_timing(sdc_dir,
                                               time_unit,
                                               hierarchical,
                                               module_path,
                                               module_manager,
                                               parent_pb_graph_node,
                                               physical_mode,
                                               constrain_zero_delay_paths);

  /* Go recursively to the lower level in the pb_graph
   * Note that we assume a full hierarchical P&R, we will only visit pb_graph_node of unique pb_type 
   */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    rec_print_pnr_sdc_constrain_pb_graph_timing(sdc_dir,
                                                time_unit, 
                                                hierarchical,
                                                format_dir_path(module_path + std::string(physical_mode->pb_type_children[ipb].name)),
                                                module_manager, 
                                                device_annotation,
                                                &(parent_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][0]),
                                                constrain_zero_delay_paths);
  }
}

/********************************************************************
 * Top-level function to print timing constraints for pb_types
 *******************************************************************/
void print_pnr_sdc_constrain_grid_timing(const std::string& sdc_dir,
                                         const float& time_unit,
                                         const bool& hierarchical,
                                         const DeviceContext& device_ctx,
                                         const VprDeviceAnnotation& device_annotation,
                                         const ModuleManager& module_manager,
                                         const ModuleId& top_module,
                                         const bool& constrain_zero_delay_paths) {

  /* Start time count */
  vtr::ScopedStartFinishTimer timer("Write SDC for constraining grid timing for P&R flow");

  std::string root_path = format_dir_path(module_manager.module_name(top_module));

  for (const t_physical_tile_type& physical_tile : device_ctx.physical_tile_types) {
    /* Bypass empty type or nullptr */
    if (true == is_empty_type(&physical_tile)) {
      continue;
    }

    VTR_ASSERT(1 == physical_tile.equivalent_sites.size());
    t_pb_graph_node* pb_graph_head = physical_tile.equivalent_sites[0]->pb_graph_head;
    if (nullptr == pb_graph_head) {
      continue;
    }

    if (true == is_io_type(&physical_tile)) {
      /* Special for I/O block:
       * We will search the grids and see where the I/O blocks are located:
       * - If a I/O block locates on border sides of FPGA fabric:
       *   i.e., one or more from {TOP, RIGHT, BOTTOM, LEFT},
       *   we will generate one module for each border side 
       * - If a I/O block locates in the center of FPGA fabric:
       *   we will generate one module with NUM_SIDES (same treatment as regular grids) 
       */
      std::set<e_side> io_type_sides = find_physical_io_tile_located_sides(device_ctx.grid,
                                                                           &physical_tile);


      /* Generate the grid module name */
      for (const e_side& io_type_side : io_type_sides) {
        std::string grid_module_name = generate_grid_block_module_name(std::string(GRID_MODULE_NAME_PREFIX), 
                                                                       std::string(physical_tile.name),
                                                                       is_io_type(&physical_tile),
                                                                       io_type_side);
        /* Find the module Id */
        ModuleId grid_module = module_manager.find_module(grid_module_name);
        VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

        std::string module_path = format_dir_path(root_path + grid_module_name);

        rec_print_pnr_sdc_constrain_pb_graph_timing(sdc_dir,
                                                    time_unit,
                                                    hierarchical,
                                                    module_path, 
                                                    module_manager, 
                                                    device_annotation,
                                                    pb_graph_head,
                                                    constrain_zero_delay_paths);
      }
    } else {
      /* For CLB and heterogenenous blocks */
      std::string grid_module_name = generate_grid_block_module_name(std::string(GRID_MODULE_NAME_PREFIX), 
                                                                     std::string(physical_tile.name),
                                                                     is_io_type(&physical_tile),
                                                                     NUM_SIDES);
      /* Find the module Id */
      ModuleId grid_module = module_manager.find_module(grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

      std::string module_path = format_dir_path(root_path + grid_module_name);

      rec_print_pnr_sdc_constrain_pb_graph_timing(sdc_dir,
                                                  time_unit,
                                                  hierarchical,
                                                  module_path, 
                                                  module_manager, 
                                                  device_annotation,
                                                  pb_graph_head,
                                                  constrain_zero_delay_paths);
    }
  }
}

} /* end namespace openfpga */
