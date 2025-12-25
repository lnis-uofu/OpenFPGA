#include "pcf_custom_command.h"

#include <algorithm>
#include <set>

#include "command_exit_codes.h"
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
  /* validate the command_id */
  VTR_ASSERT(valid_custom_command_id(custom_command_id));
  return custom_command_names_[custom_command_id];
}

std::string PcfCustomCommand::custom_command_pb_type(
  const std::string& custom_command_name) const {
  /* validate the command_id */
  auto custom_command_id = find_command_id(custom_command_name);
  return custom_command_pb_types_[custom_command_id];
}

std::string PcfCustomCommand::custom_command_type(
  const PcfCustomCommandId& custom_command_id) const {
  /* validate the command_id */
  VTR_ASSERT(valid_custom_command_id(custom_command_id));
  return custom_command_types_[custom_command_id];
}

std::string PcfCustomCommand::custom_option_name(
  const PcfCustomCommandOptionId& custom_option_id) const {
  /* validate the option_id */
  VTR_ASSERT(valid_custom_option_id(custom_option_id));
  return custom_option_names_[custom_option_id];
}

std::string PcfCustomCommand::custom_option_type(
  const PcfCustomCommandOptionId& custom_option_id) const {
  /* validate the option_id */
  VTR_ASSERT(valid_custom_option_id(custom_option_id));
  return custom_option_types_[custom_option_id];
}

std::string PcfCustomCommand::custom_option_type(
  const std::string& command_name, const std::string& option_name) const {
  /* validate the option_id */
  auto custom_option_id = find_option_id(command_name, option_name);
  return custom_option_type(custom_option_id);
}

int PcfCustomCommand::custom_mode_offset(
  const PcfCustomCommandModeId& custom_mode_id) const {
  /* validate the option_id */
  VTR_ASSERT(valid_custom_mode_id(custom_mode_id));
  return custom_mode_offset_[custom_mode_id];
}

int PcfCustomCommand::custom_mode_offset(const std::string& command_name,
                                         const std::string& option_name,
                                         const std::string& mode_name) const {
  /* validate the mode_id */
  auto custom_mode_id = find_mode_id(command_name, option_name, mode_name);
  return custom_mode_offset(custom_mode_id);
}

std::string PcfCustomCommand::custom_mode_name(
  const PcfCustomCommandModeId& custom_mode_id) const {
  /* validate the mode_id */
  VTR_ASSERT(valid_custom_mode_id(custom_mode_id));
  return custom_mode_names_[custom_mode_id];
}

std::string PcfCustomCommand::custom_mode_value(
  const PcfCustomCommandModeId& custom_mode_id) const {
  /* validate the mode_id */
  VTR_ASSERT(valid_custom_mode_id(custom_mode_id));
  return custom_mode_values_[custom_mode_id];
}

std::string PcfCustomCommand::custom_mode_value(
  const std::string& command_name, const std::string& option_name,
  const std::string& mode_name) const {
  /* validate the mode_id */
  auto custom_mode_id = find_mode_id(command_name, option_name, mode_name);
  return custom_mode_value(custom_mode_id);
}

int PcfCustomCommand::custom_decimal_mode_offset(
  const std::string& command_name, const std::string& option_name) const {
  /* validate the mode_id */
  auto custom_decimal_mode_id = find_decimal_mode_id(command_name, option_name);
  return custom_decimal_mode_offset(custom_decimal_mode_id);
}

int PcfCustomCommand::custom_decimal_mode_offset(
  const PcfCustomCommandModeId& custom_decimal_mode_id) const {
  /* validate the mode_id */
  if (!valid_custom_decimal_mode_id(custom_decimal_mode_id)) {
    VTR_LOG_ERROR("Invalid decimal mode found! \n");
    exit(1);
  }
  return custom_decimal_mode_offset_[custom_decimal_mode_id];
}

int PcfCustomCommand::custom_decimal_mode_num_bits(
  const std::string& command_name, const std::string& option_name) const {
  /* validate the mode_id */
  auto custom_decimal_mode_id = find_decimal_mode_id(command_name, option_name);
  return custom_decimal_mode_num_bits(custom_decimal_mode_id);
}

int PcfCustomCommand::custom_decimal_mode_num_bits(
  const PcfCustomCommandModeId& custom_decimal_mode_id) const {
  /* validate the mode_id */
  if (!valid_custom_decimal_mode_id(custom_decimal_mode_id)) {
    VTR_LOG_ERROR("Invalid decimal mode found! \n");
    exit(1);
  }
  return custom_decimal_mode_num_bits_[custom_decimal_mode_id];
}

int PcfCustomCommand::custom_decimal_mode_max_val(
  const std::string& command_name, const std::string& option_name) const {
  /* validate the mode_id */
  auto custom_decimal_mode_id = find_decimal_mode_id(command_name, option_name);
  return custom_decimal_mode_max_val(custom_decimal_mode_id);
}

int PcfCustomCommand::custom_decimal_mode_max_val(
  const PcfCustomCommandModeId& custom_decimal_mode_id) const {
  /* validate the mode_id */
  if (!valid_custom_decimal_mode_id(custom_decimal_mode_id)) {
    VTR_LOG_ERROR("Invalid decimal mode found! \n");
    exit(1);
  }
  return custom_decimal_mode_max_values_[custom_decimal_mode_id];
}

bool PcfCustomCommand::custom_decimal_mode_little_endian(
  const std::string& command_name, const std::string& option_name) const {
  /* validate the mode_id */
  auto custom_decimal_mode_id = find_decimal_mode_id(command_name, option_name);
  return custom_decimal_mode_little_endian(custom_decimal_mode_id);
}

bool PcfCustomCommand::custom_decimal_mode_little_endian(
  const PcfCustomCommandModeId& custom_decimal_mode_id) const {
  /* validate the mode_id */
  if (!valid_custom_decimal_mode_id(custom_decimal_mode_id)) {
    VTR_LOG_ERROR("Invalid decimal mode found! \n");
    exit(1);
  }
  return custom_decimal_mode_little_endian_[custom_decimal_mode_id];
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
  custom_command_names_.emplace_back(command_name);
  custom_command_types_.emplace_back(command_type);
  custom_command_pb_types_.emplace_back();
  custom_command_id_to_option_id_.emplace_back();

  return CMD_EXEC_SUCCESS;
}

void PcfCustomCommand::set_custom_command_pb_type(
  const std::string& command_name, const std::string& pb_type) {
  auto command_id = find_command_id(command_name);
  custom_command_pb_types_[command_id] = pb_type;
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

PcfCustomCommandModeId PcfCustomCommand::find_decimal_mode_id(
  const std::string& command_name, const std::string& option_name) const {
  auto option_id = find_option_id(command_name, option_name);
  return option_decimal_modes(option_id);
}

int PcfCustomCommand::create_custom_option(const std::string& command_name,
                                           const std::string& option_name,
                                           const std::string& option_type) {
  auto command_id = find_command_id(command_name);

  PcfCustomCommandOptionId custom_option_id =
    PcfCustomCommandOptionId(custom_option_ids_.size());
  custom_command_id_to_option_id_[command_id].push_back(custom_option_id);
  custom_option_ids_.push_back(custom_option_id);
  custom_option_names_.emplace_back(option_name);
  custom_option_types_.emplace_back(option_type);
  custom_option_id_to_mode_id_.emplace_back();
  custom_option_id_to_decimal_mode_id_.emplace_back();
  return CMD_EXEC_SUCCESS;
}

int PcfCustomCommand::create_custom_mode(const std::string& command_name,
                                         const std::string& option_name,
                                         const std::string& mode_name,
                                         const std::string& mode_value,
                                         const int& mode_offset) {
  auto option_id = find_option_id(command_name, option_name);
  PcfCustomCommandModeId custom_mode_id =
    PcfCustomCommandModeId(custom_mode_ids_.size());
  custom_option_id_to_mode_id_[option_id].push_back(custom_mode_id);
  custom_mode_ids_.push_back(custom_mode_id);
  custom_mode_names_.emplace_back(mode_name);
  custom_mode_values_.emplace_back(mode_value);
  custom_mode_offset_.emplace_back(mode_offset);
  return CMD_EXEC_SUCCESS;
}

int PcfCustomCommand::create_custom_decimal_mode(
  const std::string& command_name, const std::string& option_name,
  const int& num_bits, const int& max_val, const bool& little_endian,
  const int& mode_offset) {
  auto option_id = find_option_id(command_name, option_name);
  PcfCustomCommandModeId custom_decimal_mode_id =
    PcfCustomCommandModeId(custom_decimal_mode_ids_.size());
  custom_option_id_to_decimal_mode_id_[option_id] = custom_decimal_mode_id;
  custom_decimal_mode_ids_.push_back(custom_decimal_mode_id);
  custom_decimal_mode_num_bits_.emplace_back(num_bits);
  custom_decimal_mode_max_values_.emplace_back(max_val);
  custom_decimal_mode_little_endian_.emplace_back(little_endian);
  custom_decimal_mode_offset_.emplace_back(mode_offset);
  return CMD_EXEC_SUCCESS;
}

std::vector<PcfCustomCommandOptionId> PcfCustomCommand::command_options(
  const PcfCustomCommandId& command_id) const {
  if (!valid_custom_command_id(command_id)) {
    VTR_LOG_ERROR("Invalid command found. Please check!\n");
    exit(1);
  }
  return custom_command_id_to_option_id_[command_id];
}

std::vector<PcfCustomCommandModeId> PcfCustomCommand::option_modes(
  const PcfCustomCommandOptionId& option_id) const {
  if (!valid_custom_option_id(option_id)) {
    VTR_LOG_ERROR("Invalid option found. Please check!\n");
    exit(1);
  }
  return custom_option_id_to_mode_id_[option_id];
}

PcfCustomCommandModeId PcfCustomCommand::option_decimal_modes(
  const PcfCustomCommandOptionId& option_id) const {
  if (!valid_custom_option_id(option_id)) {
    VTR_LOG_ERROR("Invalid option found. Please check!\n");
    exit(1);
  }
  return custom_option_id_to_decimal_mode_id_[option_id];
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
bool PcfCustomCommand::valid_custom_decimal_mode_id(
  const PcfCustomCommandModeId& custom_decimal_mode_id) const {
  return (size_t(custom_decimal_mode_id) < custom_decimal_mode_ids_.size()) &&
         (custom_decimal_mode_id ==
          custom_decimal_mode_ids_[custom_decimal_mode_id]);
}

bool PcfCustomCommand::valid_command(const std::string& command_name) const {
  bool valid_command = false;
  for (auto it : custom_commands()) {
    if (command_name.find(custom_command_name(it)) == 0) {
      valid_command = true;
      break;
    }
  }
  return valid_command;
}

bool PcfCustomCommand::valid_option(const std::string& command_name,
                                    const std::string& option_name) const {
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

bool PcfCustomCommand::command_mode_offset_conflict_check(
  const std::string& command_name) const {
  if (!valid_command(command_name)) {
    VTR_LOG_ERROR("Command %s is invalid! \n", command_name.c_str());
  }
  auto command_id = find_command_id(command_name);
  std::vector<int> valid_bit_index;
  /* record the bit position of all modes of a command. If they overlap with
   * each other, then there is offset conflict*/
  for (auto option_id : command_options(command_id)) {
    std::string option_type = custom_option_type(option_id);
    if (option_type == OPTION_TYPE_MODE) {
      auto mode_id_vec = option_modes(option_id);
      auto mode_id = mode_id_vec[0]; /*all modes in a single option has the same
                          bit size and offset. therefore we just take out the
                          first mode to do the conflict check*/
      int mode_offset = custom_mode_offset(mode_id);
      int mode_bit_size = custom_mode_value(mode_id).size();
      for (int i = mode_offset; i < mode_offset + mode_bit_size; i++) {
        if (std::find(valid_bit_index.begin(), valid_bit_index.end(), i) !=
            valid_bit_index
              .end()) { /*find duplicate bit position index, there is conflict*/
          return true;
        }
        valid_bit_index.push_back(i);
      }
    } else if (option_type == OPTION_TYPE_DECIMAL) {
      auto decimal_mode_id = option_decimal_modes(option_id);
      int decimal_mode_offset = custom_decimal_mode_offset(decimal_mode_id);
      int decimal_mode_bit_size = custom_decimal_mode_num_bits(decimal_mode_id);
      for (int i = decimal_mode_offset;
           i < decimal_mode_offset + decimal_mode_bit_size; i++) {
        if (std::find(valid_bit_index.begin(), valid_bit_index.end(), i) !=
            valid_bit_index
              .end()) { /*find duplicate bit position index, there is conflict*/
          return true;
        }
        valid_bit_index.push_back(i);
      }
    }
  }
  return false;
}

} /* End namespace openfpga*/
