/************************************************************************
 * A header file for IoPinTable class, including critical data declaration
 * Please include this file only for using any IoPinTable data structure
 * Refer to IoPinTable.h for more details
 ***********************************************************************/

/************************************************************************
 * Create strong id for IoPinTable to avoid illegal type casting
 ***********************************************************************/
#ifndef IO_PIN_TABLE_FWD_H
#define IO_PIN_TABLE_FWD_H

#include "vtr_strong_id.h"

struct io_pin_table_id_tag;

typedef vtr::StrongId<io_pin_table_id_tag> IoPinTableId;

/* Short declaration of class */
class IoPinTable;

#endif
