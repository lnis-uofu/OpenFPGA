#ifndef BUILD_ROUTING_MODULE_UTILS_H
#define BUILD_ROUTING_MODULE_UTILS_H

#include <vector>
#include "rr_blocks.h"
#include "module_manager.h"
#include "sides.h"
#include "vpr_types.h"

ModulePortId find_switch_block_module_chan_port(const ModuleManager& module_manager, 
                                                const ModuleId& sb_module,
                                                const RRGSB& rr_gsb, 
                                                const e_side& chan_side,
                                                t_rr_node* cur_rr_node,
                                                const PORTS& cur_rr_node_direction);

ModulePortId find_switch_block_module_input_port(const ModuleManager& module_manager,
                                                 const ModuleId& sb_module, 
                                                 const RRGSB& rr_gsb, 
                                                 const e_side& input_side,
                                                 t_rr_node* input_rr_node);

std::vector<ModulePortId> find_switch_block_module_input_ports(const ModuleManager& module_manager,
                                                               const ModuleId& sb_module, 
                                                               const RRGSB& rr_gsb, 
                                                               const std::vector<t_rr_node*>& input_rr_nodes);

ModulePortId find_connection_block_module_chan_port(const ModuleManager& module_manager, 
                                                    const ModuleId& cb_module,
                                                    const RRGSB& rr_gsb, 
                                                    const t_rr_type& cb_type,
                                                    t_rr_node* chan_rr_node);

ModulePortId find_connection_block_module_ipin_port(const ModuleManager& module_manager,
                                                    const ModuleId& cb_module, 
                                                    const RRGSB& rr_gsb, 
                                                    t_rr_node* src_rr_node);

std::vector<ModulePortId> find_connection_block_module_input_ports(const ModuleManager& module_manager,
                                                                   const ModuleId& cb_module,
                                                                   const RRGSB& rr_gsb, 
                                                                   const t_rr_type& cb_type,
                                                                   const std::vector<t_rr_node*>& input_rr_nodes);

#endif
