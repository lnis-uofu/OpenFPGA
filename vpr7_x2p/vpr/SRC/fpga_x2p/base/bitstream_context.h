/******************************************************************************
 * This file introduces a data structure to store bitstream-related information 
 ******************************************************************************/
#ifndef BITSTREAM_CONTEXT_H
#define BITSTREAM_CONTEXT_H

#include "vtr_vector.h"
#include "bitstream_context_fwd.h"

class BitstreamContext {
  private: /* Internal data */
    enum e_sram_orgz config_scheme_;  /* The type of configuration protocol */
    CircuitModelId& sram_model_;      /* The memory circuit model used by the Bitstream generation */
    size_t num_memory_bits_;          /* Number of memory bits */ 
    size_t num_bls_;                  /* Number of Bit Lines */
    size_t num_wls_;                  /* Number of Word Lines */

    size_t num_reserved_bls_;         /* Number of reserved Bit Lines, ONLY applicable to RRAM-based FPGA */
    size_t num_reserved_wls_;         /* Number of reserved Word Lines, ONLY applicable to RRAM-based FPGA  */
    /* Unique id of a bit in the Bitstream */
    vtr::vector<ConfigBitId, ConfigBitId> bit_ids_; 
    /* Bit line address of a bit in the Bitream: ONLY applicable to memory-decoders */
    vtr::vector<ConfigBitId, size_t> bl_addr_;     
    /* Word line address of a bit in the Bitream: ONLY applicable to memory-decoders */
    vtr::vector<ConfigBitId, size_t> wl_addr_;     
    /* value of a bit in the Bitream */
    vtr::vector<ConfigBitId, bool> bit_val_;
};

#endif

