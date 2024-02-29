/***************************************************************************************
 * Header file for most utilized functions for the DecoderLibrary data structure
 ***************************************************************************************/
#ifndef DECODER_LIBRARY_UTILS_H
#define DECODER_LIBRARY_UTILS_H

#include "decoder_library.h"

/* Begin namespace openfpga */
namespace openfpga {

bool need_mux_local_decoder(const size_t& data_size);

size_t find_mux_local_decoder_addr_size(const size_t& data_size);

size_t find_memory_decoder_addr_size(const size_t& num_mems);

size_t find_memory_decoder_data_size(const size_t& num_mems,
                                     const size_t& defined_num_wl,
                                     const bool is_bl);

size_t find_memory_wl_decoder_data_size(const size_t& num_mems,
                                        const size_t& num_bls);

DecoderId add_mux_local_decoder_to_library(DecoderLibrary& decoder_lib,
                                           const size_t data_size);

} /* End namespace openfpga*/

#endif
