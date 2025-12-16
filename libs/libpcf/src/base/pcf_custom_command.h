#ifndef PCF_CUSTOM_COMMAND_H
#define PCF_CUSTOM_COMMAND_H

/********************************************************************
 * This file include the declaration of pcf custom command
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
 * A data structure to define PCF custom command
 * This data structure may include a number of custom command for design
 *constraints
 * - such as set_watch_dog
 *
 * Typical usage:
 * --------------
 *   // Create an object
 *   PcfCustomCommand pcf_custom_command;
 *   // Read command definition from a config.xml
 *  openfpga::read_pcf_conifg("pcf_config.xml", pcf_custom_command);
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

  /* Check if there are any io constraints */
  bool empty() const;

  bool valid_command(const std::string command_name) const;
  bool valid_option(const std::string command_name,
                    const std::string option_name) const;

 public: /* Public Mutators */
  /* Reserve a number of design constraints to be memory efficent */

  void set_custom_command_pb_type(const std::string& command_name,
                                  const std::string& pb_type);

  void set_custom_command_pb_type_offset(const std::string& command_name,
                                         const int& offset);

  int create_custom_command(const std::string& command_name,
                            const std::string& command_type);

  int create_custom_option(const std::string& command_name,
                           const std::string& option_name,
                           const std::string& option_type);
  int create_custom_mode(const std::string& command_name,
                         const std::string& option_name,
                         const std::string& mode_name,
                         const std::string& mode_value);

  std::string custom_mode_value(const std::string& command_name,
                                const std::string& option_name,
                                const std::string& mode_name) const;

  std::string custom_option_type(const std::string& command_name,
                                 const std::string& option_name) const;

  std::vector<PcfCustomCommandOptionId> command_options(
    const PcfCustomCommandId& command_id) const;

  std::vector<PcfCustomCommandModeId> option_modes(
    const PcfCustomCommandOptionId& option_id) const;

 private: /* Internal data */
  std::string custom_command_name(
    const PcfCustomCommandId& custom_command_id) const;

  std::string custom_command_type(
    const PcfCustomCommandId& custom_command_id) const;

  std::string custom_option_name(
    const PcfCustomCommandOptionId& custom_option_id) const;

  std::string custom_option_type(
    const PcfCustomCommandOptionId& custom_option_id) const;

  std::string custom_mode_name(
    const PcfCustomCommandModeId& custom_mode_id) const;

  std::string custom_mode_value(
    const PcfCustomCommandModeId& custom_mode_id) const;

  PcfCustomCommandId find_command_id(const std::string& command_name) const;
  PcfCustomCommandOptionId find_option_id(const std::string& command_name,
                                          const std::string& option_name) const;
  PcfCustomCommandModeId find_mode_id(const std::string& command_name,
                                      const std::string& option_name,
                                      const std::string& mode_name) const;

  bool valid_custom_command_id(
    const PcfCustomCommandId& custom_command_id) const;

  bool valid_custom_option_id(
    const PcfCustomCommandOptionId& custom_option_id) const;
  bool valid_custom_mode_id(const PcfCustomCommandModeId& custom_mode_id) const;

 private:
  vtr::vector<PcfCustomCommandId, PcfCustomCommandId> custom_command_ids_;

  vtr::vector<PcfCustomCommandId, std::string> custom_command_names_;

  vtr::vector<PcfCustomCommandId, std::string> custom_command_types_;

  vtr::vector<PcfCustomCommandId, std::string> custom_command_pb_types_;

  vtr::vector<PcfCustomCommandId, int> custom_command_pbtype_offset_;

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

  vtr::vector<PcfCustomCommandModeId, std::string> custom_mode_values_;
};

} /* End namespace openfpga*/

#endif
