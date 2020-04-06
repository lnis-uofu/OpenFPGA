#ifndef ANALYSIS_SDC_GRID_WRITER_H
#define ANALYSIS_SDC_GRID_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <vector>
#include "device_grid.h"
#include "module_manager.h"
#include "vpr_device_annotation.h"
#include "vpr_clustering_annotation.h"
#include "vpr_placement_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_analysis_sdc_disable_unused_grids(std::fstream& fp, 
                                             const DeviceGrid& grids, 
                                             const VprDeviceAnnotation& device_annotation,
                                             const VprClusteringAnnotation& cluster_annotation,
                                             const VprPlacementAnnotation& place_annotation,
                                             const ModuleManager& module_manager);

} /* end namespace openfpga */

#endif
