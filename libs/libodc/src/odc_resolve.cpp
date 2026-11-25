/********************************************************************
 * Resolving the constraints against a design (R-40 step 2): the objects that
 * each query matches, and the checks that VPR does not make or does not
 * explain (R-7, R-21, R-26, R-27, R-29, R-33).
 *******************************************************************/
#include <algorithm>
#include <map>
#include <set>

#include "odc.h"

/* begin namespace odc */
namespace odc {

namespace {

std::string join(const std::vector<std::string>& names,
                 const size_t limit = 5) {
  std::string out;
  for (size_t i = 0; i < names.size() && i < limit; ++i) {
    out += (out.empty() ? "" : ", ") + names[i];
  }
  if (names.size() > limit) {
    out += ", ...";
  }
  return out;
}

/* A pattern of a query as an anchored regular expression for VPR */
std::string vpr_pattern(const Query& query, const std::string& pattern) {
  if (query.regexp) {
    return "^(?:" + pattern + ")$";
  }
  return glob_regex(pattern, query.hierarchical);
}

class Resolver {
 public:
  Resolver(const Constraints& constraints, const Design& design)
    : constraints_(constraints), design_(design) {
    result_.checked.assign(constraints.commands.size(), true);
    if (design.sites) {
      for (const Site& site : *design.sites) {
        sites_.push_back({site.name(),
                          {{"side", site.side},
                           {"pin_direction", site.pin_direction},
                           {"pin_type", site.pin_type},
                           {"netlist_name", site.netlist_name},
                           {"customer_pin_alias", site.customer_pin_alias}}});
      }
    }
  }

  Resolution run() {
    collect_names();
    for (size_t i = 0; i < constraints_.commands.size(); ++i) {
      match_command(i);
    }
    floorplan();
    routing();
    pins();
    coverage();
    return result_;
  }

 private:
  const Command& command(const size_t index) const {
    return constraints_.commands[index];
  }

  void error(const size_t index, const std::string& message) {
    const Command& cmd = command(index);
    result_.diagnostics.push_back(
      {Diagnostic::ERROR, cmd.file, cmd.line, message});
  }

  void warning(const size_t index, const std::string& message) {
    const Command& cmd = command(index);
    result_.diagnostics.push_back(
      {Diagnostic::WARNING, cmd.file, cmd.line, message});
  }

  /* The clocks and the pblocks that the file makes */
  void collect_names() {
    for (const Command& cmd : constraints_.commands) {
      if ("create_pblock" == cmd.name) {
        pblock_objects_.push_back({cmd.pblock, {}});
      }
    }
  }

  const std::vector<Object>* objects(const std::string& kind) const {
    const Netlist* netlist = design_.netlist;
    if ("clocks" == kind) {
      return &clock_objects_;
    }
    if ("pblocks" == kind) {
      return &pblock_objects_;
    }
    if ("sites" == kind) {
      return design_.sites ? &sites_ : nullptr;
    }
    if (!netlist) {
      return nullptr;
    }
    if ("ports" == kind) {
      return &netlist->ports;
    }
    if ("cells" == kind) {
      return &netlist->cells;
    }
    if ("nets" == kind) {
      return &netlist->nets;
    }
    return &netlist->pins;
  }

  void match_command(const size_t index) {
    const Command& cmd = command(index);
    for (const size_t word : cmd.objects) {
      const Query& query = cmd.words[word].query;
      QueryMatch match;
      match.command = index;
      match.word = word;
      match.text = "[" + query.text + "]";
      match.kind = query.kind;
      /* The names of the RTL are matched by Yosys (R-23) */
      const std::vector<Object>* candidates =
        Kind::SYNTHESIS == cmd.kind ? nullptr : objects(query.kind);
      if (nullptr == candidates) {
        result_.checked[index] = false;
        matches_[{index, word}] = result_.queries.size();
        result_.queries.push_back(match);
        continue;
      }
      std::vector<const Object*> found;
      std::string problem;
      if (!match_query(query, *candidates, found, problem)) {
        error(index, problem);
      }
      match.checked = true;
      for (const Object* object : found) {
        match.matched.push_back(object->name);
      }
      const std::string object_kind =
        query.kind.substr(0, query.kind.size() - 1);
      if (found.empty()) {
        match.nearest = nearest_names(query.patterns.at(0), *candidates);
        warning(index, "no " + object_kind + " matches " + match.text +
                         (match.nearest.empty()
                            ? std::string()
                            : "; the nearest names: " + join(match.nearest)));
      } else if (query.patterns.size() > 1) {
        /* An object of a list that matches nothing: VPR drops it without a
         * word (R-21) */
        for (const std::string& pattern : query.patterns) {
          Query one = query;
          one.patterns = {pattern};
          std::vector<const Object*> alone;
          std::string ignored;
          match_query(one, *candidates, alone, ignored);
          if (alone.empty()) {
            const std::vector<std::string> nearest =
              nearest_names(pattern, *candidates);
            warning(index, "'" + pattern + "' of " + match.text +
                             " matches no " + object_kind +
                             (nearest.empty()
                                ? std::string()
                                : "; the nearest names: " + join(nearest)));
          }
        }
      }
      matches_[{index, word}] = result_.queries.size();
      result_.queries.push_back(match);
      if ("create_clock" == cmd.name) {
        add_clock(index, match.matched);
      }
    }
    if ("create_clock" == cmd.name && cmd.objects.empty()) {
      add_clock(index, {});
    }
  }

  void add_clock(const size_t index, const std::vector<std::string>& targets) {
    const Command& cmd = command(index);
    Clock clock;
    clock.command = index;
    clock.targets = targets;
    for (size_t i = 1; i + 1 < cmd.words.size(); ++i) {
      if ("-name" == cmd.words[i].text) {
        clock.name = cmd.words[i + 1].text;
      } else if ("-period" == cmd.words[i].text) {
        clock.period = cmd.words[i + 1].text;
      }
    }
    if (clock.name.empty() && !targets.empty()) {
      clock.name = targets[0];
    }
    if (!clock.name.empty()) {
      clock_objects_.push_back({clock.name, {}});
    }
    result_.timing.clocks.push_back(clock);
  }

  /* The match of the single object argument of a command */
  const QueryMatch& the_match(const size_t index) const {
    return result_.queries[matches_.at({index, command(index).objects.at(0)})];
  }

  /* The device checks ---------------------------------------------------*/

  /* A location inside the grid, at the root of its tile, with its subtile.
   * Return the tile, or null with the error */
  const Tile* check_location(const size_t index, const Location& location) {
    const Device* device = design_.device;
    if (!device) {
      result_.checked[index] = false;
      return nullptr;
    }
    if (location.x_high >= device->width || location.y_high >= device->height) {
      error(index, "the location " + location.text +
                     " is outside the grid of the device (" +
                     std::to_string(device->width) + "x" +
                     std::to_string(device->height) + ")");
      return nullptr;
    }
    if (location.layer >= device->layers) {
      error(index, "the device has no layer " + std::to_string(location.layer));
      return nullptr;
    }
    if (!location.is_tile()) {
      return nullptr;
    }
    const Tile* tile = device->tile(location.x_low, location.y_low);
    if (!tile || tile->type.empty()) {
      error(index, "there is no tile at " + location.text);
      return nullptr;
    }
    if (tile->root_x != location.x_low || tile->root_y != location.y_low) {
      error(index, location.text + " is inside the tile " + tile->type +
                     " at X" + std::to_string(tile->root_x) + "Y" +
                     std::to_string(tile->root_y) +
                     ": a location names the tile where it starts");
      return nullptr;
    }
    if (location.subtile >= tile->capacity) {
      error(index, "the tile " + tile->type + " at " + location.text + " has " +
                     std::to_string(tile->capacity) + " subtiles (0 to " +
                     std::to_string(tile->capacity - 1) + ")");
      return nullptr;
    }
    return tile;
  }

  int holds(const std::string& type, const std::string& model) const {
    const auto found = design_.device->type_models.find(type);
    if (design_.device->type_models.end() == found) {
      return 0;
    }
    const auto count = found->second.find(model);
    return found->second.end() == count ? 0 : count->second;
  }

  std::string cell_model(const std::string& name) const {
    for (const Object& cell : design_.netlist->cells) {
      if (name == cell.name) {
        return cell.properties.at("type");
      }
    }
    return std::string();
  }

  /* Floorplanning (R-25 to R-27) --------------------------------------*/

  void floorplan() {
    std::map<std::string, size_t> pblocks; /* name -> partition */
    std::map<std::string, std::string> cell_pblock;
    std::map<std::string, size_t> cell_loc; /* cell -> partition */
    for (size_t index = 0; index < constraints_.commands.size(); ++index) {
      const Command& cmd = command(index);
      if (Kind::FLOORPLAN != cmd.kind) {
        continue;
      }
      if ("create_pblock" == cmd.name) {
        pblocks[cmd.pblock] = result_.partitions.size();
        Partition partition;
        partition.name = "pblock_" + cmd.pblock;
        partition.command = index;
        result_.partitions.push_back(partition);
      } else if ("resize_pblock" == cmd.name) {
        check_location(index, cmd.location);
        result_.partitions[pblocks.at(cmd.pblock)].regions.push_back(
          cmd.location);
      } else if ("add_cells_to_pblock" == cmd.name) {
        Partition& partition = result_.partitions[pblocks.at(cmd.pblock)];
        const QueryMatch& match = the_match(index);
        if (!match.checked) {
          for (const std::string& pattern : object_query(cmd).patterns) {
            partition.patterns.push_back(
              vpr_pattern(object_query(cmd), pattern));
          }
          continue;
        }
        for (const std::string& cell : match.matched) {
          const auto [found, added] = cell_pblock.emplace(cell, cmd.pblock);
          if (!added && found->second != cmd.pblock) {
            error(index, "the cell '" + cell + "' is in the pblocks '" +
                           found->second + "' and '" + cmd.pblock +
                           "': a cell is in one pblock at most");
            continue;
          }
          if (added) {
            partition.atoms.push_back(cell);
          }
        }
      } else if ("set_property" == cmd.name && "LOC" == cmd.property) {
        const Tile* tile = check_location(index, cmd.location);
        const QueryMatch& match = the_match(index);
        if (!match.checked) {
          Partition partition;
          partition.name = "loc_" + std::to_string(cmd.line);
          partition.command = index;
          for (const std::string& pattern : object_query(cmd).patterns) {
            partition.patterns.push_back(
              vpr_pattern(object_query(cmd), pattern));
          }
          partition.regions.push_back(cmd.location);
          result_.partitions.push_back(partition);
          continue;
        }
        if (match.matched.size() > 1 && cmd.location.subtile >= 0) {
          error(index, "LOC " + cmd.location.text + " takes one cell, not " +
                         std::to_string(match.matched.size()));
          continue;
        }
        for (const std::string& cell : match.matched) {
          if (tile && design_.netlist) {
            const std::string model = cell_model(cell);
            if (0 == holds(tile->type, model)) {
              error(index, "the tile at " + cmd.location.text +
                             " is of the type " + tile->type +
                             ", which cannot hold the cell '" + cell + "' (" +
                             model + ")");
              continue;
            }
          }
          const auto found = cell_loc.find(cell);
          if (cell_loc.end() != found) {
            Partition& previous = result_.partitions[found->second];
            if (command(previous.command).from_defaults && !cmd.from_defaults) {
              previous.regions = {cmd.location};
              previous.command = index;
              continue;
            }
            error(index, "the cell '" + cell + "' has a LOC already, at line " +
                           std::to_string(command(previous.command).line));
            continue;
          }
          Partition partition;
          partition.name = "loc_" + cell;
          partition.command = index;
          partition.atoms.push_back(cell);
          partition.regions.push_back(cmd.location);
          cell_loc[cell] = result_.partitions.size();
          result_.partitions.push_back(partition);
        }
      }
    }
    /* A cell with a LOC and a pblock: the LOC is inside the pblock. It is
     * only in the partition of its LOC, as VPR takes one partition per
     * block */
    for (const auto& [cell, loc] : cell_loc) {
      const auto pblock = cell_pblock.find(cell);
      if (cell_pblock.end() == pblock) {
        continue;
      }
      Partition& in_pblock = result_.partitions[pblocks.at(pblock->second)];
      const Location& at = result_.partitions[loc].regions.at(0);
      bool inside = false;
      for (const Location& region : in_pblock.regions) {
        inside |= region.x_low <= at.x_low && at.x_high <= region.x_high &&
                  region.y_low <= at.y_low && at.y_high <= region.y_high;
      }
      if (!inside) {
        error(result_.partitions[loc].command,
              "the LOC " + at.text + " of the cell '" + cell +
                "' is outside its pblock '" + pblock->second + "'");
      }
      auto& atoms = in_pblock.atoms;
      atoms.erase(std::remove(atoms.begin(), atoms.end(), cell), atoms.end());
    }
    check_pblock_room(pblocks);
  }

  /* Each pblock has tiles that can hold its cells, and room for them (R-26):
   * VPR fails without saying why otherwise */
  void check_pblock_room(const std::map<std::string, size_t>& pblocks) {
    if (!design_.device || !design_.netlist) {
      return;
    }
    for (const auto& [name, index] : pblocks) {
      const Partition& partition = result_.partitions[index];
      if (partition.regions.empty()) {
        if (!partition.atoms.empty()) {
          error(partition.command, "the pblock '" + name +
                                     "' has cells but no region "
                                     "(resize_pblock -add)");
        }
        continue;
      }
      std::map<std::string, int> needed;
      std::map<std::string, std::string> example;
      for (const std::string& cell : partition.atoms) {
        const std::string model = cell_model(cell);
        ++needed[model];
        example.emplace(model, cell);
      }
      std::map<std::string, int> room;
      for (const Location& region : partition.regions) {
        for (int x = region.x_low; x <= region.x_high; ++x) {
          for (int y = region.y_low; y <= region.y_high; ++y) {
            const Tile* tile = design_.device->tile(x, y);
            if (!tile || tile->type.empty() || tile->root_x != x ||
                tile->root_y != y) {
              continue;
            }
            for (const auto& [model, count] : needed) {
              room[model] += holds(tile->type, model);
            }
          }
        }
      }
      for (const auto& [model, count] : needed) {
        if (0 == room[model]) {
          error(partition.command, "no tile of the pblock '" + name +
                                     "' can hold the cell '" + example[model] +
                                     "' (" + model + ")");
        } else if (room[model] < count) {
          error(partition.command, "the pblock '" + name + "' has room for " +
                                     std::to_string(room[model]) + " cells " +
                                     model + ", not " + std::to_string(count));
        }
      }
    }
  }

  /* Routing (R-28, R-29) -----------------------------------------------*/

  void routing() {
    for (size_t index = 0; index < constraints_.commands.size(); ++index) {
      const Command& cmd = command(index);
      if ("set_global_signal" != cmd.name) {
        continue;
      }
      GlobalSignal signal;
      signal.command = index;
      signal.route_model = cmd.route_model;
      signal.network = cmd.network;
      const QueryMatch& match = the_match(index);
      if (match.checked) {
        signal.nets = match.matched;
      } else {
        for (const std::string& pattern : object_query(cmd).patterns) {
          signal.patterns.push_back(vpr_pattern(object_query(cmd), pattern));
        }
      }
      if (design_.device) {
        if ("route" == cmd.route_model &&
            "route" != design_.device->clock_modeling) {
          error(index,
                "-route_model route needs the clocks to be routed: the option "
                "general.clock_modeling is '" +
                  design_.device->clock_modeling +
                  "', and VPR fails to route the net otherwise");
        }
        if ("dedicated_network" == cmd.route_model &&
            !design_.device->clock_networks.count(cmd.network)) {
          std::vector<std::string> names(design_.device->clock_networks.begin(),
                                         design_.device->clock_networks.end());
          error(index, "the architecture has no clock network '" + cmd.network +
                         "' (" +
                         (names.empty() ? std::string("it has none")
                                        : "it has " + join(names, 20)) +
                         "): VPR would route the net as an ordinary one");
        }
      } else {
        result_.checked[index] = false;
      }
      result_.global_signals.push_back(signal);
    }
  }

  /* Pins (R-31 to R-34) -------------------------------------------------*/

  bool fits(const std::string& port_direction,
            const std::string& pin_direction) const {
    if ("inout" == pin_direction) {
      return true;
    }
    return ("in" == port_direction && "input" == pin_direction) ||
           ("out" == port_direction && "output" == pin_direction);
  }

  std::string direction_of(const std::string& port) const {
    const Object* object =
      design_.netlist ? design_.netlist->port(port) : nullptr;
    return object ? object->properties.at("direction") : std::string();
  }

  void place_port(const size_t index, const std::string& port, const Site& site,
                  std::map<std::string, size_t>& by_port) {
    PinAssignment pin;
    pin.command = index;
    pin.port = port;
    pin.site = site;
    const auto found = by_port.find(port);
    if (by_port.end() != found) {
      PinAssignment& previous = result_.pins[found->second];
      if (command(previous.command).from_defaults &&
          !command(index).from_defaults) {
        previous = pin;
        return;
      }
      error(index, "the port '" + port + "' has a pin already, at line " +
                     std::to_string(command(previous.command).line));
      return;
    }
    by_port[port] = result_.pins.size();
    result_.pins.push_back(pin);
  }

  void pins() {
    std::map<std::string, size_t> by_port;
    std::vector<size_t> defaults;
    for (size_t index = 0; index < constraints_.commands.size(); ++index) {
      const Command& cmd = command(index);
      if (Kind::PIN != cmd.kind) {
        continue;
      }
      const QueryMatch& match = the_match(index);
      if ("PIN_DEFAULT" == cmd.property) {
        defaults.push_back(index);
        continue;
      }
      if ("LOC" == cmd.property) {
        const Tile* tile = check_location(index, cmd.location);
        if (tile && !tile->io) {
          error(index, "the tile at " + cmd.location.text + " is of the type " +
                         tile->type + ", which holds no port of the design");
        }
        if (!match.checked) {
          Partition partition;
          partition.name = "loc_" + std::to_string(cmd.line);
          partition.command = index;
          for (const std::string& pattern : object_query(cmd).patterns) {
            const std::string regex = vpr_pattern(object_query(cmd), pattern);
            partition.patterns.push_back("^(out:)?" + regex.substr(1));
          }
          partition.regions.push_back(cmd.location);
          result_.partitions.push_back(partition);
          continue;
        }
        if (match.matched.size() > 1) {
          error(index, "LOC " + cmd.location.text + " takes one port, not " +
                         std::to_string(match.matched.size()) + " (" +
                         join(match.matched) + ")");
          continue;
        }
        Site site;
        site.has_location = true;
        site.x = cmd.location.x_low;
        site.y = cmd.location.y_low;
        site.subtile = cmd.location.subtile;
        /* The row of the I/O map at this location, for its checks */
        if (design_.sites && cmd.location.subtile >= 0) {
          for (const Site& known : *design_.sites) {
            if (known.has_location && known.x == site.x && known.y == site.y &&
                known.subtile == site.subtile &&
                fits(
                  direction_of(match.matched.empty() ? "" : match.matched[0]),
                  known.pin_direction)) {
              site = known;
              break;
            }
          }
        }
        for (const std::string& port : match.matched) {
          place_port(index, port, site, by_port);
        }
        continue;
      }
      /* PACKAGE_PIN */
      if (!design_.sites || !design_.io_map) {
        if (design_.device) {
          error(index,
                "PACKAGE_PIN needs the I/O map of a device package "
                "(role io_map): use LOC with a grid location");
        } else {
          result_.checked[index] = false;
        }
        continue;
      }
      std::vector<const Site*> candidates;
      for (const Site& site : *design_.sites) {
        if (cmd.value == site.customer_pin_alias) {
          candidates.push_back(&site);
        }
      }
      if (candidates.empty()) {
        for (const Site& site : *design_.sites) {
          if (cmd.value == site.netlist_name) {
            candidates.push_back(&site);
          }
        }
      }
      if (candidates.empty()) {
        std::vector<Object> names;
        for (const Site& site : *design_.sites) {
          if (!site.customer_pin_alias.empty()) {
            names.push_back({site.customer_pin_alias, {}});
          }
          names.push_back({site.netlist_name, {}});
        }
        error(index, "the I/O map has no pin '" + cmd.value +
                       "'; the nearest names: " +
                       join(nearest_names(cmd.value, names)));
        continue;
      }
      if (!match.checked) {
        continue;
      }
      if (match.matched.size() > 1) {
        error(index, "PACKAGE_PIN " + cmd.value + " takes one port, not " +
                       std::to_string(match.matched.size()) + " (" +
                       join(match.matched) + ")");
        continue;
      }
      for (const std::string& port : match.matched) {
        const std::string direction = direction_of(port);
        const Site* chosen = nullptr;
        for (const Site* site : candidates) {
          if (fits(direction, site->pin_direction)) {
            chosen = site;
            break;
          }
        }
        if (!chosen) {
          error(index, "the port '" + port + "' is an " +
                         ("in" == direction ? "input" : "output") +
                         ", and the pin '" + cmd.value + "' is an " +
                         candidates[0]->pin_direction + " (line " +
                         std::to_string(candidates[0]->line) +
                         " of the I/O map)");
          continue;
        }
        place_port(index, port, *chosen, by_port);
      }
    }
    check_pins();
    for (const size_t index : defaults) {
      const QueryMatch& match = the_match(index);
      if (!match.checked) {
        continue;
      }
      for (const std::string& port : match.matched) {
        const auto found = by_port.find(port);
        if (by_port.end() == found ||
            result_.pins[found->second].site.has_location) {
          error(index,
                "PIN_DEFAULT is for a port on a pin without a "
                "location (a global port of the fabric, by "
                "PACKAGE_PIN): '" +
                  port + "' has none");
          continue;
        }
        result_.pins[found->second].default_value = command(index).value;
      }
    }
    for (const PinAssignment& pin : result_.pins) {
      if (!pin.site.has_location) {
        continue;
      }
      const Object* object =
        design_.netlist ? design_.netlist->port(pin.port) : nullptr;
      Partition partition;
      partition.name = "pin_" + pin.port;
      partition.command = pin.command;
      partition.atoms.push_back(object ? port_atom(*object) : pin.port);
      Location location;
      location.x_low = location.x_high = pin.site.x;
      location.y_low = location.y_high = pin.site.y;
      location.subtile = pin.site.subtile;
      location.text = pin.site.name();
      partition.regions.push_back(location);
      result_.partitions.push_back(partition);
    }
  }

  /* The pins against their rows: the direction, the type, one port a pin */
  void check_pins() {
    std::map<std::string, const PinAssignment*> by_site;
    for (const PinAssignment& pin : result_.pins) {
      const std::string direction = direction_of(pin.port);
      if (0 != pin.site.line) {
        if (!direction.empty() && !fits(direction, pin.site.pin_direction)) {
          error(pin.command, "the port '" + pin.port + "' is an " +
                               ("in" == direction ? "input" : "output") +
                               ", and the pin " + pin.site.name() + " is an " +
                               pin.site.pin_direction + " (line " +
                               std::to_string(pin.site.line) +
                               " of the I/O map)");
        }
        if ("UNDEF" == pin.site.pin_type) {
          warning(pin.command, "the pin " + pin.site.name() + " of the port '" +
                                 pin.port +
                                 "' has the type UNDEF in the I/O map");
        }
      }
      if (pin.site.has_location && pin.site.subtile < 0) {
        continue;
      }
      const auto [found, added] = by_site.emplace(pin.site.name(), &pin);
      if (!added) {
        error(pin.command, "the ports '" + found->second->port + "' and '" +
                             pin.port + "' are on the same pin " +
                             pin.site.name());
      }
    }
  }

  /* The coverage of the timing (R-41) -----------------------------------*/

  void coverage() {
    TimingCoverage& timing = result_.timing;
    if (!design_.netlist) {
      return;
    }
    timing.checked = true;
    std::set<std::string> clocked;
    std::set<std::string> input_delays;
    std::set<std::string> output_delays;
    for (const Clock& clock : timing.clocks) {
      clocked.insert(clock.targets.begin(), clock.targets.end());
      clocked.insert(clock.name);
    }
    for (const QueryMatch& match : result_.queries) {
      const std::string& name = command(match.command).name;
      if ("set_input_delay" == name && "ports" == match.kind) {
        input_delays.insert(match.matched.begin(), match.matched.end());
      } else if ("set_output_delay" == name && "ports" == match.kind) {
        output_delays.insert(match.matched.begin(), match.matched.end());
      }
    }
    for (const std::string& net : design_.netlist->clock_nets) {
      if (!clocked.count(net)) {
        timing.unconstrained_clocks.push_back(net);
      }
    }
    for (const Object& port : design_.netlist->ports) {
      const std::string& direction = port.properties.at("direction");
      if ("in" == direction && !input_delays.count(port.name) &&
          !clocked.count(port.name) &&
          !design_.netlist->clock_nets.count(port.name)) {
        timing.inputs_without_delay.push_back(port.name);
      } else if ("out" == direction && !output_delays.count(port.name)) {
        timing.outputs_without_delay.push_back(port.name);
      }
    }
  }

  static const Query& object_query(const Command& cmd) {
    return cmd.words[cmd.objects.at(0)].query;
  }

  const Constraints& constraints_;
  const Design& design_;
  Resolution result_;
  std::vector<Object> sites_;
  std::vector<Object> clock_objects_;
  std::vector<Object> pblock_objects_;
  std::map<std::pair<size_t, size_t>, size_t> matches_;
};

}  // namespace

bool Resolution::has_errors() const {
  for (const Diagnostic& diagnostic : diagnostics) {
    if (Diagnostic::ERROR == diagnostic.level) {
      return true;
    }
  }
  return false;
}

Resolution resolve(const Constraints& constraints, const Design& design) {
  return Resolver(constraints, design).run();
}

} /* end namespace odc */
