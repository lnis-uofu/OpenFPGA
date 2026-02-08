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

int read_xml_boundary_timing(const char* fname, BoundaryTiming& bdy_timing,
                             const bool& append = true,
                             const bool& verbose = false);
} /* End namespace openfpga*/
