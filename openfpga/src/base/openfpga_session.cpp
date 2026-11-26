#include "openfpga_session.h"

#include "app_option_stage.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

constexpr OpenFpgaStage all_stages[] = {
  OpenFpgaStage::DEVICE,   OpenFpgaStage::PACKED,   OpenFpgaStage::PLACED,
  OpenFpgaStage::ROUTED,   OpenFpgaStage::ANALYZED, OpenFpgaStage::REPACKED,
  OpenFpgaStage::BITSTREAM};

constexpr OpenFpgaInput all_inputs[] = {OpenFpgaInput::VPR_ARCH,
                                        OpenFpgaInput::OPENFPGA_ARCH,
                                        OpenFpgaInput::CIRCUIT};

unsigned bit(const OpenFpgaInput& input) {
  return 1u << static_cast<int>(input);
}

unsigned bit(const OpenFpgaStage& stage) {
  return 1u << static_cast<int>(stage);
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

OpenFpgaStage required_stage(const OpenFpgaStage& stage) {
  switch (stage) {
    case OpenFpgaStage::UNINIT:
    case OpenFpgaStage::DEVICE:
      return OpenFpgaStage::UNINIT;
    case OpenFpgaStage::PACKED:
      return OpenFpgaStage::DEVICE;
    case OpenFpgaStage::PLACED:
      return OpenFpgaStage::PACKED;
    case OpenFpgaStage::ROUTED:
      return OpenFpgaStage::PLACED;
    case OpenFpgaStage::ANALYZED:
    case OpenFpgaStage::REPACKED:
      return OpenFpgaStage::ROUTED;
    case OpenFpgaStage::BITSTREAM:
      return OpenFpgaStage::REPACKED;
    default:
      break;
  }
  return OpenFpgaStage::UNINIT;
}

std::string to_string(const OpenFpgaInput& input) {
  switch (input) {
    case OpenFpgaInput::VPR_ARCH:
      return "VPR_ARCH";
    case OpenFpgaInput::OPENFPGA_ARCH:
      return "OPENFPGA_ARCH";
    case OpenFpgaInput::CIRCUIT:
      return "CIRCUIT";
    default:
      break;
  }
  return "UNKNOWN";
}

std::string to_string(const OpenFpgaStage& stage) {
  switch (stage) {
    case OpenFpgaStage::UNINIT:
      return "UNINIT";
    case OpenFpgaStage::DEVICE:
      return "DEVICE";
    case OpenFpgaStage::PACKED:
      return "PACKED";
    case OpenFpgaStage::PLACED:
      return "PLACED";
    case OpenFpgaStage::ROUTED:
      return "ROUTED";
    case OpenFpgaStage::ANALYZED:
      return "ANALYZED";
    case OpenFpgaStage::REPACKED:
      return "REPACKED";
    case OpenFpgaStage::BITSTREAM:
      return "BITSTREAM";
    default:
      break;
  }
  return "UNKNOWN";
}

bool OpenFpgaSession::uninitialized() const {
  return 0 == inputs_ && 0 == stages_;
}

bool OpenFpgaSession::has_input(const OpenFpgaInput& input) const {
  return 0 != (inputs_ & bit(input));
}

bool OpenFpgaSession::has_all_inputs() const {
  for (const OpenFpgaInput& input : all_inputs) {
    if (!has_input(input)) {
      return false;
    }
  }
  return true;
}

bool OpenFpgaSession::can_read(const OpenFpgaInput& input) const {
  /* The circuit is read against the VPR architecture */
  if (OpenFpgaInput::CIRCUIT == input) {
    return has_input(OpenFpgaInput::VPR_ARCH);
  }
  return true;
}

bool OpenFpgaSession::reached(const OpenFpgaStage& stage) const {
  if (OpenFpgaStage::UNINIT == stage) {
    return true;
  }
  return 0 != (stages_ & bit(stage));
}

bool OpenFpgaSession::can_run(const OpenFpgaStage& stage) const {
  /* Nothing runs to get to UNINIT */
  if (OpenFpgaStage::UNINIT == stage) {
    return false;
  }
  /* The device is made from all the inputs */
  if (OpenFpgaStage::DEVICE == stage) {
    return has_all_inputs();
  }
  return reached(required_stage(stage));
}

std::vector<OpenFpgaStage> OpenFpgaSession::reached_stages() const {
  std::vector<OpenFpgaStage> stages;
  for (const OpenFpgaStage& stage : all_stages) {
    if (reached(stage)) {
      stages.push_back(stage);
    }
  }
  return stages;
}

std::string OpenFpgaSession::imported_file(const OpenFpgaStage& stage) const {
  const auto it = imported_.find(stage);
  return imported_.end() == it ? std::string() : it->second;
}

void OpenFpgaSession::set_imported(const OpenFpgaStage& stage,
                                   const std::string& file) {
  if (reached(stage)) {
    imported_[stage] = file;
  }
}

std::string OpenFpgaSession::input_file(const OpenFpgaInput& input) const {
  const auto it = input_files_.find(input);
  return input_files_.end() == it ? std::string() : it->second;
}

bool OpenFpgaSession::read_input(const OpenFpgaInput& input,
                                 const std::string& file) {
  if (!can_read(input)) {
    return false;
  }
  /* The VPR architecture comes first, and what was read against the previous
   * one cannot be kept */
  if (OpenFpgaInput::VPR_ARCH == input) {
    inputs_ &= ~bit(OpenFpgaInput::CIRCUIT);
    input_files_.erase(OpenFpgaInput::CIRCUIT);
  }
  inputs_ |= bit(input);
  input_files_[input] = file;
  if (OpenFpgaInput::CIRCUIT != input) {
    forget_package();
  }
  invalidate_all_stages();
  return true;
}

void OpenFpgaSession::fail_input(const OpenFpgaInput& input) {
  inputs_ &= ~bit(input);
  input_files_.erase(input);
  if (OpenFpgaInput::CIRCUIT != input) {
    forget_package();
  }
  if (OpenFpgaInput::VPR_ARCH == input) {
    inputs_ &= ~bit(OpenFpgaInput::CIRCUIT);
    input_files_.erase(OpenFpgaInput::CIRCUIT);
  }
  invalidate_all_stages();
}

bool OpenFpgaSession::complete(const OpenFpgaStage& stage) {
  if (!can_run(stage)) {
    return false;
  }
  /* What depends on the stage was made from its previous results */
  invalidate_stage_and_dependents(stage);
  stages_ |= bit(stage);
  stage_generations_[stage] = ++completions_;
  return true;
}

uint64_t OpenFpgaSession::stage_generation(const OpenFpgaStage& stage) const {
  if (!reached(stage)) {
    return 0;
  }
  const auto found = stage_generations_.find(stage);
  return stage_generations_.end() == found ? 0 : found->second;
}

bool OpenFpgaSession::analytical_placer() const {
  const AppOptionValue* placer = app_options_.find("place.placer");
  return nullptr != placer && "analytical" == placer->string_value;
}

OpenFpgaStage OpenFpgaSession::dropped_with(const OpenFpgaStage& stage) const {
  if (OpenFpgaStage::PLACED == stage && analytical_placer()) {
    return OpenFpgaStage::PACKED;
  }
  return stage;
}

void OpenFpgaSession::fail(const OpenFpgaStage& stage,
                           const std::string& message) {
  invalidate_stage_and_dependents(dropped_with(stage));
  if (!message.empty()) {
    failures_[stage] = message;
  }
}

const std::string* OpenFpgaSession::stage_log(
  const OpenFpgaStage& stage) const {
  const auto found = stage_logs_.find(stage);
  return stage_logs_.end() == found ? nullptr : &found->second;
}

const double* OpenFpgaSession::stage_seconds(const OpenFpgaStage& stage) const {
  const auto found = stage_seconds_.find(stage);
  return stage_seconds_.end() == found ? nullptr : &found->second;
}

void OpenFpgaSession::set_stage_log(const OpenFpgaStage& stage,
                                    const std::string& log) {
  if (reached(stage)) {
    stage_logs_[stage] = log;
  }
}

void OpenFpgaSession::set_stage_seconds(const OpenFpgaStage& stage,
                                        const double seconds) {
  if (reached(stage)) {
    stage_seconds_[stage] = seconds;
  }
}

const std::string* OpenFpgaSession::failure(const OpenFpgaStage& stage) const {
  const auto found = failures_.find(stage);
  return failures_.end() == found ? nullptr : &found->second;
}

bool OpenFpgaSession::set_app_option(const std::string& name,
                                     const std::string& text,
                                     std::string& error) {
  const AppOptionValue* option = app_options_.find(name);
  const std::string before = nullptr == option ? "" : option->to_string();
  if (!app_options_.set(name, text, error)) {
    return false;
  }
  if (before != app_options_.find(name)->to_string()) {
    invalidate_for_option(name);
  }
  return true;
}

bool OpenFpgaSession::reset_app_option(const std::string& name) {
  if (nullptr == app_options_.find(name)) {
    return false;
  }
  const bool changed = !app_options_.is_default(name);
  app_options_.reset(name);
  if (changed) {
    invalidate_for_option(name);
  }
  return true;
}

void OpenFpgaSession::reset_app_options() {
  for (const std::string& name : app_options_.names()) {
    reset_app_option(name);
  }
}

void OpenFpgaSession::reset() {
  inputs_ = 0;
  input_files_.clear();
  forget_package();
  stages_ = 0;
  linked_generation_ = 0;
  fabric_generation_ = 0;
  imported_.clear();
  failures_.clear();
  stage_logs_.clear();
  stage_seconds_.clear();
}

void OpenFpgaSession::invalidate_stage_and_dependents(
  const OpenFpgaStage& stage) {
  for (const OpenFpgaStage& other : all_stages) {
    if (is_or_depends_on(other, stage)) {
      if (OpenFpgaStage::BITSTREAM == other) {
        bitstream_output_ = BitstreamOutput();
      }
      stages_ &= ~bit(other);
      imported_.erase(other);
      failures_.erase(other);
      stage_logs_.erase(other);
      stage_seconds_.erase(other);
    }
  }
}

void OpenFpgaSession::invalidate_all_stages() {
  stages_ = 0;
  bitstream_output_ = BitstreamOutput();
  imported_.clear();
  failures_.clear();
  stage_logs_.clear();
  stage_seconds_.clear();
}

void OpenFpgaSession::forget_package() {
  device_package_.clear();
  bitstream_recipe_ = BitstreamRecipe();
}

std::vector<std::string> OpenFpgaSession::settings_files(
  const std::string& kind) const {
  const auto found = settings_files_.find(kind);
  return settings_files_.end() == found ? std::vector<std::string>()
                                        : found->second;
}

bool OpenFpgaSession::link_current() const {
  return 0 != linked_generation_ && linked_generation_ == vpr_generation_;
}

bool OpenFpgaSession::fabric_current() const {
  return link_current() && fabric_generation_ == linked_generation_;
}

void OpenFpgaSession::read_settings(const std::string& kind,
                                    const std::string& file,
                                    const bool append) {
  std::vector<std::string>& files = settings_files_[kind];
  if (!append) {
    files.clear();
  }
  files.push_back(file);
  linked_generation_ = 0;
  fabric_generation_ = 0;
  if ("bitstream_setting" == kind) {
    invalidate_stage_and_dependents(OpenFpgaStage::REPACKED);
  }
}

void OpenFpgaSession::invalidate_for_option(const std::string& name) {
  OpenFpgaStage stage = OpenFpgaStage::DEVICE;
  /* An option without a stage cannot be located: drop everything */
  if (!app_option_stage(name, stage)) {
    invalidate_all_stages();
    return;
  }
  invalidate_stage_and_dependents(dropped_with(stage));
}

} /* End namespace openfpga */
