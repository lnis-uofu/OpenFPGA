/********************************************************************
 * This file include most utilized functions to be used in SDC writers 
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <iomanip>
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_wildcard_string.h"

#include "openfpga_naming.h"

#include "sdc_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Write a head (description) in SDC file 
 *******************************************************************/
void print_sdc_file_header(std::fstream& fp,
                           const std::string& usage) {

  valid_file_stream(fp);

  auto end = std::chrono::system_clock::now(); 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  fp << "#############################################" << std::endl;
  fp << "#\tSynopsys Design Constraints (SDC)" << std::endl;
  fp << "#\tFor FPGA fabric " << std::endl;
  fp << "#\tDescription: " << usage << std::endl;
  fp << "#\tAuthor: Xifan TANG " << std::endl;
  fp << "#\tOrganization: University of Utah " << std::endl;
  fp << "#\tDate: " << std::ctime(&end_time);
  fp << "#############################################" << std::endl;
  fp << std::endl;
}

/********************************************************************
 * Write a timescale definition in SDC file 
 *******************************************************************/
void print_sdc_timescale(std::fstream& fp,
                         const std::string& timescale) {

  valid_file_stream(fp);

  fp << "#############################################" << std::endl;
  fp << "#\tDefine time unit " << std::endl;
  fp << "#############################################" << std::endl;
  fp << "set_units -time " << timescale << std::endl;
  fp << std::endl;
}

/********************************************************************
 * Write a port in SDC format
 *******************************************************************/
std::string generate_sdc_port(const BasicPort& port) {
  std::string sdc_line;

  std::string size_str = "[" + std::to_string(port.get_lsb()) + ":" + std::to_string(port.get_msb()) + "]";

  /* Only connection require a format of <port_name>[<lsb>:<msb>]
   * others require a format of <port_type> [<lsb>:<msb>] <port_name> 
   */
  /* When LSB == MSB, we can use a simplified format 
   * If LSB != 0, we need to give explicit pin number
   *   <port_type>[<lsb>]
   * Otherwise, we can keep a compact format
   *   <port_type>
   */
  if (1 == port.get_width()) {
    size_str = "[" + std::to_string(port.get_lsb()) + "]";
  }

  sdc_line = port.get_name() + size_str;

  return sdc_line;
}

/********************************************************************
 * Constrain a path between two ports of a module with a given maximum timing value
 *******************************************************************/
void print_pnr_sdc_constrain_max_delay(std::fstream& fp,
                                       const std::string& src_instance_name,
                                       const std::string& src_port_name,
                                       const std::string& des_instance_name,
                                       const std::string& des_port_name,
                                       const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_max_delay";

  fp << " -from ";
  if (!src_instance_name.empty()) {
    fp << format_dir_path(src_instance_name);
  }
  fp << src_port_name;

  fp << " -to ";
 
  if (!des_instance_name.empty()) {
    fp << format_dir_path(des_instance_name);
  }
  fp << des_port_name;

  fp << " " << std::setprecision(10) << delay;

  fp << std::endl;
}

/********************************************************************
 * Constrain a path between two ports of a module with a given maximum timing value
 * This function use regular expression and get_pins which are 
 * from open-source SDC 2.1 format
 *******************************************************************/
void print_pnr_sdc_regexp_constrain_max_delay(std::fstream& fp,
                                              const std::string& src_instance_name,
                                              const std::string& src_port_name,
                                              const std::string& des_instance_name,
                                              const std::string& des_port_name,
                                              const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_max_delay";

  fp << " -from ";
  fp << "[get_pins -regexp \"";
  if (!src_instance_name.empty()) {
    fp << format_dir_path(src_instance_name);
  }
  fp << src_port_name;

  fp << "\"]";

  fp << " -to ";
  fp << "[get_pins -regexp \"";
 
  if (!des_instance_name.empty()) {
    fp << format_dir_path(des_instance_name);
  }
  fp << des_port_name;

  fp << "\"]";

  fp << " " << std::setprecision(10) << delay;

  fp << std::endl;
}

/********************************************************************
 * Constrain a path between two ports of a module with a given minimum timing value
 *******************************************************************/
void print_pnr_sdc_constrain_min_delay(std::fstream& fp,
                                       const std::string& src_instance_name,
                                       const std::string& src_port_name,
                                       const std::string& des_instance_name,
                                       const std::string& des_port_name,
                                       const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_min_delay";

  fp << " -from ";
  if (!src_instance_name.empty()) {
    fp << format_dir_path(src_instance_name);
  }
  fp << src_port_name;

  fp << " -to ";
 
  if (!des_instance_name.empty()) {
    fp << format_dir_path(des_instance_name);
  }
  fp << des_port_name;

  fp << " " << std::setprecision(10) << delay;

  fp << std::endl;
}

/********************************************************************
 * Constrain a path between two ports of a module with a given timing value
 * Note: this function uses set_max_delay !!!
 *******************************************************************/
void print_pnr_sdc_constrain_module_port2port_timing(std::fstream& fp,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& input_parent_module_id, 
                                                     const ModulePortId& module_input_port_id, 
                                                     const ModuleId& output_parent_module_id, 
                                                     const ModulePortId& module_output_port_id, 
                                                     const float& tmax) {
  print_pnr_sdc_constrain_max_delay(fp,
                                    module_manager.module_name(input_parent_module_id),
                                    generate_sdc_port(module_manager.module_port(input_parent_module_id, module_input_port_id)),
                                    module_manager.module_name(output_parent_module_id),
                                    generate_sdc_port(module_manager.module_port(output_parent_module_id, module_output_port_id)),
                                    tmax);

}

/********************************************************************
 * Constrain a path between two ports of a module with a given timing value
 * This function will NOT output the module name
 * Note: this function uses set_max_delay !!!
 *******************************************************************/
void print_pnr_sdc_constrain_port2port_timing(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& input_parent_module_id, 
                                              const ModulePortId& module_input_port_id, 
                                              const ModuleId& output_parent_module_id, 
                                              const ModulePortId& module_output_port_id, 
                                              const float& tmax) {
  print_pnr_sdc_constrain_max_delay(fp,
                                    std::string(),
                                    generate_sdc_port(module_manager.module_port(input_parent_module_id, module_input_port_id)),
                                    std::string(),
                                    generate_sdc_port(module_manager.module_port(output_parent_module_id, module_output_port_id)),
                                    tmax);

}

/********************************************************************
 * Disable timing for a port 
 *******************************************************************/
void print_sdc_disable_port_timing(std::fstream& fp,
                                   const BasicPort& port) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_disable_timing ";

  fp << generate_sdc_port(port);

  fp << std::endl;
}

/********************************************************************
 * Set the input delay for a port in SDC format 
 * Note that the input delay will be bounded by a clock port
 *******************************************************************/
void print_sdc_set_port_input_delay(std::fstream& fp,
                                    const BasicPort& port,
                                    const BasicPort& clock_port,
                                    const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_input_delay ";

  fp << "-clock ";

  fp << generate_sdc_port(clock_port);

  fp << " -max ";
 
  fp << std::setprecision(10) << delay;

  fp << " ";

  fp << generate_sdc_port(port);

  fp << std::endl;
}

/********************************************************************
 * Set the output delay for a port in SDC format 
 * Note that the output delay will be bounded by a clock port
 *******************************************************************/
void print_sdc_set_port_output_delay(std::fstream& fp,
                                     const BasicPort& port,
                                     const BasicPort& clock_port,
                                     const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_output_delay ";

  fp << "-clock ";

  fp << generate_sdc_port(clock_port);

  fp << " -max ";
 
  fp << std::setprecision(10) << delay;

  fp << " ";

  fp << generate_sdc_port(port);

  fp << std::endl;
}

/********************************************************************
 * Print SDC commands to disable a given port of modules
 * in a given module id 
 * This function will be executed in a recursive way, 
 * using a Depth-First Search (DFS) strategy
 * It will iterate over all the configurable children under each module
 * and print a SDC command to disable its outputs
 *
 * Return code:
 *   0: success
 *   1: fatal error occurred
 *
 * Note:
 *   - When flatten_names is true
 *     this function will not apply any wildcard to names
 *   - When flatten_names is false
 *     It will straightforwardly output the instance name and port name
 *     This function will try to apply wildcard to names
 *     so that SDC file size can be minimal 
 *******************************************************************/
int rec_print_sdc_disable_timing_for_module_ports(std::fstream& fp, 
                                                  const bool& flatten_names,
                                                  const ModuleManager& module_manager, 
                                                  const ModuleId& parent_module,
                                                  const ModuleId& module_to_disable,
                                                  const std::string& parent_module_path,
                                                  const std::string& disable_port_name) {

  if (false == valid_file_stream(fp)) {
    return 1;
  }

  /* Build wildcard names for the instance names of multiple-instanced-blocks (MIB) 
   * We will find all the instance names and see there are common prefix 
   * If so, we can use wildcards
   */
  std::map<ModuleId, std::vector<std::string>> wildcard_names;

  /* For each child, we will go one level down in priority */
  for (const ModuleId& child_module : module_manager.child_modules(parent_module)) {

    /* Iterate over the child instances*/
    for (const size_t& child_instance : module_manager.child_module_instances(parent_module, child_module)) {
      std::string child_module_path = parent_module_path;

      std::string child_instance_name;
      if (true == module_manager.instance_name(parent_module, child_module, child_instance).empty()) {
        child_instance_name = generate_instance_name(module_manager.module_name(child_module), child_instance);
      } else {
        child_instance_name = module_manager.instance_name(parent_module, child_module, child_instance);
      }

      if (false == flatten_names) { 
        /* Try to adapt to a wildcard name: replace all the numbers with a wildcard character '*' */
        WildCardString wildcard_str(child_instance_name); 
        /* If the wildcard name is already in the list, we can skip this
         * Otherwise, we have to 
         *   - output this instance 
         *   - record the wildcard name in the map 
         */
        if ( (0 < wildcard_names.count(child_module)) 
          && (wildcard_names.at(child_module).end() != std::find(wildcard_names.at(child_module).begin(),
                                                                 wildcard_names.at(child_module).end(),
                                                                 wildcard_str.data())) ) {
          continue;
        }

        child_module_path += wildcard_str.data();
 
        wildcard_names[child_module].push_back(wildcard_str.data());
      } else {
        child_module_path += child_instance_name;
      }
      
      child_module_path = format_dir_path(child_module_path);

      /* If this is NOT the MUX module we want, we go recursively */
      if (module_to_disable != child_module) {
        int status = rec_print_sdc_disable_timing_for_module_ports(fp, flatten_names,
                                                                   module_manager, 
                                                                   child_module, 
                                                                   module_to_disable,
                                                                   child_module_path,
                                                                   disable_port_name);
        if (1 == status) {
          return 1; /* FATAL ERRORS */
        }
        continue;
      }

      /* Validate file stream */
      valid_file_stream(fp);

      /* Reach here, this is the MUX module we want, disable the outputs */
      ModulePortId port_to_disable = module_manager.find_module_port(module_to_disable, disable_port_name);
      if (ModulePortId::INVALID() == port_to_disable) {
        return 1; /* FATAL ERRORS */
      }
      fp << "set_disable_timing ";
      fp << child_module_path << module_manager.module_port(module_to_disable, port_to_disable).get_name();
      fp << std::endl;
    }
  }

  return 0; /* Success */
}

} /* end namespace openfpga */
