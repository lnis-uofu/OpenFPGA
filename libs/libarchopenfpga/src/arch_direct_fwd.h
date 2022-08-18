/************************************************************************
 * A header file for ArchDirect class, including critical data declaration
 * Please include this file only for using any TechnologyLibrary data structure
 * Refer to arch_direct.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for ArchDirect to avoid illegal type casting 
 ***********************************************************************/
#ifndef ARCH_DIRECT_FWD_H
#define ARCH_DIRECT_FWD_H

#include "vtr_strong_id.h"

struct arch_direct_id_tag;

typedef vtr::StrongId<arch_direct_id_tag> ArchDirectId;

/* Short declaration of class */
class ArchDirect;

#endif
