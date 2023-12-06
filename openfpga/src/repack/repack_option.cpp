/******************************************************************************
 * Memember functions for data structure RepackOption
 ******************************************************************************/
#include "repack_option.h"

#include <array>
#include <map>

#include "openfpga_port_parser.h"
#include "openfpga_tokenizer.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
RepackOption::RepackOption() {
  verbose_output_ = false;
  num_parse_errors_ = 0;
}

/**************************************************
 * Public Accessors
 *************************************************/
RepackDesignConstraints RepackOption::design_constraints() const {
  return design_constraints_;
}

bool RepackOption::is_pin_ignore_global_nets(const std::string& pb_type_name,
                                             const BasicPort& pin) const {
  auto result = ignore_global_nets_on_pins_.find(pb_type_name);
  if (result == ignore_global_nets_on_pins_.end()) {
    /* Not found, return false */
    return false;
  } else {
    /* If the pin is contained by the ignore list, return true */
    for (BasicPort existing_port : result->second) {
      if (existing_port.mergeable(pin) && existing_port.contained(pin)) {
        return true;
      }
    }
  }
  return false;
}

bool RepackOption::net_is_specified_to_be_ignored(std::string cluster_net_name,
                                                  std::string pb_type_name,
                                                  const BasicPort& pin) const {
  const RepackDesignConstraints& design_constraint = design_constraints();
  /* If found a constraint, record the net name */
  for (const RepackDesignConstraintId id_ :
       design_constraint.design_constraints()) {
    if (design_constraint.type(id_) == RepackDesignConstraints::IGNORE_NET &&
        design_constraint.pb_type(id_) == pb_type_name &&
        design_constraint.net(id_) == cluster_net_name) {
      if (design_constraint.pin(id_).mergeable(pin) &&
          design_constraint.pin(id_).contained(pin))
        return true;
    }
  }
  return false;
}

bool RepackOption::verbose_output() const { return verbose_output_; }

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
void RepackOption::set_design_constraints(
  const RepackDesignConstraints& design_constraints) {
  design_constraints_ = design_constraints;
}

void RepackOption::set_ignore_global_nets_on_pins(const std::string& content) {
  num_parse_errors_ = 0;
  /* Split the content using a tokenizer */
  StringToken tokenizer(content);
  std::vector<std::string> tokens = tokenizer.split(',');

  /* Parse each token */
  for (std::string token : tokens) {
    /* Extract the pb_type name and port name */
    StringToken pin_tokenizer(token);
    std::vector<std::string> pin_info = pin_tokenizer.split('.');
    /* Expect two contents, otherwise error out */
    if (pin_info.size() != 2) {
      std::string err_msg =
        std::string("Invalid content '") + token +
        std::string("' to skip, expect <pb_type_name>.<pin>\n");
      VTR_LOG_ERROR(err_msg.c_str());
      num_parse_errors_++;
      continue;
    }
    std::string pb_type_name = pin_info[0];
    PortParser port_parser(pin_info[1]);
    BasicPort curr_port = port_parser.port();
    if (!curr_port.is_valid()) {
      std::string err_msg =
        std::string("Invalid pin definition '") + token +
        std::string("', expect <pb_type_name>.<pin_name>[int:int]\n");
      VTR_LOG_ERROR(err_msg.c_str());
      num_parse_errors_++;
      continue;
    }

    /* Check if the existing port already in the ignore list or not */
    auto result = ignore_global_nets_on_pins_.find(pb_type_name);
    if (result == ignore_global_nets_on_pins_.end()) {
      /* Not found, push the port */
      ignore_global_nets_on_pins_[pb_type_name].push_back(curr_port);
    } else {
      /* Already a list of ports. Check one by one.
       * - It already contained, do nothing but throw a warning.
       * - If we can merge, merge it.
       * - Otherwise, create it */
      bool included_by_existing_port = false;
      for (BasicPort existing_port : result->second) {
        if (existing_port.mergeable(curr_port)) {
          if (!existing_port.contained(curr_port)) {
            result->second.push_back(curr_port);
            included_by_existing_port = true;
            break;
          } else {
            std::string warn_msg =
              std::string("Pin definition '") + token +
              std::string("' is already included by other pin\n");
            VTR_LOG_WARN(warn_msg.c_str());
          }
        }
      }
      if (!included_by_existing_port) {
        result->second.push_back(curr_port);
      }
    }
  }
}

void RepackOption::set_verbose_output(const bool& enabled) {
  verbose_output_ = enabled;
}

bool RepackOption::valid() const {
  if (num_parse_errors_) {
    return false;
  }
  return true;
}

} /* end namespace openfpga */
