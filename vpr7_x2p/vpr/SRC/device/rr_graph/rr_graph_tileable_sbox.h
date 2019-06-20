#ifndef RR_GRAPH_TILEABLE_SBOX_H
#define RR_GRAPH_TILEABLE_SBOX_H

#include <vector>

#include "vtr_ndmatrix.h"

#include "rr_blocks.h"
#include "fpga_x2p_types.h"

typedef std::vector<std::vector<std::vector<int>>> t_track2track_map;

t_track2track_map build_gsb_track_to_track_map(const t_rr_graph* rr_graph,
                                               const RRGSB& rr_gsb,
                                               const enum e_switch_block_type sb_type, 
                                               const int Fs,
                                               const std::vector<t_segment_inf> segment_inf);

#endif

