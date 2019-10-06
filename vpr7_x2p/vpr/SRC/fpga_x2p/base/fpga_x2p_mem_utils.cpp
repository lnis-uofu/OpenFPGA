/*********************************************************************
 * This file includes functions that are used for 
 * generating ports for memory modules 
 *********************************************************************/
#include "vtr_assert.h"
#include "util.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_mem_utils.h"

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
std::map<std::string, BasicPort> generate_cmos_mem_module_port2port_map(const ModuleManager& module_manager, 
                                                                        const ModuleId& mem_module,
                                                                        const BasicPort& config_bus,
                                                                        const std::vector<BasicPort>& mem_output_bus_ports,
                                                                        const e_sram_orgz& sram_orgz_type) {
  std::map<std::string, BasicPort> port2port_name_map;

  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* Nothing to do */
    break;
  case SPICE_SRAM_SCAN_CHAIN: {
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
    port2port_name_map[generate_configuration_chain_data_out_name()] = mem_output_bus_ports[0];
    port2port_name_map[generate_configuration_chain_inverted_data_out_name()] = mem_output_bus_ports[1];
    break;
  }
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO: */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
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
std::map<std::string, BasicPort> generate_rram_mem_module_port2port_map(const ModuleManager& module_manager, 
                                                                        const ModuleId& mem_module,
                                                                        const BasicPort& config_bus,
                                                                        const std::vector<BasicPort>& mem_output_bus_ports,
                                                                        const e_sram_orgz& sram_orgz_type) {
  std::map<std::string, BasicPort> port2port_name_map;

  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* Not supported */
    break;
  case SPICE_SRAM_SCAN_CHAIN: {
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
    port2port_name_map[generate_configuration_chain_data_out_name()] = mem_output_bus_ports[0];
    port2port_name_map[generate_configuration_chain_inverted_data_out_name()] = mem_output_bus_ports[1];
    break;
  }
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO: link BL/WL/Reserved Ports to the inputs of a memory module */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
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
std::map<std::string, BasicPort> generate_mem_module_port2port_map(const ModuleManager& module_manager, 
                                                                   const ModuleId& mem_module,
                                                                   const BasicPort& config_bus,
                                                                   const std::vector<BasicPort>& mem_output_bus_ports,
                                                                   const e_spice_model_design_tech& mem_design_tech,
                                                                   const e_sram_orgz& sram_orgz_type) {
  std::map<std::string, BasicPort> port2port_name_map;

  switch (mem_design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    port2port_name_map = generate_cmos_mem_module_port2port_map(module_manager, mem_module, config_bus, mem_output_bus_ports, sram_orgz_type);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    port2port_name_map = generate_rram_mem_module_port2port_map(module_manager, mem_module, config_bus, mem_output_bus_ports, sram_orgz_type);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of memory design technology !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return port2port_name_map;
}

/*********************************************************************
 * Update the LSB and MSB of a configuration bus based on the number of 
 * memory bits of a CMOS memory module. 
 **********************************************************************/
static 
void update_cmos_mem_module_config_bus(const e_sram_orgz& sram_orgz_type,
                                       const size_t& num_config_bits,
                                       BasicPort& config_bus) {
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* Not supported */
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Scan-chain of a memory module only has a head and a tail.
     * LSB and MSB of configuration bus will be shifted to the next head. 
     */
    VTR_ASSERT(true == config_bus.rotate(1));
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* In this case, a memory module has a number of BL/WL and BLB/WLB (possibly).
     * LSB and MSB of configuration bus will be shifted by the number of BL/WL/BLB/WLB. 
     */
    VTR_ASSERT(true == config_bus.rotate(num_config_bits));
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/*********************************************************************
 * Update the LSB and MSB of a configuration bus based on the number of 
 * memory bits of a ReRAM memory module. 
 **********************************************************************/
static 
void update_rram_mem_module_config_bus(const e_sram_orgz& sram_orgz_type,
                                       const size_t& num_config_bits,
                                       BasicPort& config_bus) {
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* Not supported */
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Scan-chain of a memory module only has a head and a tail.
     * LSB and MSB of configuration bus will be shifted to the next head. 
     * TODO: this may be changed later!!!
     */
    VTR_ASSERT(true == config_bus.rotate(1));
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* In this case, a memory module contains unique BL/WL or BLB/WLB,
     * which are not shared with other modules   
     * TODO: this may be changed later!!!
     */
    VTR_ASSERT(true == config_bus.rotate(1));
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
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
void update_mem_module_config_bus(const e_sram_orgz& sram_orgz_type,
                                  const e_spice_model_design_tech& mem_design_tech,
                                  const size_t& num_config_bits,
                                  BasicPort& config_bus) {
  switch (mem_design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    update_cmos_mem_module_config_bus(sram_orgz_type, num_config_bits, config_bus);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    update_rram_mem_module_config_bus(sram_orgz_type, num_config_bits, config_bus);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of memory design technology !\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/********************************************************************
 * Check if the MSB of a configuration bus of a switch block
 * matches the expected value
 ********************************************************************/
bool check_mem_config_bus(const e_sram_orgz& sram_orgz_type, 
                          const BasicPort& config_bus, 
                          const size_t& local_expected_msb) {
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* Not supported */
    return false;
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* TODO: comment on why
     */
    return (local_expected_msb == config_bus.get_msb());
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO: comment on why
     */
    return (local_expected_msb == config_bus.get_msb());
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Reach here, it means something goes wrong, return a false value */
  return false;
}
