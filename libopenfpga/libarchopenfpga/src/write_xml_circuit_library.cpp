/********************************************************************
 * This file includes functions that outputs a circuit library to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

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
}

/********************************************************************
 * A writer to output the device technology of a circuit model to XML format
 *******************************************************************/
static 
void write_xml_device_technology(std::fstream& fp,
                                 const char* fname,
                                 const CircuitLibrary& circuit_lib,
                                 const CircuitModelId& model) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
   
  if (!circuit_lib.device_model_name(model).empty()) {
    fp << "\t\t\t" << "<device_technology";
    write_xml_attribute(fp, "device_model_name", circuit_lib.device_model_name(model).c_str());
    /* Finish all the attributes, we can return here */
    fp << "/>" << "\n";
  }
}

/********************************************************************
 * A writer to output a circuit port to XML format
 *******************************************************************/
static 
void write_xml_circuit_port(std::fstream& fp,
                            const char* fname,
                            const CircuitLibrary& circuit_lib,
                            const CircuitPortId& port) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Get the parent circuit model for this port */
  const CircuitModelId& model = circuit_lib.port_parent_model(port);

  /* Generic information about a port */
  fp << "\t\t\t" << "<port";
  write_xml_attribute(fp, "type", CIRCUIT_MODEL_PORT_TYPE_STRING[circuit_lib.port_type(port)]); 
  write_xml_attribute(fp, "prefix", circuit_lib.port_prefix(port).c_str()); 
 
  /* Output the lib_name when it is different than prefix */
  if (circuit_lib.port_prefix(port) != circuit_lib.port_lib_name(port)) {
    write_xml_attribute(fp, "lib_name", circuit_lib.port_lib_name(port).c_str()); 
  } 

  write_xml_attribute(fp, "size", std::to_string(circuit_lib.port_size(port)).c_str()); 

  /* Output the default value only when this is a global port */
  if (true == circuit_lib.port_is_global(port)) {
    write_xml_attribute(fp, "default_val", std::to_string(circuit_lib.port_default_value(port)).c_str()); 
  }

  /* SRAM port may be for mode selection. Output it only when it is true */
  if (CIRCUIT_MODEL_PORT_SRAM == circuit_lib.port_type(port)) {
    if (true == circuit_lib.port_is_mode_select(port)) {
      write_xml_attribute(fp, "mode_select", "true"); 
    }
  }

  /* Output the tri-state map of the port 
   * This is only applicable to input ports of a LUT
   */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model))
    && (CIRCUIT_MODEL_PORT_INPUT == circuit_lib.port_type(port)) ) {
    if (!circuit_lib.port_tri_state_map(port).empty()) {
      write_xml_attribute(fp, "tri_state_map", circuit_lib.port_tri_state_map(port).c_str()); 
    }
  }

  /* Identify the fracturable-level of the port in LUTs, by default it will be -1
   * This is only applicable to output ports of a LUT
   */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model))
    && (CIRCUIT_MODEL_PORT_OUTPUT == circuit_lib.port_type(port)) ) {
    if (size_t(-1) != circuit_lib.port_lut_frac_level(port)) {
      write_xml_attribute(fp, "lut_frac_level", std::to_string(circuit_lib.port_lut_frac_level(port)).c_str()); 
    }
  }

  /* Output the output mask of the port 
   * This is only applicable to output ports of a LUT
   */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model))
    && (CIRCUIT_MODEL_PORT_OUTPUT == circuit_lib.port_type(port)) ) {
    std::string output_mask_string;
    for (const size_t& mask : circuit_lib.port_lut_output_mask(port)) {
      if (!output_mask_string.empty()) {
        output_mask_string += std::string(",");
      }
      output_mask_string += std::to_string(mask);
    }
    if (!output_mask_string.empty()) {
      write_xml_attribute(fp, "lut_output_mask", output_mask_string.c_str()); 
    }
  }

  /* Global, reset, set port attributes */
  if (true == circuit_lib.port_is_global(port)) {
    write_xml_attribute(fp, "is_global", "true"); 
  }

  if (true == circuit_lib.port_is_reset(port)) {
    write_xml_attribute(fp, "is_reset", "true"); 
  }

  if (true == circuit_lib.port_is_set(port)) {
    write_xml_attribute(fp, "is_set", "true"); 
  }

  if (true == circuit_lib.port_is_prog(port)) {
    write_xml_attribute(fp, "is_prog", "true"); 
  }

  if (true == circuit_lib.port_is_config_enable(port)) {
    write_xml_attribute(fp, "is_config_enable", "true"); 
  }

  if (true == circuit_lib.port_is_edge_triggered(port)) {
    write_xml_attribute(fp, "is_edge_triggered", "true"); 
  }

  /* Output the name of circuit model that this port is linked to */
  if (!circuit_lib.port_tri_state_model_name(port).empty()) {
    write_xml_attribute(fp, "circuit_model_name", circuit_lib.port_tri_state_model_name(port).c_str());
  }

  /* Find the name of circuit model that port is used for inversion of signals,
   * This is only applicable to BL/WL/BLB/WLB ports
   */
  if (  (CIRCUIT_MODEL_PORT_BL  == circuit_lib.port_type(port))
     || (CIRCUIT_MODEL_PORT_WL  == circuit_lib.port_type(port))
     || (CIRCUIT_MODEL_PORT_BLB == circuit_lib.port_type(port))
     || (CIRCUIT_MODEL_PORT_WLB == circuit_lib.port_type(port)) ) {
    if (!circuit_lib.port_inv_model_name(port).empty()) {
      write_xml_attribute(fp, "inv_circuit_model_name", circuit_lib.port_inv_model_name(port).c_str());
    }
  }

  /* Finish all the attributes, we can return here */
  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output wire parasitics of a circuit model to XML format
 *******************************************************************/
static 
void write_xml_wire_param(std::fstream& fp,
                          const char* fname,
                          const CircuitLibrary& circuit_lib,
                          const CircuitModelId& model) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<wire_param"; 

  write_xml_attribute(fp, "model_type", WIRE_MODEL_TYPE_STRING[circuit_lib.wire_type(model)]); 
  write_xml_attribute(fp, "R", std::to_string(circuit_lib.wire_r(model)).c_str()); 
  write_xml_attribute(fp, "C", std::to_string(circuit_lib.wire_c(model)).c_str()); 
  write_xml_attribute(fp, "num_level", std::to_string(circuit_lib.wire_num_level(model)).c_str()); 

  /* Finish all the attributes, we can return here */
  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output delay matrices of a circuit model to XML format
 *******************************************************************/
static 
void write_xml_delay_matrix(std::fstream& fp,
                            const char* fname,
                            const CircuitLibrary& circuit_lib,
                            const CircuitModelId& model) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  std::vector<CircuitPortId> in_ports;
  std::vector<CircuitPortId> out_ports;
  /* Collect the input ports and output ports */
  for (const auto& timing_edge : circuit_lib.timing_edges_by_model(model)) {
     /* For each input port of each edge, build a list of unique ports */
     CircuitPortId src_port = circuit_lib.timing_edge_src_port(timing_edge);
     if (in_ports.end() == std::find(in_ports.begin(), in_ports.end(), src_port)) {
       in_ports.push_back(src_port);
     }

     /* For each input port of each edge, build a list of unique ports */
     CircuitPortId sink_port = circuit_lib.timing_edge_sink_port(timing_edge);
     if (out_ports.end() == std::find(out_ports.begin(), out_ports.end(), sink_port)) {
       out_ports.push_back(sink_port);
     }
  }

  /* Build the string of in_port list */
  std::string in_port_string;
  for (const CircuitPortId& in_port : in_ports) {
    if (!in_port_string.empty()) {
      in_port_string += std::string(" ");
    }
    in_port_string += circuit_lib.port_prefix(in_port);
  } 

  /* Build the string of out_port list */
  std::string out_port_string;
  for (const CircuitPortId& out_port : out_ports) {
    if (!out_port_string.empty()) {
      out_port_string += std::string(" ");
    }
    out_port_string += circuit_lib.port_prefix(out_port);
  } 

  /* Output rising edges */
  fp << "\t\t\t";
  fp << "<delay_matrix";
  write_xml_attribute(fp, "type", CIRCUIT_MODEL_DELAY_TYPE_STRING[CIRCUIT_MODEL_DELAY_RISE]); 
  write_xml_attribute(fp, "in_port", in_port_string.c_str()); 
  write_xml_attribute(fp, "out_port", out_port_string.c_str()); 
  fp << ">\n";
  
  for (const CircuitPortId& out_port : out_ports) {
    for (const size_t& out_pin : circuit_lib.pins(out_port)) {
      fp << "\t\t\t\t";
      size_t counter = 0; /* Count the numbers of delays in one line */ 
      for (const CircuitPortId& in_port : in_ports) {
        for (const size_t& in_pin : circuit_lib.pins(in_port)) {
          for (const auto& timing_edge : circuit_lib.timing_edges_by_model(model)) {
            CircuitPortId src_port = circuit_lib.timing_edge_src_port(timing_edge);
            size_t src_pin = circuit_lib.timing_edge_src_pin(timing_edge);

            CircuitPortId sink_port = circuit_lib.timing_edge_sink_port(timing_edge);
            size_t sink_pin = circuit_lib.timing_edge_sink_pin(timing_edge);

            /* Bypass unwanted edges */
            if ( (src_port != in_port)
              || (src_pin != in_pin)
              || (sink_port != out_port)
              || (sink_pin != out_pin) ) {
              continue;
            }
            /* This is the edge we want, output the rise delay */
            if (0 < counter) {
              fp << std::string(" ");
            }
            fp << std::scientific << circuit_lib.timing_edge_delay(timing_edge, CIRCUIT_MODEL_DELAY_RISE);
            counter++;
          }
        }
      }
      /* One line of delay matrix finished here, output to the file */
      fp << "\n";
    }
  }

  fp << "\t\t\t";
  fp << "</delay_matrix>" << "\n";

  /* Output falling edges */
  fp << "\t\t\t";
  fp << "<delay_matrix";
  write_xml_attribute(fp, "type", CIRCUIT_MODEL_DELAY_TYPE_STRING[CIRCUIT_MODEL_DELAY_FALL]); 
  write_xml_attribute(fp, "in_port", in_port_string.c_str()); 
  write_xml_attribute(fp, "out_port", out_port_string.c_str()); 
  fp << ">\n";
  
  for (const CircuitPortId& out_port : out_ports) {
    for (const size_t& out_pin : circuit_lib.pins(out_port)) {
      fp << "\t\t\t\t";
      size_t counter = 0; /* Count the numbers of delays in one line */ 
      for (const CircuitPortId& in_port : in_ports) {
        for (const size_t& in_pin : circuit_lib.pins(in_port)) {
          for (const auto& timing_edge : circuit_lib.timing_edges_by_model(model)) {
            CircuitPortId src_port = circuit_lib.timing_edge_src_port(timing_edge);
            size_t src_pin = circuit_lib.timing_edge_src_pin(timing_edge);

            CircuitPortId sink_port = circuit_lib.timing_edge_sink_port(timing_edge);
            size_t sink_pin = circuit_lib.timing_edge_sink_pin(timing_edge);

            /* Bypass unwanted edges */
            if ( (src_port != in_port)
              || (src_pin != in_pin)
              || (sink_port != out_port)
              || (sink_pin != out_pin) ) {
              continue;
            }
            /* This is the edge we want, output the rise delay */
            if (0 < counter) {
              fp << std::string(" ");
            }
            fp << std::scientific << circuit_lib.timing_edge_delay(timing_edge, CIRCUIT_MODEL_DELAY_FALL);
            counter++;
          }
        }
      }
      /* One line of delay matrix finished here, output to the file */
      fp << "\n";
    }
  }

  fp << "\t\t\t";
  fp << "</delay_matrix>" << "\n";
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
  if (!circuit_lib.model_spice_netlist(model).empty()) {
    write_xml_attribute(fp, "spice_netlist", circuit_lib.model_spice_netlist(model).c_str()); 
  }
  if (!circuit_lib.model_verilog_netlist(model).empty()) {
    write_xml_attribute(fp, "verilog_netlist", circuit_lib.model_verilog_netlist(model).c_str()); 
  }
  fp << ">" << "\n";

  /* Write the design technology of circuit model */
  write_xml_design_technology(fp, fname, circuit_lib, model);

  /* Write the device technology of circuit model */
  write_xml_device_technology(fp, fname, circuit_lib, model);

  /* Write the input buffer information of circuit model, 
   * only applicable when this circuit model is neither inverter nor buffer
   */
  if (CIRCUIT_MODEL_INVBUF != circuit_lib.model_type(model)) {
    if (true == circuit_lib.is_input_buffered(model)) {
      fp << "\t\t\t" << "<input_buffer";
      write_xml_attribute(fp, "exist", circuit_lib.is_input_buffered(model)); 
      write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(circuit_lib.input_buffer_model(model)).c_str()); 
      fp << "/>" << "\n";
    }
  }

  /* Write the output buffer information of circuit model */
  if (CIRCUIT_MODEL_INVBUF != circuit_lib.model_type(model)) {
    if (true == circuit_lib.is_output_buffered(model)) {
      fp << "\t\t\t" << "<output_buffer";
      write_xml_attribute(fp, "exist", circuit_lib.is_input_buffered(model)); 
      write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(circuit_lib.output_buffer_model(model)).c_str()); 
      fp << "/>" << "\n";
    }
  }

  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model)) {
    /* Write the lut input buffer information of circuit model
     * This is a mandatory attribute for LUT, so it must exist 
     */
    fp << "\t\t\t" << "<lut_input_buffer";
    write_xml_attribute(fp, "exist", true); 
    write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(circuit_lib.lut_input_buffer_model(model)).c_str()); 
    fp << "/>" << "\n";

    /* Write the lut input inverter information of circuit model
     * This is a mandatory attribute for LUT, so it must exist 
     */
    fp << "\t\t\t" << "<lut_input_inverter";
    write_xml_attribute(fp, "exist", true); 
    write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(circuit_lib.lut_input_inverter_model(model)).c_str()); 
    fp << "/>" << "\n";

    /* Write the lut intermediate buffer information of circuit model */
    if (true == circuit_lib.is_lut_intermediate_buffered(model)) {
      fp << "\t\t\t" << "<lut_intermediate_buffer";
      write_xml_attribute(fp, "exist", circuit_lib.is_lut_intermediate_buffered(model)); 
      write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(circuit_lib.lut_intermediate_buffer_model(model)).c_str()); 
      if (!circuit_lib.lut_intermediate_buffer_location_map(model).empty()) {
        write_xml_attribute(fp, "location_map", circuit_lib.lut_intermediate_buffer_location_map(model).c_str());
      }
      fp << "/>" << "\n";
    }
  }

  /* Write the pass-gate-logic information of circuit model */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model)) 
    || (CIRCUIT_MODEL_MUX == circuit_lib.model_type(model)) ) {
    fp << "\t\t\t" << "<pass_gate_logic";
    write_xml_attribute(fp, "circuit_model_name", circuit_lib.model_name(circuit_lib.pass_gate_logic_model(model)).c_str()); 
    fp << "/>" << "\n";
  }

  /* Write the ports of circuit model */
  for (const CircuitPortId& port : circuit_lib.model_ports(model)) {
    write_xml_circuit_port(fp, fname, circuit_lib, port);
  }

  /* Write the wire parasticis of circuit model */
  if ( (CIRCUIT_MODEL_WIRE == circuit_lib.model_type(model))
    || (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) ) {
    write_xml_wire_param(fp, fname, circuit_lib, model);
  }

  /* Write the delay matrix of circuit model
   * Skip circuit models without delay matrices 
   */
  if (0 < circuit_lib.num_delay_info(model)) {
    write_xml_delay_matrix(fp, fname, circuit_lib, model);
  }

  /* Put an end to the XML definition of this circuit model */
  fp << "\t\t" << "</circuit_model>\n";
}

/********************************************************************
 * A writer to output a circuit library to XML format
 * Note: 
 * This function should be run after that the following methods of 
 * CircuitLibrary are executed 
 * 1. build_model_links();
 * 2. build_timing_graph();
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
