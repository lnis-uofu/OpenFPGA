/********************************************************************
 * This file includes functions that are used for building bitstreams
 * for grids (CLBs, heterogenerous blocks, I/Os, etc.)
 *******************************************************************/
#include <string>

#include "vtr_assert.h"
#include "vtr_vector.h"

#include "util.h"
#include "mux_utils.h"
#include "vpr_types.h"
#include "globals.h"

#include "circuit_library_utils.h"

#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"

#include "build_mux_bitstream.h"
#include "build_lut_bitstream.h"
#include "build_grid_bitstream.h"

/********************************************************************
 * Decode mode bits "01..." to a bitstream vector
 *******************************************************************/
static 
std::vector<bool> generate_mode_select_bitstream(const std::string& mode_bits) {
  std::vector<bool> mode_select_bitstream;
 
  for (size_t i = 0; i < mode_bits.length(); ++i) {
    /* Error out for unexpected bits */
    if ( ('0' != mode_bits[i]) && ('1' != mode_bits[i]) ) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d]) Invalid mode_bits(%s)!\n",
                 __FILE__, __LINE__, mode_bits.c_str());
      exit(1);
    }
    mode_select_bitstream.push_back('1' == mode_bits[i]);
  }

  return mode_select_bitstream;
}

/********************************************************************
 * Generate bitstream for a primitive node and add it to bitstream manager
 *******************************************************************/
static 
void build_primitive_bitstream(BitstreamManager& bitstream_manager,
                               const ConfigBlockId& parent_configurable_block,
                               const ModuleManager& module_manager,
                               const CircuitLibrary& circuit_lib,
                               t_phy_pb* primitive_pb,
                               t_pb_type* primitive_pb_type) {

  /* Ensure a valid physical pritimive pb */ 
  if (NULL == primitive_pb_type) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, [LINE%d]) Invalid primitive_pb_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Asserts */
  if (NULL != primitive_pb) {
    VTR_ASSERT (primitive_pb->pb_graph_node->pb_type->phy_pb_type == primitive_pb_type);
  }

  CircuitModelId primitive_model = primitive_pb_type->circuit_model;
  VTR_ASSERT(CircuitModelId::INVALID() != primitive_model);
  VTR_ASSERT( (SPICE_MODEL_IOPAD == circuit_lib.model_type(primitive_model))
           || (SPICE_MODEL_HARDLOGIC == circuit_lib.model_type(primitive_model))
           || (SPICE_MODEL_FF == circuit_lib.model_type(primitive_model)) );

  /* Find SRAM ports for mode-selection */
  std::vector<CircuitPortId> primitive_mode_select_ports = find_circuit_mode_select_sram_ports(circuit_lib, primitive_model);

  /* We may have a port for mode select or not. */
  VTR_ASSERT( (0 == primitive_mode_select_ports.size())
           || (1 == primitive_mode_select_ports.size()) );

  /* Generate bitstream for mode-select ports */
  if (0 == primitive_mode_select_ports.size()) {
    return; /* Nothing to do, return directly */
  }

  std::vector<bool> mode_select_bitstream;
  if (NULL != primitive_pb) {
    mode_select_bitstream = generate_mode_select_bitstream(std::string(primitive_pb->mode_bits));
  } else { /* get default mode_bits */
    mode_select_bitstream = generate_mode_select_bitstream(std::string(primitive_pb_type->mode_bits));
  }

  /* Ensure the length of bitstream matches the side of memory circuits */
  std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, primitive_model);
  VTR_ASSERT(1 == sram_models.size());
  std::string mem_block_name = generate_memory_module_name(circuit_lib, primitive_model, sram_models[0], std::string(MEMORY_MODULE_POSTFIX));
  ModuleId mem_module = module_manager.find_module(mem_block_name);
  VTR_ASSERT (true == module_manager.valid_module_id(mem_module));
  ModulePortId mem_out_port_id = module_manager.find_module_port(mem_module, generate_configuration_chain_data_out_name());
  VTR_ASSERT(mode_select_bitstream.size() == module_manager.module_port(mem_module, mem_out_port_id).get_width());

  /* Create a block for the bitstream which corresponds to the memory module associated to the LUT */
  ConfigBlockId mem_block = bitstream_manager.add_block(mem_block_name);
  bitstream_manager.add_child_block(parent_configurable_block, mem_block);

  /* Add the bitstream to the bitstream manager */
  for (const bool& bit : mode_select_bitstream) {
    ConfigBitId config_bit = bitstream_manager.add_bit(bit);
    /* Link the memory bits to the mux mem block */
    bitstream_manager.add_bit_to_block(mem_block, config_bit);
  }
}

/********************************************************************
 * Generate bitstream for a LUT and add it to bitstream manager
 * This function supports both single-output and fracturable LUTs
 *******************************************************************/
static 
void build_lut_bitstream(BitstreamManager& bitstream_manager,
                         const ConfigBlockId& parent_configurable_block,
                         const ModuleManager& module_manager,
                         const CircuitLibrary& circuit_lib,
                         const MuxLibrary& mux_lib,
                         t_phy_pb* lut_pb,
                         t_pb_type* lut_pb_type) {

  /* Ensure a valid physical pritimive pb */ 
  if (NULL == lut_pb_type) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, [LINE%d]) Invalid lut_pb_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Asserts */
  if (NULL != lut_pb) {
    VTR_ASSERT (lut_pb->pb_graph_node->pb_type->phy_pb_type == lut_pb_type);
  }

  CircuitModelId lut_model = lut_pb_type->circuit_model;
  VTR_ASSERT (CircuitModelId::INVALID() != lut_model);
  VTR_ASSERT (SPICE_MODEL_LUT == circuit_lib.model_type(lut_model));

  /* Find the input ports for LUT size, this is used to decode the LUT memory bits! */
  std::vector<CircuitPortId> model_input_ports = circuit_lib.model_ports_by_type(lut_model, SPICE_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == model_input_ports.size());
  size_t lut_size = circuit_lib.port_size(model_input_ports[0]);

  /* Find SRAM ports for truth tables and mode-selection */
  std::vector<CircuitPortId> lut_regular_sram_ports = find_circuit_regular_sram_ports(circuit_lib, lut_model);
  std::vector<CircuitPortId> lut_mode_select_ports = find_circuit_mode_select_sram_ports(circuit_lib, lut_model);
  /* We should always 1 regular sram port, where truth table is loaded to */
  VTR_ASSERT(1 == lut_regular_sram_ports.size());
  /* We may have a port for mode select or not. This depends on if the LUT is fracturable or not */
  VTR_ASSERT( (0 == lut_mode_select_ports.size())
           || (1 == lut_mode_select_ports.size()) );

  std::vector<bool> lut_bitstream;
  /* Generate bitstream for the LUT */ 
  if ( (NULL == lut_pb) 
    || ((NULL != lut_pb && 0 == lut_pb->num_logical_blocks)) ) {
    /* An empty pb means that this is an unused LUT, 
     * we give an empty truth table, which are full of default values (defined by users) 
     */
    for (size_t i = 0; i < circuit_lib.port_size(lut_regular_sram_ports[0]); ++i) {
      VTR_ASSERT( (0 == circuit_lib.port_default_value(lut_regular_sram_ports[0]))
               || (1 == circuit_lib.port_default_value(lut_regular_sram_ports[0])) );
      lut_bitstream.push_back(1 == circuit_lib.port_default_value(lut_regular_sram_ports[0]));
    }
  } else { 
    VTR_ASSERT (NULL != lut_pb);
    /* Pre-allocate truth tables for a LUT,
     * Note: for fracturable LUTs, there could be several truth tables
     * since multiple functions are mapped to the same LUT but to different outputs 
     */
    std::vector<LutTruthTable> truth_tables;
    truth_tables.resize(lut_pb->num_logical_blocks);
    
    /* Find truth tables and decode them one by one 
     * Fracturable LUT may have multiple truth tables,
     * which should be grouped in a unique one 
     * And then we derive the truth table 
     */
    for (int i = 0; i < lut_pb->num_logical_blocks; ++i) {
      int mapped_logical_block_index = lut_pb->logical_block[i]; 
      /* For wired LUT we provide a default truth table */
      if (TRUE == lut_pb->is_wired_lut[i]) {
        /* Build a post-routing lut truth table */
        std::vector<int> lut_pin_nets = find_mapped_lut_phy_pb_input_pin_vpack_net_num(lut_pb);
        truth_tables[i] = build_post_routing_wired_lut_truth_table(lut_pb->rr_graph->rr_node[lut_pb->lut_output_pb_graph_pin[i]->rr_node_index_physical_pb].vpack_net_num, lut_pin_nets.size(), lut_pin_nets); 
      } else {
        /* For regular LUTs, we generate the truth tables */
        VTR_ASSERT (FALSE == lut_pb->is_wired_lut[i]);
        VTR_ASSERT (VPACK_COMB == logical_block[mapped_logical_block_index].type);
        /* Get the mapped vpack_net_num of this physical LUT pb */
        std::vector<int> lut_pin_nets = find_mapped_lut_phy_pb_input_pin_vpack_net_num(lut_pb);
        /* Consider LUT pin remapping when assign lut truth tables */
        /* Match truth table and post-routing results */
        truth_tables[i] = build_post_routing_lut_truth_table(&logical_block[mapped_logical_block_index], 
                                                             lut_pin_nets.size(), lut_pin_nets); 
      }
      /* Adapt truth table for a fracturable LUT
       * TODO: Determine fixed input bits for this truth table:
       * 1. input bits within frac_level (all '-' if not specified) 
       * 2. input bits outside frac_level, decoded to its output mask (0 -> first part -> all '1') 
       */
      truth_tables[i] = adapt_truth_table_for_frac_lut(circuit_lib, lut_pb->lut_output_pb_graph_pin[i], 
                                                      truth_tables[i]);
    }
    /* Find MUX graph correlated to the LUT */
    MuxId lut_mux_id = mux_lib.mux_graph(lut_model, (size_t)pow(2., lut_size)); 
    const MuxGraph& mux_graph = mux_lib.mux_graph(lut_mux_id);
    /* Ensure the LUT MUX has the expected input and SRAM port sizes */
    VTR_ASSERT(mux_graph.num_memory_bits() == lut_size);
    VTR_ASSERT(mux_graph.num_inputs() == (size_t)pow(2., lut_size));
    /* Generate LUT bitstream */
    lut_bitstream = build_frac_lut_bitstream(circuit_lib, mux_graph,
                                             lut_pb, truth_tables, 
                                             circuit_lib.port_default_value(lut_regular_sram_ports[0]));
  }
  
  /* Generate bitstream for mode-select ports */
  if (0 != lut_mode_select_ports.size()) {
    std::vector<bool> mode_select_bitstream;
    if (NULL != lut_pb) {
      mode_select_bitstream = generate_mode_select_bitstream(std::string(lut_pb->mode_bits));
    } else { /* get default mode_bits */
      mode_select_bitstream = generate_mode_select_bitstream(std::string(lut_pb_type->mode_bits));
    }
    /* Conjunct the mode-select bitstream to the lut bitstream */
    for (const bool& bit : mode_select_bitstream) {
      lut_bitstream.push_back(bit);
    }
  }

  /* Ensure the length of bitstream matches the side of memory circuits */
  std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, lut_model);
  VTR_ASSERT(1 == sram_models.size());
  std::string mem_block_name = generate_memory_module_name(circuit_lib, lut_model, sram_models[0], std::string(MEMORY_MODULE_POSTFIX));
  ModuleId mem_module = module_manager.find_module(mem_block_name);
  VTR_ASSERT (true == module_manager.valid_module_id(mem_module));
  ModulePortId mem_out_port_id = module_manager.find_module_port(mem_module, generate_configuration_chain_data_out_name());
  VTR_ASSERT(lut_bitstream.size() == module_manager.module_port(mem_module, mem_out_port_id).get_width());

  /* Create a block for the bitstream which corresponds to the memory module associated to the LUT */
  ConfigBlockId mem_block = bitstream_manager.add_block(mem_block_name);
  bitstream_manager.add_child_block(parent_configurable_block, mem_block);

  /* Add the bitstream to the bitstream manager */
  for (const bool& bit : lut_bitstream) {
    ConfigBitId config_bit = bitstream_manager.add_bit(bit);
    /* Link the memory bits to the mux mem block */
    bitstream_manager.add_bit_to_block(mem_block, config_bit);
  }
}


/********************************************************************
 * This function generates bitstream for a programmable routing
 * multiplexer which drives an output pin of physical_pb_graph_node and its the input_edges
 *
 *   src_pb_graph_node.[in|out]_pins -----------------> des_pb_graph_node.[in|out]pins
 *                                        /|\
 *                                         |
 *                         input_pins,   edges,       output_pins
 *******************************************************************/
static 
void build_physical_block_pin_interc_bitstream(BitstreamManager& bitstream_manager,
                                               const ConfigBlockId& parent_configurable_block,
                                               const ModuleManager& module_manager,
                                               const CircuitLibrary& circuit_lib,
                                               const MuxLibrary& mux_lib,
                                               t_pb_graph_pin* des_pb_graph_pin,
                                               t_mode* physical_mode,
                                               const int& path_id) {
  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Select and print the SPICE netlist
   */
  int fan_in = 0;
  t_interconnect* cur_interc = NULL;
  find_interc_fan_in_des_pb_graph_pin(des_pb_graph_pin, physical_mode, &cur_interc, &fan_in);

  if ((NULL == cur_interc) || (0 == fan_in)) { 
    /* No interconnection matched */
    return;
  }

  enum e_interconnect interc_type = determine_actual_pb_interc_type(cur_interc, fan_in);
  switch (interc_type) {
  case DIRECT_INTERC:
    /* Nothing to do, return */
    break;
  case COMPLETE_INTERC:
  case MUX_INTERC: {
    /* Find the circuit model id of the mux, we need its design technology which matters the bitstream generation */
    CircuitModelId mux_model = cur_interc->circuit_model;
    VTR_ASSERT(SPICE_MODEL_MUX == circuit_lib.model_type(mux_model));

    /* Find the input size of the implementation of a routing multiplexer */
    size_t datapath_mux_size = fan_in;
    VTR_ASSERT(true == valid_mux_implementation_num_inputs(datapath_mux_size));

    /* Generate bitstream depend on both technology and structure of this MUX */
    std::vector<bool> mux_bitstream = build_mux_bitstream(circuit_lib, mux_model, mux_lib, datapath_mux_size, path_id); 

    /* Create the block denoting the memory instances that drives this node in physical_block */
    std::string mem_block_name = generate_pb_memory_instance_name(GRID_MEM_INSTANCE_PREFIX, des_pb_graph_pin, std::string(""));
    ConfigBlockId mux_mem_block = bitstream_manager.add_block(mem_block_name);
    bitstream_manager.add_child_block(parent_configurable_block, mux_mem_block);
  
    /* Find the module in module manager and ensure the bitstream size matches! */
    std::string mem_module_name = generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size, std::string(MEMORY_MODULE_POSTFIX)); 
    ModuleId mux_mem_module = module_manager.find_module(mem_module_name); 
    VTR_ASSERT (true == module_manager.valid_module_id(mux_mem_module));
    ModulePortId mux_mem_out_port_id = module_manager.find_module_port(mux_mem_module, generate_configuration_chain_data_out_name());
    VTR_ASSERT(mux_bitstream.size() == module_manager.module_port(mux_mem_module, mux_mem_out_port_id).get_width());
  
    /* Add the bistream to the bitstream manager */
    for (const bool& bit : mux_bitstream) {
      ConfigBitId config_bit = bitstream_manager.add_bit(bit);
      /* Link the memory bits to the mux mem block */
      bitstream_manager.add_bit_to_block(mux_mem_block, config_bit);
    }
    break;
  }
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }
}

/********************************************************************
 * This function generates bitstream for the programmable routing
 * multiplexers in a pb_graph node 
 *******************************************************************/
static 
void build_physical_block_interc_port_bitstream(BitstreamManager& bitstream_manager,
                                                const ConfigBlockId& parent_configurable_block,
                                                const ModuleManager& module_manager,
                                                const CircuitLibrary& circuit_lib,
                                                const MuxLibrary& mux_lib,
                                                t_pb_graph_node* physical_pb_graph_node,
                                                t_phy_pb* physical_pb,
                                                const e_spice_pb_port_type& pb_port_type,
                                                t_mode* physical_mode) {
  switch (pb_port_type) {
  case SPICE_PB_PORT_INPUT:
    for (int iport = 0; iport < physical_pb_graph_node->num_input_ports; ++iport) {
      for (int ipin = 0; ipin < physical_pb_graph_node->num_input_pins[iport]; ++ipin) {
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        int path_id;
        if (NULL == physical_pb) {
          path_id = DEFAULT_PATH_ID;
        } else {
          VTR_ASSERT(NULL != physical_pb);
          t_rr_node* pb_rr_nodes = physical_pb->rr_graph->rr_node;
          int node_index = physical_pb_graph_node->input_pins[iport][ipin].rr_node_index_physical_pb;
          int prev_node = pb_rr_nodes[node_index].prev_node;
          /* prev_edge = pb_rr_nodes[node_index].prev_edge; */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = DEFAULT_PATH_ID; 
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            VTR_ASSERT(DEFAULT_PATH_ID != path_id);
          }
          /* TODO: This should be done outside this function!
           * Path id for the sdc generation
           */
          pb_rr_nodes[node_index].id_path = path_id;
        }
        build_physical_block_pin_interc_bitstream(bitstream_manager, parent_configurable_block,
                                                  module_manager, circuit_lib, mux_lib,
                                                  &(physical_pb_graph_node->input_pins[iport][ipin]),
                                                  physical_mode,
                                                  path_id);
      }
    }
    break;
  case SPICE_PB_PORT_OUTPUT:
    for (int iport = 0; iport < physical_pb_graph_node->num_output_ports; ++iport) {
      for (int ipin = 0; ipin < physical_pb_graph_node->num_output_pins[iport]; ++ipin) {
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        int path_id;
        if (NULL == physical_pb) {
          path_id = DEFAULT_PATH_ID;
        } else {
          VTR_ASSERT(NULL != physical_pb);
          t_rr_node* pb_rr_nodes = physical_pb->rr_graph->rr_node;
          int node_index = physical_pb_graph_node->output_pins[iport][ipin].rr_node_index_physical_pb;
          int prev_node = pb_rr_nodes[node_index].prev_node;
          /* prev_edge = pb_rr_nodes[node_index].prev_edge; */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = DEFAULT_PATH_ID; 
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            VTR_ASSERT(DEFAULT_PATH_ID != path_id);
          }
          /* TODO: This should be done outside this function!
           * Path id for the sdc generation
           */
          pb_rr_nodes[node_index].id_path = path_id;
        }
        build_physical_block_pin_interc_bitstream(bitstream_manager, parent_configurable_block,
                                                  module_manager, circuit_lib, mux_lib,
                                                  &(physical_pb_graph_node->output_pins[iport][ipin]),
                                                  physical_mode,
                                                  path_id);
      }
    }
    break;
  case SPICE_PB_PORT_CLOCK:
    for (int iport = 0; iport < physical_pb_graph_node->num_clock_ports; ++iport) {
      for (int ipin = 0; ipin < physical_pb_graph_node->num_clock_pins[iport]; ++ipin) {
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        int path_id;
        if (NULL == physical_pb) {
          path_id = DEFAULT_PATH_ID;
        } else {
          VTR_ASSERT(NULL != physical_pb);
          t_rr_node* pb_rr_nodes = physical_pb->rr_graph->rr_node;
          int node_index = physical_pb_graph_node->clock_pins[iport][ipin].rr_node_index_physical_pb;
          int prev_node = pb_rr_nodes[node_index].prev_node;
          /* prev_edge = pb_rr_nodes[node_index].prev_edge; */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = DEFAULT_PATH_ID; 
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            VTR_ASSERT(DEFAULT_PATH_ID != path_id);
          }
          /* TODO: This should be done outside this function!
           * Path id for the sdc generation
           */
          pb_rr_nodes[node_index].id_path = path_id;
        }
        build_physical_block_pin_interc_bitstream(bitstream_manager, parent_configurable_block,
                                                  module_manager, circuit_lib, mux_lib,
                                                  &(physical_pb_graph_node->clock_pins[iport][ipin]),
                                                  physical_mode,
                                                  path_id);

      }
    }
    break;
  default:
   vpr_printf(TIO_MESSAGE_ERROR,
             "(File:%s, [LINE%d]) Invalid pb port type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/********************************************************************
 * This function generates bitstream for the programmable routing
 * multiplexers in a pb_graph node 
 *******************************************************************/
static 
void build_physical_block_interc_bitstream(BitstreamManager& bitstream_manager,
                                           const ConfigBlockId& parent_configurable_block,
                                           const ModuleManager& module_manager,
                                           const CircuitLibrary& circuit_lib,
                                           const MuxLibrary& mux_lib,
                                           t_pb_graph_node* physical_pb_graph_node,
                                           t_phy_pb* physical_pb,
                                           const int& physical_mode_index) {
  /* Check if the pb_graph node is valid or not */
  if (NULL == physical_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, [LINE%d]) Invalid physical_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Assign current mode */
  t_mode* physical_mode = &(physical_pb_graph_node->pb_type->modes[physical_mode_index]);

  /* We check output_pins of physical_pb_graph_node and its the input_edges
   * Iterate over the interconnections between outputs of physical_pb_graph_node 
   * and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> physical_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  build_physical_block_interc_port_bitstream(bitstream_manager, parent_configurable_block,
                                             module_manager, circuit_lib, mux_lib, 
                                             physical_pb_graph_node, physical_pb,  
                                             SPICE_PB_PORT_OUTPUT, physical_mode);
  
  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Iterate over the interconnections between inputs of physical_pb_graph_node 
   * and inputs of child_pb_graph_node
   *   physical_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ipb++) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; jpb++) {
      t_pb_graph_node* child_pb_graph_node = &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode_index][ipb][jpb]);
      /* branch on empty pb */
      t_phy_pb* child_pb = NULL;
      if (NULL != physical_pb) {
        child_pb = &(physical_pb->child_pbs[ipb][jpb]);
      }
      /* For each child_pb_graph_node input pins*/
      build_physical_block_interc_port_bitstream(bitstream_manager, parent_configurable_block,
                                                 module_manager, circuit_lib, mux_lib, 
                                                 child_pb_graph_node, child_pb,  
                                                 SPICE_PB_PORT_INPUT, physical_mode);
      /* For clock pins, we should do the same work */
      build_physical_block_interc_port_bitstream(bitstream_manager, parent_configurable_block,
                                                 module_manager, circuit_lib, mux_lib, 
                                                 child_pb_graph_node, child_pb,  
                                                 SPICE_PB_PORT_CLOCK, physical_mode);
    }
  }
}


/********************************************************************
 * This function generates bitstream for a physical block, which is 
 * a child block of a grid 
 * This function will follow a recursive way in generating bitstreams
 * It will follow the same sequence in visiting all the sub blocks
 * in a physical as we did during module generation
 *
 * Note: if you want to bind your bitstream with a FPGA fabric generated by FPGA-X2P
 * Please follow the same sequence in visiting pb_graph nodes!!!
 * For more details, you may refer to function rec_build_physical_block_modules()
 *******************************************************************/
static 
void rec_build_physical_block_bitstream(BitstreamManager& bitstream_manager,
                                        const ConfigBlockId& parent_configurable_block,
                                        const ModuleManager& module_manager,
                                        const CircuitLibrary& circuit_lib,
                                        const MuxLibrary& mux_lib,
                                        const e_side& border_side,
                                        t_phy_pb* physical_pb, 
                                        t_pb_graph_node* physical_pb_graph_node,
                                        const size_t& pb_graph_node_index) {
  /* Get the physical pb_type that is linked to the pb_graph node */
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  /* Find the mode that define_idle_mode*/
  int physical_mode_index = find_pb_type_physical_mode_index((*physical_pb_type));

  /* Create a block for the physical block under the grid block in bitstream manager */
  std::string pb_block_name_prefix = generate_grid_block_prefix(std::string(GRID_MODULE_NAME_PREFIX), border_side);
  std::string pb_block_name = generate_physical_block_instance_name(pb_block_name_prefix, physical_pb_type, pb_graph_node_index);
  ConfigBlockId pb_configurable_block = bitstream_manager.add_block(pb_block_name);
  bitstream_manager.add_child_block(parent_configurable_block, pb_configurable_block);

  /* Recursively finish all the child pb_types*/
  if (false == is_primitive_pb_type(physical_pb_type)) { 
    for (int ipb = 0; ipb < physical_pb_type->modes[physical_mode_index].num_pb_type_children; ++ipb) {
      for (int jpb = 0; jpb < physical_pb_type->modes[physical_mode_index].pb_type_children[ipb].num_pb; ++jpb) {
        t_phy_pb* child_pb = NULL;
        /* Find the child pb that is mapped, and the mapping info is not stored in the physical mode ! */
        if (NULL != physical_pb) {
          child_pb = get_phy_child_pb_for_phy_pb_graph_node(physical_pb, ipb, jpb);
        }
        /* Go recursively */
        rec_build_physical_block_bitstream(bitstream_manager, pb_configurable_block,
                                           module_manager, circuit_lib, mux_lib, 
                                           border_side, 
                                           child_pb,
                                           &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode_index][ipb][jpb]),
                                           jpb);
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (true == is_primitive_pb_type(physical_pb_type)) { 
    switch (physical_pb_type->class_type) {
    case LUT_CLASS: 
      /* Special case for LUT !!!
       * Mapped logical block information is stored in child_pbs of this pb!!!
       */
      build_lut_bitstream(bitstream_manager, pb_configurable_block,
                          module_manager, circuit_lib, mux_lib, 
                          physical_pb, physical_pb_type);
      break;
    case LATCH_CLASS:
    case UNKNOWN_CLASS:
    case MEMORY_CLASS:
      /* For other types of blocks, we can apply a generic therapy */
      build_primitive_bitstream(bitstream_manager, pb_configurable_block,
                                module_manager, circuit_lib, 
                                physical_pb, physical_pb_type);
      break;  
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d]) Unknown class type of pb_type(%s)!\n",
                 __FILE__, __LINE__, physical_pb_type->name);
      exit(1);
    }
    /* Finish for primitive node, return */
    return;
  }

  /* Generate the bitstream for the interconnection in this physical block */
  build_physical_block_interc_bitstream(bitstream_manager, pb_configurable_block,
                                        module_manager, circuit_lib, mux_lib,
                                        physical_pb_graph_node, physical_pb, physical_mode_index);
}

/********************************************************************
 * This function generates bitstream for a grid, which could be a
 * CLB, a heterogenerous block, an I/O, etc.
 * Note that each grid may contain a number of physical blocks,
 * this function will iterate over them
 *******************************************************************/
static 
void build_physical_block_bitstream(BitstreamManager& bitstream_manager,
                                    const ConfigBlockId& top_block,
                                    const ModuleManager& module_manager,
                                    const CircuitLibrary& circuit_lib,
                                    const MuxLibrary& mux_lib,
                                    const std::vector<std::vector<t_grid_tile>>& grids,
                                    const vtr::Point<size_t>& grid_coordinate,
                                    const e_side& border_side) {
  /* Create a block for the grid in bitstream manager */
  t_type_ptr grid_type = grids[grid_coordinate.x()][grid_coordinate.y()].type;
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_block_name = generate_grid_block_instance_name(grid_module_name_prefix, std::string(grid_type->name), 
                                                                  IO_TYPE == grid_type, border_side, grid_coordinate);
  ConfigBlockId grid_configurable_block = bitstream_manager.add_block(grid_block_name);
  bitstream_manager.add_child_block(top_block, grid_configurable_block);

  /* Iterate over the capacity of the grid */
  for (int z = 0; z < grids[grid_coordinate.x()][grid_coordinate.y()].type->capacity; ++z) {
    /* Get the top-level node of the pb_graph */
    t_pb_graph_node* top_pb_graph_node = grid_type->pb_graph_head;
    VTR_ASSERT(NULL != top_pb_graph_node);

    /* Check in all the mapped blocks(clustered logic block), there is a match x,y,z*/
    t_block* mapped_block = search_mapped_block(grid_coordinate.x(), grid_coordinate.y(), z); 
    t_phy_pb* top_pb = NULL;
    if (NULL != mapped_block) {
      top_pb = (t_phy_pb*)mapped_block->phy_pb; 
      VTR_ASSERT(NULL != top_pb);
    }

    /* Recursively traverse the pb_graph and generate bitstream */
    rec_build_physical_block_bitstream(bitstream_manager, grid_configurable_block, 
                                       module_manager, circuit_lib, mux_lib, 
                                       border_side, 
                                       top_pb, top_pb_graph_node, z);
  } 
}


/********************************************************************
 * Top-level function of this file: 
 * Generate bitstreams for all the grids, including 
 * 1. core grids that sit in the center of the fabric
 * 2. side grids (I/O grids) that sit in the borders for the fabric
 *******************************************************************/
void build_grid_bitstream(BitstreamManager& bitstream_manager,
                          const ConfigBlockId& top_block,
                          const ModuleManager& module_manager,
                          const CircuitLibrary& circuit_lib,
                          const MuxLibrary& mux_lib,
                          const vtr::Point<size_t>& device_size,
                          const std::vector<std::vector<t_grid_tile>>& grids) {

  vpr_printf(TIO_MESSAGE_INFO,
             "Generating bitstream for core grids...\n");

  /* Generate bitstream for the core logic block one by one */
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grids[ix][iy].type) {
        continue;
      } 
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[ix][iy].offset) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE != grids[ix][iy].type);
      /* Ensure a valid usage */
      VTR_ASSERT((0 == grids[ix][iy].usage)||(0 < grids[ix][iy].usage));
      /* Add a grid module to top_module*/
      vtr::Point<size_t> grid_coord(ix, iy);
      build_physical_block_bitstream(bitstream_manager, top_block, module_manager,
                                     circuit_lib, mux_lib, grids, grid_coord, NUM_SIDES);
    }
  }

  vpr_printf(TIO_MESSAGE_INFO,
             "Generating bitstream for I/O grids...\n");

  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> io_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* TOP side*/
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) { 
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, device_size.y() - 1));
  } 

  /* RIGHT side */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) { 
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(device_size.x() - 1, iy));
  } 

  /* BOTTOM side*/
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) { 
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  } 

  /* LEFT side */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) { 
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  /* Add instances of I/O grids to top_module */
  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grids[io_coordinate.x()][io_coordinate.y()].type) {
        continue;
      } 
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[io_coordinate.x()][io_coordinate.y()].offset) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE == grids[io_coordinate.x()][io_coordinate.y()].type);
      build_physical_block_bitstream(bitstream_manager, top_block, module_manager,
                                     circuit_lib, mux_lib, grids, io_coordinate, io_side);
    }
  }
}
