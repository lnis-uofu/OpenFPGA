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

struct fabric_region_id_tag;
struct fabric_key_id_tag;
struct fabric_bit_line_bank_id_tag;
struct fabric_word_line_bank_id_tag;

typedef vtr::StrongId<fabric_region_id_tag> FabricRegionId;
typedef vtr::StrongId<fabric_key_id_tag> FabricKeyId;
typedef vtr::StrongId<fabric_bit_line_bank_id_tag> FabricBitLineBankId;
typedef vtr::StrongId<fabric_word_line_bank_id_tag> FabricWordLineBankId;

/* Short declaration of class */
class FabricKey;

#endif
