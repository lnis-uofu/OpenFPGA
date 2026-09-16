#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <map>
#include <set>
#include <string>

#include "bitstream_setting.h"
#include "circuit_library.h"
#include "mif_location_map.h"
#include "mif_pipeline.h"
#include "vpr_clustering_annotation.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "vpr_placement_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Decode INIT / hex per placed physical primitive into physical_mifs_.
 * Hex paths come from read_mif; eblif INIT comes from the atom netlist
 * via PhysicalPb::MifDataInfo written by repack.
 *
 * Called from fpga_bitstream_template() at the start of the shell
 * command build_architecture_bitstream. Must run after repack.
 *******************************************************************/
int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline, const AtomContext& atom_ctx,
                       const VprClusteringAnnotation& clustering_annotation,
                       const VprPlacementAnnotation& placement_annotation);

/********************************************************************
 * Concatenate every destination PB on the FPGA-top MIF location map into
 * top_mif_. A location uses physical_mifs_ data when the pipeline has a
 * matching PB at that grid; otherwise the slice is filled with 0.
 *
 * Called from fpga_bitstream_template() immediately after
 * build_physical_mif(), still inside build_architecture_bitstream.
 * Needs the location map from build_fabric.
 *******************************************************************/
int aggregate_unified_mif(const BitstreamSetting& bitstream_setting,
                          MifPipeline& mif_pipeline,
                          const MifLocationMap& mif_location_map);

/* Top-level MIF data-bus port name -> circuit model name (from the physical
 * pb bound on each location-map slice). */
std::map<std::string, std::string> collect_mif_data_port_circuit_models(
  const MifLocationMap& mif_location_map,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */
