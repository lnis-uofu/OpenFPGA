/***************************************************************************************
 * Output instance hierarchy in SDC to file formats
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_naming.h"

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

  std::string fname(sdc_dir + std::string("sb_hierarchy.txt"));

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

    fp << sdc_fname << "\n";

    /* Go through all the instance */
    for (const size_t& instance_id : module_manager.child_module_instances(top_module, sb_module)) {
      std::string sb_instance_name = module_manager.instance_name(top_module, sb_module, instance_id);
      fp << " - ";
      fp << sb_module_name << "/" << sb_instance_name << "\n";  
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

  /* TODO: This is dirty, should use constant to handle this */
  std::string fname(sdc_dir);
  if (CHANX == cb_type) {
    fname += std::string("cbx_hierarchy.txt");
  } else {
    VTR_ASSERT(CHANY == cb_type);
    fname += std::string("cby_hierarchy.txt");
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

    fp << sdc_fname << "\n";

    /* Go through all the instance */
    for (const size_t& instance_id : module_manager.child_module_instances(top_module, cb_module)) {
      std::string cb_instance_name = module_manager.instance_name(top_module, cb_module, instance_id);
      fp << " - ";
      fp << cb_module_name << "/" << cb_instance_name << "\n";  
    } 

    fp << "\n";
  }

  /* close a file */
  fp.close();
}

} /* end namespace openfpga */
