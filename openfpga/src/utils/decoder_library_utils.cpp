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
