/************************************************************************
 * A header file for TileAnnotation class, including critical data declaration
 * Please include this file only for using any TileAnnotation data structure
 * Refer to tile_annotation.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for tile global ports to avoid illegal type casting 
 ***********************************************************************/
#ifndef TILE_ANNOTATION_FWD_H
#define TILE_ANNOTATION_FWD_H

#include "vtr_strong_id.h"

struct tile_global_port_id_tag;

typedef vtr::StrongId<tile_global_port_id_tag> TileGlobalPortId;

/* Short declaration of class */
class TileAnnotation;

#endif
