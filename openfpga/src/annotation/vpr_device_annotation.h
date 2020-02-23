#ifndef VPR_DEVICE_ANNOTATION_H
#define VPR_DEVICE_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map> 

/* Header from vtrutil library */
#include "vtr_strong_id.h"

/* Header from archfpga library */
#include "physical_types.h"

/* Header from vpr library */
#include "rr_graph_obj.h"

/* Header from openfpgautil library */
#include "openfpga_port.h"
#include "circuit_library.h"
#include "arch_direct.h"
#include "lb_rr_graph.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Unique index for pb_graph node */
struct pb_graph_node_id_tag;

typedef vtr::StrongId<pb_graph_node_id_tag> PbGraphNodeId;

/********************************************************************
 * This is the critical data structure to link the pb_type in VPR
 * to openfpga annotations
 * With a given pb_type pointer, it aims to identify:
 * 1. if the pb_type is a physical pb_type or a operating pb_type
 * 2. what is the circuit model id linked to a physical pb_type
 * 3. what is the physical pb_type for an operating pb_type
 * 4. what is the mode pointer that represents the physical mode for a pb_type
 *******************************************************************/
class VprDeviceAnnotation {
  public:  /* Constructor */
    VprDeviceAnnotation();
  public:  /* Public accessors */
    bool is_physical_pb_type(t_pb_type* pb_type) const;
    t_mode* physical_mode(t_pb_type* pb_type) const;
    t_pb_type* physical_pb_type(t_pb_type* pb_type) const;
    t_port* physical_pb_port(t_port* pb_port) const;
    BasicPort physical_pb_port_range(t_port* pb_port) const;
    CircuitModelId pb_type_circuit_model(t_pb_type* physical_pb_type) const;
    CircuitModelId interconnect_circuit_model(t_interconnect* pb_interconnect) const;
    e_interconnect interconnect_physical_type(t_interconnect* pb_interconnect) const;
    CircuitPortId pb_circuit_port(t_port* pb_port) const;
    std::vector<size_t> pb_type_mode_bits(t_pb_type* pb_type) const;
    /* Get the unique index of a pb_graph_node */
    PbGraphNodeId pb_graph_node_unique_index(t_pb_graph_node* pb_graph_node) const;
    /* Get the pointer to a pb_graph node using an unique index */
    t_pb_graph_node* pb_graph_node(t_pb_type* pb_type, const PbGraphNodeId& unique_index) const;
    t_pb_graph_node* physical_pb_graph_node(t_pb_graph_node* pb_graph_node) const;
    float physical_pb_type_index_factor(t_pb_type* pb_type) const;
    int physical_pb_type_index_offset(t_pb_type* pb_type) const;

    int physical_pb_pin_rotate_offset(t_port* pb_port) const;

    /**This function returns an accumulated offset. Note that the
     * accumulated offset is NOT the pin rotate offset specified by users
     * It is an aggregation of the offset during pin pairing
     * Each time, we manage to pair two pins, the accumulated offset will be incremented
     * by the pin rotate offset value
     * The accumulated offset will be reset to 0 when it exceeds the msb() of the physical port
     */
    int physical_pb_pin_offset(t_port* pb_port) const;
    t_pb_graph_pin* physical_pb_graph_pin(const t_pb_graph_pin* pb_graph_pin) const;
    CircuitModelId rr_switch_circuit_model(const RRSwitchId& rr_switch) const;
    CircuitModelId rr_segment_circuit_model(const RRSegmentId& rr_segment) const;
    ArchDirectId direct_annotation(const size_t& direct) const;
    LbRRGraph physical_lb_rr_graph(t_pb_graph_node* pb_graph_head) const;
  public:  /* Public mutators */
    void add_pb_type_physical_mode(t_pb_type* pb_type, t_mode* physical_mode);
    void add_physical_pb_type(t_pb_type* operating_pb_type, t_pb_type* physical_pb_type);
    void add_physical_pb_port(t_port* operating_pb_port, t_port* physical_pb_port);
    void add_physical_pb_port_range(t_port* operating_pb_port, const BasicPort& port_range);
    void add_pb_type_circuit_model(t_pb_type* physical_pb_type, const CircuitModelId& circuit_model);
    void add_interconnect_circuit_model(t_interconnect* pb_interconnect, const CircuitModelId& circuit_model);
    void add_interconnect_physical_type(t_interconnect* pb_interconnect, const e_interconnect& physical_type);
    void add_pb_circuit_port(t_port* pb_port, const CircuitPortId& circuit_port);
    void add_pb_type_mode_bits(t_pb_type* pb_type, const std::vector<size_t>& mode_bits);
    void add_pb_graph_node_unique_index(t_pb_graph_node* pb_graph_node);
    void add_physical_pb_graph_node(t_pb_graph_node* operating_pb_graph_node, 
                                    t_pb_graph_node* physical_pb_graph_node);
    void add_physical_pb_type_index_factor(t_pb_type* pb_type, const float& factor);
    void add_physical_pb_type_index_offset(t_pb_type* pb_type, const int& offset);
    void add_physical_pb_pin_rotate_offset(t_port* pb_port, const int& offset);
    void add_physical_pb_graph_pin(const t_pb_graph_pin* operating_pb_graph_pin, t_pb_graph_pin* physical_pb_graph_pin);
    void add_rr_switch_circuit_model(const RRSwitchId& rr_switch, const CircuitModelId& circuit_model);
    void add_rr_segment_circuit_model(const RRSegmentId& rr_segment, const CircuitModelId& circuit_model);
    void add_direct_annotation(const size_t& direct, const ArchDirectId& arch_direct_id);
    void add_physical_lb_rr_graph(t_pb_graph_node* pb_graph_head, const LbRRGraph& lb_rr_graph);
  private: /* Internal data */
    /* Pair a regular pb_type to its physical pb_type */
    std::map<t_pb_type*, t_pb_type*> physical_pb_types_;
    std::map<t_pb_type*, float> physical_pb_type_index_factors_;
    std::map<t_pb_type*, int> physical_pb_type_index_offsets_;

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

    /* Pair a interconnect of a physical pb_type to its circuit model
     * Note:
     * - the pb_type MUST be a physical pb_type itself
     */
    std::map<t_interconnect*, CircuitModelId> interconnect_circuit_models_;

    /* Physical type of interconnect 
     * Note:
     * - only applicable to an interconnect belongs to physical mode
     */
    std::map<t_interconnect*, e_interconnect> interconnect_physical_types_;

    /* Pair a pb_type to its mode selection bits
     * - if the pb_type is a physical pb_type, the mode bits are the default mode 
     *   where the physical pb_type will operate when used
     * - if the pb_type is an operating pb_type, the mode bits will be applied
     *   when the operating pb_type is used by packer
     */
    std::map<t_pb_type*, std::vector<size_t>> pb_type_mode_bits_;

    /* Pair a pb_port to its physical pb_port 
     * Note:
     * - the parent of physical pb_port MUST be a physical pb_type
     */
    std::map<t_port*, t_port*> physical_pb_ports_;
    std::map<t_port*, int> physical_pb_pin_rotate_offsets_;

    /* Accumulated offsets for a physical pb_type port, just for internal usage */
    std::map<t_port*, int> physical_pb_pin_offsets_;

    /* Pair a pb_port to its LSB and MSB of a physical pb_port 
     * Note:
     * - the LSB and MSB MUST be in range of the physical pb_port
     */
    std::map<t_port*, BasicPort> physical_pb_port_ranges_;

    /* Pair a pb_port to a circuit port in circuit model
     * Note:
     * - the parent of physical pb_port MUST be a physical pb_type
     */
    std::map<t_port*, CircuitPortId> pb_circuit_ports_;

    /* Pair each pb_graph_node to an unique index in the graph
     * The unique index if the index in the array of t_pb_graph_node*
     */ 
    std::map<t_pb_type*, std::vector<t_pb_graph_node*>> pb_graph_node_unique_index_;

    /* Pair a pb_graph_node to a physical pb_graph_node
     * Note:
     * - the pb_type of physical pb_graph_node must be a physical pb_type
     */
    std::map<t_pb_graph_node*, t_pb_graph_node*> physical_pb_graph_nodes_;

    /* Pair a pb_graph_pin to a physical pb_graph_pin */
    std::map<const t_pb_graph_pin*, t_pb_graph_pin*> physical_pb_graph_pins_;

    /* Pair a Routing Resource Switch (rr_switch) to a circuit model */
    std::map<RRSwitchId, CircuitModelId> rr_switch_circuit_models_;

    /* Pair a Routing Segment (rr_segment) to a circuit model */
    std::map<RRSegmentId, CircuitModelId> rr_segment_circuit_models_;

    /* Pair a direct connection (direct) to a annotation which contains circuit model id */
    std::map<size_t, ArchDirectId> direct_annotations_;

    /* Logical type routing resource graphs built from physical modes */
    std::map<t_pb_graph_node*, LbRRGraph> physical_lb_rr_graphs_;
};

} /* End namespace openfpga*/

#endif
