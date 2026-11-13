#include "json_response.h"

/* Begin namespace openfpga */
namespace openfpga {

void begin_response(JsonWriter& json, const std::string& kind,
                    const int schema_version, const std::string& stage) {
  json.begin_object();
  json.key("schema_version").value(schema_version);
  json.key("kind").value(kind);
  json.key("stage");
  if (stage.empty()) {
    json.null();
  } else {
    json.value(stage);
  }
  json.key("data");
}

void end_response(JsonWriter& json) { json.end_object(); }

} /* End namespace openfpga */
