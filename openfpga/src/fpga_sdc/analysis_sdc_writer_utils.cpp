/********************************************************************
 * This file includes most utilized functions 
 * that are used to output a SDC file
 * in order to constrain a FPGA fabric (P&Red netlist) mapped to a benchmark 
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "sdc_writer_utils.h"
#include "analysis_sdc_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Identify if a node should be disabled during analysis SDC generation
 *******************************************************************/
bool is_rr_node_to_be_disable_for_analysis(const VprRoutingAnnotation& routing_annotation,
                                           const RRNodeId& cur_rr_node) {
  /* Conditions to enable timing analysis for a node 
   * 1st condition: it have a valid net_number 
   * TODO: 2nd condition: it is not an parasitic net 
   */
  return ClusterNetId::INVALID() == routing_annotation.rr_node_net(cur_rr_node);
}

/********************************************************************
 * Disable all the unused inputs of routing multiplexers, which are not used by benchmark 
 * Here, we start from each input of a routing module, and traverse forward to the sink 
 * port of the module net whose source is the input
 * We will find the instance name which is the parent of the sink port, and search the 
 * net id through the instance_name_to_net_map 
 * The the net id does not match the net id of this input, we will disable the sink port!
 *
 *                   parent_module
 *                 +-----------------------
 *                 |           MUX instance A
 *                 |          +-----------
 *   input_port--->|--+---x-->| sink port (disable! net_id = Y) 
 *   (net_id = X)  |  |       +----------
 *                 |  |        MUX instance B
 *                 |  |       +----------
 *                 |  +------>| sink port (do not disable! net_id = X)
 *
 *******************************************************************/
void disable_analysis_module_input_pin_net_sinks(std::fstream& fp,
                                                 const ModuleManager& module_manager,
                                                 const ModuleId& parent_module,
                                                 const std::string& parent_instance_name,
                                                 const ModulePortId& module_input_port,
                                                 const size_t& module_input_pin,
                                                 const AtomNetId& mapped_net,
                                                 const std::map<std::string, AtomNetId> mux_instance_to_net_map) {
  /* Validate file stream */
  valid_file_stream(fp);

  /* Find the module net which sources from this port! */
  ModuleNetId module_net = module_manager.module_instance_port_net(parent_module, parent_module, 0, module_input_port, module_input_pin); 
  if (true != module_manager.valid_module_net_id(parent_module, module_net))
  VTR_ASSERT(true == module_manager.valid_module_net_id(parent_module, module_net));

  /* Touch each sink of the net! */
  for (const ModuleNetSinkId& sink_id : module_manager.module_net_sinks(parent_module, module_net)) {
    ModuleId sink_module = module_manager.net_sink_modules(parent_module, module_net)[sink_id]; 
    size_t sink_instance = module_manager.net_sink_instances(parent_module, module_net)[sink_id]; 

    /* Skip when sink module is the parent module, 
     * the output ports of parent modules have been disabled/enabled already! 
     */
    if (sink_module == parent_module) {
      continue;
    }

    std::string sink_instance_name = module_manager.instance_name(parent_module, sink_module, sink_instance);
    bool disable_timing = false;
    /* Check if this node is used by benchmark  */
    if (AtomNetId::INVALID() == mapped_net) {
      /* Disable all the sinks! */
      disable_timing = true;
    } else {
      std::map<std::string, AtomNetId>::const_iterator it = mux_instance_to_net_map.find(sink_instance_name);
      if (it != mux_instance_to_net_map.end()) {
        /* See if the net id matches. If does not match, we should disable! */
        if (mapped_net != mux_instance_to_net_map.at(sink_instance_name)) {
          disable_timing = true;
        }
      }
    }

    /* Time to write SDC command to disable timing or not */
    if (false == disable_timing) {
      continue;
    }

    BasicPort sink_port = module_manager.module_port(sink_module, module_manager.net_sink_ports(parent_module, module_net)[sink_id]);
    sink_port.set_width(module_manager.net_sink_pins(parent_module, module_net)[sink_id],
                        module_manager.net_sink_pins(parent_module, module_net)[sink_id]);

    VTR_ASSERT(!sink_instance_name.empty());
    /* Get the input id that is used! Disable the unused inputs! */
    fp << "set_disable_timing ";
    fp << parent_instance_name << "/";
    fp << sink_instance_name << "/";
    fp << generate_sdc_port(sink_port);
    fp << std::endl;
  }
}

/********************************************************************
 * Disable all the unused inputs of routing multiplexers, which are not used by benchmark 
 * Here, we start from each input of a routing module, and traverse forward to the sink 
 * port of the module net whose source is the input
 * We will find the instance name which is the parent of the sink port, and search the 
 * net id through the instance_name_to_net_map 
 * The the net id does not match the net id of this input, we will disable the sink port!
 *
 *                   parent_module
 *                 +-----------------------
 *                 |           MUX instance A
 *                 |          +-----------
 *   input_port--->|--+---x-->| sink port (disable! net_id = Y) 
 *   (net_id = X)  |  |       +----------
 *                 |  |        MUX instance B
 *                 |  |       +----------
 *                 |  +------>| sink port (do not disable! net_id = X)
 *
 *******************************************************************/
void disable_analysis_module_input_port_net_sinks(std::fstream& fp,
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& parent_module,
                                                  const std::string& parent_instance_name,
                                                  const ModulePortId& module_input_port,
                                                  const AtomNetId& mapped_net,
                                                  const std::map<std::string, AtomNetId> mux_instance_to_net_map) {
  /* Validate file stream */
  valid_file_stream(fp);

  /* Find the module net which sources from this port! */
  for (const size_t& pin : module_manager.module_port(parent_module, module_input_port).pins()) {
    disable_analysis_module_input_pin_net_sinks(fp, module_manager, parent_module,
                                                parent_instance_name,
                                                module_input_port, pin,
                                                mapped_net,
                                                mux_instance_to_net_map);
  }
}

/********************************************************************
 * Disable all the unused inputs of routing multiplexers, which are not used by benchmark 
 * Here, we start from each output of a child module, and traverse forward to the sink 
 * port of the module net whose source is the input
 * We will find the instance name which is the parent of the sink port, and search the 
 * net id through the instance_name_to_net_map 
 * The the net id does not match the net id of this input, we will disable the sink port!
 *
 *                         Parent_module
 *                         +---------------------------------------------
 *                         |
 *                         |    +--------------------------------------------+
 *                         |    |     MUX                  child_module      |
 *                         |    |    +-------------+       +-----------+     |
 *                         |    +--->| Routing     |------>|           |     |
 *    input_pin0(netA) --->|----x--->| Multiplexer | netA  | output_pin|-----+
 *                         |         +-------------+       |           | netA
 *                         |                               |           |
 *

 *
 *******************************************************************/
void disable_analysis_module_output_pin_net_sinks(std::fstream& fp,
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& parent_module,
                                                  const std::string& parent_instance_name,
                                                  const ModuleId& child_module,
                                                  const size_t& child_instance,
                                                  const ModulePortId& child_module_port,
                                                  const size_t& child_module_pin,
                                                  const AtomNetId& mapped_net,
                                                  const std::map<std::string, AtomNetId> mux_instance_to_net_map) {
  /* Validate file stream */
  valid_file_stream(fp);

  /* Find the module net which sources from this port! */
  ModuleNetId module_net = module_manager.module_instance_port_net(parent_module, child_module, child_instance, child_module_port, child_module_pin); 
  VTR_ASSERT(true == module_manager.valid_module_net_id(parent_module, module_net));

  /* Touch each sink of the net! */
  for (const ModuleNetSinkId& sink_id : module_manager.module_net_sinks(parent_module, module_net)) {
    ModuleId sink_module = module_manager.net_sink_modules(parent_module, module_net)[sink_id]; 
    size_t sink_instance = module_manager.net_sink_instances(parent_module, module_net)[sink_id]; 

    /* Skip when sink module is the parent module, 
     * the output ports of parent modules have been disabled/enabled already! 
     */
    if (sink_module == parent_module) {
      continue;
    }

    std::string sink_instance_name = module_manager.instance_name(parent_module, sink_module, sink_instance);
    bool disable_timing = false;
    /* Check if this node is used by benchmark  */
    if (AtomNetId::INVALID() == mapped_net) {
      /* Disable all the sinks! */
      disable_timing = true;
    } else {
      std::map<std::string, AtomNetId>::const_iterator it = mux_instance_to_net_map.find(sink_instance_name);
      if (it != mux_instance_to_net_map.end()) {
        /* See if the net id matches. If does not match, we should disable! */
        if (mapped_net != mux_instance_to_net_map.at(sink_instance_name)) {
          disable_timing = true;
        }
      }
    }

    /* Time to write SDC command to disable timing or not */
    if (false == disable_timing) {
      continue;
    }

    BasicPort sink_port = module_manager.module_port(sink_module, module_manager.net_sink_ports(parent_module, module_net)[sink_id]);
    sink_port.set_width(module_manager.net_sink_pins(parent_module, module_net)[sink_id],
                        module_manager.net_sink_pins(parent_module, module_net)[sink_id]);

    VTR_ASSERT(!sink_instance_name.empty());
    /* Get the input id that is used! Disable the unused inputs! */
    fp << "set_disable_timing ";
    fp << parent_instance_name << "/";
    fp << sink_instance_name << "/";
    fp << generate_sdc_port(sink_port);
    fp << std::endl;
  }
}

} /* end namespace openfpga */
