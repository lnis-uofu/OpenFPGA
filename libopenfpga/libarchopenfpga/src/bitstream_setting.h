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
 * This data structure includes following types of settings:
 * - Pb type: include definiting hard coded bitstream for pb_types (LUT or configurable pb_type for mode selection)
 * - Interconnect: include defining default paths for routing multiplexers in pb_types 
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
    typedef vtr::vector<BitstreamInterconnectSettingId, BitstreamInterconnectSettingId>::const_iterator bitstream_interconnect_setting_iterator;
    /* Create range */
    typedef vtr::Range<bitstream_pb_type_setting_iterator> bitstream_pb_type_setting_range;
    typedef vtr::Range<bitstream_interconnect_setting_iterator> bitstream_interconnect_setting_range;
  public:  /* Constructors */
    BitstreamSetting();
  public: /* Accessors: aggregates */
    bitstream_pb_type_setting_range pb_type_settings() const;
    bitstream_interconnect_setting_range interconnect_settings() const;
  public: /* Public Accessors */
    std::string pb_type_name(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::vector<std::string> parent_pb_type_names(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::vector<std::string> parent_mode_names(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::string pb_type_bitstream_source(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::string pb_type_bitstream_content(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    bool is_mode_select_bitstream(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    size_t bitstream_offset(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    std::string interconnect_name(const BitstreamInterconnectSettingId& interconnect_setting_id) const;
    std::vector<std::string> parent_pb_type_names(const BitstreamInterconnectSettingId& interconnect_setting_id) const;
    std::vector<std::string> parent_mode_names(const BitstreamInterconnectSettingId& interconnect_setting_id) const;
    std::string default_path(const BitstreamInterconnectSettingId& interconnect_setting_id) const;
  public: /* Public Mutators */
    BitstreamPbTypeSettingId add_bitstream_pb_type_setting(const std::string& pb_type_name,
                                                           const std::vector<std::string>& parent_pb_type_names,
                                                           const std::vector<std::string>& parent_mode_names,
                                                           const std::string& bitstream_source,
                                                           const std::string& bitstream_content);
    void set_mode_select_bitstream(const BitstreamPbTypeSettingId& pb_type_setting_id,
                                   const bool& is_mode_select_bitstream);
    void set_bitstream_offset(const BitstreamPbTypeSettingId& pb_type_setting_id,
                              const size_t& offset);

    BitstreamInterconnectSettingId add_bitstream_interconnect_setting(const std::string& interconnect_name,
                                                                      const std::vector<std::string>& parent_pb_type_names,
                                                                      const std::vector<std::string>& parent_mode_names,
                                                                      const std::string& default_path);
  public: /* Public Validators */
    bool valid_bitstream_pb_type_setting_id(const BitstreamPbTypeSettingId& pb_type_setting_id) const;
    bool valid_bitstream_interconnect_setting_id(const BitstreamInterconnectSettingId& interconnect_setting_id) const;
  private: /* Internal data */
    /* Pb type -related settings
     * - Paths to a pb_type in the pb_graph
     * - Bitstream source, data_type, offsets etc.
     */
    vtr::vector<BitstreamPbTypeSettingId, BitstreamPbTypeSettingId> pb_type_setting_ids_;
    vtr::vector<BitstreamPbTypeSettingId, std::string> pb_type_names_;
    vtr::vector<BitstreamPbTypeSettingId, std::vector<std::string>> parent_pb_type_names_;
    vtr::vector<BitstreamPbTypeSettingId, std::vector<std::string>> parent_mode_names_;
    vtr::vector<BitstreamPbTypeSettingId, std::string> pb_type_bitstream_sources_;
    vtr::vector<BitstreamPbTypeSettingId, std::string> pb_type_bitstream_contents_;
    /* Indicate if the bitstream is applied to mode selection bits of a pb_type */
    vtr::vector<BitstreamPbTypeSettingId, bool> is_mode_select_bitstreams_;
    /* The offset that the bitstream is applied to the original bitstream of a pb_type */
    vtr::vector<BitstreamPbTypeSettingId, size_t> bitstream_offsets_;

    /* Interconnect-related settings:
     * - Name of interconnect under a given pb_type
     * - The default path to be considered for a given interconnect during bitstream generation
     */
    vtr::vector<BitstreamInterconnectSettingId, BitstreamInterconnectSettingId> interconnect_setting_ids_;
    vtr::vector<BitstreamInterconnectSettingId, std::string> interconnect_names_;
    vtr::vector<BitstreamInterconnectSettingId, std::vector<std::string>> interconnect_parent_pb_type_names_;
    vtr::vector<BitstreamInterconnectSettingId, std::vector<std::string>> interconnect_parent_mode_names_;
    vtr::vector<BitstreamInterconnectSettingId, std::string> interconnect_default_paths_;
};

} /* namespace openfpga ends */

#endif
