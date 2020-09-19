/************************************************
 * This file includes functions on 
 * outputting wrapper SPICE netlists for transistor
 ***********************************************/
#include <fstream>
#include <cmath>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "circuit_library_utils.h"

#include "spice_constants.h"
#include "spice_writer_utils.h"
#include "spice_transistor_wrapper.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print a SPICE model wrapper for a transistor model
 *******************************************************************/
static 
int print_spice_transistor_model_wrapper(std::fstream& fp,
                                         const TechnologyLibrary& tech_lib,
                                         const TechnologyModelId& model) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Transistor model followed a fixed port mapping
   * [X|M]<MODEL_CARD_NAME> <DRAIN> <GATE> <SOURCE> <BULK> 
   * which is a standard in SPICE modeling
   * We will output the pmos and nmos transistors wrappers
   * which are defined in this model
   */
  for (int itype = TECH_LIB_TRANSISTOR_PMOS;
       itype < NUM_TECH_LIB_TRANSISTOR_TYPES;
       ++itype) {
    const e_tech_lib_transistor_type& trans_type = static_cast<e_tech_lib_transistor_type>(itype); 
    fp << ".subckt ";
    fp << tech_lib.transistor_model_name(model, trans_type) << TRANSISTOR_WRAPPER_POSTFIX; 
    fp << " drain gate source bulk";
    fp << " L=" << std::setprecision(10) << tech_lib.transistor_model_chan_length(model, trans_type); 
    fp << " W=" << std::setprecision(10) << tech_lib.transistor_model_min_width(model, trans_type); 
    fp << "\n";

    fp << tech_lib.model_ref(model);
    fp << "1";
    fp << " drain gate source bulk";
    fp << " " << tech_lib.transistor_model_name(model, trans_type);
    fp << " L=L W=W";
    fp << "\n";

    fp << ".ends";
    fp << "\n";
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Generate the SPICE netlist for transistors
 *******************************************************************/
int print_spice_transistor_wrapper(NetlistManager& netlist_manager,
                                   const TechnologyLibrary& tech_lib,
                                   const std::string& submodule_dir) {
  std::string spice_fname = submodule_dir + std::string(TRANSISTORS_SPICE_FILE_NAME);

  std::fstream fp;

  /* Create the file stream */
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_stream(spice_fname.c_str(), fp); 

  /* Create file */
  VTR_LOG("Generating SPICE netlist '%s' for transistors...",
          spice_fname.c_str()); 

  print_spice_file_header(fp, std::string("Transistor wrappers"));

  /* Iterate over the transistor models */
  for (const TechnologyModelId& model : tech_lib.models()) {
    /* Focus on transistor model */
    if (TECH_LIB_MODEL_TRANSISTOR != tech_lib.model_type(model)) {
      continue;
    }
    /* Write a wrapper for the transistor model */
    if (CMD_EXEC_SUCCESS != print_spice_transistor_model_wrapper(fp, tech_lib, model)) {
      return CMD_EXEC_FATAL_ERROR;
    }
  } 

  /* Close file handler*/
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Generate the SPICE modeling for the PMOS part of a logic gate
 *
 * This function is created to be shared by pass-transistor and
 * transmission-gate SPICE netlist writer
 *
 * Note: 
 * - This function does NOT create a file
 *   but requires a file stream created
 * - This function only output SPICE modeling for 
 *   a PMOS. Any preprocessing or subckt definition should not be included!
 *******************************************************************/
int print_spice_generic_pmos_modeling(std::fstream& fp,
                                      const std::string& trans_name_postfix,
                                      const std::string& input_port_name,
                                      const std::string& gate_port_name,
                                      const std::string& output_port_name,
                                      const TechnologyLibrary& tech_lib,
                                      const TechnologyModelId& tech_model,
                                      const float& trans_width) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Write transistor pairs using the technology model */
  fp << "Xpmos_" << trans_name_postfix << " ";
  fp << input_port_name << " "; 
  fp << gate_port_name << " "; 
  fp << output_port_name << " "; 
  fp << SPICE_SUBCKT_VDD_PORT_NAME << " "; 
  fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_PMOS) << TRANSISTOR_WRAPPER_POSTFIX; 
  fp << " W=" << std::setprecision(10) << trans_width;
  fp << "\n";

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Generate the SPICE modeling for the NMOS part of a logic gate
 *
 * Note: 
 * - This function does NOT create a file
 *   but requires a file stream created
 * - This function only output SPICE modeling for 
 *   a NMOS. Any preprocessing or subckt definition should not be included!
 *******************************************************************/
int print_spice_generic_nmos_modeling(std::fstream& fp,
                                      const std::string& trans_name_postfix,
                                      const std::string& input_port_name,
                                      const std::string& gate_port_name,
                                      const std::string& output_port_name,
                                      const TechnologyLibrary& tech_lib,
                                      const TechnologyModelId& tech_model,
                                      const float& trans_width) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  fp << "Xnmos_" << trans_name_postfix << " ";
  fp << input_port_name << " "; 
  fp << gate_port_name << " "; 
  fp << output_port_name << " "; 
  fp << SPICE_SUBCKT_GND_PORT_NAME << " "; 
  fp << tech_lib.transistor_model_name(tech_model, TECH_LIB_TRANSISTOR_NMOS) << TRANSISTOR_WRAPPER_POSTFIX; 
  fp << " W=" << std::setprecision(10) << trans_width;
  fp << "\n";

  return CMD_EXEC_SUCCESS;
}


} /* end namespace openfpga */
