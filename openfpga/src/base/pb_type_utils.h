#ifndef PB_TYPE_UTILS_H
#define PB_TYPE_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "physical_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

bool is_primitive_pb_type(t_pb_type* pb_type); 

bool is_root_pb_type(t_pb_type* pb_type);

t_mode* find_pb_type_mode(t_pb_type* pb_type, const char* mode_name);

t_pb_type* find_mode_child_pb_type(t_mode* mode, const char* child_name);

std::vector<t_port*> pb_type_ports(t_pb_type* pb_type);

t_port* find_pb_type_port(t_pb_type* pb_type, const std::string& port_name);

t_pb_type* try_find_pb_type_with_given_path(t_pb_type* top_pb_type, 
                                            const std::vector<std::string>& target_pb_type_names, 
                                            const std::vector<std::string>& target_pb_mode_names);

} /* end namespace openfpga */

#endif
