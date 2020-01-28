/************************************************************************
 * Member functions for class VprPbTypeAnnotation
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vpr_pb_type_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprPbTypeAnnotation::VprPbTypeAnnotation() {
  return;
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
t_mode* VprPbTypeAnnotation::physical_mode(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, t_mode*>::const_iterator it = physical_pb_modes_.find(pb_type);
  if (it == physical_pb_modes_.end()) {
    return nullptr;
  }
  return physical_pb_modes_.at(pb_type);
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprPbTypeAnnotation::add_pb_type_physical_mode(t_pb_type* pb_type, t_mode* physical_mode) {
  /* Warn any override attempt */
  std::map<t_pb_type*, t_mode*>::const_iterator it = physical_pb_modes_.find(pb_type);
  if (it != physical_pb_modes_.end()) {
    VTR_LOG_WARN("Override the annotation between pb_type '%s' and it physical mode '%s'!\n",
                 pb_type->name, physical_mode->name);
  }

  physical_pb_modes_[pb_type] = physical_mode;
}

} /* End namespace openfpga*/
