#include "session_json.h"

#include <filesystem>

#include "json_response.h"
#include "json_writer.h"
#include "staged_flow.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* The file of an input, or null */
void write_file(JsonWriter& json, const OpenFpgaSession& session,
                const OpenFpgaInput& input) {
  const std::string file = session.input_file(input);
  if (file.empty() || !session.has_input(input)) {
    json.null();
    return;
  }
  json.begin_object();
  json.key("file").value(file);
  if (OpenFpgaInput::VPR_ARCH == input) {
    json.key("layout").value(
      session.app_options().find("general.device_layout")->string_value);
  }
  json.end_object();
}

/* A path from the current directory, whole */
std::string whole_path(const std::string& file) {
  std::error_code error;
  const std::filesystem::path path = std::filesystem::absolute(file, error);
  return error ? file : path.lexically_normal().string();
}

/* The file of the results of a stage, or null */
void write_result(JsonWriter& json, const bool reached,
                  const std::string& file) {
  if (reached && !file.empty()) {
    json.value(whole_path(file));
  } else {
    json.null();
  }
}

}  // End anonymous namespace

std::string session_json(const OpenFpgaSession& session,
                         const vpr::VprRun* run) {
  /* What the session says is true of the data that is alive only */
  const bool current = staged_flow_current(session) && nullptr != run;

  JsonWriter json;
  begin_response(json, "session", SESSION_SCHEMA_VERSION);
  json.begin_object();
  json.key("reached").begin_array();
  if (current) {
    for (const OpenFpgaStage& stage : session.reached_stages()) {
      json.value(to_string(stage));
    }
  }
  json.end_array();
  json.key("inputs").begin_object();
  json.key("vpr_arch");
  write_file(json, session, OpenFpgaInput::VPR_ARCH);
  json.key("openfpga_arch");
  write_file(json, session, OpenFpgaInput::OPENFPGA_ARCH);
  json.key("circuit");
  write_file(json, session, OpenFpgaInput::CIRCUIT);
  json.end_object();
  json.key("results").begin_object();
  json.key("packing");
  write_result(json, current && session.reached(OpenFpgaStage::PACKED),
               current ? run->setup.FileNameOpts.NetFile : std::string());
  json.key("placement");
  write_result(json, current && session.reached(OpenFpgaStage::PLACED),
               current ? run->setup.FileNameOpts.PlaceFile : std::string());
  json.key("routing");
  write_result(json, current && session.reached(OpenFpgaStage::ROUTED),
               current ? run->setup.FileNameOpts.RouteFile : std::string());
  json.end_object();
  json.end_object();
  end_response(json);
  return json.str();
}

} /* End namespace openfpga */
