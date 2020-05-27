#ifndef OPENFPGA_CONTEXT_H
#define OPENFPGA_CONTEXT_H

#include <vector>
#include "vpr_context.h"
#include "openfpga_arch.h"
#include "vpr_netlist_annotation.h"
#include "vpr_device_annotation.h"
#include "vpr_clustering_annotation.h"
#include "vpr_placement_annotation.h"
#include "vpr_routing_annotation.h"
#include "mux_library.h"
#include "decoder_library.h"
#include "tile_direct.h"
#include "module_manager.h"
#include "netlist_manager.h"
#include "openfpga_flow_manager.h"
#include "bitstream_manager.h"
#include "fabric_bitstream.h"
#include "device_rr_gsb.h"
#include "io_location_map.h"

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
    const openfpga::VprDeviceAnnotation& vpr_device_annotation() const { return vpr_device_annotation_; }
    const openfpga::VprNetlistAnnotation& vpr_netlist_annotation() const { return vpr_netlist_annotation_; }
    const openfpga::VprClusteringAnnotation& vpr_clustering_annotation() const { return vpr_clustering_annotation_; }
    const openfpga::VprPlacementAnnotation& vpr_placement_annotation() const { return vpr_placement_annotation_; }
    const openfpga::VprRoutingAnnotation& vpr_routing_annotation() const { return vpr_routing_annotation_; }
    const openfpga::DeviceRRGSB& device_rr_gsb() const { return device_rr_gsb_; }
    const openfpga::MuxLibrary& mux_lib() const { return mux_lib_; }
    const openfpga::DecoderLibrary& decoder_lib() const { return decoder_lib_; }
    const openfpga::TileDirect& tile_direct() const { return tile_direct_; }
    const openfpga::ModuleManager& module_graph() const { return module_graph_; }
    const openfpga::FlowManager& flow_manager() const { return flow_manager_; }
    const openfpga::BitstreamManager& bitstream_manager() const { return bitstream_manager_; }
    const openfpga::FabricBitstream& fabric_bitstream() const { return fabric_bitstream_; }
    const openfpga::IoLocationMap& io_location_map() const { return io_location_map_; }
    const std::unordered_map<AtomNetId, t_net_power>& net_activity() const { return net_activity_; }
    const openfpga::NetlistManager& verilog_netlists() const { return verilog_netlists_; }
  public:  /* Public mutators */
    openfpga::Arch& mutable_arch() { return arch_; }
    openfpga::VprDeviceAnnotation& mutable_vpr_device_annotation() { return vpr_device_annotation_; }
    openfpga::VprNetlistAnnotation& mutable_vpr_netlist_annotation() { return vpr_netlist_annotation_; }
    openfpga::VprClusteringAnnotation& mutable_vpr_clustering_annotation() { return vpr_clustering_annotation_; }
    openfpga::VprPlacementAnnotation& mutable_vpr_placement_annotation() { return vpr_placement_annotation_; }
    openfpga::VprRoutingAnnotation& mutable_vpr_routing_annotation() { return vpr_routing_annotation_; }
    openfpga::DeviceRRGSB& mutable_device_rr_gsb() { return device_rr_gsb_; }
    openfpga::MuxLibrary& mutable_mux_lib() { return mux_lib_; }
    openfpga::DecoderLibrary& mutable_decoder_lib() { return decoder_lib_; }
    openfpga::TileDirect& mutable_tile_direct() { return tile_direct_; }
    openfpga::ModuleManager& mutable_module_graph() { return module_graph_; }
    openfpga::FlowManager& mutable_flow_manager() { return flow_manager_; }
    openfpga::BitstreamManager& mutable_bitstream_manager() { return bitstream_manager_; }
    openfpga::FabricBitstream& mutable_fabric_bitstream() { return fabric_bitstream_; }
    openfpga::IoLocationMap& mutable_io_location_map() { return io_location_map_; }
    std::unordered_map<AtomNetId, t_net_power>& mutable_net_activity() { return net_activity_; }
    openfpga::NetlistManager& mutable_verilog_netlists() { return verilog_netlists_; }
  private: /* Internal data */
    /* Data structure to store information from read_openfpga_arch library */
    openfpga::Arch arch_;

    /* Annotation to pb_type of VPR */
    openfpga::VprDeviceAnnotation vpr_device_annotation_;

    /* Naming fix to netlist */
    openfpga::VprNetlistAnnotation vpr_netlist_annotation_;

    /* Pin net fix to cluster results */
    openfpga::VprClusteringAnnotation vpr_clustering_annotation_;

    /* Placement results */
    openfpga::VprPlacementAnnotation vpr_placement_annotation_;

    /* Routing results annotation */ 
    openfpga::VprRoutingAnnotation vpr_routing_annotation_;

    /* Device-level annotation */
    openfpga::DeviceRRGSB device_rr_gsb_;
    
    /* Library of physical implmentation of routing multiplexers */
    openfpga::MuxLibrary mux_lib_;

    /* Library of physical implmentation of decoders */
    openfpga::DecoderLibrary decoder_lib_;

    /* Inner/inter-column/row tile direct connections */
    openfpga::TileDirect tile_direct_;

    /* Fabric module graph */
    openfpga::ModuleManager module_graph_;
    openfpga::IoLocationMap io_location_map_;

    /* Bitstream database */
    openfpga::BitstreamManager bitstream_manager_;
    openfpga::FabricBitstream fabric_bitstream_;

    /* Netlist database 
     * TODO: Each format should have an independent entry
     */
    openfpga::NetlistManager verilog_netlists_;

    /* Net activities of users' implementation */
    std::unordered_map<AtomNetId, t_net_power> net_activity_; 
 
    /* Flow status */
    openfpga::FlowManager flow_manager_;
};

#endif
