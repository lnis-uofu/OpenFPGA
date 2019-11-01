/********************************************************************
 * This file includes most utilized functions to manipulate data
 * structures that are related to the input benchmark circuit
 *******************************************************************/
#include "vtr_assert.h"
#include "sides.h"

#include "fpga_x2p_benchmark_utils.h"

/********************************************************************
 * Find the clock port name to be used in this testbench
 *******************************************************************/
std::vector<std::string> find_benchmark_clock_port_name(const std::vector<t_logical_block>& L_logical_blocks) {
  std::vector<std::string> clock_port_names;

  for (const t_logical_block& lb : L_logical_blocks) {
    /* Bypass non-I/O logical blocks ! */
    if ( (VPACK_INPAD != lb.type) && (VPACK_OUTPAD != lb.type) ) {
      continue;
    }

    /* Find the clock signals */
    if ( (VPACK_INPAD == lb.type) && (TRUE == lb.is_clock) ) {
      clock_port_names.push_back(std::string(lb.name));
    }
  }

  return clock_port_names;
}

/********************************************************************
 * Find the I/O index in the FPGA top-level module 
 * that an I/O logical block is mapped to 
 * Note that this function follows the sequence in I/O grid instanciation
 * in build_top_module(), where I/Os are instanciated from
 * TOP, RIGHT, BOTTOM to LEFT sides.
 * Therefore, the I/O indices will follow this sequence, where 0 starts 
 * from the TOP side
 *
 * This function will use the clb_index in each t_logical_block 
 * to spot a t_block that the I/O is mapped
 * Through the t_block, we can find a detailed coordinate (x,y,z),
 * based on which we can infer the I/O index in the top-level module
 *
 * Restrictions: if you change the sequence in I/O grid instanciation
 * in the top-level module, this function MUST be changed!!!
 *******************************************************************/
size_t find_benchmark_io_index(const t_logical_block& io_lb,
                               const vtr::Point<size_t>& device_size,
                               const std::vector<std::vector<t_grid_tile>>& L_grids, 
                               const std::vector<t_block>& L_blocks) {
  /* Ensure this is an I/O logical block */
  VTR_ASSERT(VPACK_INPAD == io_lb.type || VPACK_OUTPAD == io_lb.type);

  /* Ensure the clb index in the range */
  VTR_ASSERT((size_t)io_lb.clb_index < L_blocks.size() );

  /* Get the block (x, y, z) */
  size_t x = L_blocks[(size_t)io_lb.clb_index].x;
  size_t y = L_blocks[(size_t)io_lb.clb_index].y;
  size_t z = L_blocks[(size_t)io_lb.clb_index].z;

  /* Ensure the (x,y,z) is in the range of device */
  VTR_ASSERT( x < device_size.x() && y < device_size.y() ); 
  VTR_ASSERT( z < (size_t)L_grids[x][y].type->capacity );

  /* Infer the I/O index:
   * If the I/O is on the top side, the index will start from 0 
   * If the I/O is on the right side, the index will start from capacity * nx 
   * If the I/O is on the bottom side, the index will start from capacity * (nx + ny) 
   * If the I/O is on the bottom side, the index will start from capacity * (2 * nx + ny) 
   */
  std::map<e_side, size_t> io_index_offset;
  io_index_offset[TOP] = 0;

  /* For RIGHT side, sum the capacity of TOP side grids */
  io_index_offset[RIGHT] = 0;
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    io_index_offset[RIGHT] += L_grids[ix][device_size.y() - 1].type->capacity; 
  }

  /* For BOTTOM side, sum the capacity of RIGHT side grids */
  io_index_offset[BOTTOM] = io_index_offset[RIGHT];
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
    io_index_offset[BOTTOM] += L_grids[device_size.x() - 1][iy].type->capacity; 
  }

  /* For LEFT side, sum the capacity of BOTTOM side grids */
  io_index_offset[LEFT] = io_index_offset[BOTTOM];
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    io_index_offset[LEFT] += L_grids[ix][0].type->capacity; 
  }

  /* Find I/O grid side, I do not capasulate this in a function, because it is not so general */
  e_side io_side = NUM_SIDES;
  if (0 == y) {
    io_side = BOTTOM;
  }
  if (0 == x) { 
    io_side = LEFT;
  }
  if (device_size.x() - 1 == x) {
    io_side = RIGHT;
  }
  if (device_size.y() - 1 == y) {
    io_side = TOP;
  }
  VTR_ASSERT(NUM_SIDES != io_side);

  /* Now generate the io index */
  size_t io_index = size_t(-1);
  /* TOP side I/Os */
  if (device_size.y() - 1 == y) {
    io_index = io_index_offset[io_side];
    for (size_t ix = 1; ix < x ; ++ix) {
      io_index += L_grids[ix][y].type->capacity; 
    }
    io_index += z;
  }
  /* RIGHT side I/Os */
  if (device_size.x() - 1 == x) {
    io_index = io_index_offset[io_side];
    for (size_t iy = 1; iy < y; ++iy) {
      io_index += L_grids[x][iy].type->capacity; 
    }
    io_index += z;
  }
  /* BOTTOM side I/Os */
  if (0 == y) {
    io_index = io_index_offset[io_side];
    for (size_t ix = 1; ix < x; ++ix) {
      io_index += L_grids[ix][y].type->capacity; 
    }
    io_index += z;
  }
  /* LEFT side I/Os */
  if (0 == x) {
    io_index = io_index_offset[io_side];
    for (size_t iy = 1; iy < y; ++iy) {
      io_index += L_grids[x][iy].type->capacity; 
    }
    io_index += z;
  }

  return io_index;
}
