#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "boundary_timing.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* Begin namespace openfpga */
namespace openfpga {
constexpr const char* XML_BOUNDARY_TIMING_ROOT_NAME = "boundary_timing";
constexpr const char* XML_BOUNDARY_TIMING_TREE_NODE = "pin";
constexpr const char* XML_BOUNDARY_TIMING_TREE_NODE_PIN_NAME = "name";
constexpr const char* XML_BOUNDARY_TIMING_TREE_NODE_PIN_MIN_DELAY = "min";
constexpr const char* XML_BOUNDARY_TIMING_TREE_NODE_PIN_MAX_DELAY = "max";
int write_xml_boundary_timing(const char* fname,
                              const BoundaryTiming& bdy_timing);
} /* End namespace openfpga*/
