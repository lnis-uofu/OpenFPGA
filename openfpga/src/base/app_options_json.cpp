#include "app_options_json.h"

#include "app_option_stage.h"
#include "json_response.h"
#include "json_writer.h"
#include "openfpga_session.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* Write the value of an option, typed after its type */
void write_value(JsonWriter& json, const AppOptionValue& option) {
  switch (option.type) {
    case AppOptionValue::INT:
      json.value(option.int_value);
      break;
    case AppOptionValue::FLOAT:
      json.value(option.float_value);
      break;
    case AppOptionValue::BOOLEAN:
      json.value(option.bool_value);
      break;
    case AppOptionValue::STRING:
    case AppOptionValue::SELECTION:
    default:
      json.value(option.string_value);
      break;
  }
}

}  // End anonymous namespace

void write_app_option_json(JsonWriter& json, const AppOptions& options,
                           const std::string& name) {
  const AppOptionValue* option = options.find(name);
  json.begin_object();
  json.key("name").value(name);
  json.key("type").value(option->type_name());
  json.key("value");
  write_value(json, *option);
  json.key("default");
  write_value(json, options.default_of(name));
  json.key("is_default").value(options.is_default(name));
  json.key("help").value(option->help_message);
  json.key("choices").begin_array();
  for (const std::string& choice : option->choices()) {
    json.value(choice);
  }
  json.end_array();
  OpenFpgaStage stage = OpenFpgaStage::DEVICE;
  app_option_stage(name, stage);
  json.key("stage").value(to_string(stage));
  json.end_object();
}

std::string app_options_json(const AppOptions& options,
                             const std::vector<std::string>& names) {
  JsonWriter json;
  begin_response(json, "app_options", APP_OPTIONS_SCHEMA_VERSION);
  json.begin_object();
  json.key("options").begin_array();
  for (const std::string& name : names) {
    write_app_option_json(json, options, name);
  }
  json.end_array();
  json.end_object();
  end_response(json);
  return json.str();
}

} /* End namespace openfpga */
