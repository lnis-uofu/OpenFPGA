#ifndef VPR_PB_TYPE_ANNOTATION_H
#define VPR_PB_TYPE_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map> 

/* Header from archfpga library */
#include "physical_types.h"

/* Header from openfpgautil library */
#include "circuit_library.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This is the critical data structure to link the pb_type in VPR
 * to openfpga annotations
 * With a given pb_type pointer, it aims to identify:
 * 1. if the pb_type is a physical pb_type or a operating pb_type
 * 2. what is the circuit model id linked to a physical pb_type
 * 3. what is the physical pb_type for an operating pb_type
 * 4. what is the mode pointer that represents the physical mode for a pb_type
 *******************************************************************/
class VprPbTypeAnnotation {
  public:  /* Constructor */
    VprPbTypeAnnotation();
  private: /* Internal data */
    std::map<t_pb_type*, bool> is_physical_pb_types_;
    std::map<t_pb_type*, t_pb_type*> physical_pb_types_;
    std::map<t_pb_type*, t_mode*> physical_pb_modes_;
    std::map<t_pb_type*, CircuitModelId> pb_type_circuit_models_;
};

} /* End namespace openfpga*/

#endif
