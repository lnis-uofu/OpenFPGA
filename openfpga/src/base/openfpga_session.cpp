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

/* The stage that a stage requires: its parent in the tree of stages */
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

bool OpenFpgaSession::read_input(const OpenFpgaInput& input) {
  if (!can_read(input)) {
    return false;
  }
  /* The VPR architecture comes first, and what was read against the previous
   * one cannot be kept */
  if (OpenFpgaInput::VPR_ARCH == input) {
    inputs_ &= ~bit(OpenFpgaInput::CIRCUIT);
  }
  inputs_ |= bit(input);
  invalidate_all_stages();
  return true;
}

void OpenFpgaSession::fail_input(const OpenFpgaInput& input) {
  inputs_ &= ~bit(input);
  if (OpenFpgaInput::VPR_ARCH == input) {
    inputs_ &= ~bit(OpenFpgaInput::CIRCUIT);
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
  return true;
}

void OpenFpgaSession::fail(const OpenFpgaStage& stage) {
  invalidate_stage_and_dependents(stage);
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
  stages_ = 0;
}

void OpenFpgaSession::invalidate_stage_and_dependents(
  const OpenFpgaStage& stage) {
  for (const OpenFpgaStage& other : all_stages) {
    if (is_or_depends_on(other, stage)) {
      stages_ &= ~bit(other);
    }
  }
}

void OpenFpgaSession::invalidate_all_stages() { stages_ = 0; }

void OpenFpgaSession::invalidate_for_option(const std::string& name) {
  OpenFpgaStage stage = OpenFpgaStage::DEVICE;
  /* An option without a stage cannot be located: drop everything */
  if (!app_option_stage(name, stage)) {
    invalidate_all_stages();
    return;
  }
  invalidate_stage_and_dependents(stage);
}

} /* End namespace openfpga */
