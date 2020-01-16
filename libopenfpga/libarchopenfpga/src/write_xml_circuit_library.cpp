/********************************************************************
 * This file includes functions that outputs a circuit library to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>

/* Headers from vtr util library */
#include "vtr_log.h"
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_utils.h" 
#include "write_xml_circuit_library.h"

/********************************************************************
 * A writer to output the design technology of a circuit model to XML format
 *******************************************************************/
static 
void write_xml_design_technology(std::fstream& fp,
                                 const char* fname,
                                 const CircuitLibrary& circuit_lib,
                                 const CircuitModelId& model) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<design_technology";
  write_xml_attribute(fp, "type", CIRCUIT_MODEL_DESIGN_TECH_TYPE_STRING[circuit_lib.design_tech_type(model)]);

  if (true == circuit_lib.is_power_gated(model)) {
    write_xml_attribute(fp, "power_gated", "true");
  }

  /* Buffer-related information */
  if (CIRCUIT_MODEL_INVBUF == circuit_lib.model_type(model)) {
    write_xml_attribute(fp, "topology", CIRCUIT_MODEL_BUFFER_TYPE_STRING[circuit_lib.buffer_type(model)]); 
    write_xml_attribute(fp, "size", std::to_string(circuit_lib.buffer_size(model)).c_str()); 
    if (1 < circuit_lib.buffer_num_levels(model)) {
      write_xml_attribute(fp, "num_level", std::to_string(circuit_lib.buffer_num_levels(model)).c_str()); 
      write_xml_attribute(fp, "f_per_stage", std::to_string(circuit_lib.buffer_f_per_stage(model)).c_str()); 
    }
  }

  /* Pass-gate-logic -related information */
  if (CIRCUIT_MODEL_PASSGATE == circuit_lib.model_type(model)) {
    write_xml_attribute(fp, "topology", CIRCUIT_MODEL_PASSGATE_TYPE_STRING[circuit_lib.pass_gate_logic_type(model)]); 
    write_xml_attribute(fp, "pmos_size", std::to_string(circuit_lib.pass_gate_logic_pmos_size(model)).c_str()); 
    write_xml_attribute(fp, "nmos_size", std::to_string(circuit_lib.pass_gate_logic_nmos_size(model)).c_str()); 
  }

  /* Look-Up Table (LUT)-related information */
  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model)) {
    if (true == circuit_lib.is_lut_fracturable(model)) {
      write_xml_attribute(fp, "fracturable_lut", "true");
    }
  }

  /* Multiplexer-related information */
  if (CIRCUIT_MODEL_MUX == circuit_lib.model_type(model)) {
    write_xml_attribute(fp, "structure", CIRCUIT_MODEL_STRUCTURE_TYPE_STRING[circuit_lib.mux_structure(model)]); 
    if (true == circuit_lib.mux_add_const_input(model)) {
      write_xml_attribute(fp, "add_const_input", "true"); 
      write_xml_attribute(fp, "const_input_val", std::to_string(circuit_lib.mux_const_input_value(model)).c_str()); 
    }
    if (CIRCUIT_MODEL_STRUCTURE_MULTILEVEL == circuit_lib.mux_structure(model)) {
      write_xml_attribute(fp, "num_level", std::to_string(circuit_lib.mux_num_levels(model)).c_str()); 
    }
    if (true == circuit_lib.mux_use_advanced_rram_design(model)) {
      write_xml_attribute(fp, "advanced_rram_design", "true"); 
    }
    if (true == circuit_lib.mux_use_local_encoder(model)) {
      write_xml_attribute(fp, "local_encoder", "true"); 
    }
  }

  /* Gate-related information */
  if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(model)) {
    write_xml_attribute(fp, "topology", CIRCUIT_MODEL_GATE_TYPE_STRING[circuit_lib.gate_type(model)]); 
  }
 
  /* ReRAM-related information */
  if (CIRCUIT_MODEL_DESIGN_RRAM == circuit_lib.design_tech_type(model)) {
    write_xml_attribute(fp, "ron", std::to_string(circuit_lib.rram_rlrs(model)).c_str()); 
    write_xml_attribute(fp, "roff", std::to_string(circuit_lib.rram_rhrs(model)).c_str()); 
    write_xml_attribute(fp, "wprog_set_pmos", std::to_string(circuit_lib.rram_wprog_set_pmos(model)).c_str()); 
    write_xml_attribute(fp, "wprog_set_nmos", std::to_string(circuit_lib.rram_wprog_set_nmos(model)).c_str()); 
    write_xml_attribute(fp, "wprog_reset_pmos", std::to_string(circuit_lib.rram_wprog_reset_pmos(model)).c_str()); 
    write_xml_attribute(fp, "wprog_reset_nmos", std::to_string(circuit_lib.rram_wprog_reset_nmos(model)).c_str()); 
  }

  /* Finish all the attributes, we can return here */
  fp << "/>" << "\n";
  return;
}

/********************************************************************
 * A writer to output a circuit model to XML format
 *******************************************************************/
static 
void write_xml_circuit_model(std::fstream& fp,
                             const char* fname,
                             const CircuitLibrary& circuit_lib,
                             const CircuitModelId& model) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the definition of circuit model */
  fp << "\t\t" << "<circuit_model";
  write_xml_attribute(fp, "type", CIRCUIT_MODEL_TYPE_STRING[circuit_lib.model_type(model)]); 
  write_xml_attribute(fp, "name", circuit_lib.model_name(model).c_str()); 
  write_xml_attribute(fp, "prefix", circuit_lib.model_prefix(model).c_str()); 
  if (true == circuit_lib.model_is_default(model)) { 
    write_xml_attribute(fp, "is_default", "true"); 
  }
  if (true == circuit_lib.dump_structural_verilog(model)) { 
    write_xml_attribute(fp, "dump_structural_verilog", "true"); 
  }
  if (!circuit_lib.model_circuit_netlist(model).empty()) {
    write_xml_attribute(fp, "circuit_netlist", circuit_lib.model_circuit_netlist(model).c_str()); 
  }
  if (!circuit_lib.model_verilog_netlist(model).empty()) {
    write_xml_attribute(fp, "verilog_netlist", circuit_lib.model_verilog_netlist(model).c_str()); 
  }
  fp << ">" << "\n";

  /* Write the design technology of circuit model */
  write_xml_design_technology(fp, fname, circuit_lib, model);

  /* TODO: Write the ports of circuit model */

  /* TODO: Write the wire parasticis of circuit model */

  /* TODO: Write the delay matrix of circuit model */

  /* Put an end to the XML definition of this circuit model */
  fp << "\t\t" << "</circuit_model>\n";
}

/********************************************************************
 * A writer to output a circuit library to XML format
 *******************************************************************/
void write_xml_circuit_library(std::fstream& fp,
                               const char* fname,
                               const CircuitLibrary& circuit_lib) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node for circuit_library, 
   * we apply a tab becuase circuit library is a subnode 
   * under the root node <openfpga_arch>
   */
  fp << "\t" << "<circuit_library>" << "\n";

  /* Write circuit model one by one */ 
  for (const CircuitModelId& model : circuit_lib.models()) {
    write_xml_circuit_model(fp, fname, circuit_lib, model);
  } 

  /* Write the root node for circuit_library */
  fp << "\t" << "</circuit_library>" << "\n";
}
