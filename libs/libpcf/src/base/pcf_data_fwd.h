/************************************************************************
 * A header file for PinConstraints class, including critical data declaration
 * Please include this file only for using any PinConstraints data structure
 * Refer to pin_constraints.h for more details
 ***********************************************************************/

/************************************************************************
 * Create strong id for PinConstraints to avoid illegal type casting
 ***********************************************************************/
#ifndef PCF_DATA_FWD_H
#define PCF_DATA_FWD_H

#include "vtr_strong_id.h"

struct pcf_io_constraint_id_tag;

typedef vtr::StrongId<pcf_io_constraint_id_tag> PcfIoConstraintId;

struct pcf_custom_constraint_id_tag;

typedef vtr::StrongId<pcf_custom_constraint_id_tag> PcfCustomConstraintId;

/* Short declaration of class */
class PcfData;

#endif
