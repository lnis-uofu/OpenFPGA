/********************************************************************
 * This file includes functions that are used to add duplicated 
 * pins to each side of a grid
 *
 * These functions are located in this file, being separated from
 * the default functions in build_grid_module.cpp
 * This allows us to keep new features easy to be maintained.
 *
 * Please follow this rules when creating new features!
 *******************************************************************/
#include "build_grid_module_duplicated_pins.h"

/********************************************************************
 * This function adds pb_type ports to top-level grid module with duplication
 * For each pin at each side, we create two pins which are short-wired
 * They are driven by the same pin, e.g., pinA in the child module
 * But in this top module, we will create two pins, each of which indicates
 * the physical location of pin.
 * Take the following example: 
 * One is called pinA_upper which is located close to the top side of this grid
 * The other is called pinA_lower which is located close to the bottom side of this grid
 *
 * Similarly, we duplicate pins at TOP, RIGHT, BOTTOM and LEFT sides.
 * For LEFT side, upper and lower pins carry the indication in physical location as RIGHT side.
 * For TOP and BOTTOM side, upper pin is located close to the left side of a grid, while lower
 * pin is located close to the right side of a grid  
 *
 *             pinB_upper   pinB_lower
 *                 ^          ^
 *                 |          |
 *               ---------------+
 *                              |--->pinA_upper
 *                              |
 *                       Grid   |  
 *                              |
 *                              |--->pinA_lower
 *               ---------------+
 *******************************************************************/
void add_grid_module_duplicated_pb_type_ports(ModuleManager& module_manager,
                                              const ModuleId& grid_module,
                                              t_type_ptr grid_type_descriptor,
                                              const e_side& border_side) {
}

void add_grid_module_nets_connect_duplicated_pb_type_ports(ModuleManager& module_manager,
                                                           const ModuleId& grid_module,
                                                           const ModuleId& child_module,
                                                           const size_t& child_instance,
                                                           t_type_ptr grid_type_descriptor,
                                                           const e_side& border_side) {
}
