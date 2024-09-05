/***************************************************************************************
 * Output internal structure of module graph to XML format
 ***************************************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "command_exit_codes.h"
#include "openfpga_digest.h"
#include "report_reference.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function
 *******************************************************************/
int report_reference(const char* fname, const std::string& module_name,
                     const ModuleManager& module_manager,
                     const bool& include_time_stamp, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Report reference");

  std::fstream fp;
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);
  openfpga::check_file_stream(fname, fp);

  if (include_time_stamp) {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    fp << "Date: " << std::ctime(&end_time) << std::endl;
  }

  ModuleId parent_module = module_manager.find_module(module_name);
  if (ModuleId::INVALID() == parent_module) {
    VTR_LOG_ERROR("Module %s doesn't exist\n", module_name.c_str());
    return CMD_EXEC_MINOR_ERROR;
  }

  if (module_manager.child_modules(parent_module).size() < 1) {
    VTR_LOG_ERROR("Module %s contains no child module\n", module_name.c_str());
    return CMD_EXEC_MINOR_ERROR;
  }

  VTR_LOG(
    "--------------------------------------------------------------------------"
    "----\n");
  VTR_LOG(
    "Module                                                         Reference "
    "count\n");
  VTR_LOG(
    "--------------------------------------------------------------------------"
    "----\n");
  size_t ref_cnt = 0;
  for (ModuleId child_module : module_manager.child_modules(parent_module)) {
    std::string child_module_name = module_manager.module_name(child_module);
    std::vector<size_t> child_inst_vec =
      module_manager.child_module_instances(parent_module, child_module);
    VTR_LOG("%-s %d\n", child_module_name.c_str(), child_inst_vec.size());
    ref_cnt += child_inst_vec.size();
  }
  VTR_LOG(
    "--------------------------------------------------------------------------"
    "----\n");
  VTR_LOG("Total %zu modules %zu references\n",
          module_manager.child_modules(parent_module).size(), ref_cnt);
  VTR_LOG(
    "--------------------------------------------------------------------------"
    "----\n");

  if (verbose) {
    fp << "\nTotal " << module_manager.child_modules(parent_module).size()
       << " modules " << ref_cnt << " references\n";
  }

  fp << "references:" << std::endl;
  for (ModuleId child_module : module_manager.child_modules(parent_module)) {
    std::string child_module_name = module_manager.module_name(child_module);
    std::vector<size_t> child_inst_vec =
      module_manager.child_module_instances(parent_module, child_module);
    fp << "- module: " << child_module_name.c_str() << "\n"
       << "  reference count: " << child_inst_vec.size() << "\n"
       << "  instances:"
       << "\n";
    for (size_t inst_id : child_inst_vec) {
      std::string inst_name =
        module_manager.instance_name(parent_module, child_module, inst_id);
      if (inst_name.size() > 0) fp << "    - " << inst_name.c_str() << "\n";
    }
  }

  fp.close();

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
