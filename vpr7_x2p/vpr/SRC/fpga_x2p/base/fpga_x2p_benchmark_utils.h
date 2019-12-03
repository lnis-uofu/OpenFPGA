#ifndef FPGA_X2P_BENCHMARK_UTILS_H
#define FPGA_X2P_BENCHMARK_UTILS_H

#include <vector>
#include <string>
#include "vpr_types.h"
#include "vtr_geometry.h"

std::vector<std::string> find_benchmark_clock_port_name(const std::vector<t_logical_block>& L_logical_blocks);

size_t find_benchmark_io_index(const t_logical_block& io_lb,
                               const vtr::Point<size_t>& device_size,
                               const std::vector<std::vector<t_grid_tile>>& L_grids, 
                               const std::vector<t_block>& L_blocks);

#endif
