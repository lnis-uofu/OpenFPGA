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

int write_xml_boundary_timing(const char* fname,
                              const BoundaryTiming& bdy_timing);
} /* End namespace openfpga*/
