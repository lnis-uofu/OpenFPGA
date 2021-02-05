#ifndef BITSTREAM_SETTING_H
#define BITSTREAM_SETTING_H

/********************************************************************
 * This file include the declaration of simulation settings
 * which are used by OpenFPGA 
 *******************************************************************/
#include <string>

#include "vtr_vector.h" 

#include "bitstream_setting_fwd.h" 

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * A data structure to describe bitstream settings
 *
 * Typical usage:
 * --------------
 *   // Create an empty bitstream setting
 *   BitstreamSetting bitstream_setting;
 *   // call your builder for bitstream_setting
 *
 *******************************************************************/
class BitstreamSetting {
  public: /* Types */
    typedef vtr::vector<BitstreamPbTypeSettingId, BitstreamPbTypeSettingId>::const_iterator bitstream_pb_type_setting_iterator;
    /* Create range */
    typedef vtr::Range<bitstream_pb_type_setting_iterator> bitstream_pb_type_setting_range;
  public:  /* Constructors */
    BitstreamSetting();
  public: /* Accessors: aggregates */
    bitstream_pb_type_setting_range pb_type_settings() const;
  public: /* Public Accessors */
    std::string pb_type_name(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::vector<std::string> parent_pb_type_names(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::vector<std::string> parent_mode_names(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::string pb_type_bitstream_source(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::string pb_type_bitstream_content(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
  public: /* Public Mutators */
    BitstreamPbTypeSettingId add_bitstream_pb_type_setting(const std::string& pb_type_name,
                                                           const std::vector<std::string>& parent_pb_type_names,
                                                           const std::vector<std::string>& parent_mode_names,
                                                           const std::string& bitstream_source,
                                                           const std::string& bitstream_content);
  public: /* Public Validators */
    bool valid_bitstream_pb_type_setting_id(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
  private: /* Internal data */
    vtr::vector<BitstreamPbTypeSettingId, BitstreamPbTypeSettingId> pb_type_setting_ids_;
    vtr::vector<BitstreamPbTypeSettingId, std::string> pb_type_names_;
    vtr::vector<BitstreamPbTypeSettingId, std::vector<std::string>> parent_pb_type_names_;
    vtr::vector<BitstreamPbTypeSettingId, std::vector<std::string>> parent_mode_names_;
    vtr::vector<BitstreamPbTypeSettingId, std::string> pb_type_bitstream_sources_;
    vtr::vector<BitstreamPbTypeSettingId, std::string> pb_type_bitstream_contents_;
};

} /* namespace openfpga ends */

#endif
