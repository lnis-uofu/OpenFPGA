/************************************************
 * Header file for spice_writer_utils.cpp
 * Include function declaration for most frequently
 * used Verilog writers 
 ***********************************************/
#ifndef SPICE_WRITER_UTILS_H 
#define SPICE_WRITER_UTILS_H 

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <vector>
#include <string>
#include "openfpga_port.h"
#include "circuit_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* Tips: for naming your function in this header/source file
 * If a function outputs to a file, its name should begin with "print_spice"
 * If a function creates a string without outputting to a file, its name should begin with "generate_spice"
 * Please show respect to this naming convention, in order to keep a clean header/source file
 * as well maintain a easy way to identify the functions
 */

void print_spice_file_header(std::fstream& fp,
                             const std::string& usage);

void print_spice_include_netlist(std::fstream& fp, 
                                 const std::string& netlist_name);

void print_spice_comment(std::fstream& fp, 
                         const std::string& comment);

std::string generate_spice_port(const BasicPort& port,
                                const bool& omit_pin_zero = false);

void print_spice_subckt_definition(std::fstream& fp, 
                                   const ModuleManager& module_manager, const ModuleId& module_id);

void print_spice_subckt_end(std::fstream& fp, 
                            const std::string& module_name);

} /* end namespace openfpga */

#endif
