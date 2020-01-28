#ifndef VPR_PB_TYPE_ANNOTATION_H
#define VPR_PB_TYPE_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map> 

/* Header from archfpga library */
#include "physical_types.h"

/* Header from openfpgautil library */
#include "openfpga_port.h"
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
  public:  /* Public accessors */
    bool is_physical_pb_type(t_pb_type* pb_type) const;
    t_mode* physical_mode(t_pb_type* pb_type) const;
    t_pb_type* physical_pb_type(t_pb_type* pb_type) const;
    t_port* physical_pb_port(t_port* pb_port) const;
    BasicPort physical_pb_port_range(t_port* pb_port) const;
    CircuitModelId pb_type_circuit_model(t_pb_type* physical_pb_type) const;
  public:  /* Public mutators */
    void add_pb_type_physical_mode(t_pb_type* pb_type, t_mode* physical_mode);
    void add_physical_pb_type(t_pb_type* operating_pb_type, t_pb_type* physical_pb_type);
    void add_physical_pb_port(t_port* operating_pb_port, t_port* physical_pb_port);
    void add_physical_pb_port_range(t_port* operating_pb_port, const BasicPort& port_range);
    void add_pb_type_circuit_model(t_pb_type* physical_pb_type, const CircuitModelId& circuit_model);
  private: /* Internal data */
    /* Pair a regular pb_type to its physical pb_type */
    std::map<t_pb_type*, t_pb_type*> physical_pb_types_;

    /* Pair a physical mode for a pb_type
     * Note:
     * - the physical mode MUST be a child mode of the pb_type
     * - the pb_type MUST be a physical pb_type itself
     */
    std::map<t_pb_type*, t_mode*> physical_pb_modes_;

    /* Pair a physical pb_type to its circuit model
     * Note:
     * - the pb_type MUST be a physical pb_type itself
     */
    std::map<t_pb_type*, CircuitModelId> pb_type_circuit_models_;

    /* Pair a pb_type to its mode selection bits
     * - if the pb_type is a physical pb_type, the mode bits are the default mode 
     *   where the physical pb_type will operate when used
     * - if the pb_type is an operating pb_type, the mode bits will be applied
     *   when the operating pb_type is used by packer
     */
    std::map<t_pb_type*, std::vector<bool>> pb_type_mode_bits_;

    /* Pair a pb_port to its physical pb_port 
     * Note:
     * - the parent of physical pb_port MUST be a physical pb_type
     */
    std::map<t_port*, t_port*> physical_pb_ports_;

    /* Pair a pb_port to its LSB and MSB of a physical pb_port 
     * Note:
     * - the LSB and MSB MUST be in range of the physical pb_port
     */
    std::map<t_port*, BasicPort> physical_pb_port_ranges_;

    /* Pair a pb_graph_node to a physical pb_graph_node
     * Note:
     * - the pb_type of physical pb_graph_node must be a physical pb_type
     */
    std::map<t_pb_graph_node*, t_pb_graph_node*> physical_pb_graph_nodes_;
};

} /* End namespace openfpga*/

#endif
