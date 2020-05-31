/***************************************************************************************
 * This file includes most utilized functions for the DecoderLibrary data structure
 ***************************************************************************************/
#include <cmath>

#include "vtr_assert.h"

#include "decoder_library_utils.h"

/* Begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * NOTE: This function is mainly designed for local decoders inside multiplexers
 * Find the size of address lines for a decoder with a given data output size
 *                   Addr lines 
 *                   | | ... | 
 *                   v v     v
 *                 +-----------+
 *                /    Local    \
 *               /    Decoder    \
 *              +-----------------+
 *                | | | ... | | |
 *                v v v     v v v
 *                   Data outputs
 *               
 *  The outputs are assumes to be one-hot codes (at most only one '1' exist)
 *  Considering this fact, there are only num_of_outputs + 1 conditions to be encoded.
 *  Therefore, the number of inputs is ceil(log(num_of_outputs+1)/log(2))
 *  We plus 1, which is all-zero condition for outputs
 ***************************************************************************************/
size_t find_mux_local_decoder_addr_size(const size_t& data_size) {
  /* if data size is 1, it is an corner case for the decoder (addr = 1) */
  if (1 == data_size) {
    return 1;
  }
  VTR_ASSERT (2 <= data_size);
  return ceil(log(data_size) / log(2));
}

/***************************************************************************************
 * Find the size of address lines for a memory decoder to access a memory array
 *                   Addr lines 
 *                   | | ... | 
 *                   v v     v
 *                 +-----------+
 *                /    Local    \
 *               /    Decoder    \
 *              +-----------------+
 *                | | | ... | | |
 *                v v v     v v v
 *                   Data outputs
 *
 *          +------+  +------+      +------+
 *          | SRAM |  | SRAM |  ... | SRAM |
 *          | [0]  |  | [1]  |      |  [i] |
 *          +------+  +------+      +------+
 *
 *          +------+  +------+      +------+
 *          | SRAM |  | SRAM |  ... | SRAM |
 *          | [i+1]|  | [i+2]|      |[2i-1]|
 *          +------+  +------+      +------+
 *
 *            ...       ...            ...
 *
 *          +------+  +------+      +------+
 *          | SRAM |  | SRAM |  ... | SRAM |
 *          | [x]  |  | [x+1]|      |  [N] |
 *          +------+  +------+      +------+
 *
 *  Due to the shared lines in the array, 
 *  each memory decoder (BL or WL) will access sqrt(N) control lins (BL or WL)
 *  Then we can use the function for mux local encoder to compute the address size 
 *               
 ***************************************************************************************/
size_t find_memory_decoder_addr_size(const size_t& num_mems) {
  return find_mux_local_decoder_addr_size(find_memory_decoder_data_size(num_mems));
}

/***************************************************************************************
 * Find the size of data lines (BLs and WLs) for a memory decoder to access a memory array
 * As the memory cells are organized in an array with shared bit lines and word lines,
 * the number of data lines will be a square root of the number of memory cells.
 ***************************************************************************************/
size_t find_memory_decoder_data_size(const size_t& num_mems) {
  return (size_t)std::ceil(std::sqrt((float)num_mems));
}

/***************************************************************************************
 * Try to find if the decoder already exists in the library, 
 * If there is no such decoder, add it to the library 
 ***************************************************************************************/
DecoderId add_mux_local_decoder_to_library(DecoderLibrary& decoder_lib, 
                                           const size_t data_size) {
  size_t addr_size = find_mux_local_decoder_addr_size(data_size);

  DecoderId decoder_id = decoder_lib.find_decoder(addr_size, data_size, false, false, true);

  if (DecoderId::INVALID() == decoder_id) { 
    /* Add the decoder */
    return decoder_lib.add_decoder(addr_size, data_size, false, false, true);
  }

  /* There is already a decoder in the library, return the decoder id */
  return decoder_id;
}

} /* End namespace openfpga*/
