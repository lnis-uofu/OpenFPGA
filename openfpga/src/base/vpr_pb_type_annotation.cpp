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

t_pb_type* VprPbTypeAnnotation::physical_pb_type(t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, t_pb_type*>::const_iterator it = physical_pb_types_.find(pb_type);
  if (it == physical_pb_types_.end()) {
    return nullptr;
  }
  return physical_pb_types_.at(pb_type);
}

t_port* VprPbTypeAnnotation::physical_pb_port(t_port* pb_port) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_port*, t_port*>::const_iterator it = physical_pb_ports_.find(pb_port);
  if (it == physical_pb_ports_.end()) {
    return nullptr;
  }
  return physical_pb_ports_.at(pb_port);
}

BasicPort VprPbTypeAnnotation::physical_pb_port_range(t_port* pb_port) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_port*, BasicPort>::const_iterator it = physical_pb_port_ranges_.find(pb_port);
  if (it == physical_pb_port_ranges_.end()) {
    /* Return an invalid port. As such the port width will be 0, which is an invalid value */
    return BasicPort();
  }
  return physical_pb_port_ranges_.at(pb_port);
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

void VprPbTypeAnnotation::add_physical_pb_type(t_pb_type* operating_pb_type, t_pb_type* physical_pb_type) {
  /* Warn any override attempt */
  std::map<t_pb_type*, t_pb_type*>::const_iterator it = physical_pb_types_.find(operating_pb_type);
  if (it != physical_pb_types_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_type '%s' and it physical pb_type '%s'!\n",
                 operating_pb_type->name, physical_pb_type->name);
  }

  physical_pb_types_[operating_pb_type] = physical_pb_type;
}

void VprPbTypeAnnotation::add_physical_pb_port(t_port* operating_pb_port, t_port* physical_pb_port) {
  /* Warn any override attempt */
  std::map<t_port*, t_port*>::const_iterator it = physical_pb_ports_.find(operating_pb_port);
  if (it != physical_pb_ports_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_port '%s' and it physical pb_port '%s'!\n",
                 operating_pb_port->name, physical_pb_port->name);
  }

  physical_pb_ports_[operating_pb_port] = physical_pb_port;
}

void VprPbTypeAnnotation::add_physical_pb_port_range(t_port* operating_pb_port, const BasicPort& port_range) {
  /* The port range must satify the port width*/
  VTR_ASSERT((size_t)operating_pb_port->num_pins == port_range.get_width());

  /* Warn any override attempt */
  std::map<t_port*, BasicPort>::const_iterator it = physical_pb_port_ranges_.find(operating_pb_port);
  if (it != physical_pb_port_ranges_.end()) {
    VTR_LOG_WARN("Override the annotation between operating pb_port '%s' and it physical pb_port range '[%ld:%ld]'!\n",
                 operating_pb_port->name, port_range.get_lsb(), port_range.get_msb());
  }

  physical_pb_port_ranges_[operating_pb_port] = port_range;
}

} /* End namespace openfpga*/
