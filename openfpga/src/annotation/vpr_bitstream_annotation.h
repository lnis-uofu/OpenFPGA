#ifndef VPR_BITSTREAM_ANNOTATION_H
#define VPR_BITSTREAM_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string> 
#include <map> 

/* Header from vpr library */
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
  public:  /* Type */
    enum e_bitstream_source_type {
      BITSTREAM_SOURCE_EBLIF,
      NUM_BITSTREAM_SOURCE_TYPES
    };
  public:  /* Constructor */
    VprBitstreamAnnotation();
  public:  /* Public accessors */
    e_bitstream_source_type pb_type_bitstream_source(t_pb_type* pb_type) const;
    std::string pb_type_bitstream_content(t_pb_type* pb_type) const;
  public:  /* Public mutators */
    void set_pb_type_bitstream_source(t_pb_type* pb_type,
                                      const e_bitstream_source_type& bitstream_source);
    void set_pb_type_bitstream_content(t_pb_type* pb_type,
                                       const std::string& bitstream_content);
  private: /* Internal data */
    /* A look up for pb type to find bitstream source type */
    std::map<t_pb_type*, e_bitstream_source_type> bitstream_sources_;
    /* Binding from pb type to bitstream content */
    std::map<t_pb_type*, std::string> bitstream_contents_;
};

} /* End namespace openfpga*/

#endif
