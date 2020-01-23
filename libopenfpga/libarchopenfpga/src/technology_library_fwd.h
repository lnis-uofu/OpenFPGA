/************************************************************************
 * A header file for TechnologyLibrary class, including critical data declaration
 * Please include this file only for using any TechnologyLibrary data structure
 * Refer to circuit_library.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for TechnologyDevice to avoid illegal type casting 
 ***********************************************************************/
#ifndef TECHNOLOGY_LIBRARY_FWD_H
#define TECHNOLOGY_LIBRARY_FWD_H

#include "vtr_strong_id.h"

struct technology_model_id_tag;
struct technology_variation_id_tag;

typedef vtr::StrongId<technology_model_id_tag> TechnologyModelId;
typedef vtr::StrongId<technology_variation_id_tag> TechnologyVariationId;

/* Short declaration of class */
class TechnologyLibrary;

#endif
