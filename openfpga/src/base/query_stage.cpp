#include "query_stage.h"

#include <algorithm>
#include <cctype>
#include <vector>

#include "app_option_stage.h"
#include "app_options_json.h"
#include "glob_match.h"
#include "json_response.h"
#include "json_writer.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

const OpenFpgaStage FLOW_STAGES[] = {
  OpenFpgaStage::DEVICE,   OpenFpgaStage::PACKED,   OpenFpgaStage::PLACED,
  OpenFpgaStage::ROUTED,   OpenFpgaStage::ANALYZED, OpenFpgaStage::REPACKED,
  OpenFpgaStage::BITSTREAM};

/* The commands that reach a stage */
std::vector<std::string> stage_commands(const OpenFpgaStage& stage) {
  switch (stage) {
    case OpenFpgaStage::DEVICE:
      return {"read_vpr_arch", "read_openfpga_arch", "read_circuit"};
    case OpenFpgaStage::PACKED:
      return {"pack", "read_packing"};
    case OpenFpgaStage::PLACED:
      return {"place", "read_placement"};
    case OpenFpgaStage::ROUTED:
      return {"route", "read_routing"};
    case OpenFpgaStage::ANALYZED:
      return {"analysis"};
    case OpenFpgaStage::REPACKED:
      return {"repack"};
    case OpenFpgaStage::BITSTREAM:
      return {"build_architecture_bitstream"};
    default:
      break;
  }
  return {};
}

std::string lower(std::string text) {
  std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return text;
}

/* Return true if a stage is the other one, or comes after it in the tree */
bool is_or_depends_on(const OpenFpgaStage& stage, const OpenFpgaStage& other) {
  OpenFpgaStage cur = stage;
  while (OpenFpgaStage::UNINIT != cur) {
    if (cur == other) {
      return true;
    }
    cur = required_stage(cur);
  }
  return false;
}

}  // End anonymous namespace

bool stage_info_json(const OpenFpgaSession& session, const std::string& stage,
                     const bool changed_only, const std::string& query,
                     const size_t limit, std::string& json_text) {
  if (limit < 1) {
    VTR_LOG_ERROR("The limit must be at least 1\n");
    return false;
  }
  std::vector<OpenFpgaStage> asked;
  if (stage.empty()) {
    asked.assign(std::begin(FLOW_STAGES), std::end(FLOW_STAGES));
  } else {
    for (const OpenFpgaStage& candidate : FLOW_STAGES) {
      const auto commands = stage_commands(candidate);
      if (lower(to_string(candidate)) == lower(stage) ||
          std::find(commands.begin(), commands.end(), lower(stage)) !=
            commands.end()) {
        asked.push_back(candidate);
      }
    }
    if (asked.empty()) {
      std::string valid;
      for (const OpenFpgaStage& candidate : FLOW_STAGES) {
        valid += (valid.empty() ? "" : ", ") + to_string(candidate);
      }
      VTR_LOG_ERROR("The stage '%s' does not exist. The stages are: %s\n",
                    stage.c_str(), valid.c_str());
      return false;
    }
  }

  const AppOptions& options = session.app_options();
  JsonWriter json;
  begin_response(json, "stage_info", STAGE_INFO_SCHEMA_VERSION, "DEVICE");
  json.begin_object();
  json.key("filter").begin_object();
  json.key("stage");
  stage.empty() ? json.null() : json.value(to_string(asked.front()));
  json.key("changed").value(changed_only);
  json.key("query");
  query.empty() ? json.null() : json.value(query);
  json.end_object();

  json.key("stages").begin_array();
  for (const OpenFpgaStage& current : asked) {
    std::vector<std::string> matching;
    size_t total = 0;
    for (const std::string& name : options.names()) {
      OpenFpgaStage tuned = OpenFpgaStage::DEVICE;
      app_option_stage(name, tuned);
      if (tuned != current) {
        continue;
      }
      ++total;
      if ((changed_only && options.is_default(name)) ||
          (!query.empty() && !glob_match(query, name))) {
        continue;
      }
      matching.push_back(name);
    }
    json.begin_object();
    json.key("stage").value(to_string(current));
    json.key("requires");
    const OpenFpgaStage before = required_stage(current);
    OpenFpgaStage::UNINIT == before ? json.null()
                                    : json.value(to_string(before));
    json.key("commands").begin_array();
    for (const std::string& command : stage_commands(current)) {
      json.value(command);
    }
    json.end_array();
    json.key("reached").value(session.reached(current));
    json.key("failed");
    const std::string* failure = session.failure(current);
    failure != nullptr ? json.value(*failure) : json.null();
    json.key("dropped_by_change").begin_array();
    for (const OpenFpgaStage& other : FLOW_STAGES) {
      if (is_or_depends_on(other, current)) {
        json.value(to_string(other));
      }
    }
    json.end_array();
    const size_t returned = std::min(limit, matching.size());
    json.key("options").begin_object();
    json.key("total").value(static_cast<int64_t>(total));
    json.key("matched").value(static_cast<int64_t>(matching.size()));
    json.key("returned").value(static_cast<int64_t>(returned));
    json.key("truncated").value(returned < matching.size());
    json.key("items").begin_array();
    for (size_t i = 0; i < returned; ++i) {
      write_app_option_json(json, options, matching[i]);
    }
    json.end_array();
    json.end_object();
    json.end_object();
  }
  json.end_array();
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

} /* End namespace openfpga */
