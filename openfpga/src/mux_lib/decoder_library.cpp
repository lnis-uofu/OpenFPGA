/***************************************************************************************
 * This file includes memeber functions for data structure DecoderLibrary
 **************************************************************************************/
#include "vtr_assert.h"
#include "decoder_library.h"

/* Begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Public Accessors: Aggregators
 **************************************************************************************/
DecoderLibrary::decoder_range DecoderLibrary::decoders() const {
  return vtr::make_range(decoder_ids_.begin(), decoder_ids_.end());
}

/***************************************************************************************
 * Public Accessors: Data query 
 **************************************************************************************/
/* Get the size of address input of a decoder */
size_t DecoderLibrary::addr_size(const DecoderId& decoder) const {
  VTR_ASSERT_SAFE(valid_decoder_id(decoder));
  return addr_sizes_[decoder];
}

/* Get the size of data output of a decoder */
size_t DecoderLibrary::data_size(const DecoderId& decoder) const { 
  VTR_ASSERT_SAFE(valid_decoder_id(decoder));
  return data_sizes_[decoder];
}

/* Get the flag if a decoder includes an ENABLE signal */
bool DecoderLibrary::use_enable(const DecoderId& decoder) const {
  VTR_ASSERT_SAFE(valid_decoder_id(decoder));
  return use_enable_[decoder];
}

/* Get the flag if a decoder includes an DATA_IN signal */
bool DecoderLibrary::use_data_in(const DecoderId& decoder) const {
  VTR_ASSERT_SAFE(valid_decoder_id(decoder));
  return use_data_in_[decoder];
}

/* Get the flag if a decoder includes a data_inv port which is an inversion of the regular data output port */
bool DecoderLibrary::use_data_inv_port(const DecoderId& decoder) const {
  VTR_ASSERT_SAFE(valid_decoder_id(decoder));
  return use_data_inv_port_[decoder];
}

bool DecoderLibrary::use_readback(const DecoderId& decoder) const {
  VTR_ASSERT_SAFE(valid_decoder_id(decoder));
  return use_readback_[decoder];
}

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
DecoderId DecoderLibrary::find_decoder(const size_t& addr_size, 
                                       const size_t& data_size, 
                                       const bool& use_enable, 
                                       const bool& use_data_in, 
                                       const bool& use_data_inv_port,
                                       const bool& use_readback) const {
  for (auto decoder : decoders()) {
    if (  (addr_size == addr_sizes_[decoder])
       && (data_size == data_sizes_[decoder])
       && (use_enable == use_enable_[decoder])
       && (use_data_in == use_data_in_[decoder])
       && (use_data_inv_port == use_data_inv_port_[decoder])
       && (use_readback == use_readback_[decoder]) ) {
      return decoder;
    }
  }

  /* Not found, return an invalid id by default */
  return DecoderId::INVALID();
}

/***************************************************************************************
 * Public Validators
 **************************************************************************************/
/* Validate ids */
bool DecoderLibrary::valid_decoder_id(const DecoderId& decoder) const {
  return size_t(decoder) < decoder_ids_.size() && decoder_ids_[decoder] == decoder;
}

/***************************************************************************************
 * Public Mutators : Basic Operations 
 **************************************************************************************/
/* Add a decoder to the library */
DecoderId DecoderLibrary::add_decoder(const size_t& addr_size, 
                                      const size_t& data_size, 
                                      const bool& use_enable, 
                                      const bool& use_data_in, 
                                      const bool& use_data_inv_port,
                                      const bool& use_readback) {
  DecoderId decoder = DecoderId(decoder_ids_.size());
  /* Push to the decoder list */
  decoder_ids_.push_back(decoder);
  /* Resize the other related vectors */
  addr_sizes_.push_back(addr_size);
  data_sizes_.push_back(data_size);
  use_enable_.push_back(use_enable);
  use_data_in_.push_back(use_data_in);
  use_data_inv_port_.push_back(use_data_inv_port);
  use_readback_.push_back(use_readback);

  return decoder;
}

} /* End namespace openfpga*/
