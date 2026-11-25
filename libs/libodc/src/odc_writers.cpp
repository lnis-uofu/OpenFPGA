/********************************************************************
 * The files of the tools, made from the constraints: an SDC for VPR, the VPR
 * constraints XML, the pin constraints XML of OpenFPGA, the Yosys commands
 * of the synthesis, and ODC itself.
 *******************************************************************/
#include <sstream>

#include "odc.h"

/* begin namespace odc */
namespace odc {

namespace {

std::string xml(const std::string& text) {
  std::string out;
  for (const char c : text) {
    switch (c) {
      case '&':
        out += "&amp;";
        break;
      case '<':
        out += "&lt;";
        break;
      case '>':
        out += "&gt;";
        break;
      case '"':
        out += "&quot;";
        break;
      default:
        out += c;
    }
  }
  return out;
}

std::string where(const Command& command) {
  return command.file + ":" + std::to_string(command.line);
}

/* A name as Yosys selects it: its special characters escaped, the globs
 * kept */
std::string yosys_pattern(const std::string& pattern) {
  std::string out;
  for (const char c : pattern) {
    if (' ' == c || '\\' == c || '"' == c || ';' == c) {
      out += '\\';
    }
    out += c;
  }
  return out;
}

/* The selection of the objects of a synthesis command, with a prefix per
 * kind of object of Yosys (c: cells, w: wires, m: memories) */
std::string selection(const Command& command,
                      const std::vector<std::string>& prefixes) {
  const Query& query = command.words[command.objects.at(0)].query;
  std::string out;
  for (const std::string& pattern : query.patterns) {
    for (const std::string& prefix : prefixes) {
      out += " " + prefix + yosys_pattern(pattern);
    }
  }
  return out;
}

}  // namespace

std::string write_sdc(const Constraints& constraints) {
  std::ostringstream out;
  bool any = false;
  for (const Command& command : constraints.commands) {
    if (Kind::TIMING != command.kind) {
      continue;
    }
    if (!any) {
      out << "# The timing commands of ODC, for VPR\n";
      any = true;
    }
    out << "# " << where(command) << "\n" << command.text() << "\n";
  }
  return out.str();
}

std::string write_vpr_constraints(const Resolution& resolution) {
  std::ostringstream partitions;
  for (const Partition& partition : resolution.partitions) {
    if ((partition.atoms.empty() && partition.patterns.empty()) ||
        partition.regions.empty()) {
      continue;
    }
    partitions << "    <partition name=\"" << xml(partition.name) << "\">\n";
    for (const std::string& atom : partition.atoms) {
      partitions << "      <add_atom name_pattern=\"" << xml(atom) << "\"/>\n";
    }
    for (const std::string& pattern : partition.patterns) {
      partitions << "      <add_atom name_pattern=\"" << xml(pattern)
                 << "\" is_regex=\"true\"/>\n";
    }
    for (const Location& region : partition.regions) {
      partitions << "      <add_region x_low=\"" << region.x_low
                 << "\" y_low=\"" << region.y_low << "\" x_high=\""
                 << region.x_high << "\" y_high=\"" << region.y_high << "\"";
      if (region.subtile >= 0) {
        partitions << " subtile=\"" << region.subtile << "\"";
      }
      if (0 != region.layer) {
        partitions << " layer_low=\"" << region.layer << "\" layer_high=\""
                   << region.layer << "\"";
      }
      partitions << "/>\n";
    }
    partitions << "    </partition>\n";
  }
  std::ostringstream signals;
  for (const GlobalSignal& signal : resolution.global_signals) {
    std::vector<std::string> names = signal.nets;
    names.insert(names.end(), signal.patterns.begin(), signal.patterns.end());
    for (const std::string& name : names) {
      signals << "    <set_global_signal name=\"" << xml(name)
              << "\" route_model=\"" << signal.route_model << "\"";
      if (!signal.network.empty()) {
        signals << " network_name=\"" << xml(signal.network) << "\"";
      }
      signals << "/>\n";
    }
  }
  if (partitions.str().empty() && signals.str().empty()) {
    return std::string();
  }
  std::ostringstream out;
  out << "<!-- The floorplanning, the pins and the global routing of ODC -->\n"
      << "<vpr_constraints tool_name=\"openfpga\">\n";
  if (!partitions.str().empty()) {
    out << "  <partition_list>\n"
        << partitions.str() << "  </partition_list>\n";
  }
  if (!signals.str().empty()) {
    out << "  <global_route_constraints>\n"
        << signals.str() << "  </global_route_constraints>\n";
  }
  out << "</vpr_constraints>\n";
  return out.str();
}

std::string write_pin_constraints(const Resolution& resolution) {
  std::ostringstream pins;
  for (const PinAssignment& pin : resolution.pins) {
    if (pin.site.has_location) {
      continue;
    }
    pins << "  <set_io pin=\"" << xml(pin.site.netlist_name) << "\" net=\""
         << xml(pin.port) << "\"";
    if (!pin.default_value.empty()) {
      pins << " default_value=\"" << pin.default_value << "\"";
    }
    pins << "/>\n";
  }
  if (pins.str().empty()) {
    return std::string();
  }
  return "<!-- The pins of ODC without a location of the grid -->\n"
         "<pin_constraints>\n" +
         pins.str() + "</pin_constraints>\n";
}

std::string write_yosys(const Constraints& constraints) {
  std::ostringstream out;
  for (const Command& command : constraints.commands) {
    if (Kind::SYNTHESIS != command.kind) {
      continue;
    }
    std::string line;
    if ("set_property" == command.name) {
      const bool on = "true" == command.value || "1" == command.value ||
                      "TRUE" == command.value || "True" == command.value;
      if ("KEEP" == command.property) {
        if (!on) {
          continue;
        }
        const bool nets =
          "nets" == command.words[command.objects[0]].query.kind;
        line = "setattr -set keep 1" +
               selection(command, nets ? std::vector<std::string>{"w:"}
                                       : std::vector<std::string>{"c:", "w:"});
      } else if ("DONT_TOUCH" == command.property) {
        if (!on) {
          continue;
        }
        line = "setattr -set keep 1" + selection(command, {"c:", "w:"}) +
               "\nsetattr -set keep_hierarchy 1" + selection(command, {"c:"});
      } else if ("MAX_FANOUT" == command.property) {
        line = "setattr -set max_fanout " + command.value +
               selection(command, {"w:"});
      } else if ("RAM_STYLE" == command.property) {
        line = "setattr -set ram_style \"" + command.value + "\"" +
               selection(command, {"m:", "c:"});
      }
    } else if ("set_synth_option" == command.name) {
      if ("flatten" == command.property) {
        line = "flatten";
      } else if ("keep_hierarchy" == command.property) {
        line = "setattr -mod -set keep_hierarchy 1 *";
      } else {
        continue; /* -top and -define change the plan, not the script */
      }
    }
    if (!line.empty()) {
      out << "# " << where(command) << "\n" << line << "\n";
    }
  }
  return out.str();
}

SynthesisOptions synthesis_options(const Constraints& constraints) {
  SynthesisOptions options;
  for (const Command& command : constraints.commands) {
    if ("set_synth_option" != command.name) {
      continue;
    }
    if ("top" == command.property) {
      options.top = command.value;
    } else if ("define" == command.property) {
      const size_t equal = command.value.find('=');
      if (std::string::npos == equal) {
        options.defines.emplace_back(command.value, "");
      } else {
        options.defines.emplace_back(command.value.substr(0, equal),
                                     command.value.substr(equal + 1));
      }
    }
  }
  return options;
}

std::string write_odc(const Constraints& constraints) {
  std::ostringstream out;
  out << "odc_version " << ODC_VERSION << "\n";
  for (const Command& command : constraints.commands) {
    if (!command.from_defaults) {
      out << command.text() << "\n";
    }
  }
  return out.str();
}

} /* end namespace odc */
