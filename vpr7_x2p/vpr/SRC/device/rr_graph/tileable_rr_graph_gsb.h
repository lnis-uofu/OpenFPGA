#ifndef TILEABLE_RR_GRAPH_GSB_H
#define TILEABLE_RR_GRAPH_GSB_H

#include <vector>

#include "vtr_ndmatrix.h"

#include "rr_blocks.h"
#include "fpga_x2p_types.h"


/************************************************************************
 * Data stuctures related to the functions 
 ***********************************************************************/
typedef std::vector<std::vector<std::vector<int>>> t_track2track_map;
typedef std::vector<std::vector<std::vector<int>>> t_track2pin_map;
typedef std::vector<std::vector<std::vector<int>>> t_pin2track_map;

/************************************************************************
 * Functions 
 ***********************************************************************/
t_track2track_map build_gsb_track_to_track_map(const t_rr_graph* rr_graph,
                                               const RRGSB& rr_gsb,
                                               const enum e_switch_block_type sb_type, 
                                               const int Fs,
                                               const std::vector<t_segment_inf> segment_inf);

RRGSB build_one_tileable_rr_gsb(const DeviceCoordinator& device_range, 
                                const std::vector<size_t> device_chan_width, 
                                const std::vector<t_segment_inf> segment_inf,
                                const DeviceCoordinator& gsb_coordinator, 
                                t_rr_graph* rr_graph);

void build_edges_for_one_tileable_rr_gsb(const t_rr_graph* rr_graph, 
                                         const RRGSB* rr_gsb,
                                         const t_track2pin_map track2ipin_map,
                                         const t_pin2track_map opin2track_map,
                                         const t_track2track_map track2track_map);

t_track2pin_map build_gsb_track_to_ipin_map(t_rr_graph* rr_graph,
                                            const RRGSB& rr_gsb, 
                                            const std::vector<std::vector<t_grid_tile>> grids, 
                                            const std::vector<t_segment_inf> segment_inf, 
                                            int** Fc_in);

t_pin2track_map build_gsb_opin_to_track_map(t_rr_graph* rr_graph,
                                            const RRGSB& rr_gsb, 
                                            const std::vector<std::vector<t_grid_tile>> grids, 
                                            const std::vector<t_segment_inf> segment_inf, 
                                            int** Fc_out);

void build_direct_connections_for_one_gsb(t_rr_graph* rr_graph,
                                          const DeviceCoordinator& device_size,
                                          const std::vector<std::vector<t_grid_tile>> grids,
                                          const DeviceCoordinator& from_grid_coordinator,
                                          const t_grid_tile& from_grid,
                                          const int delayless_switch, 
                                          const int num_directs, 
                                          const t_direct_inf *directs, 
                                          const t_clb_to_clb_directs *clb_to_clb_directs);



#endif

