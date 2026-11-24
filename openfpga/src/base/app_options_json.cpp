#include "app_options_json.h"

#include "app_option_stage.h"
#include "json_response.h"
#include "json_writer.h"
#include "openfpga_session.h"
#include "vpr_option_table.h"

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

/* The flag of VPR of an option of VPR, e.g., --device for
 * general.device_layout. Null for an option of OpenFPGA */
const char* vpr_flag(const std::string& name) {
  static const std::map<std::string, const char*> flags = [] {
    std::map<std::string, const char*> table;
    for (const VprOptionSpec& spec : vpr_option_specs()) {
      table[std::string(spec.group) + "." + spec.name] = spec.flag;
    }
    return table;
  }();
  const auto found = flags.find(name);
  return flags.end() == found ? nullptr : found->second;
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
  json.key("vpr_flag");
  if (const char* flag = vpr_flag(name)) {
    json.value(flag);
  } else {
    json.null();
  }
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
