/**************************************************
 * This file includes only declarations for 
 * the data structures for bitstream database
 * Please refer to bitstream_context.h for more details
 *************************************************/
#ifndef BITSTREAM_CONTEXT_FWD_H
#define BITSTREAM_CONTEXT_MANAGER_FWD_H

#include "vtr_strong_id.h"

/* Strong Ids for BitstreamContext */
struct config_bit_id_tag;

typedef vtr::StrongId<config_bit_id_tag> ConfigBitId;

class BitstreamContext;

#endif 
