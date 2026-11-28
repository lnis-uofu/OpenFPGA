/********************************************************************
 * The default values of the application options.
 *
 * The options of VPR are described by the table generated from its sources
 * (vpr_option_table.h). Their default value is the one VPR has when it is
 * given nothing but an architecture and a circuit, so it is always the one
 * of the version of VPR in use. Only the options of OpenFPGA are given here.
 *******************************************************************/
#include <stdexcept>

#include "app_options.h"
#include "vpr_app_options.h"
#include "vpr_option_table.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* The option, with the value that VPR has by default */
AppOptionValue make_vpr_default(const VprOptionSpec& spec,
                                const t_options& vpr) {
  const std::string help = spec.help;
  AppOptionValue value;
  switch (spec.type) {
    case AppOptionValue::BOOLEAN:
      value = AppOptionValue::make_boolean(false, help);
      break;
    case AppOptionValue::INT:
      value = AppOptionValue::make_int(0, help);
      break;
    case AppOptionValue::FLOAT:
      value = AppOptionValue::make_float(0., help);
      break;
    case AppOptionValue::SELECTION:
      value = AppOptionValue::make_selection("", *spec.choices, help);
      break;
    default:
      value = AppOptionValue::make_string("", help);
      break;
  }
  /* VPR names these after the circuit when it is not told: keep them empty */
  if (spec.derived_default) {
    return value;
  }

  spec.get(vpr, value);

  /* A selection is known by the text that VPR accepts for the enumeration */
  if (AppOptionValue::SELECTION == spec.type) {
    bool found = false;
    for (const auto& choice : *spec.choices) {
      if (choice.second == value.int_value) {
        value.string_value = choice.first;
        found = true;
        break;
      }
    }
    if (!found) {
      throw std::runtime_error(std::string("The default of the option ") +
                               spec.group + "." + spec.name +
                               " is not one of the values it accepts");
    }
  }
  return value;
}

}  // End anonymous namespace

namespace {

/* The defaults, which the options of the plugins are added to */
std::map<std::string, AppOptionValue>& defaults_table() {
  static std::map<std::string, AppOptionValue> defaults = [] {
    std::map<std::string, AppOptionValue> table;

    /* The options of VPR, with the defaults of VPR */
    const t_options vpr = read_vpr_options({"vpr", "arch.xml", "circuit"});
    for (const VprOptionSpec& spec : vpr_option_specs()) {
      table[std::string(spec.group) + "." + spec.name] =
        make_vpr_default(spec, vpr);
    }

    /* The options of OpenFPGA, which VPR does not have */
    table["place.placer"] = AppOptionValue::make_selection(
      "classic", {{"classic", 0}, {"analytical", 1}},
      "The placer of the flow that is run stage by stage. classic: pack, then "
      "place. analytical: the analytical placement of VPR, which packs and "
      "places at once ('vpr --analytical_place'): pack and place both run it, "
      "and reach PACKED and PLACED together. The options aplace.* tune it");
    table["build_fabric.reorder_incoming_edges"] = AppOptionValue::make_boolean(
      true, "Reorder the incoming edges of the routing nodes");
    table["build_fabric.sort_gsb_chan_node_in_edges"] =
      AppOptionValue::make_boolean(
        true, "Sort the incoming edges of the channel nodes of a GSB");
    return table;
  }();
  return defaults;
}

}  // End anonymous namespace

const std::map<std::string, AppOptionValue>& app_option_defaults() {
  return defaults_table();
}

void add_app_option_default(const std::string& name,
                            const AppOptionValue& value) {
  defaults_table()[name] = value;
}

} /* End namespace openfpga */
