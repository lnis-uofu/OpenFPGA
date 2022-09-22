/************************************************************************
 * A header file for RepackDesignConstraints class, including critical data declaration
 * Please include this file only for using any TechnologyLibrary data structure
 * Refer to repack_design_constraints.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for RepackDesignConstraints to avoid illegal type casting 
 ***********************************************************************/
#ifndef REPACK_DESIGN_CONSTRAINTS_FWD_H
#define REPACK_DESIGN_CONSTRAINTS_FWD_H

#include "vtr_strong_id.h"

struct repack_design_constraint_id_tag;

typedef vtr::StrongId<repack_design_constraint_id_tag> RepackDesignConstraintId;

/* Short declaration of class */
class RepackDesignConstraints;

#endif
