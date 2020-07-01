/***************************************************************************************
 * Output fabric key of Module Graph to file formats
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from archopenfpga library */
#include "write_xml_fabric_key.h"

#include "openfpga_naming.h"

#include "fabric_key_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Write the fabric key of top module to an XML file
 * We will use the writer API in libfabrickey
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 ***************************************************************************************/
int write_fabric_key_to_xml_file(const ModuleManager& module_manager,
                                 const std::string& fname,
                                 const bool& verbose) {
  std::string timer_message = std::string("Write fabric key to XML file '") + fname + std::string("'");

  std::string dir_path = format_dir_path(find_path_dir_name(fname));

  /* Create directories */
  create_directory(dir_path);

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Use default name if user does not provide one */
  VTR_ASSERT(true != fname.empty());

  /* Find top-level module */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  if (true != module_manager.valid_module_id(top_module)) {
    VTR_LOGV_ERROR(verbose,
                   "Unable to find the top-level module '%s'!\n",
                   top_module_name.c_str());
    return 1;
  }
  
  /* Build a fabric key database by visiting all the configurable children */
  FabricKey fabric_key;
  size_t num_keys = module_manager.configurable_children(top_module).size(); 
  fabric_key.reserve_keys(num_keys);

  for (size_t ichild = 0; ichild < num_keys; ++ichild) {
    ModuleId child_module = module_manager.configurable_children(top_module)[ichild];
    size_t child_instance = module_manager.configurable_child_instances(top_module)[ichild];

    FabricKeyId key = fabric_key.create_key();
    fabric_key.set_key_name(key, module_manager.module_name(child_module));
    fabric_key.set_key_value(key, child_instance);

    if (false == module_manager.instance_name(top_module, child_module, child_instance).empty()) {
      fabric_key.set_key_alias(key, module_manager.instance_name(top_module, child_module, child_instance));
    }
  }

  VTR_LOGV(verbose,
           "Created %lu keys for the top module %s.\n",
           num_keys, top_module_name.c_str());

  /* Call the XML writer for fabric key */
  int err_code = write_xml_fabric_key(fname.c_str(), fabric_key);

  return err_code;
}

} /* end namespace openfpga */
