/***************************************************************************************
 * Output instance hierarchy in SDC to file formats
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "pb_type_utils.h"
#include "openfpga_physical_tile_utils.h"

#include "sdc_writer_naming.h"
#include "sdc_hierarchy_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Write the hierarchy of Switch Block module and its instances to a plain text file
 * e.g.,
 *    <sdc_file_for_the_module>
 *      <module_name>/<instance_name>
 *        ...
 * This file is mainly used by hierarchical P&R flow 
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 ***************************************************************************************/
void print_pnr_sdc_routing_sb_hierarchy(const std::string& sdc_dir,
                                        const ModuleManager& module_manager,
                                        const ModuleId& top_module,
                                        const DeviceRRGSB& device_rr_gsb) {

  std::string fname(sdc_dir + std::string(SDC_SB_HIERARCHY_FILE_NAME));

  std::string timer_message = std::string("Write Switch Block hierarchy to plain-text file '") + fname + std::string("'");

  std::string dir_path = format_dir_path(find_path_dir_name(fname));

  /* Create directories */
  create_directory(dir_path);

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Use default name if user does not provide one */
  VTR_ASSERT(true != fname.empty());

  /* Create a file handler*/
  std::fstream fp;
  /* Open a file */
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(fname.c_str(), fp);

  for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& rr_gsb = device_rr_gsb.get_sb_unique_module(isb);
    if (false == rr_gsb.is_sb_exist()) {
      continue;
    }

    /* Find all the sb instance under this module
     * Create a regular expression to include these instance names 
     */
    vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
    std::string sb_module_name = generate_switch_block_module_name(gsb_coordinate); 

    ModuleId sb_module = module_manager.find_module(sb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

    /* Create the file name for SDC */
    std::string sdc_fname(sdc_dir + generate_switch_block_module_name(gsb_coordinate) + std::string(SDC_FILE_NAME_POSTFIX));

    fp << "- " << sb_module_name << "\n";

    /* Go through all the instance */
    for (const size_t& instance_id : module_manager.child_module_instances(top_module, sb_module)) {
      std::string sb_instance_name = module_manager.instance_name(top_module, sb_module, instance_id);
      fp << "  ";
      fp << "- " << sb_instance_name << "\n";  
    } 

    fp << "\n";
  }

  /* close a file */
  fp.close();
}

/***************************************************************************************
 * Write the hierarchy of Switch Block module and its instances to a plain text file
 * e.g.,
 *    <sdc_file_for_the_module>
 *      <module_name>/<instance_name>
 *        ...
 * This file is mainly used by hierarchical P&R flow 
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 ***************************************************************************************/
void print_pnr_sdc_routing_cb_hierarchy(const std::string& sdc_dir,
                                        const ModuleManager& module_manager,
                                        const ModuleId& top_module,
                                        const t_rr_type& cb_type,
                                        const DeviceRRGSB& device_rr_gsb) {

  std::string fname(sdc_dir);
  if (CHANX == cb_type) {
    fname += std::string(SDC_CBX_HIERARCHY_FILE_NAME);
  } else {
    VTR_ASSERT(CHANY == cb_type);
    fname += std::string(SDC_CBY_HIERARCHY_FILE_NAME);
  }

  std::string timer_message = std::string("Write Connection Block hierarchy to plain-text file '") + fname + std::string("'");

  std::string dir_path = format_dir_path(find_path_dir_name(fname));

  /* Create directories */
  create_directory(dir_path);

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Use default name if user does not provide one */
  VTR_ASSERT(true != fname.empty());

  /* Create a file handler*/
  std::fstream fp;
  /* Open a file */
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(fname.c_str(), fp);

  /* Print SDC for unique X-direction connection block modules */
  for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(cb_type); ++icb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(cb_type, icb);

    /* Find all the cb instance under this module
     * Create a regular expression to include these instance names 
     */
    vtr::Point<size_t> gsb_coordinate(unique_mirror.get_cb_x(cb_type), unique_mirror.get_cb_y(cb_type));
    std::string cb_module_name = generate_connection_block_module_name(cb_type, gsb_coordinate); 
    ModuleId cb_module = module_manager.find_module(cb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

    /* Create the file name for SDC */
    std::string sdc_fname(sdc_dir + generate_connection_block_module_name(cb_type, gsb_coordinate) + std::string(SDC_FILE_NAME_POSTFIX));

    fp << "- " << cb_module_name << "\n";

    /* Go through all the instance */
    for (const size_t& instance_id : module_manager.child_module_instances(top_module, cb_module)) {
      std::string cb_instance_name = module_manager.instance_name(top_module, cb_module, instance_id);
      fp << "  ";
      fp << "- " << cb_instance_name << "\n";  
    } 

    fp << "\n";
  }

  /* close a file */
  fp.close();
}

/********************************************************************
 * Recursively write the hierarchy of pb_type and its instances to a plain text file
 * e.g.,
 *    <sdc_file_for_the_module>
 *      <module_name>/<instance_name>
 *        ...
 * This file is mainly used by hierarchical P&R flow 
 *******************************************************************/
static 
void rec_print_pnr_sdc_grid_pb_graph_hierarchy(std::fstream& fp,
                                               const size_t& depth,
                                               const ModuleManager& module_manager,
                                               const ModuleId& parent_pb_module,
                                               const VprDeviceAnnotation& device_annotation,
                                               t_pb_graph_node* parent_pb_graph_node) {
  /* Validate the file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Validate pb_graph node */
  if (nullptr == parent_pb_graph_node) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid parent_pb_graph_node.\n");
    exit(1);
  }

  /* Get the pb_type */
  t_pb_type* parent_pb_type = parent_pb_graph_node->pb_type;

  std::string pb_module_name = generate_physical_block_module_name(parent_pb_type);

  /* Find the pb module in module manager */
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  write_space_to_file(fp, depth * 2);
  fp << "- " << pb_module_name << "\n";

  /* Go through all the instance */
  for (const size_t& instance_id : module_manager.child_module_instances(parent_pb_module, pb_module)) {
    std::string child_instance_name = module_manager.instance_name(parent_pb_module, pb_module, instance_id);
    write_space_to_file(fp, depth * 2);
    fp << "  ";
    fp << "- " << child_instance_name << "\n";  

    if (true == is_primitive_pb_type(parent_pb_type)) {
      return;
    }

    /* Note we only go through the graph through the physical modes. 
     * which we build the modules 
     */
    t_mode* physical_mode = device_annotation.physical_mode(parent_pb_type);  

    /* Go recursively to the lower level in the pb_graph
     * Note that we assume a full hierarchical P&R, we will only visit pb_graph_node of unique pb_type 
     */
    for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
      for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
        rec_print_pnr_sdc_grid_pb_graph_hierarchy(fp,
                                                  depth + 2,
                                                  module_manager, 
                                                  pb_module,
                                                  device_annotation,
                                                  &(parent_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]));
      }
    }
  } 
}


/***************************************************************************************
 * Write the hierarchy of grid module and its instances to a plain text file
 * e.g.,
 *    <sdc_file_for_the_module>
 *      <module_name>/<instance_name>
 *        ...
 * This file is mainly used by hierarchical P&R flow 
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 ***************************************************************************************/
void print_pnr_sdc_grid_hierarchy(const std::string& sdc_dir,
                                  const DeviceContext& device_ctx,
                                  const VprDeviceAnnotation& device_annotation,
                                  const ModuleManager& module_manager,
                                  const ModuleId& top_module) {

  std::string fname(sdc_dir + std::string(SDC_GRID_HIERARCHY_FILE_NAME));

  std::string timer_message = std::string("Write Grid hierarchy to plain-text file '") + fname + std::string("'");

  std::string dir_path = format_dir_path(find_path_dir_name(fname));

  /* Create directories */
  create_directory(dir_path);

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Use default name if user does not provide one */
  VTR_ASSERT(true != fname.empty());

  /* Create a file handler*/
  std::fstream fp;
  /* Open a file */
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(fname.c_str(), fp);

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

        fp << "- " << grid_module_name << "\n";

        /* Go through all the instance */
        for (const size_t& instance_id : module_manager.child_module_instances(top_module, grid_module)) {
          std::string grid_instance_name = module_manager.instance_name(top_module, grid_module, instance_id);
          fp << "  ";
          fp << "- " << grid_instance_name << "\n";  

          rec_print_pnr_sdc_grid_pb_graph_hierarchy(fp,
                                                    2,
                                                    module_manager, 
                                                    grid_module, 
                                                    device_annotation,
                                                    pb_graph_head);
        } 
        fp << "\n";

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

      fp << "- " << grid_module_name << "\n";

      /* Go through all the instance */
      for (const size_t& instance_id : module_manager.child_module_instances(top_module, grid_module)) {
        std::string grid_instance_name = module_manager.instance_name(top_module, grid_module, instance_id);
        fp << "  ";
        fp << "- " << grid_instance_name << "\n";  

        rec_print_pnr_sdc_grid_pb_graph_hierarchy(fp,
                                                  2,
                                                  module_manager, 
                                                  grid_module, 
                                                  device_annotation,
                                                  pb_graph_head);
      } 

      fp << "\n";

    }
  }

  /* close a file */
  fp.close();
}


} /* end namespace openfpga */
