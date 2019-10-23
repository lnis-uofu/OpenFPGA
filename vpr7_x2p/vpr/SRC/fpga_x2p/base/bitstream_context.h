/******************************************************************************
 * This file introduces a data structure to store bitstream-related information 
 ******************************************************************************/
#ifndef BITSTREAM_CONTEXT_H
#define BITSTREAM_CONTEXT_H

#include <vector>
#include "vtr_vector.h"
#include "module_manager.h"

#include "bitstream_context_fwd.h"

class BitstreamContext {
  public: /* Types and ranges */
    typedef vtr::vector<ConfigBitId, ConfigBitId>::const_iterator config_bit_iterator;

    typedef vtr::Range<config_bit_iterator> config_bit_range;

  public: /* Public aggregators */
    /* Find all the configuration bits */
    config_bit_range bits() const;

  public:  /* Public Accessors */
    bool bit_value(const ConfigBitId& bit_id) const;

  public:  /* Public Mutators */
    ConfigBitId add_bit(const bool& bit_value);

  public:  /* Public Validators */
    bool valid_bit_id(const ConfigBitId& bit_id) const;

  private: /* Internal data */
    size_t num_shared_bits_;         /* Number of reserved Bit/WL Lines, ONLY applicable to RRAM-based FPGA */

    /* Unique id of a bit in the Bitstream */
    vtr::vector<ConfigBitId, ConfigBitId> bit_ids_; 
    /* value of a bit in the Bitstream */
    vtr::vector<ConfigBitId, bool> bit_values_;
    /* value of a shared configuration bits in the Bitstream */
    vtr::vector<ConfigBitId, std::vector<bool>> shared_config_bit_values_;

    /* Back-annotation for the bits */
    /* Parent Module of a bit in the Bitstream 
     * For each bit, the list of ModuleId and instance ids reflect its position in the module tree
     * The first ModuleId/Instance is the direct parent module/instance of the bit 
     * while the last ModuleId/instance is the top-level module/instance of the bit
     * For example: a bit could be back traced by
     * <LastModuleId>[<LastInstanceId>]/.../<FirstModuleId>[<FirstInstanceId>]
     */
    vtr::vector<ConfigBitId, std::vector<ModuleId>> bit_parent_modules_;
    vtr::vector<ConfigBitId, std::vector<size_t>> bit_parent_instances_;

    /* Fast lookup for bitstream */
    std::map<std::string, ConfigBitId> bit_lookup_;
};

#endif

