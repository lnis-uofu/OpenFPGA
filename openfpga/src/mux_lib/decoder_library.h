/***************************************************************************************
 * This file includes key data structures to describe decoders which are used
 * in FPGA fabrics 
 * A decoder is a circuit to convert a binary input to one-hot codes
 * The outputs are assumes to be one-hot codes (at most only one '1' exist)
 * Therefore, the number of inputs is ceil(log(num_of_outputs)/log(2))
 * All the decoders are assumed to follow the port map : 
 *
 *                     Inputs 
 *                   | | ... | 
 *                   v v     v
 *                 +-----------+
 *                /             \
 *               /    Decoder    \
 *              +-----------------+
 *                | | | ... | | |
 *                v v v     v v v
 *                    Outputs

 ***************************************************************************************/

#ifndef DECODER_LIBRARY_H
#define DECODER_LIBRARY_H

#include "vtr_vector.h"
#include "vtr_range.h"
#include "decoder_library_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

class DecoderLibrary {
  public: /* Types and ranges */
    typedef vtr::vector<DecoderId, DecoderId>::const_iterator decoder_iterator;

    typedef vtr::Range<decoder_iterator> decoder_range;

  public: /* Public accessors: Aggregates */
    /* Get all the decoders */
    decoder_range decoders() const;

  public: /* Public accessors: Data query */
    /* Get the size of address input of a decoder */
    size_t addr_size(const DecoderId& decoder) const;
    /* Get the size of data output of a decoder */
    size_t data_size(const DecoderId& decoder) const;
    /* Get the flag if a decoder includes an ENABLE signal */
    bool use_enable(const DecoderId& decoder) const;
    /* Get the flag if a decoder includes an DATA_IN signal */
    bool use_data_in(const DecoderId& decoder) const;
    /* Get the flag if a decoder includes a data_inv port which is an inversion of the regular data output port */
    bool use_data_inv_port(const DecoderId& decoder) const;
    /* Get the flag if a decoder includes a readback port which enables readback from configurable memories */
    bool use_readback(const DecoderId& decoder) const;
    /* Find a decoder to the library, with the specification.
     * If found, return the id of decoder.
     * If not found, return an invalid id of decoder
     * To avoid duplicated decoders, this function should be used before adding a decoder 
     * Example: 
     *   DecoderId decoder_id == decoder_lib.find_decoder();
     *   if (DecoderId::INVALID() == decoder_id) {
     *     // Add decoder
     *   } 
     */
    DecoderId find_decoder(const size_t& addr_size, 
                           const size_t& data_size, 
                           const bool& use_enable, 
                           const bool& use_data_in, 
                           const bool& use_data_inv_port,
                           const bool& use_readback) const;

  public: /* Public validators */
    /* valid ids */
    bool valid_decoder_id(const DecoderId& decoder) const;

  public: /* Private mutators : basic operations */
    /* Add a decoder to the library */
    DecoderId add_decoder(const size_t& addr_size, 
                          const size_t& data_size, 
                          const bool& use_enable, 
                          const bool& use_data_in, 
                          const bool& use_data_inv_port,
                          const bool& use_readback);
    
  private: /* Internal Data */
    vtr::vector<DecoderId, DecoderId> decoder_ids_;
    vtr::vector<DecoderId, size_t> addr_sizes_;
    vtr::vector<DecoderId, size_t> data_sizes_;
    vtr::vector<DecoderId, bool> use_enable_;
    vtr::vector<DecoderId, bool> use_data_in_;
    vtr::vector<DecoderId, bool> use_data_inv_port_;
    vtr::vector<DecoderId, bool> use_readback_;
};

} /* End namespace openfpga*/



#endif

