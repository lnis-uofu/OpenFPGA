/**************************************************
 * This file includes only declarations for 
 * the data structures to describe decoders
 * Please refer to decoder_library.h for more details
 *************************************************/
#ifndef DECODER_LIBRARY_FWD_H
#define DECODER_LIBRARY_FWD_H

#include "vtr_strong_id.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Strong Ids for MUXes */
struct decoder_id_tag;

typedef vtr::StrongId<decoder_id_tag> DecoderId;

class DecoderLibrary;

} /* End namespace openfpga*/

#endif 
