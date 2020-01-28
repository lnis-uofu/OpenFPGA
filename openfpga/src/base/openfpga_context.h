#ifndef OPENFPGA_CONTEXT_H
#define OPENFPGA_CONTEXT_H

#include "vpr_context.h"
#include "openfpga_arch.h"
#include "vpr_pb_type_annotation.h"

/********************************************************************
 * This file includes the declaration of the date structure 
 * OpenfpgaContext, which is used for data exchange between 
 * different modules in OpenFPGA shell environment 
 *
 * If a command of OpenFPGA needs to exchange data with other commands,
 * it must use this data structure to access/mutate.
 * In such case, you must add data structures to OpenfpgaContext
 *
 * Note:
 * Please respect to the following rules when using the OpenfpgaContext
 * 1. This data structure will be created only once in the main() function
 *    The data structure is design to be large and contain all the 
 *    data structure required by each module of OpenFPGA core engine.
 *    Do NOT create or duplicate in your own module!     
 * 2. Be clear in your mind if you want to access/mutate the data inside OpenfpgaContext
 *    Read-only data should be accessed by 
 *      const OpenfpgaContext& 
 *    Mutate should use reference
 *      OpenfpgaContext&
 * 3. Please keep the definition of OpenfpgaContext short
 *    Do put ONLY well-modularized data structure under this root.
 * 4. We build this data structure based on the Context from VPR
 *    which does NOT allow users to copy the internal members
 *    This is due to that the data structures in the OpenFPGA context
 *    are typically big in terms of memory
 *******************************************************************/
class OpenfpgaContext : public Context  {
  public:  /* Public accessors */
    const openfpga::Arch& arch() const { return arch_; }
    const openfpga::VprPbTypeAnnotation& vpr_pb_type_annotation() const { return vpr_pb_type_annotation_; }
  public:  /* Public mutators */
    openfpga::Arch& mutable_arch() { return arch_; }
    openfpga::VprPbTypeAnnotation& mutable_vpr_pb_type_annotation() { return vpr_pb_type_annotation_; }
  private: /* Internal data */
    /* Data structure to store information from read_openfpga_arch library */
    openfpga::Arch arch_;
    /* Annotation to pb_type of VPR */
    openfpga::VprPbTypeAnnotation vpr_pb_type_annotation_;
};

#endif
