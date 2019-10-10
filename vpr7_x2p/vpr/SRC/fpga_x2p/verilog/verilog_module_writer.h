/********************************************************************
 * Header file for verilog_module_writer.cpp
 *******************************************************************/

#ifndef VERILOG_MODULE_WRITER_H
#define VERILOG_MODULE_WRITER_H

#include <fstream>
#include "module_manager.h"

void writer_verilog_module_to_file(std::fstream& fp,
                                   const ModuleManager& module_manager,
                                   const ModuleId& module_id);

#endif
