#ifndef COMMAND_FWD_H
#define COMMAND_FWD_H

#include "vtr_strong_id.h"

/* Begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * A strong id for the options used by a command 
 ********************************************************************/
struct command_option_id_tag;

typedef vtr::StrongId<command_option_id_tag> CommandOptionId;

} /* End namespace openfpga */

#endif
