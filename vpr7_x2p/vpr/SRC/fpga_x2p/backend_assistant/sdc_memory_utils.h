#ifndef SDC_MEMORY_UTILS_H
#define SDC_MEMORY_UTILS_H

#include <fstream>
#include <string>
#include "module_manager.h"

void rec_print_pnr_sdc_disable_configurable_memory_module_output(std::fstream& fp, 
                                                                 const ModuleManager& module_manager, 
                                                                 const ModuleId& parent_module,
                                                                 const std::string& parent_module_path);

#endif
