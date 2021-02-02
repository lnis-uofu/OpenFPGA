/************************************************************************
 * A header file for BitstreamSetting class, including critical data declaration
 * Please include this file only for using any BitstreamSetting data structure
 * Refer to bitstream_setting.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for the pb_type annotation in Bitstream setting to avoid illegal type casting 
 ***********************************************************************/
#ifndef BITSTREAM_SETTING_FWD_H
#define BITSTREAM_SETTING_FWD_H

#include "vtr_strong_id.h"

struct bitstream_pb_type_setting_id_tag;

typedef vtr::StrongId<bitstream_pb_type_setting_id_tag> BitstreamPbTypeSettingId;

/* Short declaration of class */
class BitstreamSetting;

#endif
