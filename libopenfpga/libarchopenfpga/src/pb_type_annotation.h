#ifndef PB_TYPE_ANNOTATION_H
#define PB_TYPE_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <vector>
#include <map>
#include <array>

#include "openfpga_port.h"

/* namespace openfpga begins */
namespace openfpga {

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
class PbTypeAnnotation {
  public:  /* Constructor */
    PbTypeAnnotation();
  public:  /* Public accessors */
    std::string operating_pb_type_name() const;
    std::vector<std::string> operating_parent_pb_type_names() const;
    std::vector<std::string> operating_parent_mode_names() const;
    bool is_operating_pb_type() const;
    std::string physical_pb_type_name() const;
    std::vector<std::string> physical_parent_pb_type_names() const;
    std::vector<std::string> physical_parent_mode_names() const;
    bool is_physical_pb_type() const;
    std::string physical_mode_name() const;
    std::string idle_mode_name() const;
    std::vector<size_t> mode_bits() const;
    std::string circuit_model_name() const;
    float physical_pb_type_index_factor() const;
    int physical_pb_type_index_offset() const;
    std::vector<std::string> port_names() const;
    std::map<BasicPort, std::array<int, 2>> physical_pb_type_port(const std::string& port_name) const;
    std::vector<std::string> interconnect_names() const;
    std::string interconnect_circuit_model_name(const std::string& interc_name) const;
  public: /* Public mutators */
    void set_operating_pb_type_name(const std::string& name);
    void set_operating_parent_pb_type_names(const std::vector<std::string>& names);
    void set_operating_parent_mode_names(const std::vector<std::string>& names);
    void set_physical_pb_type_name(const std::string& name);
    void set_physical_parent_pb_type_names(const std::vector<std::string>& names);
    void set_physical_parent_mode_names(const std::vector<std::string>& names);
    void set_physical_mode_name(const std::string& name);
    void set_idle_mode_name(const std::string& name);
    void set_mode_bits(const std::vector<size_t>& mode_bits);
    void set_circuit_model_name(const std::string& name);
    void set_physical_pb_type_index_factor(const float& value);
    void set_physical_pb_type_index_offset(const int& value);
    void add_pb_type_port_pair(const std::string& operating_pb_port_name,
                               const BasicPort& physical_pb_port);
    void set_physical_pin_initial_offset(const std::string& operating_pb_port_name,
                                         const BasicPort& physical_pb_port,
                                         const int& physical_pin_initial_offset);
    void set_physical_pin_rotate_offset(const std::string& operating_pb_port_name,
                                        const BasicPort& physical_pb_port,
                                        const int& physical_pin_rotate_offset);
    void add_interconnect_circuit_model_pair(const std::string& interc_name,
                                             const std::string& circuit_model_name);
  private: /* Internal data */
    /* Binding between physical pb_type and operating pb_type 
     * both operating and physial pb_type names contain the full names 
     * defined in the hierarchy of pb_types
     * e.g.
     *    clb.fle[frac_logic].frac_lut6
     *                ^
     *                |
     *             mode_name
     *
     * The pb_type name 'frac_lut6' will be stored in 
     * either operating or physical pb_type_name
     * The parent pb_type and mode names will be stored in
     * either operating or physical parent_pb_type_name and parent_mode_names
     *
     */
    std::string operating_pb_type_name_;
    std::vector<std::string> operating_parent_pb_type_names_;
    std::vector<std::string> operating_parent_mode_names_;

    std::string physical_pb_type_name_;
    std::vector<std::string> physical_parent_pb_type_names_;
    std::vector<std::string> physical_parent_mode_names_;

    /* Identify which mode is the physical implementation of an operating pb_type */
    std::string physical_mode_name_;

    /* Identify in which mode is the pb_type will operate when it is not used */
    std::string idle_mode_name_;

    /* Configuration bits to select an operting mode for the circuit mode name */
    std::vector<size_t> mode_bits_;

    /* Circuit mode name linked to a physical pb_type.
     * This is only applicable to the physical pb_type
     */
    std::string circuit_model_name_;

    /* The factor aims to align the indices for pb_type between operating and physical modes, 
     * especially when an operating mode contains multiple pb_type (num_pb>1) 
     * that are linked to the same physical pb_type. 
     * When number of physical_pb_type  is larger than 1, 
     * the index of pb_type will be multipled by the given factor.
     *
     * For example, a factor of 2 is used to map 
     * operating pb_type adder[5] with a full path clb.fle[arith].adder[5]
     * to physical pb_type adder[10] with a full path clb.fle[physical].adder[10]
     */
    float physical_pb_type_index_factor_;

    /* The offset aims to align the indices for pb_type between operating and physical modes, 
     * especially when an operating mode contains multiple pb_type (num_pb>1) 
     * that are linked to the same physical pb_type. 
     * When number of physical_pb_type is larger than 1, 
     * the index of pb_type will be shifted by the given factor.
     *
     * For example, an offset of 1 is used to map 
     * operating pb_type adder[0] with a full path clb.fle[arith].adder[0]
     * to physical pb_type adder[1] with a full path clb.fle[physical].adder[1]
     */
    int physical_pb_type_index_offset_;

    /* Link from the pins under an operating pb_type to pairs of
     * its physical pb_type and its pin initial & rotating offset
     *
     * Note that initial offset is the first element of the std::array
     * Note that rotating offset is the second element of the std::array
     *
     * The offsets aim to align the pin indices for port of pb_type 
     * between operating and physical modes, especially when an operating 
     * mode contains multiple pb_type (num_pb>1) that are linked to 
     * the same physical pb_type. 
     * When physical_mode_pin_rotate_offset is larger than zero, 
     * the pin index of pb_type (whose index is large than 1) 
     * will be shifted by the given offset.
     *
     * For example, an initial offset of -32 is used to map 
     *   operating pb_type bram[0].dout[32] with a full path memory[dual_port].bram[0]
     *   operating pb_type bram[0].dout[33] with a full path memory[dual_port].bram[0]
     * to 
     *   physical pb_type bram[0].dout_a[0] with a full path memory[physical].bram[0]
     *   physical pb_type bram[0].dout_a[1] with a full path memory[physical].bram[0]
     *
     * For example, a rotating offset of 1 is used to map 
     *   operating pb_type mult_9x9[0].a[0:8] with a full path mult[frac].mult_9x9[0]
     *   operating pb_type mult_9x9[1].a[0:8] with a full path mult[frac].mult_9x9[1]
     * to 
     *   physical pb_type mult_36x36.a[0:8] with a full path mult[physical].mult_36x36[0]
     *   physical pb_type mult_36x36.a[9:17] with a full path mult[physical].mult_36x36[0]
     */
    std::map<std::string, std::map<BasicPort, std::array<int, 2>>> operating_pb_type_ports_;

    /* Link between the interconnects under this pb_type and circuit model names */
    std::map<std::string, std::string> interconnect_circuit_model_names_;
};

} /* namespace openfpga ends */

#endif

