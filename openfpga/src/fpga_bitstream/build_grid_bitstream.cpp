/********************************************************************
 * This file includes functions that are used for building bitstreams
 * for grids (CLBs, heterogenerous blocks, I/Os, etc.)
 *******************************************************************/
#include <cmath>
#include <string>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "build_grid_bitstream.h"
#include "build_mux_bitstream.h"
#include "circuit_library_utils.h"
#include "command_exit_codes.h"
#include "lut_utils.h"
#include "module_manager_utils.h"
#include "mux_bitstream_constants.h"
#include "mux_utils.h"
#include "openfpga_device_grid_utils.h"
#include "openfpga_interconnect_types.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "pb_graph_utils.h"
#include "pb_type_utils.h"
#include "physical_types_util.h"
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Decode mode bits "01x..." to a bitstream vector
 * Apply final correction on the dont care bits by referring to the base mode
 *bits from physical mode
 *******************************************************************/
static std::vector<bool> generate_mode_select_bitstream(
  const std::vector<char>& mode_bits,
  const std::vector<char>& base_mode_bits = std::vector<char>()) {
  std::vector<bool> mode_select_bitstream;

  /* In absence of base mode bits, all the mode bits must be valid!
   * If a base mode bits is provided, use it to correct the 'x' bits in the mode
   * bits */
  if (base_mode_bits.empty()) {
    for (const char& mode_bit : mode_bits) {
      /* Error out for unexpected bits */
      if ('0' != mode_bit && '1' != mode_bit) {
        VTR_LOG_ERROR(
          "Final mode bits still contain dont care bits! Expect [0 | 1]! "
          "Please ensure all the operating mode bits and physical mode bits "
          "can fully resolve dont care bits!\n");
        exit(openfpga::CMD_EXEC_FATAL_ERROR);
      }
      mode_select_bitstream.push_back('1' == mode_bit);
    }
  } else {
    if (mode_bits.size() != base_mode_bits.size()) {
      VTR_LOG_ERROR(
        "Physical mode bits (%lu) does not match the size of operating mode "
        "bits (%lu)!\n",
        base_mode_bits.size(), mode_bits.size());
      exit(openfpga::CMD_EXEC_FATAL_ERROR);
    }
    for (size_t ibit = 0; ibit < mode_bits.size(); ++ibit) {
      if (mode_bits[ibit] == 'x') {
        mode_select_bitstream.push_back('1' == base_mode_bits[ibit]);
      } else if ('0' == mode_bits[ibit] || '1' == mode_bits[ibit]) {
        mode_select_bitstream.push_back('1' == mode_bits[ibit]);
      } else {
        VTR_LOG_ERROR("Invalid final mode bits ('%c')! Expect [0 | 1 | x]!\n",
                      mode_bits[ibit]);
        exit(openfpga::CMD_EXEC_FATAL_ERROR);
      }
    }
  }

  return mode_select_bitstream;
}

/********************************************************************
 * Generate bitstream for a primitive node and add it to bitstream manager
 *******************************************************************/
static void build_primitive_bitstream(
  BitstreamManager& bitstream_manager,
  std::map<std::string, size_t>& grouped_mem_inst_scoreboard,
  const ConfigBlockId& parent_configurable_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const VprDeviceAnnotation& device_annotation, const PhysicalPb& physical_pb,
  const PhysicalPbId& primitive_pb_id, t_pb_type* primitive_pb_type,
  const bool& verbose) {
  /* Ensure a valid physical pritimive pb */
  if (nullptr == primitive_pb_type) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid primitive_pb_type!\n");
    exit(1);
  }

  CircuitModelId primitive_model =
    device_annotation.pb_type_circuit_model(primitive_pb_type);
  VTR_ASSERT(CircuitModelId::INVALID() != primitive_model);
  VTR_ASSERT(
    (CIRCUIT_MODEL_IOPAD == circuit_lib.model_type(primitive_model)) ||
    (CIRCUIT_MODEL_HARDLOGIC == circuit_lib.model_type(primitive_model)) ||
    (CIRCUIT_MODEL_FF == circuit_lib.model_type(primitive_model)));

  /* Find SRAM ports for mode-selection */
  std::vector<CircuitPortId> primitive_mode_select_ports =
    find_circuit_mode_select_sram_ports(circuit_lib, primitive_model);

  /* We may have a port for mode select or not. */
  VTR_ASSERT((0 == primitive_mode_select_ports.size()) ||
             (1 == primitive_mode_select_ports.size()));

  /* Generate bitstream for mode-select ports */
  if (0 == primitive_mode_select_ports.size()) {
    return; /* Nothing to do, return directly */
  }

  std::vector<bool> mode_select_bitstream;
  if (true == physical_pb.valid_pb_id(primitive_pb_id)) {
    mode_select_bitstream = generate_mode_select_bitstream(
      physical_pb.mode_bits(primitive_pb_id),
      device_annotation.pb_type_mode_bits(primitive_pb_type));
    /* If the physical pb contains fixed mode-select bitstream, overload here */
    for (const auto& fix_bitstrm :
         physical_pb.fixed_mode_select_bitstreams(primitive_pb_id)) {
      std::string fixed_mode_select_bitstream = fix_bitstrm.content;
      size_t mode_bits_start_index = fix_bitstrm.offset;
      /* Ensure the length matches!!! */
      if (mode_select_bitstream.size() - mode_bits_start_index <
          fixed_mode_select_bitstream.size()) {
        VTR_LOG_ERROR(
          "Unmatched length of fixed mode_select_bitstream %s!Expected to be "
          "less than %ld bits\n",
          fixed_mode_select_bitstream.c_str(),
          mode_select_bitstream.size() - mode_bits_start_index);
        exit(1);
      }
      /* Overload the bitstream here */
      for (size_t bit_index = 0; bit_index < fixed_mode_select_bitstream.size();
           ++bit_index) {
        VTR_ASSERT('0' == fixed_mode_select_bitstream[bit_index] ||
                   '1' == fixed_mode_select_bitstream[bit_index]);
        mode_select_bitstream[bit_index + mode_bits_start_index] =
          ('1' == fixed_mode_select_bitstream[bit_index]);
      }
    }
  } else { /* get default mode_bits */
    mode_select_bitstream = generate_mode_select_bitstream(
      device_annotation.pb_type_mode_bits(primitive_pb_type));
  }

  /* Ensure the length of bitstream matches the side of memory circuits */
  std::vector<CircuitModelId> sram_models =
    find_circuit_sram_models(circuit_lib, primitive_model);
  VTR_ASSERT(1 == sram_models.size());
  std::string mem_block_name =
    generate_memory_module_name(circuit_lib, primitive_model, sram_models[0],
                                std::string(MEMORY_MODULE_POSTFIX));
  ModuleId mem_module = module_manager.find_module(mem_block_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));
  ModulePortId mem_out_port_id = module_manager.find_module_port(
    mem_module, generate_configurable_memory_data_out_name());
  VTR_ASSERT(
    mode_select_bitstream.size() ==
    module_manager.module_port(mem_module, mem_out_port_id).get_width());

  /* If there is a feedthrough module, we should consider the scoreboard */
  std::string feedthru_mem_block_name =
    generate_memory_module_name(circuit_lib, primitive_model, sram_models[0],
                                std::string(MEMORY_MODULE_POSTFIX), true);
  ModuleId feedthru_mem_module =
    module_manager.find_module(feedthru_mem_block_name);
  if (module_manager.valid_module_id(feedthru_mem_module)) {
    auto result = grouped_mem_inst_scoreboard.find(mem_block_name);
    if (result == grouped_mem_inst_scoreboard.end()) {
      /* Update scoreboard */
      grouped_mem_inst_scoreboard[mem_block_name] = 0;
    } else {
      grouped_mem_inst_scoreboard[mem_block_name]++;
      mem_block_name = generate_instance_name(
        mem_block_name, grouped_mem_inst_scoreboard[mem_block_name]);
    }
  }

  /* Create a block for the bitstream which corresponds to the memory module
   * associated to the LUT */
  ConfigBlockId mem_block = bitstream_manager.add_block(mem_block_name);
  bitstream_manager.add_child_block(parent_configurable_block, mem_block);

  VTR_LOGV(verbose, "Added %lu bits to '%s' under '%s'\n",
           mode_select_bitstream.size(),
           bitstream_manager.block_name(mem_block).c_str(),
           bitstream_manager.block_name(parent_configurable_block).c_str());

  /* Add the bitstream to the bitstream manager */
  bitstream_manager.add_block_bits(mem_block, mode_select_bitstream);
}

/********************************************************************
 * This function generates bitstream for a programmable routing
 * multiplexer which drives an output pin of physical_pb_graph_node and its the
 *input_edges
 *
 *   src_pb_graph_node.[in|out]_pins ----------------->
 *des_pb_graph_node.[in|out]pins
 *                                        /|\
 *                                         |
 *                         input_pins,   edges,       output_pins
 *******************************************************************/
static void build_physical_block_pin_interc_bitstream(
  BitstreamManager& bitstream_manager,
  std::map<std::string, size_t>& grouped_mem_inst_scoreboard,
  const ConfigBlockId& parent_configurable_block,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprBitstreamAnnotation& bitstream_annotation,
  const PhysicalPb& physical_pb, t_pb_graph_pin* des_pb_graph_pin,
  t_mode* physical_mode, const bool& verbose) {
  /* Identify the number of fan-in (Consider interconnection edges of only
   * selected mode) */
  t_interconnect* cur_interc =
    pb_graph_pin_interc(des_pb_graph_pin, physical_mode);
  size_t fan_in = pb_graph_pin_inputs(des_pb_graph_pin, cur_interc).size();

  if ((nullptr == cur_interc) || (0 == fan_in)) {
    /* No interconnection matched */
    return;
  }

  /* Identify pin interconnection type */
  enum e_interconnect interc_type =
    device_annotation.interconnect_physical_type(cur_interc);
  switch (interc_type) {
    case DIRECT_INTERC:
      /* Nothing to do, return */
      break;
    case COMPLETE_INTERC:
    case MUX_INTERC: {
      /* Find the circuit model id of the mux, we need its design technology
       * which matters the bitstream generation */
      CircuitModelId mux_model =
        device_annotation.interconnect_circuit_model(cur_interc);
      VTR_ASSERT(CIRCUIT_MODEL_MUX == circuit_lib.model_type(mux_model));

      /* Find the input size of the implementation of a routing multiplexer */
      size_t datapath_mux_size = fan_in;
      VTR_ASSERT(true ==
                 valid_mux_implementation_num_inputs(datapath_mux_size));

      /* Cache input and output nets */
      std::vector<AtomNetId> input_nets;
      AtomNetId output_net = AtomNetId::INVALID();

      /* Find the path id:
       * - if des pb is not valid, this is an unmapped pb, we can set a default
       * path_id
       * - There is no net mapped to des_pb_graph_pin we use default path id
       * - There is a net mapped to des_pin_graph_pin: we find the path id
       */
      const PhysicalPbId& des_pb_id =
        physical_pb.find_pb(des_pb_graph_pin->parent_node);
      size_t mux_input_pin_id = 0;
      if (true != physical_pb.valid_pb_id(des_pb_id)) {
        mux_input_pin_id = DEFAULT_PATH_ID;
      } else if (AtomNetId::INVALID() == physical_pb.pb_graph_pin_atom_net(
                                           des_pb_id, des_pb_graph_pin)) {
        mux_input_pin_id = DEFAULT_PATH_ID;
      } else {
        output_net =
          physical_pb.pb_graph_pin_atom_net(des_pb_id, des_pb_graph_pin);

        for (t_pb_graph_pin* src_pb_graph_pin :
             pb_graph_pin_inputs(des_pb_graph_pin, cur_interc)) {
          const PhysicalPbId& src_pb_id =
            physical_pb.find_pb(src_pb_graph_pin->parent_node);
          input_nets.push_back(
            physical_pb.pb_graph_pin_atom_net(src_pb_id, src_pb_graph_pin));
        }

        for (t_pb_graph_pin* src_pb_graph_pin :
             pb_graph_pin_inputs(des_pb_graph_pin, cur_interc)) {
          const PhysicalPbId& src_pb_id =
            physical_pb.find_pb(src_pb_graph_pin->parent_node);
          /* If the src pb id is not valid, we bypass it */
          if ((true == physical_pb.valid_pb_id(src_pb_id)) &&
              (AtomNetId::INVALID() != output_net) &&
              (physical_pb.pb_graph_pin_atom_net(src_pb_id, src_pb_graph_pin) ==
               output_net)) {
            break;
          }
          mux_input_pin_id++;
        }
        VTR_ASSERT(mux_input_pin_id <= fan_in);
        /* Unmapped pin, use default path id */
        if (fan_in == mux_input_pin_id) {
          mux_input_pin_id = DEFAULT_PATH_ID;
        }
      }

      /* Overwrite the default path if defined in bitstream annotation */
      if ((size_t(DEFAULT_PATH_ID) == mux_input_pin_id) &&
          (mux_input_pin_id !=
           bitstream_annotation.interconnect_default_path_id(cur_interc))) {
        mux_input_pin_id =
          bitstream_annotation.interconnect_default_path_id(cur_interc);
      }

      /* Generate bitstream depend on both technology and structure of this MUX
       */
      std::vector<bool> mux_bitstream = build_mux_bitstream(
        circuit_lib, mux_model, mux_lib, datapath_mux_size, mux_input_pin_id);

      /* Create the block denoting the memory instances that drives this node in
       * physical_block */
      std::string mem_block_name = generate_pb_memory_instance_name(
        GRID_MEM_INSTANCE_PREFIX, des_pb_graph_pin, std::string(""));

      /* Find the module in module manager and ensure the bitstream size
       * matches! */
      std::string mem_module_name =
        generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                                 std::string(MEMORY_MODULE_POSTFIX));
      mem_module_name = module_name_map.name(mem_module_name);
      ModuleId mux_mem_module = module_manager.find_module(mem_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(mux_mem_module));
      ModulePortId mux_mem_out_port_id = module_manager.find_module_port(
        mux_mem_module, generate_configurable_memory_data_out_name());
      VTR_ASSERT(mux_bitstream.size() ==
                 module_manager.module_port(mux_mem_module, mux_mem_out_port_id)
                   .get_width());

      /* If there is a feedthrough module, we should consider the scoreboard */
      std::string feedthru_mem_block_name = generate_mux_subckt_name(
        circuit_lib, mux_model, datapath_mux_size,
        std::string(MEMORY_FEEDTHROUGH_MODULE_POSTFIX));
      if (module_name_map.name_exist(feedthru_mem_block_name)) {
        feedthru_mem_block_name = module_name_map.name(feedthru_mem_block_name);
      }
      ModuleId feedthru_mem_module =
        module_manager.find_module(feedthru_mem_block_name);
      if (module_manager.valid_module_id(feedthru_mem_module)) {
        auto result = grouped_mem_inst_scoreboard.find(mem_block_name);
        if (result == grouped_mem_inst_scoreboard.end()) {
          /* Update scoreboard */
          grouped_mem_inst_scoreboard[mem_block_name] = 0;
        } else {
          grouped_mem_inst_scoreboard[mem_block_name]++;
          mem_block_name = generate_instance_name(
            mem_block_name, grouped_mem_inst_scoreboard[mem_block_name]);
        }
      }
      ConfigBlockId mux_mem_block = bitstream_manager.add_block(mem_block_name);
      bitstream_manager.add_child_block(parent_configurable_block,
                                        mux_mem_block);

      VTR_LOGV(verbose, "Added %lu bits to '%s' under '%s'\n",
               mux_bitstream.size(),
               bitstream_manager.block_name(mux_mem_block).c_str(),
               bitstream_manager.block_name(parent_configurable_block).c_str());

      /* Add the bistream to the bitstream manager */
      bitstream_manager.add_block_bits(mux_mem_block, mux_bitstream);
      /* Record path ids, input and output nets */
      bitstream_manager.add_path_id_to_block(mux_mem_block, mux_input_pin_id);

      /* Add input nets */
      std::string input_net_ids;

      bool need_splitter = false;
      for (const AtomNetId& input_net : input_nets) {
        /* Add a space as a splitter*/
        if (true == need_splitter) {
          input_net_ids += std::string(" ");
        }
        if (true == atom_ctx.netlist().valid_net_id(input_net)) {
          input_net_ids += atom_ctx.netlist().net_name(input_net);
        } else {
          input_net_ids += std::string("unmapped");
        }
        need_splitter = true;
      }
      bitstream_manager.add_input_net_id_to_block(mux_mem_block, input_net_ids);

      /* Add output nets */
      std::string output_net_ids;
      if (true == atom_ctx.netlist().valid_net_id(output_net)) {
        output_net_ids += atom_ctx.netlist().net_name(output_net);
      } else {
        output_net_ids += std::string("unmapped");
      }
      bitstream_manager.add_output_net_id_to_block(mux_mem_block,
                                                   output_net_ids);

      break;
    }
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid interconnection type for %s (Arch[LINE%d])!\n",
                     cur_interc->name, cur_interc->line_num);
      exit(1);
  }
}

/********************************************************************
 * This function generates bitstream for the programmable routing
 * multiplexers in a pb_graph node
 *******************************************************************/
static void build_physical_block_interc_port_bitstream(
  BitstreamManager& bitstream_manager,
  std::map<std::string, size_t>& grouped_mem_inst_scoreboard,
  const ConfigBlockId& parent_configurable_block,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprBitstreamAnnotation& bitstream_annotation,
  t_pb_graph_node* physical_pb_graph_node, const PhysicalPb& physical_pb,
  const e_circuit_pb_port_type& pb_port_type, t_mode* physical_mode,
  const bool& verbose) {
  switch (pb_port_type) {
    case CIRCUIT_PB_PORT_INPUT:
      for (int iport = 0; iport < physical_pb_graph_node->num_input_ports;
           ++iport) {
        for (int ipin = 0; ipin < physical_pb_graph_node->num_input_pins[iport];
             ++ipin) {
          build_physical_block_pin_interc_bitstream(
            bitstream_manager, grouped_mem_inst_scoreboard,
            parent_configurable_block, module_manager, module_name_map,
            circuit_lib, mux_lib, atom_ctx, device_annotation,
            bitstream_annotation, physical_pb,
            &(physical_pb_graph_node->input_pins[iport][ipin]), physical_mode,
            verbose);
        }
      }
      break;
    case CIRCUIT_PB_PORT_OUTPUT:
      for (int iport = 0; iport < physical_pb_graph_node->num_output_ports;
           ++iport) {
        for (int ipin = 0;
             ipin < physical_pb_graph_node->num_output_pins[iport]; ++ipin) {
          build_physical_block_pin_interc_bitstream(
            bitstream_manager, grouped_mem_inst_scoreboard,
            parent_configurable_block, module_manager, module_name_map,
            circuit_lib, mux_lib, atom_ctx, device_annotation,
            bitstream_annotation, physical_pb,
            &(physical_pb_graph_node->output_pins[iport][ipin]), physical_mode,
            verbose);
        }
      }
      break;
    case CIRCUIT_PB_PORT_CLOCK:
      for (int iport = 0; iport < physical_pb_graph_node->num_clock_ports;
           ++iport) {
        for (int ipin = 0; ipin < physical_pb_graph_node->num_clock_pins[iport];
             ++ipin) {
          build_physical_block_pin_interc_bitstream(
            bitstream_manager, grouped_mem_inst_scoreboard,
            parent_configurable_block, module_manager, module_name_map,
            circuit_lib, mux_lib, atom_ctx, device_annotation,
            bitstream_annotation, physical_pb,
            &(physical_pb_graph_node->clock_pins[iport][ipin]), physical_mode,
            verbose);
        }
      }
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid pb port type!\n");
      exit(1);
  }
}

/********************************************************************
 * This function generates bitstream for the programmable routing
 * multiplexers in a pb_graph node
 *******************************************************************/
static void build_physical_block_interc_bitstream(
  BitstreamManager& bitstream_manager,
  std::map<std::string, size_t>& grouped_mem_inst_scoreboard,
  const ConfigBlockId& parent_configurable_block,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprBitstreamAnnotation& bitstream_annotation,
  t_pb_graph_node* physical_pb_graph_node, const PhysicalPb& physical_pb,
  t_mode* physical_mode, const bool& verbose) {
  /* Check if the pb_graph node is valid or not */
  if (nullptr == physical_pb_graph_node) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid physical_pb_graph_node.\n");
    exit(1);
  }

  /* We check output_pins of physical_pb_graph_node and its the input_edges
   * Iterate over the interconnections between outputs of physical_pb_graph_node
   * and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins ----------------->
   * physical_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   * Note: it is not applied to primitive pb_type!
   */
  build_physical_block_interc_port_bitstream(
    bitstream_manager, grouped_mem_inst_scoreboard, parent_configurable_block,
    module_manager, module_name_map, circuit_lib, mux_lib, atom_ctx,
    device_annotation, bitstream_annotation, physical_pb_graph_node,
    physical_pb, CIRCUIT_PB_PORT_OUTPUT, physical_mode, verbose);

  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Iterate over the interconnections between inputs of physical_pb_graph_node
   * and inputs of child_pb_graph_node
   *   physical_pb_graph_node.input_pins ----------------->
   * child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ipb++) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb;
         jpb++) {
      t_pb_graph_node* child_pb_graph_node =
        &(physical_pb_graph_node
            ->child_pb_graph_nodes[physical_mode->index][ipb][jpb]);

      /* For each child_pb_graph_node input pins*/
      build_physical_block_interc_port_bitstream(
        bitstream_manager, grouped_mem_inst_scoreboard,
        parent_configurable_block, module_manager, module_name_map, circuit_lib,
        mux_lib, atom_ctx, device_annotation, bitstream_annotation,
        child_pb_graph_node, physical_pb, CIRCUIT_PB_PORT_INPUT, physical_mode,
        verbose);
      /* For clock pins, we should do the same work */
      build_physical_block_interc_port_bitstream(
        bitstream_manager, grouped_mem_inst_scoreboard,
        parent_configurable_block, module_manager, module_name_map, circuit_lib,
        mux_lib, atom_ctx, device_annotation, bitstream_annotation,
        child_pb_graph_node, physical_pb, CIRCUIT_PB_PORT_CLOCK, physical_mode,
        verbose);
    }
  }
}

/********************************************************************
 * Generate bitstream for a LUT and add it to bitstream manager
 * This function supports both single-output and fracturable LUTs
 *******************************************************************/
static void build_lut_bitstream(
  BitstreamManager& bitstream_manager,
  std::map<std::string, size_t>& grouped_mem_inst_scoreboard,
  const ConfigBlockId& parent_configurable_block,
  const VprDeviceAnnotation& device_annotation,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const PhysicalPb& physical_pb,
  const PhysicalPbId& lut_pb_id, t_pb_type* lut_pb_type, const bool& verbose) {
  /* Ensure a valid physical pritimive pb */
  if (nullptr == lut_pb_type) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid lut_pb_type!\n");
    exit(1);
  }

  CircuitModelId lut_model =
    device_annotation.pb_type_circuit_model(lut_pb_type);
  VTR_ASSERT(CircuitModelId::INVALID() != lut_model);
  VTR_ASSERT(CIRCUIT_MODEL_LUT == circuit_lib.model_type(lut_model));

  /* Find the input ports for LUT size, this is used to decode the LUT memory
   * bits! */
  std::vector<CircuitPortId> model_input_ports =
    find_lut_circuit_model_input_port(circuit_lib, lut_model, false);
  VTR_ASSERT(1 == model_input_ports.size());
  size_t lut_size = circuit_lib.port_size(model_input_ports[0]);

  /* Find SRAM ports for truth tables and mode-selection */
  std::vector<CircuitPortId> lut_regular_sram_ports =
    find_circuit_regular_sram_ports(circuit_lib, lut_model);
  std::vector<CircuitPortId> lut_mode_select_ports =
    find_circuit_mode_select_sram_ports(circuit_lib, lut_model);
  /* We should always 1 regular sram port, where truth table is loaded to */
  VTR_ASSERT(1 == lut_regular_sram_ports.size());
  /* We may have a port for mode select or not. This depends on if the LUT is
   * fracturable or not */
  VTR_ASSERT((0 == lut_mode_select_ports.size()) ||
             (1 == lut_mode_select_ports.size()));

  std::vector<bool> lut_bitstream;
  /* Generate bitstream for the LUT */
  if (false == physical_pb.valid_pb_id(lut_pb_id)) {
    /* An empty pb means that this is an unused LUT,
     * we give an empty truth table, which are full of default values (defined
     * by users)
     */
    for (size_t i = 0; i < circuit_lib.port_size(lut_regular_sram_ports[0]);
         ++i) {
      VTR_ASSERT(
        (0 == circuit_lib.port_default_value(lut_regular_sram_ports[0])) ||
        (1 == circuit_lib.port_default_value(lut_regular_sram_ports[0])));
      lut_bitstream.push_back(
        1 == circuit_lib.port_default_value(lut_regular_sram_ports[0]));
    }
  } else {
    VTR_ASSERT(true == physical_pb.valid_pb_id(lut_pb_id));

    /* Find MUX graph correlated to the LUT */
    MuxId lut_mux_id = mux_lib.mux_graph(lut_model, (size_t)pow(2., lut_size));
    const MuxGraph& mux_graph = mux_lib.mux_graph(lut_mux_id);
    /* Ensure the LUT MUX has the expected input and SRAM port sizes */
    VTR_ASSERT(mux_graph.num_memory_bits() == lut_size);
    VTR_ASSERT(mux_graph.num_inputs() == (size_t)pow(2., lut_size));
    /* Generate LUT bitstream */
    lut_bitstream = build_frac_lut_bitstream(
      circuit_lib, mux_graph, device_annotation,
      physical_pb.truth_tables(lut_pb_id),
      circuit_lib.port_default_value(lut_regular_sram_ports[0]));
    /* If the physical pb contains fixed bitstream, overload here */
    for (const auto& fix_bitstrm : physical_pb.fixed_bitstreams(lut_pb_id)) {
      std::string fixed_bitstream = fix_bitstrm.content;
      size_t start_index = fix_bitstrm.offset;
      /* Ensure the length matches!!! */
      if (lut_bitstream.size() - start_index < fixed_bitstream.size()) {
        VTR_LOG_ERROR(
          "Unmatched length of fixed bitstream %s!Expected to be less than %ld "
          "bits\n",
          fixed_bitstream.c_str(), lut_bitstream.size() - start_index);
        exit(1);
      }
      /* Overload the bitstream here */
      for (size_t bit_index = 0; bit_index < lut_bitstream.size();
           ++bit_index) {
        VTR_ASSERT('0' == fixed_bitstream[bit_index] ||
                   '1' == fixed_bitstream[bit_index]);
        lut_bitstream[bit_index + start_index] =
          ('1' == fixed_bitstream[bit_index]);
      }
    }
  }

  /* Generate bitstream for mode-select ports */
  if (0 != lut_mode_select_ports.size()) {
    std::vector<bool> mode_select_bitstream;
    if (true == physical_pb.valid_pb_id(lut_pb_id)) {
      mode_select_bitstream = generate_mode_select_bitstream(
        physical_pb.mode_bits(lut_pb_id),
        device_annotation.pb_type_mode_bits(lut_pb_type));

      /* If the physical pb contains fixed mode-select bitstream, overload here
       */
      for (const auto& fix_bitstrm :
           physical_pb.fixed_mode_select_bitstreams(lut_pb_id)) {
        std::string fixed_mode_select_bitstream = fix_bitstrm.content;
        size_t mode_bits_start_index = fix_bitstrm.offset;
        /* Ensure the length matches!!! */
        if (mode_select_bitstream.size() - mode_bits_start_index <
            fixed_mode_select_bitstream.size()) {
          VTR_LOG_ERROR(
            "Unmatched length of fixed mode_select_bitstream %s!Expected to be "
            "less than %ld bits\n",
            fixed_mode_select_bitstream.c_str(),
            mode_select_bitstream.size() - mode_bits_start_index);
          exit(1);
        }
        /* Overload the bitstream here */
        for (size_t bit_index = 0;
             bit_index < fixed_mode_select_bitstream.size(); ++bit_index) {
          VTR_ASSERT('0' == fixed_mode_select_bitstream[bit_index] ||
                     '1' == fixed_mode_select_bitstream[bit_index]);
          mode_select_bitstream[bit_index + mode_bits_start_index] =
            ('1' == fixed_mode_select_bitstream[bit_index]);
        }
      }
    } else { /* get default mode_bits */
      mode_select_bitstream = generate_mode_select_bitstream(
        device_annotation.pb_type_mode_bits(lut_pb_type));
    }

    /* Conjunct the mode-select bitstream to the lut bitstream */
    for (const bool& bit : mode_select_bitstream) {
      lut_bitstream.push_back(bit);
    }
  }

  /* Ensure the length of bitstream matches the side of memory circuits */
  std::vector<CircuitModelId> sram_models =
    find_circuit_sram_models(circuit_lib, lut_model);
  VTR_ASSERT(1 == sram_models.size());
  std::string mem_block_name = generate_memory_module_name(
    circuit_lib, lut_model, sram_models[0], std::string(MEMORY_MODULE_POSTFIX));
  ModuleId mem_module = module_manager.find_module(mem_block_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));
  ModulePortId mem_out_port_id = module_manager.find_module_port(
    mem_module, generate_configurable_memory_data_out_name());
  VTR_ASSERT(
    lut_bitstream.size() ==
    module_manager.module_port(mem_module, mem_out_port_id).get_width());

  /* If there is a feedthrough module, we should consider the scoreboard */
  std::string feedthru_mem_block_name =
    generate_memory_module_name(circuit_lib, lut_model, sram_models[0],
                                std::string(MEMORY_MODULE_POSTFIX), true);
  ModuleId feedthru_mem_module =
    module_manager.find_module(feedthru_mem_block_name);
  if (module_manager.valid_module_id(feedthru_mem_module)) {
    auto result = grouped_mem_inst_scoreboard.find(mem_block_name);
    if (result == grouped_mem_inst_scoreboard.end()) {
      /* Update scoreboard */
      grouped_mem_inst_scoreboard[mem_block_name] = 0;
    } else {
      grouped_mem_inst_scoreboard[mem_block_name]++;
      mem_block_name = generate_instance_name(
        mem_block_name, grouped_mem_inst_scoreboard[mem_block_name]);
    }
  }

  /* Create a block for the bitstream which corresponds to the memory module
   * associated to the LUT */
  ConfigBlockId mem_block = bitstream_manager.add_block(mem_block_name);
  bitstream_manager.add_child_block(parent_configurable_block, mem_block);

  VTR_LOGV(verbose, "Added %lu bits to '%s' under '%s'\n", lut_bitstream.size(),
           bitstream_manager.block_name(mem_block).c_str(),
           bitstream_manager.block_name(parent_configurable_block).c_str());

  /* Add the bitstream to the bitstream manager */
  bitstream_manager.add_block_bits(mem_block, lut_bitstream);
}

/********************************************************************
 * This function generates bitstream for a physical block, which is
 * a child block of a grid
 * This function will follow a recursive way in generating bitstreams
 * It will follow the same sequence in visiting all the sub blocks
 * in a physical as we did during module generation
 *
 * Note: if you want to bind your bitstream with a FPGA fabric generated by
 *FPGA-X2P Please follow the same sequence in visiting pb_graph nodes!!! For
 *more details, you may refer to function rec_build_physical_block_modules()
 *******************************************************************/
static void rec_build_physical_block_bitstream(
  BitstreamManager& bitstream_manager,
  std::map<std::string, size_t>& grouped_mem_inst_scoreboard,
  const ConfigBlockId& parent_configurable_block,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprBitstreamAnnotation& bitstream_annotation, const e_side& border_side,
  const PhysicalPb& physical_pb, const PhysicalPbId& pb_id,
  t_pb_graph_node* physical_pb_graph_node, const size_t& pb_graph_node_index,
  const bool& verbose) {
  /* Get the physical pb_type that is linked to the pb_graph node */
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  /* Find the mode that define_idle_mode*/
  t_mode* physical_mode = device_annotation.physical_mode(physical_pb_type);

  /* Early exit if this parent module has no configurable child modules */
  std::string pb_module_name =
    generate_physical_block_module_name(physical_pb_type);
  pb_module_name = module_name_map.name(pb_module_name);
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Skip module with no configurable children */
  if (0 == module_manager.num_configurable_children(
             pb_module, ModuleManager::e_config_child_type::LOGICAL)) {
    return;
  }

  /* Create a block for the physical block under the grid block in bitstream
   * manager */
  std::string pb_block_name = generate_physical_block_instance_name(
    physical_pb_type, pb_graph_node_index);
  /* If there are no physical memory blocks under the current module, use the
   * previous module, which is the physical memory block */
  ConfigBlockId pb_configurable_block = parent_configurable_block;
  if (0 < module_manager.num_configurable_children(
            pb_module, ModuleManager::e_config_child_type::PHYSICAL)) {
    pb_configurable_block = bitstream_manager.add_block(pb_block_name);
    bitstream_manager.add_child_block(parent_configurable_block,
                                      pb_configurable_block);
    /* Reserve child blocks for new created block */
    bitstream_manager.reserve_child_blocks(
      parent_configurable_block,
      count_module_manager_module_configurable_children(
        module_manager, pb_module,
        ModuleManager::e_config_child_type::PHYSICAL));
  }

  /* Recursively finish all the child pb_types*/
  if (false == is_primitive_pb_type(physical_pb_type)) {
    for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
      for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb;
           ++jpb) {
        PhysicalPbId child_pb = PhysicalPbId::INVALID();
        /* Find the child pb that is mapped, and the mapping info is not stored
         * in the physical mode ! */
        if (true == physical_pb.valid_pb_id(pb_id)) {
          child_pb = physical_pb.child(
            pb_id, &(physical_mode->pb_type_children[ipb]), jpb);
          VTR_ASSERT(true == physical_pb.valid_pb_id(child_pb));
        }
        /* Go recursively */
        rec_build_physical_block_bitstream(
          bitstream_manager, grouped_mem_inst_scoreboard, pb_configurable_block,
          module_manager, module_name_map, circuit_lib, mux_lib, atom_ctx,
          device_annotation, bitstream_annotation, border_side, physical_pb,
          child_pb,
          &(physical_pb_graph_node
              ->child_pb_graph_nodes[physical_mode->index][ipb][jpb]),
          jpb, verbose);
      }
    }
  }

  /* Check if this has defined a circuit_model*/
  if (true == is_primitive_pb_type(physical_pb_type)) {
    CircuitModelId primitive_circuit_model =
      device_annotation.pb_type_circuit_model(physical_pb_type);
    VTR_ASSERT(CircuitModelId::INVALID() != primitive_circuit_model);
    switch (circuit_lib.model_type(primitive_circuit_model)) {
      case CIRCUIT_MODEL_LUT:
        /* Special case for LUT !!!
         * Mapped logical block information is stored in child_pbs of this pb!!!
         */
        build_lut_bitstream(bitstream_manager, grouped_mem_inst_scoreboard,
                            pb_configurable_block, device_annotation,
                            module_manager, circuit_lib, mux_lib, physical_pb,
                            pb_id, physical_pb_type, verbose);
        break;
      case CIRCUIT_MODEL_FF:
      case CIRCUIT_MODEL_HARDLOGIC:
      case CIRCUIT_MODEL_IOPAD:
        /* For other types of blocks, we can apply a generic therapy */
        build_primitive_bitstream(
          bitstream_manager, grouped_mem_inst_scoreboard, pb_configurable_block,
          module_manager, circuit_lib, device_annotation, physical_pb, pb_id,
          physical_pb_type, verbose);
        break;
      default:
        VTR_LOGF_ERROR(__FILE__, __LINE__,
                       "Unknown circuit model type of pb_type '%s'!\n",
                       physical_pb_type->name);
        exit(1);
    }
    /* Finish for primitive node, return */
    return;
  }

  /* Generate the bitstream for the interconnection in this physical block */
  build_physical_block_interc_bitstream(
    bitstream_manager, grouped_mem_inst_scoreboard, pb_configurable_block,
    module_manager, module_name_map, circuit_lib, mux_lib, atom_ctx,
    device_annotation, bitstream_annotation, physical_pb_graph_node,
    physical_pb, physical_mode, verbose);
}

/********************************************************************
 * This function generates bitstream for a grid, which could be a
 * CLB, a heterogenerous block, an I/O, etc.
 * Note that each grid may contain a number of physical blocks,
 * this function will iterate over them
 *******************************************************************/
static void build_physical_block_bitstream(
  BitstreamManager& bitstream_manager, const ConfigBlockId& top_block,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const FabricTile& fabric_tile, const FabricTileId& curr_tile,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprClusteringAnnotation& cluster_annotation,
  const VprPlacementAnnotation& place_annotation,
  const VprBitstreamAnnotation& bitstream_annotation, const DeviceGrid& grids,
  const size_t& layer, const vtr::Point<size_t>& grid_coord,
  const e_side& border_side, const bool& verbose) {
  /* Create a block for the grid in bitstream manager */
  t_physical_tile_type_ptr grid_type = grids.get_physical_type(
    t_physical_tile_loc(grid_coord.x(), grid_coord.y(), layer));
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);

  /* Early exit if this parent module has no configurable child modules */
  std::string grid_module_name = generate_grid_block_module_name(
    grid_module_name_prefix, std::string(grid_type->name), grid_type->is_io(),
    border_side);
  grid_module_name = module_name_map.name(grid_module_name);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

  /* Skip module with no configurable children */
  if (0 == module_manager.num_configurable_children(
             grid_module, ModuleManager::e_config_child_type::LOGICAL)) {
    return;
  }

  vtr::Point<size_t> grid_coord_in_unique_tile = grid_coord;
  if (fabric_tile.valid_tile_id(curr_tile)) {
    /* For tile modules, need to find the specific instance name under its
     * unique tile */
    grid_coord_in_unique_tile =
      fabric_tile.find_pb_coordinate_in_unique_tile(curr_tile, grid_coord);
  }
  std::string grid_block_name = generate_grid_block_instance_name(
    grid_module_name_prefix, std::string(grid_type->name), grid_type->is_io(),
    border_side, grid_coord_in_unique_tile);

  ConfigBlockId grid_configurable_block =
    bitstream_manager.add_block(grid_block_name);
  bitstream_manager.add_child_block(top_block, grid_configurable_block);

  /* Reserve child blocks for new created block */
  bitstream_manager.reserve_child_blocks(
    grid_configurable_block, count_module_manager_module_configurable_children(
                               module_manager, grid_module,
                               ModuleManager::e_config_child_type::PHYSICAL));

  /* Create a dedicated block for the non-unified configurable child */
  if (!module_manager.unified_configurable_children(grid_module)) {
    VTR_ASSERT(1 ==
               module_manager
                 .configurable_children(
                   grid_module, ModuleManager::e_config_child_type::PHYSICAL)
                 .size());
    std::string phy_mem_instance_name = module_manager.instance_name(
      grid_module,
      module_manager.configurable_children(
        grid_module, ModuleManager::e_config_child_type::PHYSICAL)[0],
      module_manager.configurable_child_instances(
        grid_module, ModuleManager::e_config_child_type::PHYSICAL)[0]);
    ConfigBlockId grid_grouped_config_block =
      bitstream_manager.add_block(phy_mem_instance_name);
    VTR_LOGV(
      verbose,
      "Added grouped configurable memory block '%s' as a child to '%s'\n",
      bitstream_manager.block_name(grid_grouped_config_block).c_str(),
      bitstream_manager.block_name(grid_configurable_block).c_str());
    bitstream_manager.add_child_block(grid_configurable_block,
                                      grid_grouped_config_block);
    grid_configurable_block = grid_grouped_config_block;
  }

  /* Iterate over the capacity of the grid
   * Now each physical tile may have a number of logical blocks
   * OpenFPGA only considers the physical implementation of the tiles.
   * So, we do not allow multiple equivalent sites to be defined
   * under a physical tile type.
   * If you need different equivalent sites, you can always define
   * it as a mode under a <pb_type>
   */
  std::map<std::string, size_t> grouped_mem_inst_scoreboard;
  for (size_t z = 0; z < place_annotation.grid_blocks(grid_coord).size(); ++z) {
    int sub_tile_index =
      device_annotation.physical_tile_z_to_subtile_index(grid_type, z);
    VTR_ASSERT(1 ==
               grid_type->sub_tiles[sub_tile_index].equivalent_sites.size());
    for (t_logical_block_type_ptr lb_type :
         grid_type->sub_tiles[sub_tile_index].equivalent_sites) {
      /* Bypass empty pb_graph */
      if (nullptr == lb_type->pb_graph_head) {
        continue;
      }

      if (ClusterBlockId::INVALID() ==
          place_annotation.grid_blocks(grid_coord)[z]) {
        /* Recursively traverse the pb_graph and generate bitstream */
        rec_build_physical_block_bitstream(
          bitstream_manager, grouped_mem_inst_scoreboard,
          grid_configurable_block, module_manager, module_name_map, circuit_lib,
          mux_lib, atom_ctx, device_annotation, bitstream_annotation,
          border_side, PhysicalPb(), PhysicalPbId::INVALID(),
          lb_type->pb_graph_head, z, verbose);
      } else {
        const PhysicalPb& phy_pb = cluster_annotation.physical_pb(
          place_annotation.grid_blocks(grid_coord)[z]);

        /* Get the top-level node of the pb_graph */
        t_pb_graph_node* pb_graph_head = lb_type->pb_graph_head;
        VTR_ASSERT(nullptr != pb_graph_head);
        const PhysicalPbId& top_pb_id = phy_pb.find_pb(pb_graph_head);

        /* Recursively traverse the pb_graph and generate bitstream */
        rec_build_physical_block_bitstream(
          bitstream_manager, grouped_mem_inst_scoreboard,
          grid_configurable_block, module_manager, module_name_map, circuit_lib,
          mux_lib, atom_ctx, device_annotation, bitstream_annotation,
          border_side, phy_pb, top_pb_id, pb_graph_head, z, verbose);
      }
    }
  }
}

/********************************************************************
 * Top-level function of this file:
 * Generate bitstreams for all the grids, including
 * 1. core grids that sit in the center of the fabric
 * 2. side grids (I/O grids) that sit in the borders for the fabric
 *******************************************************************/
void build_grid_bitstream(
  BitstreamManager& bitstream_manager, const ConfigBlockId& top_block,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const FabricTile& fabric_tile, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const DeviceGrid& grids, const size_t& layer,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprClusteringAnnotation& cluster_annotation,
  const VprPlacementAnnotation& place_annotation,
  const VprBitstreamAnnotation& bitstream_annotation, const bool& verbose) {
  VTR_LOGV(verbose, "Generating bitstream for core grids...");

  /* Generate bitstream for the core logic block one by one */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      t_physical_tile_loc phy_tile_loc(ix, iy, layer);
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grids.get_physical_type(phy_tile_loc))) {
        continue;
      }
      /* Skip width > 1 or height > 1 tiles (mostly heterogeneous blocks) */
      if ((0 < grids.get_width_offset(phy_tile_loc)) ||
          (0 < grids.get_height_offset(phy_tile_loc))) {
        continue;
      }
      /* Add a grid module to top_module*/
      vtr::Point<size_t> grid_coord(ix, iy);
      /* TODO: If the fabric tile is not empty, find the tile module and create
       * the block accordingly. Also to support future hierarchy changes, when
       * creating the blocks, trace backward until reach the current top block.
       * If any block is missing during the back tracing, create it. */
      ConfigBlockId parent_block = top_block;
      FabricTileId curr_tile =
        fabric_tile.find_tile_by_pb_coordinate(grid_coord);
      if (fabric_tile.valid_tile_id(curr_tile)) {
        vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(curr_tile);
        std::string tile_inst_name = generate_tile_module_name(tile_coord);
        parent_block = bitstream_manager.find_or_create_child_block(
          top_block, tile_inst_name);
        VTR_LOGV(verbose,
                 "Add configurable block '%s' as a child under configurable "
                 "block '%s'\n",
                 tile_inst_name.c_str(),
                 bitstream_manager.block_name(top_block).c_str());
      }

      build_physical_block_bitstream(
        bitstream_manager, parent_block, module_manager, module_name_map,
        fabric_tile, curr_tile, circuit_lib, mux_lib, atom_ctx,
        device_annotation, cluster_annotation, place_annotation,
        bitstream_annotation, grids, layer, grid_coord, NUM_2D_SIDES, verbose);
    }
  }
  VTR_LOGV(verbose, "Done\n");

  VTR_LOGV(verbose, "Generating bitstream for I/O grids...");

  /* Create the coordinate range for each side of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_grid_coordinates(grids);

  /* Add instances of I/O grids to top_module */
  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      t_physical_tile_loc phy_tile_loc(io_coordinate.x(), io_coordinate.y(),
                                       layer);
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grids.get_physical_type(phy_tile_loc))) {
        continue;
      }
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if ((0 < grids.get_width_offset(phy_tile_loc)) ||
          (0 < grids.get_height_offset(phy_tile_loc))) {
        continue;
      }
      /* TODO: If the fabric tile is not empty, find the tile module and create
       * the block accordingly. Also to support future hierarchy changes, when
       * creating the blocks, trace backward until reach the current top block.
       * If any block is missing during the back tracing, create it. */
      ConfigBlockId parent_block = top_block;
      FabricTileId curr_tile =
        fabric_tile.find_tile_by_pb_coordinate(io_coordinate);
      if (fabric_tile.valid_tile_id(curr_tile)) {
        vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(curr_tile);
        std::string tile_inst_name = generate_tile_module_name(tile_coord);
        parent_block = bitstream_manager.find_or_create_child_block(
          top_block, tile_inst_name);
        VTR_LOGV(verbose,
                 "Add configurable block '%s' as a child under configurable "
                 "block '%s'\n",
                 tile_inst_name.c_str(),
                 bitstream_manager.block_name(parent_block).c_str());
      }

      build_physical_block_bitstream(
        bitstream_manager, parent_block, module_manager, module_name_map,
        fabric_tile, curr_tile, circuit_lib, mux_lib, atom_ctx,
        device_annotation, cluster_annotation, place_annotation,
        bitstream_annotation, grids, layer, io_coordinate, io_side, verbose);
    }
  }
  VTR_LOGV(verbose, "Done\n");
}

} /* end namespace openfpga */
