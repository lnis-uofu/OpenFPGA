/***************************************************************************************
 * Header file for most utilized functions for the DecoderLibrary data structure
 ***************************************************************************************/
#ifndef DECODER_LIBRARY_UTILS_H
#define DECODER_LIBRARY_UTILS_H

#include "decoder_library.h"

size_t find_mux_local_decoder_addr_size(const size_t& data_size);

DecoderId add_mux_local_decoder_to_library(DecoderLibrary& decoder_lib, 
                                           const size_t data_size);

#endif
