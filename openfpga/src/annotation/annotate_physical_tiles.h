#ifndef ANNOTATE_PHYSICAL_TILES_H
#define ANNOTATE_PHYSICAL_TILES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "openfpga_context.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_physical_tile_equivalent_sites(
  const DeviceContext& vpr_device_ctx,
  const TileAnnotation& tile_annotation,
  VprDeviceAnnotation& vpr_device_annotation);

void build_physical_tile_pin2port_info(
  const DeviceContext& vpr_device_ctx,
  VprDeviceAnnotation& vpr_device_annotation);

} /* end namespace openfpga */

#endif
