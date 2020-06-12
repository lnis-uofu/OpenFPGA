/**************************************************
 * This file includes only declarations for 
 * the data structures to describe multiplexer structures
 * Please refer to mux_library.h for more details
 *************************************************/
#ifndef MUX_LIBRARY_FWD_H
#define MUX_LIBRARY_FWD_H

#include "vtr_strong_id.h"

/* begin namespace openfpga */
namespace openfpga {

/* Strong Ids for MUXes */
struct mux_id_tag;
struct mux_local_decoder_id_tag;

typedef vtr::StrongId<mux_id_tag> MuxId;
typedef vtr::StrongId<mux_local_decoder_id_tag> MuxLocalDecoderId;

class MuxLibrary;

} /* end namespace openfpga */

#endif 
