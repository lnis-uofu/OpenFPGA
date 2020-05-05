/***************************************************************************************
 * Output internal structure of Module Graph hierarchy to file formats
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_naming.h"

#include "fabric_hierarchy_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Recursively output child module of the parent_module to a text file
 * We use Depth-First Search (DFS) here so that we can output a tree down to leaf first
 * Add space (indent) based on the depth in hierarchy
 * e.g. depth = 1 means a space as indent
 ***************************************************************************************/
static 
int rec_output_module_hierarchy_to_text_file(std::fstream& fp,
                                             const size_t& hie_depth,
                                             const ModuleManager& module_manager,  
                                             const ModuleId& parent_module,
                                             const bool& verbose) {
  if (false == valid_file_stream(fp)) {
    return 2;
  }

  /* Iterate over all the child module */
  for (const ModuleId& child_module : module_manager.child_modules(parent_module)) {
    if (false == write_space_to_file(fp, hie_depth)) {
      return 2;
    }

    if (true != module_manager.valid_module_id(child_module)) {
      VTR_LOGV_ERROR(verbose,
                     "Unable to find the child module '%u'!\n",
                     size_t(child_module));
      return 1;
    }

    fp << module_manager.module_name(child_module);
    fp << "\n";

    /* Go to next level */
    int status = rec_output_module_hierarchy_to_text_file(fp,
                                                          hie_depth + 1, /* Increment the depth for the next level */
                                                          module_manager,
                                                          child_module,
                                                          verbose);
    if (0 != status) {
      return status;
    }
  }

  return 0;
}

/***************************************************************************************
 * Write the hierarchy of modules to a plain text file
 * e.g.,
 *    <module_name>
 *      <child_module_name>
 *        ...
 * This file is mainly used by hierarchical P&R flow 
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 ***************************************************************************************/
int write_fabric_hierarchy_to_text_file(const ModuleManager& module_manager,
                                        const std::string& fname,
                                        const bool& verbose) {
  std::string timer_message = std::string("Write fabric hierarchy to plain-text file '") + fname + std::string("'");

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

  /* Find top-level module */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  if (true != module_manager.valid_module_id(top_module)) {
    VTR_LOGV_ERROR(verbose,
                   "Unable to find the top-level module '%s'!\n",
                   top_module_name.c_str());
    return 1;
  }
  
  /* Record current depth of module: top module is the root with 0 depth */
  size_t hie_depth = 0;

  fp << top_module_name << "\n";

  /* Visit child module recursively and output the hierarchy */
  int err_code = rec_output_module_hierarchy_to_text_file(fp,
                                                          hie_depth + 1, /* Start with level 1 */
                                                          module_manager,  
                                                          top_module,
                                                          verbose);

  /* close a file */
  fp.close();

  return err_code;
}

} /* end namespace openfpga */
