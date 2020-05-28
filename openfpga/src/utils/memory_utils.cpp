/*********************************************************************
 * This file includes functions that are used for 
 * generating ports for memory modules 
 *********************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "openfpga_naming.h"
#include "memory_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Create a port-to-port map for a CMOS memory module 
 *
 * Configuration Chain 
 * -------------------
 *
 *        config_bus (head)   config_bus (tail) 
 *            |                   ^
 *            v                   |
 *      +-------------------------------------+
 *      |        CMOS-based Memory Module     |
 *      +-------------------------------------+
 *            |                   |
 *            v                   v
 *         sram_out             sram_outb
 *
 *
 * Memory bank 
 * -----------
 *
 *        config_bus (BL)   config_bus (WL) 
 *            |                   |
 *            v                   v
 *      +-------------------------------------+
 *      |        CMOS-based Memory Module     |
 *      +-------------------------------------+
 *            |                   |
 *            v                   v
 *         sram_out             sram_outb
 *
 **********************************************************************/
static 
std::map<std::string, BasicPort> generate_cmos_mem_module_port2port_map(const BasicPort& config_bus,
                                                                        const std::vector<BasicPort>& mem_output_bus_ports,
                                                                        const e_config_protocol_type& sram_orgz_type) {
  std::map<std::string, BasicPort> port2port_name_map;

  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Nothing to do */
    break;
  case CONFIG_MEM_SCAN_CHAIN: {
    /* Link the head port of the memory module: 
     * the LSB of config bus port is the head port index
     */
    std::vector<BasicPort> config_bus_ports;
    config_bus_ports.push_back(BasicPort(generate_local_config_bus_port_name(), config_bus.get_msb(), config_bus.get_msb() + 1));
    BasicPort head_port(config_bus_ports[0].get_name(), config_bus_ports[0].get_lsb(), config_bus_ports[0].get_lsb()); 
    port2port_name_map[generate_configuration_chain_head_name()] = head_port;

    /* Link the tail port of the memory module: 
     * the MSB of config bus port is the tail port index 
     */
    BasicPort tail_port(config_bus_ports[0].get_name(), config_bus_ports[0].get_msb(), config_bus_ports[0].get_msb()); 
    port2port_name_map[generate_configuration_chain_tail_name()] = tail_port;

    /* Link the SRAM output ports of the memory module */ 
    VTR_ASSERT( 2 == mem_output_bus_ports.size() );
    port2port_name_map[generate_configurable_memory_data_out_name()] = mem_output_bus_ports[0];
    port2port_name_map[generate_configurable_memory_inverted_data_out_name()] = mem_output_bus_ports[1];
    break;
  }
  case CONFIG_MEM_MEMORY_BANK:
    /* TODO: */
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n");
    exit(1);
  }

  return port2port_name_map;
}

/*********************************************************************
 * Create a port-to-port map for a ReRAM-based memory module 
 * Memory bank 
 * -----------
 *
 *        config_bus (BL)   config_bus (WL) 
 *            |                   |
 *            v                   v
 *      +-------------------------------------+
 *      |        ReRAM-based Memory Module    |
 *      +-------------------------------------+
 *            |                   |
 *            v                   v
 *         Mem_out              Mem_outb
 **********************************************************************/
static 
std::map<std::string, BasicPort> generate_rram_mem_module_port2port_map(const BasicPort& config_bus,
                                                                        const std::vector<BasicPort>& mem_output_bus_ports,
                                                                        const e_config_protocol_type& sram_orgz_type) {
  std::map<std::string, BasicPort> port2port_name_map;

  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Not supported */
    break;
  case CONFIG_MEM_SCAN_CHAIN: {
    /* Link the head port of the memory module: 
     * the LSB of config bus port is the head port index
     */
    std::vector<BasicPort> config_bus_ports;
    config_bus_ports.push_back(BasicPort(generate_local_config_bus_port_name(), config_bus.get_msb(), config_bus.get_msb() + 1));
    BasicPort head_port(config_bus_ports[0].get_name(), config_bus_ports[0].get_lsb(), config_bus_ports[0].get_lsb()); 
    port2port_name_map[generate_configuration_chain_head_name()] = head_port;

    /* Link the tail port of the memory module: 
     * the MSB of config bus port is the tail port index 
     */
    BasicPort tail_port(config_bus_ports[0].get_name(), config_bus_ports[0].get_msb(), config_bus_ports[0].get_msb()); 
    port2port_name_map[generate_configuration_chain_tail_name()] = tail_port;

    /* Link the SRAM output ports of the memory module */ 
    VTR_ASSERT( 2 == mem_output_bus_ports.size() );
    port2port_name_map[generate_configurable_memory_data_out_name()] = mem_output_bus_ports[0];
    port2port_name_map[generate_configurable_memory_inverted_data_out_name()] = mem_output_bus_ports[1];
    break;
  }
  case CONFIG_MEM_MEMORY_BANK:
    /* TODO: link BL/WL/Reserved Ports to the inputs of a memory module */
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization!\n");
    exit(1);
  }

  return port2port_name_map;
}

/*********************************************************************
 * Create a port-to-port map for a memory module 
 * The content of the port-to-port map will depend not only 
 * the design technology of the memory cells but also the 
 * configuration styles of FPGA fabric.
 * Here we will branch on the design technology
 **********************************************************************/
std::map<std::string, BasicPort> generate_mem_module_port2port_map(const BasicPort& config_bus,
                                                                   const std::vector<BasicPort>& mem_output_bus_ports,
                                                                   const e_circuit_model_design_tech& mem_design_tech,
                                                                   const e_config_protocol_type& sram_orgz_type) {
  std::map<std::string, BasicPort> port2port_name_map;

  switch (mem_design_tech) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    port2port_name_map = generate_cmos_mem_module_port2port_map(config_bus, mem_output_bus_ports, sram_orgz_type);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    port2port_name_map = generate_rram_mem_module_port2port_map(config_bus, mem_output_bus_ports, sram_orgz_type);
    break;
  default:
    VTR_LOG_ERROR("Invalid type of memory design technology!\n");
    exit(1);
  }

  return port2port_name_map;
}

/*********************************************************************
 * Update the LSB and MSB of a configuration bus based on the number of 
 * memory bits of a CMOS memory module. 
 **********************************************************************/
static 
void update_cmos_mem_module_config_bus(const e_config_protocol_type& sram_orgz_type,
                                       const size_t& num_config_bits,
                                       BasicPort& config_bus) {
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Not supported */
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    /* Scan-chain of a memory module only has a head and a tail.
     * LSB and MSB of configuration bus will be shifted to the next head. 
     */
    VTR_ASSERT(true == config_bus.rotate(1));
    break;
  case CONFIG_MEM_MEMORY_BANK:
    /* In this case, a memory module has a number of BL/WL and BLB/WLB (possibly).
     * LSB and MSB of configuration bus will be shifted by the number of BL/WL/BLB/WLB. 
     */
    VTR_ASSERT(true == config_bus.rotate(num_config_bits));
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n");
    exit(1);
  }
}

/*********************************************************************
 * Update the LSB and MSB of a configuration bus based on the number of 
 * memory bits of a ReRAM memory module. 
 **********************************************************************/
static 
void update_rram_mem_module_config_bus(const e_config_protocol_type& sram_orgz_type,
                                       BasicPort& config_bus) {
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Not supported */
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    /* Scan-chain of a memory module only has a head and a tail.
     * LSB and MSB of configuration bus will be shifted to the next head. 
     * TODO: this may be changed later!!!
     */
    VTR_ASSERT(true == config_bus.rotate(1));
    break;
  case CONFIG_MEM_MEMORY_BANK:
    /* In this case, a memory module contains unique BL/WL or BLB/WLB,
     * which are not shared with other modules   
     * TODO: this may be changed later!!!
     */
    VTR_ASSERT(true == config_bus.rotate(1));
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n");
    exit(1);
  }
}

/*********************************************************************
 * Update the LSB and MSB of a configuration bus based on the number of 
 * memory bits of a module. 
 * Note that this function is designed to do such simple job, in purpose of
 * being independent from adding ports or printing ports.
 * As such, this function can be re-used in bitstream generation 
 * when Verilog generation is not needed.  
 * DO NOT update the configuration bus in the function of adding/printing ports
 **********************************************************************/
void update_mem_module_config_bus(const e_config_protocol_type& sram_orgz_type,
                                  const e_circuit_model_design_tech& mem_design_tech,
                                  const size_t& num_config_bits,
                                  BasicPort& config_bus) {
  switch (mem_design_tech) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    update_cmos_mem_module_config_bus(sram_orgz_type, num_config_bits, config_bus);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    update_rram_mem_module_config_bus(sram_orgz_type, config_bus);
    break;
  default:
    VTR_LOG_ERROR("Invalid type of memory design technology!\n");
    exit(1);
  }
}

/********************************************************************
 * Check if the MSB of a configuration bus of a switch block
 * matches the expected value
 ********************************************************************/
bool check_mem_config_bus(const e_config_protocol_type& sram_orgz_type, 
                          const BasicPort& config_bus, 
                          const size_t& local_expected_msb) {
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Not supported */
    return false;
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    /* TODO: comment on why
     */
    return (local_expected_msb == config_bus.get_msb());
    break;
  case CONFIG_MEM_MEMORY_BANK:
    /* TODO: comment on why
     */
    return (local_expected_msb == config_bus.get_msb());
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n");
    exit(1);
  }
  /* Reach here, it means something goes wrong, return a false value */
  return false;
}

/********************************************************************
 * Generate a list of ports that are used for SRAM configuration to a module
 * The type and names of added ports strongly depend on the 
 * organization of SRAMs.
 * 1. Standalone SRAMs: 
 *    two ports will be added, which are regular output and inverted output 
 * 2. Scan-chain Flip-flops:
 *    two ports will be added, which are the head of scan-chain 
 *    and the tail of scan-chain
 *    IMPORTANT: the port size will be forced to 1 in this case 
 *               because the head and tail are both 1-bit ports!!!
 * 3. Memory decoders:
 *    2-4 ports will be added, depending on the ports available in the SRAM
 *    Among these, two ports are mandatory: BL and WL 
 *    The other two ports are optional: BLB and WLB
 *    Note that the constraints are correletated to the checking rules 
 *    in check_circuit_library()
 ********************************************************************/
std::vector<std::string> generate_sram_port_names(const CircuitLibrary& circuit_lib,
                                                  const CircuitModelId& sram_model,
                                                  const e_config_protocol_type sram_orgz_type) {
  std::vector<std::string> sram_port_names;
  /* Prepare a list of port types to be added, the port type will be used to create port names */
  std::vector<e_circuit_model_port_type> model_port_types; 

  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE: 
    model_port_types.push_back(CIRCUIT_MODEL_PORT_INPUT);
    model_port_types.push_back(CIRCUIT_MODEL_PORT_OUTPUT);
    break;
  case CONFIG_MEM_SCAN_CHAIN: 
    model_port_types.push_back(CIRCUIT_MODEL_PORT_INPUT);
    model_port_types.push_back(CIRCUIT_MODEL_PORT_OUTPUT);
    break;
  case CONFIG_MEM_MEMORY_BANK: {
    std::vector<e_circuit_model_port_type> ports_to_search;
    ports_to_search.push_back(CIRCUIT_MODEL_PORT_BL);
    ports_to_search.push_back(CIRCUIT_MODEL_PORT_WL);
    ports_to_search.push_back(CIRCUIT_MODEL_PORT_BLB);
    ports_to_search.push_back(CIRCUIT_MODEL_PORT_WLB);
    /* Try to find a BL/WL/BLB/WLB port and update the port types/module port types to be added */
    for (const auto& port_to_search : ports_to_search) {
      std::vector<CircuitPortId> found_port = circuit_lib.model_ports_by_type(sram_model, port_to_search);
      if (0 == found_port.size()) {
        continue;
      }
      model_port_types.push_back(port_to_search);
    }
    break;
  }
  case CONFIG_MEM_FRAME_BASED: {
    model_port_types.push_back(CIRCUIT_MODEL_PORT_INPUT);
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization !\n");
    exit(1);
  }

  /* Add ports to the module manager */
  for (size_t iport = 0; iport < model_port_types.size(); ++iport) {
    /* Create a port */
    std::string port_name = generate_sram_port_name(sram_orgz_type, model_port_types[iport]);
    sram_port_names.push_back(port_name);
  }

  return sram_port_names;
}

/********************************************************************
 * Generate a list of ports that are used for SRAM configuration to a module
 * 1. Standalone SRAMs: 
 *    use the suggested port_size 
 * 2. Scan-chain Flip-flops:
 *    IMPORTANT: the port size will be forced to 1 in this case 
 * 3. Memory decoders:
 *    use the suggested port_size 
 ********************************************************************/
size_t generate_sram_port_size(const e_config_protocol_type sram_orgz_type,
                               const size_t& num_config_bits) {
  size_t sram_port_size = num_config_bits;

  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE: 
    break;
  case CONFIG_MEM_SCAN_CHAIN: 
    /* CCFF head/tail are single-bit ports */
    sram_port_size = 1;
    break;
  case CONFIG_MEM_MEMORY_BANK:
    break;
  case CONFIG_MEM_FRAME_BASED:
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization!\n");
    exit(1);
  }

  return sram_port_size;
}

} /* end namespace openfpga */
