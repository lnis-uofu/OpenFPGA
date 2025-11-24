#ifndef PCF_CUSTOM_COMMAND_H
#define PCF_CUSTOM_COMMAND_H

/********************************************************************
 * This file include the declaration of pcf data
 *******************************************************************/
#include <array>
#include <map>
#include <string>

/* Headers from vtrutil library */
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "pcf_data_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A data structure to constain PCF data
 * This data structure may include a number of design constraints
 * - I/O constraint, for instance, force a net to be mapped to specific pin
 *
 * Typical usage:
 * --------------
 *   // Create an object
 *   PcfData pcf_data;
 *   // Add a constraint
 *   PcfIoConstraintId io_id = pcf_data.create_io_constraint();
 *   pcf_data.set_io_net(io_id, net_name);
 *   pcf_data.set_io_pin(io_id, pin_name);
 *
 *******************************************************************/
class PcfCustomCommand {
 public: /* Types */
  typedef vtr::vector<PcfCustomCommandId, PcfCustomCommandId>::const_iterator
    pcf_custom_command_iterator;
  /* Create range */
  typedef vtr::Range<pcf_custom_command_iterator> pcf_custom_command_range;

 public: /* Constructors */
  PcfCustomCommand();

 public: /* Accessors: aggregates */
  pcf_custom_command_range custom_commands() const;

  std::vector<PcfCustomCommandOptionId> command_options(
    const PcfCustomCommandId& command_id) const {
    return custom_command_id_to_option_id_[command_id];
  }

  std::vector<PcfCustomCommandModeId> option_modes(
    const PcfCustomCommandOptionId& option_id) const {
    return custom_option_id_to_mode_id_[option_id];
  }

 public: /* Public Accessors: Basic data query */
  /* Get the pin to be constrained */

  std::string custom_command_name(
    const PcfCustomCommandId& custom_command_id) const;

  std::string custom_command_type(
    const PcfCustomCommandId& custom_command_id) const;

  std::string custom_option_name(
    const PcfCustomCommandOptionId& custom_option_id) const;

  std::string custom_option_type(
    const PcfCustomCommandOptionId& custom_option_id) const;

  std::string custom_mode_name(const PcfCustomCommandModeId& custom_mode_id) const;

  std::string custom_mode_type(const PcfCustomCommandModeId& custom_mode_id) const;

  /* Check if there are any io constraints */
  bool empty() const;

 public: /* Public Mutators */
  /* Reserve a number of design constraints to be memory efficent */

  PcfCustomCommandId create_custom_command();
  PcfCustomCommandOptionId create_custom_option(const PcfCustomCommandId& command_id,
                                         const std::string& option_name, const std::string& option_type);
  PcfCustomCommandModeId create_custom_mode(const PcfCustomCommandOptionId& option_id,
                                     const std::string& mode_name,
                                     const std::string& mode_type);

  void set_custom_command_name(const PcfCustomCommandId& custom_command_id,
                               const std::string& value);

  void set_custom_command_type(const PcfCustomCommandId& custom_command_id,
                               const std::string& value);

 public: /* Public invalidators/validators */
  /* Show if the constraint id is a valid for data queries */
  bool valid_custom_command_id(
    const PcfCustomCommandId& custom_command_id) const;

  bool valid_custom_option_id(const PcfCustomCommandOptionId& custom_option_id) const;
  bool valid_custom_mode_id(const PcfCustomCommandModeId& custom_mode_id) const;

 private: /* Internal data */
  vtr::vector<PcfCustomCommandId, PcfCustomCommandId> custom_command_ids_;

  vtr::vector<PcfCustomCommandId, std::string> custom_command_names_;

  vtr::vector<PcfCustomCommandId, std::string> custom_command_types_;

  vtr::vector<PcfCustomCommandId, std::vector<PcfCustomCommandOptionId>>
    custom_command_id_to_option_id_;

  vtr::vector<PcfCustomCommandOptionId, std::vector<PcfCustomCommandModeId>>
    custom_option_id_to_mode_id_;

  vtr::vector<PcfCustomCommandOptionId, PcfCustomCommandOptionId>
    custom_option_ids_;

  vtr::vector<PcfCustomCommandOptionId, std::string> custom_option_names_;

  vtr::vector<PcfCustomCommandOptionId, std::string> custom_option_types_;

  vtr::vector<PcfCustomCommandModeId, PcfCustomCommandModeId> custom_mode_ids_;

  vtr::vector<PcfCustomCommandModeId, std::string> custom_mode_names_;

  vtr::vector<PcfCustomCommandModeId, std::string> custom_mode_types_;
};

} /* End namespace openfpga*/

#endif
