#ifndef BUILD_PHYSICAL_TRUTH_TABLE_H
#define BUILD_PHYSICAL_TRUTH_TABLE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "vpr_clustering_annotation.h"
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_physical_lut_truth_tables(VprClusteringAnnotation& cluster_annotation,
                                     const AtomContext& atom_ctx,
                                     const ClusteringContext& cluster_ctx,
                                     const VprDeviceAnnotation& device_annotation,
                                     const CircuitLibrary& circuit_lib,
                                     const bool& verbose);

} /* end namespace openfpga */

#endif
