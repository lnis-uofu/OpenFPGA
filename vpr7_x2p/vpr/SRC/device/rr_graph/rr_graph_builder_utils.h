#ifndef RR_GRAPH_BUILDER_UTILS_H
#define RR_GRAPH_BUILDER_UTILS_H

#include "vpr_types.h"
#include "fpga_x2p_types.h"
#include "device_coordinator.h"

void tileable_rr_graph_init_rr_node(t_rr_node* cur_rr_node);

int get_grid_pin_class_index(const t_grid_tile& cur_grid,
                             const int pin_index);

enum e_side determine_io_grid_pin_side(const DeviceCoordinator& device_size, 
                                       const DeviceCoordinator& grid_coordinator);

std::vector<int> get_grid_side_pins(const t_grid_tile& cur_grid, 
                                    const enum e_pin_type pin_type, 
                                    const enum e_side pin_side, 
                                    const int pin_height);

size_t get_grid_num_pins(const t_grid_tile& cur_grid, 
                         const enum e_pin_type pin_type, 
                         const enum e_side io_side);

size_t get_grid_num_classes(const t_grid_tile& cur_grid, 
                            const enum e_pin_type pin_type);

void add_one_edge_for_two_rr_nodes(const t_rr_graph* rr_graph,
                                   const int src_rr_node_id, 
                                   const int des_rr_node_id,
                                   const short switch_id);

void add_edges_for_two_rr_nodes(const t_rr_graph* rr_graph,  
                                const int src_rr_node_id, 
                                const std::vector<int> des_rr_node, 
                                const std::vector<short> driver_switches);

DeviceCoordinator get_track_rr_node_start_coordinator(const t_rr_node* track_rr_node);

DeviceCoordinator get_track_rr_node_end_coordinator(const t_rr_node* track_rr_node);

short get_track_rr_node_end_track_id(const t_rr_node* track_rr_node);

#endif

