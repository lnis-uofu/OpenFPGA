/******************************************************************************
 * This file introduces a data structure to store fabric-dependent bitstream information 
 *
 * General concept
 * ---------------
 * The idea is to create a unified data structure that stores the sequence of configuration 
 * bit in the architecture bitstream database
 * as well as the information (such as address of each bit) required by a specific 
 * configuration protocol
 *
 * Cross-reference 
 * ---------------
 * By using the link between ArchBitstreamManager and FabricBitstream, 
 * we can build a sequence of configuration bits to fit different configuration protocols.
 *
 *     +----------------------+                 +--------------------------+
 *     |                      |   ConfigBitId   |                          |
 *     | ArchBitstreamManager |---------------->|  FabricBitstream  |
 *     |                      |                 |                          |   
 *     +----------------------+                 +--------------------------+ 
 *
 * Restrictions:
 * 1. Each block inside BitstreamManager should have only 1 parent block 
 *    and multiple child block
 * 2. Each bit inside BitstreamManager should have only 1 parent block 
 * 
 ******************************************************************************/
#ifndef FABRIC_BITSTREAM_H
#define FABRIC_BITSTREAM_H

#include <vector>
#include "vtr_vector.h"

#include "bitstream_manager_fwd.h"
#include "fabric_bitstream_fwd.h"

/* begin namespace openfpga */
namespace openfpga {

class FabricBitstream {
  public: /* Types and ranges */
    typedef vtr::vector<FabricBitId, FabricBitId>::const_iterator fabric_bit_iterator;

    typedef vtr::Range<fabric_bit_iterator> fabric_bit_range;

  public: /* Public aggregators */
    /* Find all the configuration bits */
    fabric_bit_range bits() const;

  public:  /* Public Accessors */
    /* Find the configuration bit id in architecture bitstream database */
    ConfigBitId config_bit(const FabricBitId& bit_id) const;

    /* Find the address of bitstream */
    std::vector<bool> bit_address(const FabricBitId& bit_id) const;

    /* Find the data-in of bitstream */
    bool bit_din(const FabricBitId& bit_id) const;

  public:  /* Public Mutators */
    /* Add a new configuration bit to the bitstream manager */
    FabricBitId add_bit(const ConfigBitId& config_bit_id);

    void set_bit_address(const FabricBitId& bit_id,
                         const std::vector<bool>& address);

    void set_bit_din(const FabricBitId& bit_id,
                     const bool& din);

    /* Reverse bit sequence of the fabric bitstream
     * This is required by configuration chain protocol 
     */
    void reverse();

  public:  /* Public Validators */
    bool valid_bit_id(const FabricBitId& bit_id) const;

  private: /* Internal data */
    /* Unique id of a bit in the Bitstream */
    vtr::vector<FabricBitId, FabricBitId> bit_ids_; 
    vtr::vector<FabricBitId, ConfigBitId> config_bit_ids_; 

    /* Address bits: this is designed for memory decoders
     * Here we store the binary format of the address, which can be loaded
     * to the configuration protocol directly 
     */
    vtr::vector<FabricBitId, std::vector<bool>> bit_addresses_;

    /* Data input (Din) bits: this is designed for memory decoders */
    vtr::vector<FabricBitId, bool> bit_dins_;
};

} /* end namespace openfpga */

#endif
