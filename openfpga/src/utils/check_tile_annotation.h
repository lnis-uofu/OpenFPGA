#ifndef CHECK_TILE_ANNOTATION_H
#define CHECK_TILE_ANNOTATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "tile_annotation.h"
#include "circuit_library.h"
#include "physical_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

bool check_tile_annotation(const TileAnnotation& tile_annotations,
                           const CircuitLibrary& circuit_lib,
                           const std::vector<t_physical_tile_type>& physical_tile_types);

} /* end namespace openfpga */

#endif
