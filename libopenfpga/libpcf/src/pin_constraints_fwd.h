/************************************************************************
 * A header file for PinConstraints class, including critical data declaration
 * Please include this file only for using any PinConstraints data structure
 * Refer to pin_constraints.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for PinConstraints to avoid illegal type casting 
 ***********************************************************************/
#ifndef PIN_CONSTRAINTS_FWD_H
#define PIN_CONSTRAINTS_FWD_H

#include "vtr_strong_id.h"

struct pin_constraint_id_tag;

typedef vtr::StrongId<pin_constraint_id_tag> PinConstraintId;

/* Short declaration of class */
class PinConstraints;

#endif
