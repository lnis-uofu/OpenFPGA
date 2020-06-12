/************************************************************************
 * A header file for FabricKey class, including critical data declaration
 * Please include this file only for using any TechnologyLibrary data structure
 * Refer to fabric_key.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for FabricKey to avoid illegal type casting 
 ***********************************************************************/
#ifndef FABRIC_KEY_FWD_H
#define FABRIC_KEY_FWD_H

#include "vtr_strong_id.h"

struct fabric_key_id_tag;

typedef vtr::StrongId<fabric_key_id_tag> FabricKeyId;

/* Short declaration of class */
class FabricKey;

#endif
