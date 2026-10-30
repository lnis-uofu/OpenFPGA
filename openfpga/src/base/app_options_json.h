#ifndef APP_OPTIONS_JSON_H
#define APP_OPTIONS_JSON_H

#include <string>
#include <vector>

#include "app_options.h"

/********************************************************************
 * The application options as the answer of a query, of the kind app_options.
 * Its schema is openfpga/schemas/app_options.schema.json
 *
 * data is {"options": [...]}, one entry per option, in the order of the
 * names:
 *   name        group.option
 *   type        string, int, float, bool or selection
 *   value       the value, typed after the type: a selection is its name
 *   default     the default value, typed the same way
 *   is_default  true if value is the default
 *   help        the help message
 *   choices     the values a selection accepts, empty for other types
 *   stage       the stage that the option tunes, as to_string(OpenFpgaStage)
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* Version of the schema of the kind app_options */
constexpr int APP_OPTIONS_SCHEMA_VERSION = 1;

/* The answer for the options with these names, which must exist */
std::string app_options_json(const AppOptions& options,
                             const std::vector<std::string>& names);

} /* End namespace openfpga */

#endif
