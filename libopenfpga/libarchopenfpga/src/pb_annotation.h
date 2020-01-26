#ifndef PB_PHYSICAL_ANNOTATION_H
#ifndef PB_PHYSICAL_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <vector>
#include <map>

/********************************************************************
 * This file include the declaration of data structures
 * to store physical pb_type annotation, including
 * 1. the binding between operating pb_type and physical_pb_type
 *    this also cover the binding between pins of any pair of pb_types
 * 2. the binding between physical pb_type and circuit models
 * 3. the binding between interconnect under pb_types and circuit models
 *    This is only applicable to physical pb_types
 *
 * Each PhysicalPbAnnotation includes the binding for ONLY a pb_type
 * defined in the architecture XML
 *
 * Note:
 * 1. Keep this data structure as general as possible. It is supposed
 *    to contain the raw data from architecture XML! If you want to link
 *    to other data structures, please create another one in other header files
 *******************************************************************/
class PhysicalPbAnnotation {
  private: /* Internal data */
    /* Binding between physical pb_type and operating pb_type 
     * both operating and physial pb_type names are the full names 
     * defined in the hierarchy of pb_types
     * e.g.
     *    clb.fle[frac_logic].frac_lut6
     */
    std::string operating_pb_type_name_;
    std::string physical_pb_type_name_;
    std::string mode_bits_;
    std::string circuit_model_name_;
    int physical_pb_type_index_factor_;
    int physical_pb_type_index_offset_;

    /* Link from the pins under an operating pb_type to physical pb_type  */
    std::vector<std::string> operating_pb_type_pin_names_;
    std::vector<std::string> physical_pb_type_pin_names_;
    std::vector<int> physical_pin_rotate_offset_;

    /* Link between the interconnects under this pb_type and circuit model names */
    std::vector<std:map<std::string, std::string>> interconnect_circuit_model_names_;
};

#endif

