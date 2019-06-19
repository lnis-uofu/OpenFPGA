#ifndef RR_GRAPH_TILEABLE_SBOX_H
#define RR_GRAPH_TILEABLE_SBOX_H

#include <vector>

#include "vtr_ndmatrix.h"

vtr::NdMatrix<std::vector<int>,3> alloc_and_load_tileable_switch_block_conn(size_t nodes_per_chan,
		enum e_switch_block_type switch_block_type, int Fs);

#endif

