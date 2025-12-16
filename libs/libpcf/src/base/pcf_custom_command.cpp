#include "pcf_custom_command.h"

#include <algorithm>

#include "openfpga_port_parser.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Member functions for class PcfData
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PcfCustomCommand::PcfCustomCommand() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
PcfCustomCommand::pcf_custom_command_range PcfCustomCommand::custom_commands()
  const {
  return vtr::make_range(custom_command_ids_.begin(),
                         custom_command_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/

std::string PcfCustomCommand::custom_command_name(
  const PcfCustomCommandId& custom_command_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_command_id(custom_command_id));
  return custom_command_names_[custom_command_id];
}

std::string PcfCustomCommand::custom_command_pb_type(
  const std::string& custom_command_name) const {
  /* validate the io_id */
  auto custom_command_id = find_command_id(custom_command_name);
  return custom_command_pb_types_[custom_command_id];
}

int PcfCustomCommand::custom_command_pb_type_offset(
  const std::string& custom_command_name) const {
  /* validate the io_id */
  auto custom_command_id = find_command_id(custom_command_name);
  return custom_command_pb_type_offset_[custom_command_id];
}

std::string PcfCustomCommand::custom_command_type(
  const PcfCustomCommandId& custom_command_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_command_id(custom_command_id));
  return custom_command_types_[custom_command_id];
}

std::string PcfCustomCommand::custom_option_name(
  const PcfCustomCommandOptionId& custom_option_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_option_id(custom_option_id));
  return custom_option_names_[custom_option_id];
}

std::string PcfCustomCommand::custom_option_type(
  const PcfCustomCommandOptionId& custom_option_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_option_id(custom_option_id));
  return custom_option_types_[custom_option_id];
}

std::string PcfCustomCommand::custom_option_type(
  const std::string& command_name, const std::string& option_name) const {
  /* validate the io_id */
  auto custom_option_id = find_option_id(command_name, option_name);
  return custom_option_type(custom_option_id);
}

std::string PcfCustomCommand::custom_mode_name(
  const PcfCustomCommandModeId& custom_mode_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_mode_id(custom_mode_id));
  return custom_mode_names_[custom_mode_id];
}

std::string PcfCustomCommand::custom_mode_value(
  const PcfCustomCommandModeId& custom_mode_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_mode_id(custom_mode_id));
  return custom_mode_values_[custom_mode_id];
}

std::string PcfCustomCommand::custom_mode_value(
  const std::string& command_name, const std::string& option_name,
  const std::string& mode_name) const {
  /* validate the io_id */
  auto custom_mode_id = find_mode_id(command_name, option_name, mode_name);
  return custom_mode_value(custom_mode_id);
}

bool PcfCustomCommand::empty() const { return 0 == custom_command_ids_.size(); }

/************************************************************************
 * Public Mutators
 ***********************************************************************/

int PcfCustomCommand::create_custom_command(const std::string& command_name,
                                            const std::string& command_type) {
  /* Create a new id */
  PcfCustomCommandId custom_command_id =
    PcfCustomCommandId(custom_command_ids_.size());

  custom_command_ids_.push_back(custom_command_id);
  custom_command_names_.emplace_back();
  custom_command_types_.emplace_back();
  custom_command_pb_types_.emplace_back();
  custom_command_pb_type_offset_.emplace_back();
  custom_command_names_[custom_command_id] = command_name;
  custom_command_types_[custom_command_id] = command_type;
  custom_command_id_to_option_id_.emplace_back();

  return 0;
}

void PcfCustomCommand::set_custom_command_pb_type(
  const std::string& command_name, const std::string& pb_type) {
  auto command_id = find_command_id(command_name);
  custom_command_pb_types_[command_id] = pb_type;
}

void PcfCustomCommand::set_custom_command_pb_type_offset(
  const std::string& command_name, const int& offset) {
  auto command_id = find_command_id(command_name);
  custom_command_pb_type_offset_[command_id] = offset;
}

PcfCustomCommandId PcfCustomCommand::find_command_id(
  const std::string& command_name) const {
  for (auto it : custom_commands()) {
    if (custom_command_name(it) == command_name) {
      return it;
    }
  }
  VTR_LOG_ERROR("Command %s is not found", command_name.c_str());
  exit(1);
}
PcfCustomCommandOptionId PcfCustomCommand::find_option_id(
  const std::string& command_name, const std::string& option_name) const {
  auto command_id = find_command_id(command_name);
  for (auto it : command_options(command_id)) {
    if (custom_option_name(it) == option_name) {
      return it;
    }
  }
  VTR_LOG_ERROR("Option %s for command %s is not found", option_name.c_str(),
                command_name.c_str());
  exit(1);
}

PcfCustomCommandModeId PcfCustomCommand::find_mode_id(
  const std::string& command_name, const std::string& option_name,
  const std::string& mode_name) const {
  auto option_id = find_option_id(command_name, option_name);
  for (auto it : option_modes(option_id)) {
    if (custom_mode_name(it) == mode_name) {
      return it;
    }
  }
  VTR_LOG_ERROR("Mode %s of Option %s for command %s is not found",
                mode_name.c_str(), option_name.c_str(), command_name.c_str());
  exit(1);
}

int PcfCustomCommand::create_custom_option(const std::string& command_name,
                                           const std::string& option_name,
                                           const std::string& option_type) {
  auto command_id = find_command_id(command_name);

  PcfCustomCommandOptionId custom_option_id =
    PcfCustomCommandOptionId(custom_option_ids_.size());
  custom_command_id_to_option_id_[command_id].push_back(custom_option_id);
  custom_option_ids_.push_back(custom_option_id);
  custom_option_names_.emplace_back();
  custom_option_types_.emplace_back();
  custom_option_names_[custom_option_id] = option_name;
  custom_option_types_[custom_option_id] = option_type;
  custom_option_id_to_mode_id_.emplace_back();
  return 0;
}

int PcfCustomCommand::create_custom_mode(const std::string& command_name,
                                         const std::string& option_name,
                                         const std::string& mode_name,
                                         const std::string& mode_value) {
  auto option_id = find_option_id(command_name, option_name);
  PcfCustomCommandModeId custom_mode_id =
    PcfCustomCommandModeId(custom_mode_ids_.size());
  custom_option_id_to_mode_id_[option_id].push_back(custom_mode_id);
  custom_mode_ids_.push_back(custom_mode_id);
  custom_mode_names_.emplace_back();
  custom_mode_values_.emplace_back();
  custom_mode_names_[custom_mode_id] = mode_name;
  custom_mode_values_[custom_mode_id] = mode_value;
  return 0;
}

std::vector<PcfCustomCommandOptionId> PcfCustomCommand::command_options(
  const PcfCustomCommandId& command_id) const {
  return custom_command_id_to_option_id_[command_id];
}

std::vector<PcfCustomCommandModeId> PcfCustomCommand::option_modes(
  const PcfCustomCommandOptionId& option_id) const {
  return custom_option_id_to_mode_id_[option_id];
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
/* Validators */
bool PcfCustomCommand::valid_custom_command_id(
  const PcfCustomCommandId& custom_command_id) const {
  return (size_t(custom_command_id) < custom_command_ids_.size()) &&
         (custom_command_id == custom_command_ids_[custom_command_id]);
}
bool PcfCustomCommand::valid_custom_option_id(
  const PcfCustomCommandOptionId& custom_option_id) const {
  return (size_t(custom_option_id) < custom_option_ids_.size()) &&
         (custom_option_id == custom_option_ids_[custom_option_id]);
}
bool PcfCustomCommand::valid_custom_mode_id(
  const PcfCustomCommandModeId& custom_mode_id) const {
  return (size_t(custom_mode_id) < custom_mode_ids_.size()) &&
         (custom_mode_id == custom_mode_ids_[custom_mode_id]);
}

bool PcfCustomCommand::valid_command(const std::string command_name) const {
  bool valid_command = false;
  for (auto it : custom_commands()) {
    if (command_name.find(custom_command_name(it)) == 0) {
      valid_command = true;
      break;
    }
  }
  return valid_command;
}

bool PcfCustomCommand::valid_option(const std::string command_name,
                                    const std::string option_name) const {
  bool valid_option = false;
  auto command_id = find_command_id(command_name);
  for (auto it : command_options(command_id)) {
    if (option_name.find(custom_option_name(it)) == 0) {
      valid_option = true;
      break;
    }
  }
  return valid_option;
}

} /* End namespace openfpga*/
