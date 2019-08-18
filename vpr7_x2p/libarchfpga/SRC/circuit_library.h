/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    circuit_library.h
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/06  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef CIRCUIT_LIBRARY_H
#define CIRCUIT_LIBRARY_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <string>

#include "vtr_strong_id.h"
#include "vtr_geometry.h"

#include "vtr_vector.h"
#include "vtr_range.h"

#include "circuit_types.h"

/************************************************************************
 * Create strong id for Circuit Models/Ports to avoid illegal type casting 
 ***********************************************************************/
struct circuit_model_id_tag;
struct circuit_port_id_tag;
struct circuit_edge_id_tag;

typedef vtr::StrongId<circuit_model_id_tag> CircuitModelId;
typedef vtr::StrongId<circuit_port_id_tag> CircuitPortId;
typedef vtr::StrongId<circuit_edge_id_tag> CircuitEdgeId;


/* Alias for open ids */
#define CIRCUIT_MODEL_OPEN_ID CircuitModelId(-1)
#define CIRCUIT_PORT_OPEN_ID CircuitPortId(-1)
#define CIRCUIT_EDGE_OPEN_ID CircuitEdgeId(-1)

/************************************************************************
 * The class CircuitLibrary is a critical data structure for OpenFPGA
 * It stores all the circuit-level details from XML architecture file
 *
 * It includes the following data:
 *
 *  ------ Fundamental Information -----
 * 1. circuit_model_ids_ : unique identifier to find a circuit model
 *                         Use a strong id for search, to avoid illegal type casting
 * 2. circuit_model_types_: types of the circuit model, see details in the definition of enum e_spice_model_type
 * 3. circuit_model_names_: unique names for each circuit models. 
 *                          It should be the same as user-defined Verilog modules, if it is not auto-generated
 * 4. circuit_model_prefix_: the prefix of a circuit model when it is instanciated  
 * 5. verilog_netlist_: specified path and file name of Verilog netlist if a circuit model is not auto-generated
 * 6. spice_netlist_: specified path and file name of SPICE netlist if a circuit model is not auto-generated
 * 7. is_default_: indicate if the circuit model is the default one among all those in the same type 
 *
 *  ------ Fast look-ups-----
 *  1. circuit_model_lookup_: A multi-dimension vector to provide fast look-up on circuit models for users 
 *                            It classifies CircuitModelIds by their type and set the default model in the first element for each type.
 *  2. circuit_model_port_lookup_: A multi-dimension vector to provide fast look-up on ports of circuit models for users
 *                                 It classifies Ports by their types 
 *
 *  ------ Verilog generation options -----
 * 1. dump_structural_verilog_: if Verilog generator will output structural Verilog syntax for the circuit model
 * 2. dump_explicit_port_map_: if Verilog generator will use explicit port mapping when instanciate the circuit model
 *
 *  ------ Design technology information -----
 * 1. design_tech_types_: the design technology [cmos|rram] for each circuit model 
 * 2. is_power_gated_: specify if the circuit model is power-gated (contain a input to turn on/off VDD and GND) 
 *
 *  ------ Buffer existence -----
 *  Use vectors to simplify the defition of buffer existence:
 *  index (low=0 to high) represents INPUT, OUTPUT, LUT_INPUT_BUF, LUT_INPUT_INV, LUT_INTER_BUFFER
 *  1. buffer_existence_: specify if this circuit model has an buffer 
 *  2. buffer_circuit_model_name_: specify the name of circuit model for the buffer 
 *  3. buffer_circuit_model_id_: specify the id of circuit model for the buffer 
 *
 *  ------ Pass-gate-related parameters ------
 *  1. pass_gate_logic_circuit_model_name_: specify the name of circuit model for the pass gate logic 
 *  2. pass_gate_logic_circuit_model_id_: specify the id of circuit model for the pass gate logic 
 *
 *  ------ Port information ------
 * 1. port_ids_: unique id of ports belonging to a circuit model 
 * 2. port_types_: types of ports belonging to a circuit model 
 * 3. port_sizes_: width of ports belonging to a circuit model
 * 4. port_prefix_: prefix of a port when instance of a circuit model 
 * 5. port_lib_names_: port name in the standard cell library, only used when explicit_port_mapping is enabled   
 * 6. port_inv_prefix_: the prefix to be added for the inverted port. This is mainly used by SRAM ports, which have an coupled inverterd port 
 * 7. port_is_mode_select: specify if this port is used to select operating modes of the circuit model  
 * 8. port_is_global: specify if this port is a global signal shared by other circuit model
 * 9. port_is_reset: specify if this port is a reset signal which needs special pulse widths in testbenches 
 * 10. port_is_set: specify if this port is a set signal which needs special pulse widths in testbenches 
 * 11. port_is_config_enable: specify if this port is a config_enable signal which needs special pulse widths in testbenches 
 * 12. port_is_prog: specify if this port is for FPGA programming use which needs special pulse widths in testbenches 
 * 13. port_circuit_model_name: the name of circuit model linked to the port  
 * 14. port_circuit_model_ids_: the Id of circuit model linked to the port 
 * 15. port_inv_circuit_model_names_: the name of inverter circuit model linked to the port 
 * 16. port_inv_circuit_model_ids_: the Id of inverter circuit model linked to the port
 * 17. port_tri_state_map_: only applicable to inputs of LUTs, the tri-state map applied to each pin of this port 
 * 18. port_lut_frac_level_:  only applicable to outputs of LUTs, indicate which level of outputs inside LUT multiplexing structure will be used
 * 19. port_lut_output_mask_: only applicable to outputs of LUTs, indicate which output at an internal level of LUT multiplexing structure will be used
 * 20. port_sram_orgz_: only applicable to SRAM ports, indicate how the SRAMs will be organized, either memory decoders or scan-chains
 *
 *  ------ Delay information ------
 * 1. delay_types_: type of pin-to-pin delay, either rising_edge of falling_edge
 * 2. delay_in_port_names_: name of input ports that the pin-to-pin delay is linked to
 * 3. delay_in_port_names_: name of output ports that the pin-to-pin delay is linked to
 * 4. delay_values_: delay values of the pin-to-pin delay 
 *
 *  ------ Timing graph information: TODO: consider using tatum? ------
 *  Timing graph is allocated when delay information is made
 *  1. edge_ids_ : ids of edges in the timing graph  
 *  2. port_in_edge_ids_: ids of input edges for each pin of a circuit port  
 *  3. port_out_edge_ids_: ids of output edges for each pin of a circuit port  
 *  4. edge_src_port_ids_: ids of source ports that each edge is connected to 
 *  5. edge_src_pin_ids_: ids of source pin that each edge is connected to 
 *  6. edge_sink_port_ids_: ids of sink ports that each edge is connected to 
 *  7. edge_sink_pin_ids_: ids of sink pin that each edge is connected to 
 *  8. edge_trise_: rising delay of the edge 
 *  9. edge_tfall_: falling delay of the edge 
 *
 *  ------ Buffer/Inverter-related parameters ------
 *  Note: only applicable to circuit models whose type is buffer or inverter
 *  1. buffer_types_: type of the buffer, either buffer or inverter
 *  2. buffer_location_maps_: location of the buffer, only applicable to LUTs
 *  3. buffer_sizes_: size of buffer (transistor size for the first stage)
 *  4. buffer_is_tapered_: specify if this buffer has multiple stages 
 *  5. buffer_num_levels: specify the number of levels of this buffer (if this is defined as multi-level buffer) 
 *  6. buffer_f_per_stage: specify the driving strength of the buffer by stage
 *
 *  ------ Pass-gate-logic-related parameters ------
 *  Note: only applicable to circuit models whose type is pass-gate-logic
 *  1. pass_gate_logic_types_: types of the pass-gate-logic, either transmission-gate or pass-transistor
 *  2. pass_gate_logic_nmos_sizes_: size of NMOS transistor in the pass-gate-logic 
 *  3. pass_gate_logic_pmos_sizes_: size of PMOS transistor in the pass-gate-logic, only applicable for transmission-gates
 *
 *  ------ Multiplexer-related parameters ------
 *  Note: only applicable to circuit models whose type is MUX
 *  1. mux_structure_: specify the structure of a multiplexer, one-level, multi-level or tree-like
 *  2. mux_num_levels_: specify the number of levels for a multiplexer 
 *  3. mux_add_const_input_: specify if this multiplexer has a constant input
 *  4. mux_const_input_values_: specify the value of the constant input for this multiplexer (valid only when mux_add_const_input is true)
 *  5. mux_use_local_encoder_: specify if the mux as a local encoder between SRAMs and multiplexing structure
 *  6. mux_advanced_rram_design_: specify if the multiplexer will use advanced RRAM circuit design topology
 *
 *  ------ LUT-related parameters ------
 *  Note: only applicable to circuit models whose type is LUT
 *  1. lut_is_fracturable_: specify if this LUT is built with fracturable structure  
 *
 *  ------ RRAM-related parameters ------
 *  Note: only applicable to circuit models whose design technology is RRAM
 *  1. rlrs: RRAM resistance in Low-Resistance State (LRS)
 *  2. rhrs: RRAM resistance in High-Resistance State (HRS)
 *  The following transistor sizes are applicable for 4T1R programming structures 
 *  3. wprog_set_nmos: size of n-type programming transistor used to set a RRAM
 *  4. wprog_set_pmos: size of p-type programming transistor used to set a RRAM
 *  5. wprog_reset_nmos: size of n-type programming transistor used to reset a RRAM
 *  6. wprog_reset_pmos: size of p-type programming transistor used to reset a RRAM
 *
 *  ------ Metal wire-related parameters ------
 *  Note: only applicable to circuit models whose type is wires or channel wires
 * 1. wire_types_: types of the metal wire for the circuit_model  
 * 2. wire_res_val_: resistance value of the metal wire for the circuit model 
 * 3. wire_cap_val_: capacitance value of the metal wire for the circuit model 
 * 4. wire_num_levels_: number of levels of the metal wire model for the circuit model 
 ***********************************************************************/
class CircuitLibrary {
  public: /* Types */
    typedef vtr::vector<CircuitModelId, CircuitModelId>::const_iterator circuit_model_iterator;
    typedef vtr::vector<CircuitModelId, std::string>::const_iterator circuit_model_string_iterator;
    typedef vtr::vector<CircuitPortId, CircuitPortId>::const_iterator circuit_port_iterator;
    typedef vtr::vector<CircuitEdgeId, CircuitEdgeId>::const_iterator circuit_edge_iterator;
    /* Create range */
    typedef vtr::Range<circuit_model_iterator> circuit_model_range;
    typedef vtr::Range<circuit_port_iterator> circuit_port_range;
    typedef vtr::Range<circuit_edge_iterator> circuit_edge_range;
    /* local enumeration for buffer existence */
    enum e_buffer_type: unsigned char{ 
      INPUT = 0, OUTPUT, LUT_INPUT_BUFFER, LUT_INPUT_INVERTER, LUT_INTER_BUFFER, NUM_BUFFER_TYPE /* Last one is a counter */ 
    };
  public: /* Constructors */
    CircuitLibrary();
  public: /* Accessors: aggregates */
    circuit_model_range circuit_models() const;
    circuit_port_range ports(const CircuitModelId& circuit_model_id) const;
    std::vector<CircuitModelId> circuit_models_by_type(const enum e_spice_model_type& type) const;
    std::vector<CircuitPortId> ports_by_type(const CircuitModelId& circuit_model_id, const enum e_spice_model_port_type& port_type) const;
    std::vector<CircuitPortId> ports_by_type(const CircuitModelId& circuit_model_id, const enum e_spice_model_port_type& port_type, const bool& include_global_port) const;
    std::vector<CircuitPortId> input_ports(const CircuitModelId& circuit_model_id) const;
    std::vector<CircuitPortId> output_ports(const CircuitModelId& circuit_model_id) const;
    std::vector<size_t> pins(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
  public: /* Public Accessors: Basic data query on Circuit Models*/
    size_t num_circuit_models() const;
    enum e_spice_model_type circuit_model_type(const CircuitModelId& circuit_model_id) const;
    std::string circuit_model_name(const CircuitModelId& circuit_model_id) const;
    std::string circuit_model_prefix(const CircuitModelId& circuit_model_id) const;
    std::string circuit_model_verilog_netlist(const CircuitModelId& circuit_model_id) const;
    std::string circuit_model_spice_netlist(const CircuitModelId& circuit_model_id) const;
    bool circuit_model_is_default(const CircuitModelId& circuit_model_id) const;
    bool dump_structural_verilog(const CircuitModelId& circuit_model_id) const;
    bool dump_explicit_port_map(const CircuitModelId& circuit_model_id) const;
    enum e_spice_model_design_tech design_tech_type(const CircuitModelId& circuit_model_id) const;
    bool is_power_gated(const CircuitModelId& circuit_model_id) const;
    bool is_input_buffered(const CircuitModelId& circuit_model_id) const;
    bool is_output_buffered(const CircuitModelId& circuit_model_id) const;
    bool is_lut_intermediate_buffered(const CircuitModelId& circuit_model_id) const;
    enum e_spice_model_pass_gate_logic_type pass_gate_logic_type(const CircuitModelId& circuit_model_id) const;
    enum e_spice_model_structure mux_structure(const CircuitModelId& circuit_model_id) const;
    size_t mux_num_levels(const CircuitModelId& circuit_model_id) const;
    bool mux_add_const_input(const CircuitModelId& circuit_model_id) const;
    size_t mux_const_input_value(const CircuitModelId& circuit_model_id) const;
  public: /* Public Accessors: Basic data query on Circuit Ports*/
    bool is_input_port(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool is_output_port(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    CircuitPortId port(const CircuitModelId& circuit_model_id, const std::string& name) const;
    size_t num_ports(const CircuitModelId& circuit_model_id) const;
    size_t num_ports_by_type(const CircuitModelId& circuit_model_id, const enum e_spice_model_port_type& port_type, const bool& include_global_port) const;
    enum e_spice_model_port_type port_type(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    size_t port_size(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    std::string port_prefix(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    std::string port_lib_name(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    std::string port_inv_prefix(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    size_t port_default_value(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool port_is_mode_select(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool port_is_global(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool port_is_reset(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool port_is_set(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool port_is_config_enable(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool port_is_prog(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
  public: /* Public Accessors: Methods to find circuit model */
    CircuitModelId circuit_model(const char* name) const;
    CircuitModelId circuit_model(const std::string& name) const;
    CircuitModelId default_circuit_model(const enum e_spice_model_type& type) const;
  public: /* Public Accessors: Timing graph */
    CircuitEdgeId edge(const CircuitModelId& circuit_model_id,
                       const CircuitPortId& from_port, const size_t from_pin,
                       const CircuitPortId& to_port, const size_t to_pin);
  public: /* Public Mutators */
    CircuitModelId add_circuit_model();
    /* Fundamental information */
    void set_circuit_model_type(const CircuitModelId& circuit_model_id, const enum e_spice_model_type& type);
    void set_circuit_model_name(const CircuitModelId& circuit_model_id, const std::string& name);
    void set_circuit_model_prefix(const CircuitModelId& circuit_model_id, const std::string& prefix);
    void set_circuit_model_verilog_netlist(const CircuitModelId& circuit_model_id, const std::string& verilog_netlist);
    void set_circuit_model_spice_netlist(const CircuitModelId& circuit_model_id, const std::string& spice_netlist);
    void set_circuit_model_is_default(const CircuitModelId& circuit_model_id, const bool& is_default);
    /* Verilog generator options */ 
    void set_circuit_model_dump_structural_verilog(const CircuitModelId& circuit_model_id, const bool& dump_structural_verilog);
    void set_circuit_model_dump_explicit_port_map(const CircuitModelId& circuit_model_id, const bool& dump_explicit_port_map);
    /* Design technology information */ 
    void set_circuit_model_design_tech_type(const CircuitModelId& circuit_model_id, const enum e_spice_model_design_tech& design_tech_type);
    void set_circuit_model_is_power_gated(const CircuitModelId& circuit_model_id, const bool& is_power_gated);
    /* Buffer existence */
    void set_circuit_model_input_buffer(const CircuitModelId& circuit_model_id, 
                                        const bool& existence, const std::string& circuit_model_name);
    void set_circuit_model_output_buffer(const CircuitModelId& circuit_model_id, 
                                         const bool& existence, const std::string& circuit_model_name);
    void set_circuit_model_lut_input_buffer(const CircuitModelId& circuit_model_id, 
                                            const bool& existence, const std::string& circuit_model_name);
    void set_circuit_model_lut_input_inverter(const CircuitModelId& circuit_model_id, 
                                              const bool& existence, const std::string& circuit_model_name);
    void set_circuit_model_lut_intermediate_buffer(const CircuitModelId& circuit_model_id, 
                                                   const bool& existence, const std::string& circuit_model_name);
    void set_circuit_model_lut_intermediate_buffer_location_map(const CircuitModelId& circuit_model_id,
                                                                const std::string& location_map);
    /* Pass-gate-related parameters */
    void set_circuit_model_pass_gate_logic(const CircuitModelId& circuit_model_id, const std::string& circuit_model_name);
    /* Port information */
    CircuitPortId add_circuit_model_port(const CircuitModelId& circuit_model_id);
    void set_port_type(const CircuitModelId& circuit_model_id, 
                       const CircuitPortId& circuit_port_id, 
                       const enum e_spice_model_port_type& port_type);
    void set_port_size(const CircuitModelId& circuit_model_id, 
                       const CircuitPortId& circuit_port_id, 
                       const size_t& port_size);
    void set_port_prefix(const CircuitModelId& circuit_model_id, 
                         const CircuitPortId& circuit_port_id, 
                         const std::string& port_prefix);
    void set_port_lib_name(const CircuitModelId& circuit_model_id, 
                           const CircuitPortId& circuit_port_id, 
                           const std::string& lib_name);
    void set_port_inv_prefix(const CircuitModelId& circuit_model_id, 
                             const CircuitPortId& circuit_port_id, 
                             const std::string& inv_prefix);
    void set_port_default_value(const CircuitModelId& circuit_model_id, 
                                const CircuitPortId& circuit_port_id, 
                                const size_t& default_val);
    void set_port_is_mode_select(const CircuitModelId& circuit_model_id, 
                                 const CircuitPortId& circuit_port_id, 
                                 const bool& is_mode_select);
    void set_port_is_global(const CircuitModelId& circuit_model_id, 
                            const CircuitPortId& circuit_port_id, 
                            const bool& is_global);
    void set_port_is_reset(const CircuitModelId& circuit_model_id, 
                           const CircuitPortId& circuit_port_id, 
                           const bool& is_reset);
    void set_port_is_set(const CircuitModelId& circuit_model_id, 
                         const CircuitPortId& circuit_port_id, 
                         const bool& is_set);
    void set_port_is_config_enable(const CircuitModelId& circuit_model_id, 
                                   const CircuitPortId& circuit_port_id, 
                                   const bool& is_config_enable);
    void set_port_is_prog(const CircuitModelId& circuit_model_id, 
                          const CircuitPortId& circuit_port_id, 
                          const bool& is_prog);
    void set_port_circuit_model_name(const CircuitModelId& circuit_model_id, 
                                     const CircuitPortId& circuit_port_id, 
                                     const std::string& circuit_model_name);
    void set_port_circuit_model_id(const CircuitModelId& circuit_model_id, 
                                   const CircuitPortId& circuit_port_id, 
                                   const CircuitModelId& port_circuit_model_id);
    void set_port_inv_circuit_model_name(const CircuitModelId& circuit_model_id, 
                                         const CircuitPortId& circuit_port_id, 
                                         const std::string& inv_circuit_model_name);
    void set_port_inv_circuit_model_id(const CircuitModelId& circuit_model_id, 
                                       const CircuitPortId& circuit_port_id, 
                                       const CircuitModelId& inv_circuit_model_id);
    void set_port_tri_state_map(const CircuitModelId& circuit_model_id, 
                                const CircuitPortId& circuit_port_id, 
                                const std::string& tri_state_map);
    void set_port_lut_frac_level(const CircuitModelId& circuit_model_id, 
                                 const CircuitPortId& circuit_port_id, 
                                 const size_t& lut_frac_level);
    void set_port_lut_output_mask(const CircuitModelId& circuit_model_id, 
                                  const CircuitPortId& circuit_port_id, 
                                  const std::vector<size_t>& lut_output_masks);
    void set_port_sram_orgz(const CircuitModelId& circuit_model_id, 
                            const CircuitPortId& circuit_port_id, 
                            const enum e_sram_orgz& sram_orgz);
    /* Delay information */
    void add_delay_info(const CircuitModelId& circuit_model_id,
                        const enum spice_model_delay_type& delay_type);
    void set_delay_in_port_names(const CircuitModelId& circuit_model_id,
                                 const enum spice_model_delay_type& delay_type,
                                 const std::string& in_port_names);
    void set_delay_out_port_names(const CircuitModelId& circuit_model_id,
                                  const enum spice_model_delay_type& delay_type,
                                  const std::string& out_port_names);
    void set_delay_values(const CircuitModelId& circuit_model_id,
                          const enum spice_model_delay_type& delay_type,
                          const std::string& delay_values);
    /* Buffer/Inverter-related parameters */
    void set_buffer_type(const CircuitModelId& circuit_model_id,
                         const enum e_spice_model_buffer_type& buffer_type);
    void set_buffer_size(const CircuitModelId& circuit_model_id,
                         const float& buffer_size);
    void set_buffer_num_levels(const CircuitModelId& circuit_model_id,
                               const size_t& num_levels);
    void set_buffer_f_per_stage(const CircuitModelId& circuit_model_id,
                                const size_t& f_per_stage);
    /* Pass-gate-related parameters */
    void set_pass_gate_logic_type(const CircuitModelId& circuit_model_id,
                                  const enum e_spice_model_pass_gate_logic_type& pass_gate_logic_type);
    void set_pass_gate_logic_nmos_size(const CircuitModelId& circuit_model_id,
                                       const float& nmos_size);
    void set_pass_gate_logic_pmos_size(const CircuitModelId& circuit_model_id,
                                       const float& pmos_size);
    /* Multiplexer-related parameters */
    void set_mux_structure(const CircuitModelId& circuit_model_id,
                           const enum e_spice_model_structure& mux_structure);
    void set_mux_num_levels(const CircuitModelId& circuit_model_id,
                            const size_t& num_levels);
    void set_mux_const_input_value(const CircuitModelId& circuit_model_id,
                                   const size_t& const_input_value);
    void set_mux_use_local_encoder(const CircuitModelId& circuit_model_id,
                                   const bool& use_local_encoder);
    void set_mux_use_advanced_rram_design(const CircuitModelId& circuit_model_id,
                                          const bool& use_advanced_rram_design);
    /* LUT-related parameters */
    void set_lut_is_fracturable(const CircuitModelId& circuit_model_id,
                                const bool& is_fracturable);
    /* Gate-related parameters */
    void set_gate_type(const CircuitModelId& circuit_model_id,
                       const enum e_spice_model_gate_type& gate_type);
    /* RRAM-related design technology information */
    void set_rram_rlrs(const CircuitModelId& circuit_model_id,
                       const float& rlrs);
    void set_rram_rhrs(const CircuitModelId& circuit_model_id,
                       const float& rhrs);
    void set_rram_wprog_set_nmos(const CircuitModelId& circuit_model_id,
                                 const float& wprog_set_nmos);
    void set_rram_wprog_set_pmos(const CircuitModelId& circuit_model_id,
                                 const float& wprog_set_pmos);
    void set_rram_wprog_reset_nmos(const CircuitModelId& circuit_model_id,
                                   const float& wprog_reset_nmos);
    void set_rram_wprog_reset_pmos(const CircuitModelId& circuit_model_id,
                                   const float& wprog_reset_pmos);
    /* Wire parameters */
    void set_wire_type(const CircuitModelId& circuit_model_id,
                       const enum e_wire_model_type& wire_type);
    void set_wire_r(const CircuitModelId& circuit_model_id,
                    const float& r_val);
    void set_wire_c(const CircuitModelId& circuit_model_id,
                    const float& c_val);
    void set_wire_num_levels(const CircuitModelId& circuit_model_id,
                             const size_t& num_level);
  public: /* Public Mutators: builders */
    void set_circuit_model_buffer(const CircuitModelId& circuit_model_id, const enum e_buffer_type buffer_type, const bool& existence, const std::string& circuit_model_name);
    void link_port_circuit_model(const CircuitModelId& circuit_model_id);      
    void link_port_inv_circuit_model(const CircuitModelId& circuit_model_id);      
    void link_port_circuit_models(const CircuitModelId& circuit_model_id);      
    void link_buffer_circuit_model(const CircuitModelId& circuit_model_id);      
    void link_pass_gate_logic_circuit_model(const CircuitModelId& circuit_model_id);      
    void build_circuit_model_links();
    void build_circuit_model_timing_graph(const CircuitModelId& circuit_model_id);
    void build_timing_graphs();
  public: /* Internal mutators: build timing graphs */
    void add_edge(const CircuitModelId& circuit_model_id,
                  const CircuitPortId& from_port, const size_t& from_pin, 
                  const CircuitPortId& to_port, const size_t& to_pin);
    void set_edge_delay(const CircuitModelId& circuit_model_id, 
                        const CircuitEdgeId& circuit_edge_id, 
                        const enum spice_model_delay_type& delay_type, 
                        const float& delay_value);
    /* validate the circuit_edge_id */
    void set_timing_graph_delays(const CircuitModelId& circuit_model_id);
  public: /* Internal mutators: build fast look-ups */
    void build_circuit_model_lookup();
    void build_circuit_model_port_lookup(const CircuitModelId& circuit_model_id);
  private: /* Internal invalidators/validators */
    /* Validators */
    bool valid_circuit_model_id(const CircuitModelId& circuit_model_id) const;
    bool valid_circuit_port_id(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const;
    bool valid_circuit_pin_id(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id, const size_t& pin_id) const;
    bool valid_delay_type(const CircuitModelId& circuit_model_id, const enum spice_model_delay_type& delay_type) const;
    bool valid_circuit_edge_id(const CircuitModelId& circuit_model_id, const CircuitEdgeId& circuit_edge_id) const;
    bool valid_mux_const_input_value(const size_t& const_input_value) const;
    /* Invalidators */
    void invalidate_circuit_model_lookup() const;
    void invalidate_circuit_model_port_lookup(const CircuitModelId& circuit_model_id) const;
    void invalidate_circuit_model_timing_graph(const CircuitModelId& circuit_model_id);
  private: /* Internal data */
    /* Fundamental information */
    vtr::vector<CircuitModelId, CircuitModelId> circuit_model_ids_;
    vtr::vector<CircuitModelId, enum e_spice_model_type> circuit_model_types_;
    vtr::vector<CircuitModelId, std::string> circuit_model_names_;
    vtr::vector<CircuitModelId, std::string> circuit_model_prefix_;
    vtr::vector<CircuitModelId, std::string> circuit_model_verilog_netlists_;
    vtr::vector<CircuitModelId, std::string> circuit_model_spice_netlists_;
    vtr::vector<CircuitModelId, bool> circuit_model_is_default_;

    /* fast look-up for circuit models to categorize by types 
     * [type][num_ids]
     * Important: we force the default circuit model in the first element for each type
     */
    typedef std::vector<std::vector<CircuitModelId>> CircuitModelLookup;
    mutable CircuitModelLookup circuit_model_lookup_; /* [circuit_model_type][circuit_model_ids] */
    typedef std::vector<std::vector<std::vector<CircuitPortId>>> CircuitModelPortLookup;
    mutable CircuitModelPortLookup circuit_model_port_lookup_; /* [circuit_model_id][port_type][port_ids] */

    /* Verilog generator options */ 
    vtr::vector<CircuitModelId, bool> dump_structural_verilog_;
    vtr::vector<CircuitModelId, bool> dump_explicit_port_map_;
    
    /* Design technology information */ 
    vtr::vector<CircuitModelId, enum e_spice_model_design_tech> design_tech_types_;
    vtr::vector<CircuitModelId, bool> is_power_gated_;

    /* Buffer existence */
    vtr::vector<CircuitModelId, std::vector<bool>> buffer_existence_;
    vtr::vector<CircuitModelId, std::vector<std::string>> buffer_circuit_model_names_;
    vtr::vector<CircuitModelId, std::vector<CircuitModelId>> buffer_circuit_model_ids_;
    vtr::vector<CircuitModelId, std::vector<std::string>> buffer_location_maps_;

    /* Pass-gate-related parameters */
    vtr::vector<CircuitModelId, std::string> pass_gate_logic_circuit_model_names_;
    vtr::vector<CircuitModelId, CircuitModelId> pass_gate_logic_circuit_model_ids_;

    /* Port information */
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, CircuitPortId>> port_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, enum e_spice_model_port_type>> port_types_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, size_t>> port_sizes_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, std::string>> port_prefix_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, std::string>> port_lib_names_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, std::string>> port_inv_prefix_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, size_t>> port_default_values_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, bool>> port_is_mode_select_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, bool>> port_is_global_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, bool>> port_is_reset_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, bool>> port_is_set_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, bool>> port_is_config_enable_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, bool>> port_is_prog_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, std::string>> port_circuit_model_names_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, CircuitModelId>> port_circuit_model_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, std::string>> port_inv_circuit_model_names_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, CircuitModelId>> port_inv_circuit_model_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, std::string>> port_tri_state_maps_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, size_t>> port_lut_frac_level_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, std::vector<size_t>>> port_lut_output_masks_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, enum e_sram_orgz>> port_sram_orgz_;

    /* Timing graphs */
    vtr::vector<CircuitModelId, vtr::vector<CircuitEdgeId, CircuitEdgeId>> edge_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, vtr::vector<size_t, CircuitEdgeId>>> port_in_edge_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitPortId, vtr::vector<size_t, CircuitEdgeId>>> port_out_edge_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitEdgeId, CircuitPortId>> edge_src_port_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitEdgeId, size_t>> edge_src_pin_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitEdgeId, CircuitPortId>> edge_sink_port_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitEdgeId, size_t>> edge_sink_pin_ids_;
    vtr::vector<CircuitModelId, vtr::vector<CircuitEdgeId, std::vector<float>>> edge_timing_info_; /* x0 => trise, x1 => tfall */

    /* Delay information */
    vtr::vector<CircuitModelId, std::vector<enum spice_model_delay_type>> delay_types_;
    vtr::vector<CircuitModelId, std::vector<std::string>> delay_in_port_names_;
    vtr::vector<CircuitModelId, std::vector<std::string>> delay_out_port_names_;
    vtr::vector<CircuitModelId, std::vector<std::string>> delay_values_;

    /* Buffer/Inverter-related parameters */
    vtr::vector<CircuitModelId, enum e_spice_model_buffer_type> buffer_types_;
    vtr::vector<CircuitModelId, float> buffer_sizes_;
    vtr::vector<CircuitModelId, size_t> buffer_num_levels_;
    vtr::vector<CircuitModelId, size_t> buffer_f_per_stage_;

    /* Pass-gate-related parameters */
    vtr::vector<CircuitModelId, enum e_spice_model_pass_gate_logic_type> pass_gate_logic_types_;
    vtr::vector<CircuitModelId, vtr::Point<float>> pass_gate_logic_sizes_; /* x=> nmos_size; y => pmos_size */

    /* Multiplexer-related parameters */
    vtr::vector<CircuitModelId, enum e_spice_model_structure> mux_structure_;
    vtr::vector<CircuitModelId, size_t> mux_num_levels_;
    vtr::vector<CircuitModelId, size_t> mux_const_input_values_;
    vtr::vector<CircuitModelId, bool> mux_use_local_encoder_;
    vtr::vector<CircuitModelId, bool> mux_use_advanced_rram_design_;

    /* LUT-related parameters */
    vtr::vector<CircuitModelId, bool> lut_is_fracturable_;

    /* Gate-related parameters */
    vtr::vector<CircuitModelId, enum e_spice_model_gate_type> gate_types_;

    /* RRAM-related design technology information */
    vtr::vector<CircuitModelId, vtr::Point<float>> rram_res_; /* x => R_LRS, y => R_HRS */
    vtr::vector<CircuitModelId, vtr::Point<float>> wprog_set_; /* x => wprog_set_nmos, y=> wprog_set_pmos */
    vtr::vector<CircuitModelId, vtr::Point<float>> wprog_reset_; /* x => wprog_reset_nmos, y=> wprog_reset_pmos */
    
    /* Wire parameters */
    vtr::vector<CircuitModelId, enum e_wire_model_type> wire_types_;
    vtr::vector<CircuitModelId, vtr::Point<float>> wire_rc_; /* x => wire_res_val, y=> wire_cap_val */
    vtr::vector<CircuitModelId, size_t> wire_num_levels_;
     
};

#endif

/************************************************************************
 * End of file : circuit_library.cpp 
 ***********************************************************************/
