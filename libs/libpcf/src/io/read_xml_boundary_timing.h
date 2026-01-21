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

BoundaryTiming read_xml_boundary_timing(const char* fname);
int read_xml_boundary_timing(const char* fname, BoundaryTiming& bdy_timing,
                             const bool& append, const bool& verbose);
} /* End namespace openfpga*/
