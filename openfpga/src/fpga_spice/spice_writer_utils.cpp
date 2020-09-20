/************************************************
 * Include functions for most frequently
 * used Spice writers 
 ***********************************************/
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga library */
#include "circuit_types.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "spice_constants.h"
#include "spice_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************
 * Generate header comments for a Spice netlist
 * include the description 
 ***********************************************/
void print_spice_file_header(std::fstream& fp,
                             const std::string& usage) {
  VTR_ASSERT(true == valid_file_stream(fp));
 
  auto end = std::chrono::system_clock::now(); 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  fp << "*********************************************" << std::endl;
  fp << "*\tFPGA-SPICE Netlist" << std::endl;
  fp << "*\tDescription: " << usage << std::endl;
  fp << "*\tAuthor: Xifan TANG" << std::endl;
  fp << "*\tOrganization: University of Utah" << std::endl;
  fp << "*\tDate: " << std::ctime(&end_time) ;
  fp << "*********************************************" << std::endl;
  fp << std::endl;
}

/********************************************************************
 * Print Spice codes to include a netlist  
 *******************************************************************/
void print_spice_include_netlist(std::fstream& fp, 
                                 const std::string& netlist_name) {
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << ".include \"" << netlist_name << "\"" << std::endl; 
}

/************************************************
 * Print a Spice comment line
 ***********************************************/
void print_spice_comment(std::fstream& fp, 
                         const std::string& comment) {
  VTR_ASSERT(true == valid_file_stream(fp));

  std::string comment_cover(comment.length() + 4, '*');
  fp << comment_cover << std::endl;
  fp << "* " << comment << " *" << std::endl;
  fp << comment_cover << std::endl;
}


/************************************************
 * Generate a string for a port in SPICE format
 * If the pin id is zero, e.g., A[0], the option
 * 'omit_pin_zero' may be turned on for compact port
 * print-out, e.g., A
 ***********************************************/
std::string generate_spice_port(const BasicPort& port,
                                const bool& omit_pin_zero) {
  VTR_ASSERT(1 == port.get_width());

  std::string ret = port.get_name();

  if ((true == omit_pin_zero)
     && (0 == port.get_lsb())) {
    return ret;
  }

  ret += "[";
  ret += std::to_string(port.get_lsb());
  ret += "]";

  return ret;
}

/************************************************
 * Print a SPICE subckt definition
 * We use the following format:
 * module <module_name> (<ports without directions>);
 ***********************************************/
void print_spice_subckt_definition(std::fstream& fp, 
                                   const ModuleManager& module_manager,
                                   const ModuleId& module_id,
                                   const bool& include_supply_ports) {
  VTR_ASSERT(true == valid_file_stream(fp));

  print_spice_comment(fp, std::string("SPICE module for " + module_manager.module_name(module_id)));

  std::string module_head_line = ".subckt " + module_manager.module_name(module_id) + " ";
  fp << module_head_line;

  /* Port sequence: global, inout, input, output and clock ports, */
  bool new_line = false;
  size_t pin_cnt = 0;
  for (int port_type = ModuleManager::MODULE_GLOBAL_PORT;
       port_type < ModuleManager::NUM_MODULE_PORT_TYPES;
       ++port_type) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, static_cast<ModuleManager::e_module_port_type>(port_type))) {
      ModulePortId port_id = module_manager.find_module_port(module_id, port.get_name());
      VTR_ASSERT(ModulePortId::INVALID() != port_id);

      /* Print port: only the port name is enough */
      for (const auto& pin : port.pins()) {

        if (true == new_line) {
          std::string port_whitespace(module_head_line.length() - 2, ' ');
          fp << "+ " << port_whitespace;
        }
 
        if (0 != pin_cnt) {
          write_space_to_file(fp, 1);
        }
        
        BasicPort port_pin(port.get_name(), pin, pin);

        /* For single-bit port,
         * we can print the port name directly
         */
        bool omit_pin_zero = false;
        if ((1 == port.pins().size())
           && (0 == pin)) {
          omit_pin_zero = true;
        }
        fp << generate_spice_port(port_pin, omit_pin_zero);

        /* Increase the counter */
        pin_cnt++;

        /* Currently we limit 10 ports per line to keep a clean netlist */
        new_line = false;
        if (SPICE_NETLIST_MAX_NUM_PORTS_PER_LINE == pin_cnt) {
          pin_cnt = 0;
          fp << std::endl;
          new_line = true;
        }
      }
    }
  }
  
  /* Add supply ports if specified */
  if (true == include_supply_ports) {
    /* Print VDD and VSS ports
     * TODO: the supply ports should be derived from module manager
     */
    if (true == new_line) {
      std::string port_whitespace(module_head_line.length() - 2, ' ');
      fp << "+ " << port_whitespace;
    }
    write_space_to_file(fp, 1);
    fp << SPICE_SUBCKT_VDD_PORT_NAME;
    write_space_to_file(fp, 1);
    fp << SPICE_SUBCKT_GND_PORT_NAME;
  }

  fp << std::endl;
}

/************************************************
 * Print an end line for a Spice module
 ***********************************************/
void print_spice_subckt_end(std::fstream& fp, 
                            const std::string& module_name) {
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << ".ends" << std::endl;
  print_spice_comment(fp, std::string("***** END SPICE module for " + module_name + " *****"));
  fp << std::endl;
}

/************************************************
 * Print a resistor in SPICE syntax
 ***********************************************/
void print_spice_resistor(std::fstream& fp,
                          const std::string& input_port,
                          const std::string& output_port,
                          const float& resistance) {
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Set an unique name to the resistor */
  fp << "R" << input_port << "_to_"  << output_port;
  fp << " " << input_port;
  fp << " " << output_port;
  fp << " " << std::setprecision(10) << resistance;
  fp << std::endl;
}

/************************************************
 * Print a capacitor in SPICE syntax
 ***********************************************/
void print_spice_capacitor(std::fstream& fp,
                          const std::string& input_port,
                          const std::string& output_port,
                          const float& capacitance) {
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Set an unique name to the capacitor */
  fp << "C" << input_port << "_to_"  << output_port;
  fp << " " << input_port;
  fp << " " << output_port;
  fp << " " << std::setprecision(10) << capacitance;
  fp << std::endl;
}

/************************************************
 * Print a short-connected wire using zero resistance in SPICE syntax
 ***********************************************/
void print_spice_short_connection(std::fstream& fp,
                                  const std::string& input_port,
                                  const std::string& output_port) {
  print_spice_resistor(fp, input_port, output_port, 0.);
}

/********************************************************************
 * Print an instance in SPICE format (a generic version)
 * This function will require user to provide an instance name
 *
 * This function will output the port map by referring to a port-to-port 
 * mapping:
 *   <module_port_name> -> <instance_port_name>
 * The key of the port-to-port mapping is the port name of the module: 
 * The value of the port-to-port mapping is the port information of the instance
 * With link between module and instance, the function can output a SPICE 
 * instance easily, by following the define port sequence of the module
 *
 * Note that, it is not necessary that the port-to-port mapping 
 * covers all the module ports.
 * Any instance/module port which are not specified in the port-to-port 
 * mapping will be output by the module port name.
 *******************************************************************/
void print_spice_subckt_instance(std::fstream& fp, 
                                 const ModuleManager& module_manager, 
                                 const ModuleId& module_id,
                                 const std::string& instance_name,
                                 const std::map<std::string, BasicPort>& port2port_name_map) {

  VTR_ASSERT(true == valid_file_stream(fp));

  /* Check: all the key ports in the port2port_name_map does exist in the child module */
  for (const auto& kv : port2port_name_map) {
    ModulePortId module_port_id = module_manager.find_module_port(module_id, kv.first);
    VTR_ASSERT(ModulePortId::INVALID() != module_port_id);
  }

  /* Print instance name */
  std::string instance_head_line = "X " + instance_name + " ";
  fp << instance_head_line;
  
  /* Port sequence: global, inout, input, output and clock ports, */
  bool fit_one_line = true;
  bool new_line = false;
  size_t pin_cnt = 0;
  for (int port_type = ModuleManager::MODULE_GLOBAL_PORT;
       port_type < ModuleManager::NUM_MODULE_PORT_TYPES;
       ++port_type) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, static_cast<ModuleManager::e_module_port_type>(port_type))) {
      /* Deposit a default port name */
      BasicPort port_to_print = port;
      /* Try to find the instanced port name in the name map */
      auto port_search_result = port2port_name_map.find(port.get_name());
      if (port_search_result != port2port_name_map.end()) {
        /* Found it, we assign the port name */ 
        /* TODO: make sure the port width matches! */
        ModulePortId module_port_id = module_manager.find_module_port(module_id, port.get_name());
        /* Get the port from module */
        BasicPort module_port = module_manager.module_port(module_id, module_port_id);
        VTR_ASSERT(module_port.get_width() == port_search_result->second.get_width());

        port_to_print = port_search_result->second;
      }

      /* Print port: only the port name is enough */
      for (const auto& pin : port_to_print.pins()) {

        if (true == new_line) {
          std::string port_whitespace(instance_head_line.length() - 2, ' ');
          fp << "+ " << port_whitespace;
        }
 
        if (0 != pin_cnt) {
          write_space_to_file(fp, 1);
        }
        
        BasicPort port_pin(port.get_name(), pin, pin);

        /* For single-bit port,
         * we can print the port name directly
         */
        bool omit_pin_zero = false;
        if ((1 == port.pins().size())
           && (0 == pin)) {
          omit_pin_zero = true;
        }

        fp << generate_spice_port(port_pin, omit_pin_zero);

        /* Increase the counter */
        pin_cnt++;

        /* Currently we limit 10 ports per line to keep a clean netlist */
        new_line = false;
        if (SPICE_NETLIST_MAX_NUM_PORTS_PER_LINE == pin_cnt) {
          pin_cnt = 0;
          fp << std::endl;
          new_line = true;
          fit_one_line = false;
        }
      }
    }
  }

  /* Print VDD and VSS ports
   * TODO: the supply ports should be derived from module manager
   */
  if (true == new_line) {
    std::string port_whitespace(instance_head_line.length() - 2, ' ');
    fp << "+ " << port_whitespace;
  }
  write_space_to_file(fp, 1);
  fp << SPICE_SUBCKT_VDD_PORT_NAME;
  write_space_to_file(fp, 1);
  fp << SPICE_SUBCKT_GND_PORT_NAME;

  pin_cnt += 2;

  /* Check if we need a new line */
  new_line = false;
  if (SPICE_NETLIST_MAX_NUM_PORTS_PER_LINE == pin_cnt) {
    pin_cnt = 0;
    fp << std::endl;
    new_line = true;
    fit_one_line = false;
  }

  /* Print module name: 
   * if port print cannot fit one line, we create a new line for the module for a clean format
   */
  if (false == fit_one_line) {
    fp << std::endl;
    fp << "+";
  }
  write_space_to_file(fp, 1);
  fp << module_manager.module_name(module_id);
  
  /* Print an end to the instance */
  fp << std::endl;
}

} /* end namespace openfpga */
