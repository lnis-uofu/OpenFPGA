/**************************************************
 * This file includes only declarations for 
 * the data structures for bitstream database
 * Please refer to bitstream_manager.h for more details
 *************************************************/
#ifndef BITSTREAM_MANAGER_FWD_H
#define BITSTREAM_MANAGER_FWD_H

#include "vtr_strong_id.h"

/* Strong Ids for BitstreamContext */
struct config_block_id_tag;
struct config_bit_id_tag;

typedef vtr::StrongId<config_block_id_tag> ConfigBlockId;
typedef vtr::StrongId<config_bit_id_tag> ConfigBitId;

class BitstreamManager;

#endif 
