/***************************************************************************************
 * Output internal structure of Module Graph hierarchy to file formats
 ***************************************************************************************/
#include <regex>
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "command_exit_codes.h"
#include "fabric_hierarchy_writer.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"

/* begin namespace openfpga */
namespace openfpga {

/** Identify if the module has no child whose name matches the filter */
static bool module_filter_all_children(const ModuleManager& module_manager,
                                       const ModuleId& curr_module,
                                       const ModuleNameMap& module_name_map,
                                       const std::string& module_name_filter) {
  for (const ModuleId& child_module :
       module_manager.child_modules(curr_module)) {
    /* Filter out the names which do not match the pattern */
    std::string child_module_name = module_manager.module_name(child_module);
    if (module_name_map.name_exist(child_module_name)) {
      child_module_name = module_name_map.name(child_module_name);
    }
    std::string pattern = module_name_filter;
    std::regex star_replace("\\*");
    std::regex questionmark_replace("\\?");
    std::string wildcard_pattern =
      std::regex_replace(std::regex_replace(pattern, star_replace, ".*"),
                         questionmark_replace, ".");
    std::regex wildcard_regex(wildcard_pattern);
    if (std::regex_match(child_module_name, wildcard_regex)) {
      return false;
    }
  }
  return true;
}

/***************************************************************************************
 * Recursively output child module of the parent_module to a text file
 * We use Depth-First Search (DFS) here so that we can output a tree down to
 *leaf first Add space (indent) based on the depth in hierarchy e.g. depth = 1
 *means a space as indent
 ***************************************************************************************/
static int rec_output_module_hierarchy_to_text_file(
  std::fstream& fp, const size_t& hie_depth_to_stop,
  const size_t& current_hie_depth, const ModuleManager& module_manager,
  const ModuleId& parent_module, const ModuleNameMap& module_name_map,
  const std::string& module_name_filter, const bool& verbose) {
  /* Stop if hierarchy depth is beyond the stop line */
  if (hie_depth_to_stop < current_hie_depth) {
    return CMD_EXEC_SUCCESS;
  }

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Check if all the child module has not qualified grand-child, use leaf for
   * this level */
  bool use_list = true;
  for (const ModuleId& child_module :
       module_manager.child_modules(parent_module)) {
    if (!module_filter_all_children(module_manager, child_module,
                                    module_name_map, module_name_filter)) {
      use_list = false;
      break;
    }
  }
  /* For debug use only
  VTR_LOGV(verbose, "Current depth: %lu, Target depth: %lu\n",
           current_hie_depth, hie_depth_to_stop);
  */
  std::string parent_module_name = module_manager.module_name(parent_module);
  if (module_name_map.name_exist(parent_module_name)) {
    parent_module_name = module_name_map.name(parent_module_name);
  }
  VTR_LOGV(use_list && verbose,
           "Use list as module '%s' contains only leaf nodes\n",
           parent_module_name.c_str());

  /* Iterate over all the child module */
  for (const ModuleId& child_module :
       module_manager.child_modules(parent_module)) {
    if (true != module_manager.valid_module_id(child_module)) {
      VTR_LOGV_ERROR(
        verbose,
        "Unable to find the child module '%s' under its parent '%s'!\n",
        module_manager.module_name(child_module).c_str(),
        module_manager.module_name(parent_module).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    /* Filter out the names which do not match the pattern */
    std::string child_module_name = module_manager.module_name(child_module);
    if (module_name_map.name_exist(child_module_name)) {
      child_module_name = module_name_map.name(child_module_name);
    }
    std::string pattern = module_name_filter;
    std::regex star_replace("\\*");
    std::regex questionmark_replace("\\?");
    std::string wildcard_pattern =
      std::regex_replace(std::regex_replace(pattern, star_replace, ".*"),
                         questionmark_replace, ".");
    std::regex wildcard_regex(wildcard_pattern);
    if (!std::regex_match(child_module_name, wildcard_regex)) {
      continue;
    }

    if (false == write_space_to_file(fp, current_hie_depth * 2)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    if (hie_depth_to_stop == current_hie_depth || use_list) {
      fp << "- " << child_module_name.c_str() << "\n";
    } else {
      fp << child_module_name.c_str() << ":\n";
    }
    /* Go to next level */
    int status = rec_output_module_hierarchy_to_text_file(
      fp, hie_depth_to_stop,
      current_hie_depth + 1, /* Increment the depth for the next level */
      module_manager, child_module, module_name_map, module_name_filter,
      verbose);
    if (status != CMD_EXEC_SUCCESS) {
      return status;
    }
  }

  return CMD_EXEC_SUCCESS;
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
int write_fabric_hierarchy_to_text_file(
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const std::string& fname, const std::string& root_module_names,
  const std::string& module_name_filter, const size_t& hie_depth_to_stop,
  const bool& exclude_empty_modules, const bool& verbose) {
  std::string timer_message =
    std::string("Write fabric hierarchy to plain-text file '") + fname +
    std::string("'");

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

  size_t cnt = 0;
  /* Use regular expression to capture the module whose name matches the pattern
   */
  for (ModuleId curr_module : module_manager.modules()) {
    std::string curr_module_name = module_manager.module_name(curr_module);
    if (module_name_map.name_exist(curr_module_name)) {
      curr_module_name = module_name_map.name(curr_module_name);
    }
    std::string pattern = root_module_names;
    std::regex star_replace("\\*");
    std::regex questionmark_replace("\\?");
    std::string wildcard_pattern =
      std::regex_replace(std::regex_replace(pattern, star_replace, ".*"),
                         questionmark_replace, ".");
    std::regex wildcard_regex(wildcard_pattern);
    if (!std::regex_match(curr_module_name, wildcard_regex)) {
      continue;
    }
    /* Filter out module without children if required */
    if (exclude_empty_modules &&
        module_filter_all_children(module_manager, curr_module, module_name_map,
                                   module_name_filter)) {
      continue;
    }
    VTR_LOGV(verbose, "Select module '%s' as root\n", curr_module_name.c_str());
    /* Record current depth of module: top module is the root with 0 depth */
    size_t hie_depth = 0;

    fp << curr_module_name << ":"
       << "\n";

    /* Visit child module recursively and output the hierarchy */
    int err_code = rec_output_module_hierarchy_to_text_file(
      fp, hie_depth_to_stop, hie_depth + 1, /* Start with level 1 */
      module_manager, curr_module, module_name_map, module_name_filter,
      verbose);
    /* Catch error code and exit if required */
    if (err_code == CMD_EXEC_FATAL_ERROR) {
      return err_code;
    }
    cnt++;
  }

  if (cnt == 0) {
    VTR_LOG_ERROR(
      "Unable to find any module matching the root module name pattern '%s'!\n",
      root_module_names.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Outputted %lu modules as root\n", cnt);

  /* close a file */
  fp.close();

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
