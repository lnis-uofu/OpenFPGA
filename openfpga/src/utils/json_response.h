#ifndef JSON_RESPONSE_H
#define JSON_RESPONSE_H

#include <string>

#include "json_writer.h"

/********************************************************************
 * The envelope of the answer of a query. Every answer is
 *
 *   {"schema_version": <int>, "kind": "<name>", "stage": "<stage>"|null,
 *    "data": <the answer>}
 *
 * - kind names the answer, e.g., app_options. The schema of the answer is
 *   openfpga_flow/schemas/<kind>.schema.json
 * - schema_version is the version of the schema of that kind, and each kind
 *   has its own. Adding a field keeps the version. Removing or renaming a
 *   field, or changing what a field means, increases it
 * - stage is the stage of the flow the answer was read from, as in
 *   to_string(OpenFpgaStage), or null for an answer that is not read from a
 *   stage, e.g., the application options
 * - data is the answer, which the caller writes
 *
 * Example:
 *   JsonWriter json;
 *   begin_response(json, "app_options", 1);
 *   json.begin_object(); ... json.end_object();
 *   end_response(json);
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* Write the start of the envelope. The writer is then ready for the value of
 * data. An empty stage is written as null */
void begin_response(JsonWriter& json, const std::string& kind,
                    const int schema_version,
                    const std::string& stage = std::string());

/* Write the end of the envelope, after the value of data */
void end_response(JsonWriter& json);

} /* End namespace openfpga */

#endif
