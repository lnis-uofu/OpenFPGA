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

std::string PcfCustomCommand::custom_mode_name(
  const PcfCustomCommandModeId& custom_mode_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_mode_id(custom_mode_id));
  return custom_mode_names_[custom_mode_id];
}

std::string PcfCustomCommand::custom_mode_type(
  const PcfCustomCommandModeId& custom_mode_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_mode_id(custom_mode_id));
  return custom_mode_types_[custom_mode_id];
}

bool PcfCustomCommand::empty() const { return 0 == custom_command_ids_.size(); }

/************************************************************************
 * Public Mutators
 ***********************************************************************/

PcfCustomCommandId PcfCustomCommand::create_custom_command() {
  /* Create a new id */
  PcfCustomCommandId custom_command_id =
    PcfCustomCommandId(custom_command_ids_.size());

  custom_command_ids_.push_back(custom_command_id);
  custom_command_names_.emplace_back();
  custom_command_types_.emplace_back();
  custom_command_id_to_option_id_.emplace_back();

  return custom_command_id;
}

void PcfCustomCommand::set_custom_command_name(
  const PcfCustomCommandId& custom_command_id, const std::string& value) {
  VTR_ASSERT(valid_custom_command_id(custom_command_id));
  custom_command_names_[custom_command_id] = value;
}

void PcfCustomCommand::set_custom_command_type(
  const PcfCustomCommandId& custom_command_id, const std::string& value) {
  VTR_ASSERT(valid_custom_command_id(custom_command_id));
  custom_command_types_[custom_command_id] = value;
}

PcfCustomCommandOptionId PcfCustomCommand::create_custom_option(
  const PcfCustomCommandId& command_id, const std::string& option_name,
  const std::string& option_type) {
  PcfCustomCommandOptionId custom_option_id =
    PcfCustomCommandOptionId(custom_option_ids_.size());
  custom_command_id_to_option_id_[command_id].push_back(custom_option_id);
  custom_option_ids_.push_back(custom_option_id);
  custom_option_names_[custom_option_id] = option_name;
  custom_option_types_[custom_option_id] = option_type;
  custom_option_id_to_mode_id_.emplace_back();
  return custom_option_id;
}

PcfCustomCommandModeId PcfCustomCommand::create_custom_mode(
  const PcfCustomCommandOptionId& option_id, const std::string& mode_name,
  const std::string& mode_type) {
  PcfCustomCommandModeId custom_mode_id = PcfCustomCommandModeId(custom_mode_ids_.size());
  custom_option_id_to_mode_id_[option_id].push_back(custom_mode_id);
  custom_mode_ids_.push_back(custom_mode_id);
  custom_mode_names_[custom_mode_id] = mode_name;
  custom_mode_types_[custom_mode_id] = mode_type;
  return custom_mode_id;
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
} /* End namespace openfpga*/
