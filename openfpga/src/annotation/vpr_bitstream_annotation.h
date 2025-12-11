#ifndef VPR_BITSTREAM_ANNOTATION_H
#define VPR_BITSTREAM_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map>
#include <string>

/* Header from vpr library */
#include "clock_network.h"
#include "vpr_context.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This is the critical data structure to link the pb_type in VPR
 * to openfpga annotations
 * With a given pb_type pointer, it aims to identify:
 * 1. if the pb_type requires another bitstream source than .blif file
 *    which may be .eblif file
 * 2. if the pb_type requires a fixed bitstream value
 *    or an attribute line in the .eblif file
 *******************************************************************/
class VprBitstreamAnnotation {
 public: /* Type */
  enum class e_bitstream_source_type { EBLIF, NUM_TYPES };
  struct BitstreamSourceInfo {
    e_bitstream_source_type type = e_bitstream_source_type::NUM_TYPES;
    std::string content;
    size_t offset;
    bool overlap(const BitstreamSourceInfo& ref) const {
      if (type == ref.type && content == ref.content) {
        return true;
      }
      return false;
    }
    BitstreamSourceInfo(const e_bitstream_source_type& t,
                        const std::string& cont, const size_t& ofs)
      : type(t), content(cont), offset(ofs) {}
  };

 public: /* Constructor */
  VprBitstreamAnnotation();

 public: /* Public accessors */
  std::vector<BitstreamSourceInfo> pb_type_bitstream_sources(
    t_pb_type* pb_type) const;
  std::string pb_type_default_mode_bits(t_pb_type* pb_type) const;

  std::vector<BitstreamSourceInfo> pb_type_mode_select_bitstream_sources(
    t_pb_type* pb_type) const;
  size_t interconnect_default_path_id(t_interconnect* interconnect) const;
  ClockTreePinId clock_tap_routing_pin(const ClockTreeId& tree_id) const;

 public: /* Public mutators */
  bool add_pb_type_bitstream_source(t_pb_type* pb_type,
                                    const BitstreamSourceInfo& src_info);

  void set_pb_type_default_mode_bits(t_pb_type* pb_type,
                                     const std::string& default_mode_bits);

  bool add_pb_type_mode_select_bitstream_source(
    t_pb_type* pb_type, const BitstreamSourceInfo& src_info);
  void set_interconnect_default_path_id(t_interconnect* interconnect,
                                        const size_t& default_path_id);
  void set_clock_tap_routing_pin(const ClockTreeId& tree_id,
                                 const ClockTreePinId& tree_pin_id);

 private: /* Internal data */
  /* For regular bitstreams */
  /* A look up for pb type to find bitstream source type */
  std::map<t_pb_type*, std::vector<BitstreamSourceInfo>> bitstream_sources_;
  /* Binding from pb type to default mode bits */
  std::map<t_pb_type*, std::string> default_mode_bits_;

  /* For mode-select bitstreams */
  /* A look up for pb type to find bitstream source type */
  std::map<t_pb_type*, std::vector<BitstreamSourceInfo>>
    mode_select_bitstream_sources_;

  /* A look up for interconnect to find default path indices
   * Note: this is different from the default path in bitstream setting which is
   * the index of inputs in the context of the interconnect input string
   */
  std::map<t_interconnect*, size_t> interconnect_default_path_ids_;

  /* Mark the clock tree pin for which all the tap points of clock tree should
   * be routed through Note that for each clock tree, only one pin is allowed
   */
  std::map<ClockTreeId, ClockTreePinId> clock_tap_routing_pins_;
};

} /* End namespace openfpga*/

#endif
