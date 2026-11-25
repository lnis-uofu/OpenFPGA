#include "odc_design.h"

#include <algorithm>
#include <filesystem>

#include "atom_netlist.h"
#include "device_package.h"
#include "globals.h"
#include "nlohmann/json.hpp"
#include "package_io_map.h"
#include "setup_grid.h"
#include "vpr_run.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

/* The model of a primitive as the netlist names it: names, latch, input,
 * output, or the model of a .subckt */
std::string model_of(const std::string& blif_model) {
  const std::string subckt = ".subckt ";
  if (0 == blif_model.rfind(subckt, 0)) {
    return blif_model.substr(subckt.size());
  }
  return '.' == blif_model[0] ? blif_model.substr(1) : blif_model;
}

/* How many primitives of each model a block can hold: the modes are
 * alternatives, the children add up */
std::map<std::string, int> models_of(const t_pb_type* pb_type) {
  std::map<std::string, int> models;
  if (nullptr != pb_type->blif_model) {
    models[model_of(pb_type->blif_model)] = 1;
    return models;
  }
  for (int m = 0; m < pb_type->num_modes; ++m) {
    const t_mode& mode = pb_type->modes[m];
    std::map<std::string, int> in_mode;
    for (int c = 0; c < mode.num_pb_type_children; ++c) {
      const t_pb_type& child = mode.pb_type_children[c];
      for (const auto& [model, count] : models_of(&child)) {
        in_mode[model] += count * child.num_pb;
      }
    }
    for (const auto& [model, count] : in_mode) {
      models[model] = std::max(models[model], count);
    }
  }
  return models;
}

std::shared_ptr<odc::Device> read_device(const OpenFpgaSession& session) {
  const vpr::VprRun* run = vpr::current_vpr_run();
  if (nullptr == run || !session.has_input(OpenFpgaInput::VPR_ARCH) ||
      session.vpr_generation() != vpr::vpr_run_generation()) {
    return nullptr;
  }
  const AppOptionValue* layout =
    session.app_options().find("general.device_layout");
  if (nullptr == layout || layout->string_value.empty() ||
      "auto" == layout->string_value) {
    return nullptr;
  }
  const DeviceGrid grid =
    create_device_grid(layout->string_value, run->arch.grid_layouts);
  auto device = std::make_shared<odc::Device>();
  device->width = static_cast<int>(grid.width());
  device->height = static_cast<int>(grid.height());
  device->layers = static_cast<int>(grid.get_num_layers());
  device->tiles.resize(static_cast<size_t>(device->width) * device->height);
  for (int x = 0; x < device->width; ++x) {
    for (int y = 0; y < device->height; ++y) {
      const t_physical_tile_loc loc(x, y, 0);
      const t_physical_tile_type_ptr type = grid.get_physical_type(loc);
      odc::Tile& tile =
        device->tiles[static_cast<size_t>(x) * device->height + y];
      tile.root_x = x - grid.get_width_offset(loc);
      tile.root_y = y - grid.get_height_offset(loc);
      if (nullptr == type || 0 == type->capacity) {
        continue;
      }
      tile.type = type->name;
      tile.capacity = type->capacity;
      tile.io = type->is_io();
    }
  }
  for (const t_physical_tile_type& type :
       g_vpr_ctx.device().physical_tile_types) {
    std::map<std::string, int>& holds = device->type_models[type.name];
    for (const t_sub_tile& sub_tile : type.sub_tiles) {
      std::map<std::string, int> one;
      for (const t_logical_block_type_ptr site : sub_tile.equivalent_sites) {
        if (nullptr == site->pb_type) {
          continue;
        }
        for (const auto& [model, count] : models_of(site->pb_type)) {
          one[model] = std::max(one[model], count);
        }
      }
      for (const auto& [model, count] : one) {
        holds[model] += count * sub_tile.capacity.total();
      }
    }
  }
  for (const t_clock_network_arch& network :
       run->arch.clock_arch.clock_networks_arch) {
    device->clock_networks.insert(network.name);
  }
  const AppOptionValue* clocks =
    session.app_options().find("general.clock_modeling");
  device->clock_modeling = nullptr == clocks ? "ideal" : clocks->string_value;
  return device;
}

/* The output ports of the models of the architecture, to name the blocks of
 * a .subckt as VPR does */
std::map<std::string, std::vector<std::string>> model_outputs() {
  std::map<std::string, std::vector<std::string>> outputs;
  const vpr::VprRun* run = vpr::current_vpr_run();
  if (nullptr == run) {
    return outputs;
  }
  for (const LogicalModelId id : run->arch.models.all_models()) {
    const t_model& model = run->arch.models.get_model(id);
    std::vector<std::string>& ports = outputs[model.name];
    for (const t_model_ports* port = model.outputs; nullptr != port;
         port = port->next) {
      ports.push_back(port->name);
    }
  }
  return outputs;
}

/* The netlist that VPR read and cleaned (buffers absorbed, dangling blocks
 * swept), named as its blocks are: the names that its constraints are
 * matched against */
std::shared_ptr<odc::Netlist> read_vpr_netlist(const std::string& file) {
  const vpr::VprRun* run = vpr::current_vpr_run();
  const AtomNetlist& atoms = g_vpr_ctx.atom().netlist();
  if (nullptr == run || !run->initialized || 0 == atoms.blocks().size()) {
    return nullptr;
  }
  auto netlist = std::make_shared<odc::Netlist>();
  netlist->file = file;
  netlist->top = atoms.netlist_name();
  const std::string out = "out:";
  for (const AtomBlockId block : atoms.blocks()) {
    const std::string& name = atoms.block_name(block);
    switch (atoms.block_type(block)) {
      case AtomBlockType::INPAD:
        netlist->ports.push_back({name, {{"direction", "in"}}});
        break;
      case AtomBlockType::OUTPAD:
        netlist->ports.push_back(
          {0 == name.rfind(out, 0) ? name.substr(out.size()) : name,
           {{"direction", "out"}}});
        break;
      default: {
        const std::string model =
          model_of(run->arch.models.model_name(atoms.block_model(block)));
        netlist->cells.push_back({name, {{"type", model}}});
        for (const AtomPortId port : atoms.block_ports(block)) {
          const bool output = PortType::OUTPUT == atoms.port_type(port);
          netlist->pins.push_back({name + "/" + atoms.port_name(port),
                                   {{"direction", output ? "out" : "in"}}});
        }
        for (const AtomPinId pin : atoms.block_clock_pins(block)) {
          netlist->clock_nets.insert(atoms.net_name(atoms.pin_net(pin)));
        }
        break;
      }
    }
  }
  for (const AtomNetId net : atoms.nets()) {
    netlist->nets.push_back({atoms.net_name(net), {}});
  }
  return netlist;
}

std::string side_of(const odc::Device& device, const int x, const int y) {
  if (0 == y) {
    return "BOTTOM";
  }
  if (device.height - 1 == y) {
    return "TOP";
  }
  if (0 == x) {
    return "LEFT";
  }
  if (device.width - 1 == x) {
    return "RIGHT";
  }
  return "CORE";
}

}  // namespace

odc::Design OdcDesignData::design() const {
  odc::Design design;
  design.netlist = netlist.get();
  design.device = device.get();
  design.sites = sites.get();
  design.io_map = io_map;
  return design;
}

OdcDesignData odc_design_data(const OpenFpgaSession& session,
                              const std::string& netlist_file) {
  OdcDesignData data;
  try {
    data.device = read_device(session);
  } catch (const std::exception& error) {
    data.problems.push_back(std::string("the grid of the device: ") +
                            error.what());
  }
  const std::string circuit = netlist_file.empty()
                                ? session.input_file(OpenFpgaInput::CIRCUIT)
                                : netlist_file;
  /* The circuit of the session is the one VPR read; a circuit about to be
   * read is read here, as VPR names its blocks */
  if (netlist_file.empty() && session.has_input(OpenFpgaInput::CIRCUIT) &&
      session.vpr_generation() == vpr::vpr_run_generation()) {
    data.netlist = read_vpr_netlist(circuit);
  }
  if (!data.netlist && !circuit.empty() &&
      (!netlist_file.empty() || session.has_input(OpenFpgaInput::CIRCUIT))) {
    try {
      data.netlist = std::make_shared<odc::Netlist>(
        odc::read_blif_netlist(circuit, model_outputs()));
    } catch (const std::exception& error) {
      data.problems.push_back(error.what());
    }
  }
  /* The pins: the I/O map of the device package, or the I/O subtiles of the
   * grid */
  if (!session.device_package().empty()) {
    try {
      const nlohmann::json answer =
        nlohmann::json::parse(session.device_package());
      const DevicePackage package =
        load_device_package(answer["directory"].get<std::string>(), false);
      const auto maps = package.files_of("io_map");
      if (!maps.empty()) {
        data.io_map_file = maps[0].string();
        const PackageIoMap map = read_package_io_map(maps[0]);
        data.sites = std::make_shared<std::vector<odc::Site>>();
        data.io_map = true;
        for (const PackageIoMapRow& row : map.rows) {
          odc::Site site;
          site.line = row.line;
          site.side = row.side;
          site.pin_direction = row.pin_direction;
          site.pin_type = row.pin_type;
          site.has_location = row.has_location();
          site.x = row.col.value_or(0);
          site.y = row.row.value_or(0);
          site.subtile = row.subtile.value_or(0);
          site.netlist_name = row.netlist_name;
          site.customer_pin_alias = row.customer_pin_alias;
          data.sites->push_back(site);
        }
      }
      const auto defaults = package.files_of("odc_defaults");
      if (!defaults.empty()) {
        data.defaults_file = defaults[0].string();
      }
    } catch (const std::exception& error) {
      data.problems.push_back(std::string("the device package: ") +
                              error.what());
    }
  }
  if (!data.sites && data.device) {
    data.sites = std::make_shared<std::vector<odc::Site>>();
    for (int x = 0; x < data.device->width; ++x) {
      for (int y = 0; y < data.device->height; ++y) {
        const odc::Tile* tile = data.device->tile(x, y);
        if (!tile->io || tile->root_x != x || tile->root_y != y) {
          continue;
        }
        for (int s = 0; s < tile->capacity; ++s) {
          odc::Site site;
          site.side = side_of(*data.device, x, y);
          site.pin_direction = "inout";
          site.pin_type = "GPIO";
          site.has_location = true;
          site.x = x;
          site.y = y;
          site.subtile = s;
          data.sites->push_back(site);
        }
      }
    }
  }
  return data;
}

} /* end namespace openfpga */
