#include "query_design.h"

#include <algorithm>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#include "glob_match.h"
#include "globals.h"
#include "json_response.h"
#include "json_writer.h"
#include "stats.h"
#include "vpr_utils.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

void write_grid(JsonWriter& json) {
  const auto& grid = g_vpr_ctx.device().grid;
  json.key("grid").begin_object();
  json.key("width").value(static_cast<int64_t>(grid.width()));
  json.key("height").value(static_cast<int64_t>(grid.height()));
  json.end_object();
}

/* used, capacity and occupancy of a set of channel segments */
void write_usage(JsonWriter& json, const char* name, const int64_t used,
                 const int64_t capacity) {
  json.key(name).begin_object();
  json.key("used").value(used);
  json.key("capacity").value(capacity);
  json.key("occupancy")
    .value(capacity > 0 ? static_cast<double>(used) / capacity : 0.);
  json.end_object();
}

double ratio(const size_t used, const size_t total) {
  return total > 0 ? static_cast<double>(used) / total : 0.;
}

}  // End anonymous namespace

bool device_json(const bool fixed_channel_width, const std::string& type,
                 const size_t limit, std::string& json_text) {
  const auto& device_ctx = g_vpr_ctx.device();
  const auto& grid = device_ctx.grid;
  if (0 == grid.width() || 0 == grid.height()) {
    VTR_LOG_ERROR(
      "The device has not been created: read the VPR architecture, the "
      "OpenFPGA architecture and the circuit first\n");
    return false;
  }
  if (limit < 1) {
    VTR_LOG_ERROR("The limit must be at least 1\n");
    return false;
  }
  const t_physical_tile_type* asked = nullptr;
  if (!type.empty()) {
    std::string names;
    for (const t_physical_tile_type& tile : device_ctx.physical_tile_types) {
      if (tile.is_empty()) {
        continue;
      }
      if (tile.name == type) {
        asked = &tile;
      }
      names += (names.empty() ? "" : ", ") + tile.name;
    }
    if (nullptr == asked) {
      VTR_LOG_ERROR("The device has no tile type '%s' (the types are: %s)\n",
                    type.c_str(), names.c_str());
      return false;
    }
  }

  JsonWriter json;
  begin_response(json, "device", DEVICE_SCHEMA_VERSION, "DEVICE");
  json.begin_object();
  json.key("grid").begin_object();
  json.key("width").value(static_cast<int64_t>(grid.width()));
  json.key("height").value(static_cast<int64_t>(grid.height()));
  json.key("layers").value(static_cast<int64_t>(grid.get_num_layers()));
  json.key("layout").value(grid.name());
  json.end_object();

  /* The routing resource graph is made with the device: with the width that
   * is set, or with the one of the estimate of the delays of the placement */
  const auto& chan_width = device_ctx.chan_width;
  json.key("channel_width");
  if (chan_width.x_max > 0 || chan_width.y_max > 0) {
    json.begin_object();
    json.key("x").value(static_cast<int64_t>(chan_width.x_max));
    json.key("y").value(static_cast<int64_t>(chan_width.y_max));
    json.key("fixed").value(fixed_channel_width);
    json.end_object();
  } else {
    json.null();
  }

  /* The tiles of each type: the lower left corner of each one */
  std::map<const t_physical_tile_type*, int64_t> tile_count;
  for (size_t layer = 0; layer < grid.get_num_layers(); ++layer) {
    for (size_t x = 0; x < grid.width(); ++x) {
      for (size_t y = 0; y < grid.height(); ++y) {
        const t_physical_tile_loc loc(static_cast<int>(x), static_cast<int>(y),
                                      static_cast<int>(layer));
        if (grid.is_root_location(loc)) {
          ++tile_count[grid.get_physical_type(loc)];
        }
      }
    }
  }

  json.key("tile_types").begin_array();
  for (const t_physical_tile_type& tile : device_ctx.physical_tile_types) {
    if (tile.is_empty()) {
      continue;
    }
    json.begin_object();
    json.key("name").value(tile.name);
    json.key("width").value(static_cast<int64_t>(tile.width));
    json.key("height").value(static_cast<int64_t>(tile.height));
    json.key("capacity").value(static_cast<int64_t>(tile.capacity));
    json.key("is_io").value(tile.is_io());
    json.key("input_pins").value(static_cast<int64_t>(tile.num_input_pins));
    json.key("output_pins").value(static_cast<int64_t>(tile.num_output_pins));
    json.key("clock_pins").value(static_cast<int64_t>(tile.num_clock_pins));
    const auto found = tile_count.find(&tile);
    json.key("count").value(tile_count.end() == found ? 0 : found->second);
    json.key("slots").value(
      static_cast<int64_t>(grid.num_instances(&tile, -1)));
    json.key("sub_tiles").begin_array();
    for (const t_sub_tile& sub_tile : tile.sub_tiles) {
      json.begin_object();
      json.key("name").value(sub_tile.name);
      json.key("capacity")
        .value(static_cast<int64_t>(sub_tile.capacity.high -
                                    sub_tile.capacity.low + 1));
      json.key("equivalent_blocks").begin_array();
      for (const t_logical_block_type_ptr block : sub_tile.equivalent_sites) {
        json.value(block->name);
      }
      json.end_array();
      json.end_object();
    }
    json.end_array();
    json.end_object();
  }
  json.end_array();

  /* The I/O tiles on the sides of the perimeter. A corner belongs to no side:
   * it is a tile of a type that is not an I/O in the architectures that put
   * the I/O on the sides, and it would be counted twice otherwise */
  int64_t left = 0;
  int64_t right = 0;
  int64_t bottom = 0;
  int64_t top = 0;
  const int width = static_cast<int>(grid.width());
  const int height = static_cast<int>(grid.height());
  for (size_t layer = 0; layer < grid.get_num_layers(); ++layer) {
    const int layer_num = static_cast<int>(layer);
    for (int x = 1; x < width - 1; ++x) {
      bottom += grid.get_physical_type({x, 0, layer_num})->is_io() ? 1 : 0;
      top +=
        grid.get_physical_type({x, height - 1, layer_num})->is_io() ? 1 : 0;
    }
    for (int y = 1; y < height - 1; ++y) {
      left += grid.get_physical_type({0, y, layer_num})->is_io() ? 1 : 0;
      right +=
        grid.get_physical_type({width - 1, y, layer_num})->is_io() ? 1 : 0;
    }
  }
  json.key("io_sides").begin_object();
  json.key("left").value(left);
  json.key("right").value(right);
  json.key("bottom").value(bottom);
  json.key("top").value(top);
  json.end_object();

  if (nullptr != asked) {
    const auto found = tile_count.find(asked);
    const size_t total = tile_count.end() == found ? 0 : found->second;
    std::vector<t_physical_tile_loc> tiles;
    for (size_t layer = 0;
         layer < grid.get_num_layers() && tiles.size() < limit; ++layer) {
      for (int x = 0; x < width && tiles.size() < limit; ++x) {
        for (int y = 0; y < height && tiles.size() < limit; ++y) {
          const t_physical_tile_loc loc(x, y, static_cast<int>(layer));
          if (grid.get_physical_type(loc) == asked &&
              grid.is_root_location(loc)) {
            tiles.push_back(loc);
          }
        }
      }
    }
    json.key("locations").begin_object();
    json.key("type").value(asked->name);
    json.key("total").value(static_cast<int64_t>(total));
    json.key("returned").value(static_cast<int64_t>(tiles.size()));
    json.key("truncated").value(tiles.size() < total);
    json.key("tiles").begin_array();
    for (const t_physical_tile_loc& loc : tiles) {
      json.begin_object();
      json.key("x").value(static_cast<int64_t>(loc.x));
      json.key("y").value(static_cast<int64_t>(loc.y));
      json.key("layer").value(static_cast<int64_t>(loc.layer_num));
      json.end_object();
    }
    json.end_array();
    json.end_object();
  }
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

bool utilization_json(std::string& json_text) {
  const auto& device_ctx = g_vpr_ctx.device();
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  if (0 == clb_nlist.blocks().size()) {
    VTR_LOG_ERROR("The design has no packing: run the stage PACKED first\n");
    return false;
  }
  std::map<t_logical_block_type_ptr, size_t> used;
  for (const ClusterBlockId blk : clb_nlist.blocks()) {
    ++used[clb_nlist.block_type(blk)];
  }

  JsonWriter json;
  begin_response(json, "utilization", UTILIZATION_SCHEMA_VERSION, "PACKED");
  json.begin_object();
  json.key("device_utilization")
    .value(
      static_cast<double>(calculate_device_utilization(device_ctx.grid, used)));
  write_grid(json);
  json.key("block_types").begin_array();
  for (const t_logical_block_type& type : device_ctx.logical_block_types) {
    if (is_empty_type(&type)) {
      continue;
    }
    size_t available = 0;
    for (const t_physical_tile_type_ptr tile : type.equivalent_tiles) {
      /* Already the places for blocks: a tile counts as much as its capacity,
       * as in the report of the resource usage of VPR */
      available += device_ctx.grid.num_instances(tile, -1);
    }
    const auto found = used.find(&type);
    const size_t num_used = used.end() == found ? 0 : found->second;
    json.begin_object();
    json.key("name").value(type.name);
    json.key("is_io").value(type.is_io());
    json.key("used").value(static_cast<int64_t>(num_used));
    json.key("available").value(static_cast<int64_t>(available));
    json.key("utilization")
      .value(available > 0 ? static_cast<double>(num_used) / available : 0.);
    json.end_object();
  }
  json.end_array();
  json.key("limiting_resources").begin_array();
  for (const t_logical_block_type_ptr type :
       device_ctx.grid.limiting_resources()) {
    json.value(type->name);
  }
  json.end_array();
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

bool placement_json(const std::string& type, const std::string& prefix,
                    const size_t limit, std::string& json_text) {
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const auto& block_locs = g_vpr_ctx.placement().block_locs();
  if (0 == clb_nlist.blocks().size() || block_locs.size() == 0) {
    VTR_LOG_ERROR("The design has no placement: run the stage PLACED first\n");
    return false;
  }
  if (limit < 1) {
    VTR_LOG_ERROR("The limit must be at least 1\n");
    return false;
  }

  std::vector<ClusterBlockId> matching;
  for (const ClusterBlockId blk : clb_nlist.blocks()) {
    if (!type.empty() && clb_nlist.block_type(blk)->name != type) {
      continue;
    }
    if (!prefix.empty() &&
        0 != clb_nlist.block_name(blk).compare(0, prefix.size(), prefix)) {
      continue;
    }
    matching.push_back(blk);
  }

  JsonWriter json;
  begin_response(json, "placement", PLACEMENT_SCHEMA_VERSION, "PLACED");
  json.begin_object();
  write_grid(json);
  json.key("filter").begin_object();
  json.key("type");
  type.empty() ? json.null() : json.value(type);
  json.key("prefix");
  prefix.empty() ? json.null() : json.value(prefix);
  json.end_object();
  const size_t returned = std::min(limit, matching.size());
  json.key("total").value(static_cast<int64_t>(matching.size()));
  json.key("returned").value(static_cast<int64_t>(returned));
  json.key("truncated").value(returned < matching.size());
  json.key("blocks").begin_array();
  for (size_t i = 0; i < returned; ++i) {
    const ClusterBlockId blk = matching[i];
    const t_pl_loc& loc = block_locs[blk].loc;
    json.begin_object();
    json.key("name").value(clb_nlist.block_name(blk));
    json.key("type").value(clb_nlist.block_type(blk)->name);
    json.key("x").value(loc.x);
    json.key("y").value(loc.y);
    json.key("sub_tile").value(loc.sub_tile);
    json.key("layer").value(loc.layer);
    json.end_object();
  }
  json.end_array();
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

bool find_json(const std::string& pattern, const std::string& kind,
               const size_t limit, std::string& json_text) {
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const auto& block_locs = g_vpr_ctx.placement().block_locs();
  if (0 == clb_nlist.blocks().size()) {
    VTR_LOG_ERROR("The design has no packing: run the stage PACKED first\n");
    return false;
  }
  if (!kind.empty() && kind != "block" && kind != "net") {
    VTR_LOG_ERROR("The kind must be 'block' or 'net', not '%s'\n",
                  kind.c_str());
    return false;
  }
  if (pattern.empty()) {
    VTR_LOG_ERROR("The pattern is empty\n");
    return false;
  }
  if (limit < 1) {
    VTR_LOG_ERROR("The limit must be at least 1\n");
    return false;
  }
  const bool with_blocks = kind.empty() || kind == "block";
  const bool with_nets = kind.empty() || kind == "net";

  JsonWriter json;
  begin_response(json, "find", FIND_SCHEMA_VERSION, "PACKED");
  json.begin_object();
  json.key("pattern").value(pattern);
  json.key("kind");
  kind.empty() ? json.null() : json.value(kind);

  json.key("blocks");
  if (with_blocks) {
    std::vector<ClusterBlockId> matching;
    for (const ClusterBlockId blk : clb_nlist.blocks()) {
      if (glob_match(pattern, clb_nlist.block_name(blk))) {
        matching.push_back(blk);
      }
    }
    const size_t returned = std::min(limit, matching.size());
    json.begin_object();
    json.key("total").value(static_cast<int64_t>(clb_nlist.blocks().size()));
    json.key("matched").value(static_cast<int64_t>(matching.size()));
    json.key("returned").value(static_cast<int64_t>(returned));
    json.key("truncated").value(returned < matching.size());
    json.key("items").begin_array();
    for (size_t i = 0; i < returned; ++i) {
      const ClusterBlockId blk = matching[i];
      json.begin_object();
      json.key("name").value(clb_nlist.block_name(blk));
      json.key("type").value(clb_nlist.block_type(blk)->name);
      /* Where it is, once the design is placed */
      const bool placed = size_t(blk) < block_locs.size() &&
                          block_locs[blk].loc.x >= 0 &&
                          block_locs[blk].loc.y >= 0;
      json.key("placed").value(placed);
      const t_pl_loc loc = placed ? block_locs[blk].loc : t_pl_loc();
      auto write_coordinate = [&](const char* name, const int value) {
        json.key(name);
        placed ? json.value(value) : json.null();
      };
      write_coordinate("x", loc.x);
      write_coordinate("y", loc.y);
      write_coordinate("sub_tile", loc.sub_tile);
      write_coordinate("layer", loc.layer);
      json.end_object();
    }
    json.end_array();
    json.end_object();
  } else {
    json.null();
  }

  json.key("nets");
  if (with_nets) {
    std::vector<ClusterNetId> matching;
    for (const ClusterNetId net : clb_nlist.nets()) {
      if (glob_match(pattern, clb_nlist.net_name(net))) {
        matching.push_back(net);
      }
    }
    const size_t returned = std::min(limit, matching.size());
    json.begin_object();
    json.key("total").value(static_cast<int64_t>(clb_nlist.nets().size()));
    json.key("matched").value(static_cast<int64_t>(matching.size()));
    json.key("returned").value(static_cast<int64_t>(returned));
    json.key("truncated").value(returned < matching.size());
    json.key("items").begin_array();
    for (size_t i = 0; i < returned; ++i) {
      const ClusterNetId net = matching[i];
      json.begin_object();
      json.key("name").value(clb_nlist.net_name(net));
      json.key("fanout").value(
        static_cast<int64_t>(clb_nlist.net_sinks(net).size()));
      json.key("ignored").value(clb_nlist.net_is_ignored(net));
      json.end_object();
    }
    json.end_array();
    json.end_object();
  } else {
    json.null();
  }
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

bool net_route_json(const std::string& net, std::string& json_text) {
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const auto& route_ctx = g_vpr_ctx.routing();
  const auto& rr_graph = g_vpr_ctx.device().rr_graph;
  if (route_ctx.route_trees.empty()) {
    VTR_LOG_ERROR("The design has no routing: run the stage ROUTED first\n");
    return false;
  }
  const ClusterNetId net_id = clb_nlist.find_net(net);
  if (!net_id.is_valid()) {
    VTR_LOG_ERROR("The net '%s' does not exist\n", net.c_str());
    return false;
  }
  const auto& tree = route_ctx.route_trees[ParentNetId(size_t(net_id))];
  if (!tree) {
    VTR_LOG_ERROR(
      "The net '%s' has no routing: it is not routed by VPR, e.g., a clock\n",
      net.c_str());
    return false;
  }

  JsonWriter json;
  begin_response(json, "net_route", NET_ROUTE_SCHEMA_VERSION, "ROUTED");
  json.begin_object();
  json.key("net").value(net);
  json.key("num_sinks").value(static_cast<int64_t>(tree->num_sinks()));
  json.key("nodes").begin_array();
  std::map<const RouteTreeNode*, int> index;
  for (const RouteTreeNode& node : tree->all_nodes()) {
    const int here = static_cast<int>(index.size());
    index[&node] = here;
    json.begin_object();
    json.key("index").value(here);
    json.key("parent");
    const auto parent = node.parent();
    if (parent) {
      json.value(index.at(&(*parent)));
    } else {
      json.null();
    }
    const RRNodeId inode = node.inode;
    json.key("rr_node").value(static_cast<int64_t>(size_t(inode)));
    json.key("type").value(rr_graph.node_type_string(inode));
    json.key("xlow").value(static_cast<int>(rr_graph.node_xlow(inode)));
    json.key("ylow").value(static_cast<int>(rr_graph.node_ylow(inode)));
    json.key("xhigh").value(static_cast<int>(rr_graph.node_xhigh(inode)));
    json.key("yhigh").value(static_cast<int>(rr_graph.node_yhigh(inode)));
    json.key("ptc").value(static_cast<int>(rr_graph.node_ptc_num(inode)));
    json.key("layer").value(static_cast<int>(rr_graph.node_layer_low(inode)));
    json.key("sink_pin");
    if (node.net_pin_index > 0) {
      json.value(node.net_pin_index);
    } else {
      json.null();
    }
    json.end_object();
  }
  json.end_array();
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

bool congestion_json(const int region_size, const size_t hotspots,
                     std::string& json_text) {
  const auto& device_ctx = g_vpr_ctx.device();
  const auto& route_ctx = g_vpr_ctx.routing();
  const auto& rr_graph = device_ctx.rr_graph;
  if (route_ctx.route_trees.empty()) {
    VTR_LOG_ERROR("The design has no routing: run the stage ROUTED first\n");
    return false;
  }
  if (region_size < 1) {
    VTR_LOG_ERROR("The size of a region must be at least 1 tile\n");
    return false;
  }
  const int width = static_cast<int>(device_ctx.grid.width());
  const int height = static_cast<int>(device_ctx.grid.height());

  /* The nets that use each node */
  std::vector<int> occupancy(rr_graph.num_nodes(), 0);
  for (const auto& tree : route_ctx.route_trees) {
    if (!tree) {
      continue;
    }
    for (const RouteTreeNode& node : tree->all_nodes()) {
      ++occupancy[size_t(node.inode)];
    }
  }

  /* The wires in use that cross each segment of a channel, and the number of
   * wires of the channel there */
  std::vector<std::vector<int>> used_x(height, std::vector<int>(width, 0));
  std::vector<std::vector<int>> used_y(height, std::vector<int>(width, 0));
  /* The wires in use by more than one net that cross each segment */
  std::vector<std::vector<int>> overused_x(height, std::vector<int>(width, 0));
  std::vector<std::vector<int>> overused_y(height, std::vector<int>(width, 0));
  int64_t overused = 0;
  for (const RRNodeId inode : rr_graph.nodes()) {
    const int occ = occupancy[size_t(inode)];
    if (0 == occ) {
      continue;
    }
    const e_rr_type type = rr_graph.node_type(inode);
    if (type != e_rr_type::CHANX && type != e_rr_type::CHANY) {
      continue;
    }
    const bool is_overused = occ > rr_graph.node_capacity(inode);
    if (is_overused) {
      ++overused;
    }
    for (int x = rr_graph.node_xlow(inode); x <= rr_graph.node_xhigh(inode);
         ++x) {
      for (int y = rr_graph.node_ylow(inode); y <= rr_graph.node_yhigh(inode);
           ++y) {
        (type == e_rr_type::CHANX ? used_x : used_y)[y][x] += 1;
        if (is_overused) {
          (type == e_rr_type::CHANX ? overused_x : overused_y)[y][x] += 1;
        }
      }
    }
  }

  struct Region {
    int64_t used_x = 0, cap_x = 0, used_y = 0, cap_y = 0;
  };
  std::map<std::pair<int, int>, Region> regions;
  Region total;
  const auto& chan_width = device_ctx.chan_width;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      /* The horizontal channels run above the rows 0 to height-2, between the
       * columns 1 and width-2. The vertical ones are beside the columns 0 to
       * width-2, between the rows 1 and height-2 */
      const bool has_x = y <= height - 2 && x >= 1 && x <= width - 2;
      const bool has_y = x <= width - 2 && y >= 1 && y <= height - 2;
      if (!has_x && !has_y) {
        continue;
      }
      Region& region = regions[{x / region_size, y / region_size}];
      if (has_x) {
        region.used_x += used_x[y][x];
        region.cap_x += chan_width.x_list[y];
        total.used_x += used_x[y][x];
        total.cap_x += chan_width.x_list[y];
      }
      if (has_y) {
        region.used_y += used_y[y][x];
        region.cap_y += chan_width.y_list[x];
        total.used_y += used_y[y][x];
        total.cap_y += chan_width.y_list[x];
      }
    }
  }

  /* The most used segments of the channels: the fullest first. A segment is
   * a place along a channel that one wire or more cross */
  struct Hotspot {
    bool horizontal;
    int x, y, used, capacity, overused;
    double occupancy;
    std::vector<ClusterNetId> nets;
  };
  std::vector<Hotspot> spots;
  if (hotspots > 0) {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        const bool has_x = y <= height - 2 && x >= 1 && x <= width - 2;
        const bool has_y = x <= width - 2 && y >= 1 && y <= height - 2;
        if (has_x && used_x[y][x] > 0) {
          const int cap = chan_width.x_list[y];
          spots.push_back({true,
                           x,
                           y,
                           used_x[y][x],
                           cap,
                           overused_x[y][x],
                           ratio(used_x[y][x], cap),
                           {}});
        }
        if (has_y && used_y[y][x] > 0) {
          const int cap = chan_width.y_list[x];
          spots.push_back({false,
                           x,
                           y,
                           used_y[y][x],
                           cap,
                           overused_y[y][x],
                           ratio(used_y[y][x], cap),
                           {}});
        }
      }
    }
    /* The fullest first. The rest of the order is fixed, so that the answer is
     * the same each time */
    std::sort(spots.begin(), spots.end(),
              [](const Hotspot& a, const Hotspot& b) {
                if (a.occupancy != b.occupancy) {
                  return a.occupancy > b.occupancy;
                }
                if (a.used != b.used) {
                  return a.used > b.used;
                }
                if (a.horizontal != b.horizontal) {
                  return a.horizontal;
                }
                return a.y != b.y ? a.y < b.y : a.x < b.x;
              });
    if (spots.size() > hotspots) {
      spots.resize(hotspots);
    }
    /* The nets that cross them: look at the routing of each net once, for the
     * segments that were kept */
    std::map<std::tuple<bool, int, int>, size_t> kept;
    for (size_t i = 0; i < spots.size(); ++i) {
      kept[std::make_tuple(spots[i].horizontal, spots[i].x, spots[i].y)] = i;
    }
    for (const ClusterNetId net : g_vpr_ctx.clustering().clb_nlist.nets()) {
      const auto& tree = route_ctx.route_trees[ParentNetId(size_t(net))];
      if (!tree) {
        continue;
      }
      std::set<size_t> crossed;
      for (const RouteTreeNode& node : tree->all_nodes()) {
        const e_rr_type type = rr_graph.node_type(node.inode);
        if (type != e_rr_type::CHANX && type != e_rr_type::CHANY) {
          continue;
        }
        for (int x = rr_graph.node_xlow(node.inode);
             x <= rr_graph.node_xhigh(node.inode); ++x) {
          for (int y = rr_graph.node_ylow(node.inode);
               y <= rr_graph.node_yhigh(node.inode); ++y) {
            const auto found =
              kept.find(std::make_tuple(type == e_rr_type::CHANX, x, y));
            if (found != kept.end()) {
              crossed.insert(found->second);
            }
          }
        }
      }
      for (const size_t i : crossed) {
        spots[i].nets.push_back(net);
      }
    }
  }

  JsonWriter json;
  begin_response(json, "congestion", CONGESTION_SCHEMA_VERSION, "ROUTED");
  json.begin_object();
  write_grid(json);
  json.key("channel_width").begin_object();
  json.key("x").value(chan_width.x_max);
  json.key("y").value(chan_width.y_max);
  json.end_object();
  json.key("region_size").value(region_size);
  json.key("summary").begin_object();
  write_usage(json, "chanx", total.used_x, total.cap_x);
  write_usage(json, "chany", total.used_y, total.cap_y);
  json.key("overused_nodes").value(overused);
  json.end_object();
  json.key("regions").begin_array();
  for (const auto& entry : regions) {
    const Region& region = entry.second;
    if (0 == region.used_x && 0 == region.used_y) {
      continue;
    }
    json.begin_object();
    json.key("x_low").value(entry.first.first * region_size);
    json.key("y_low").value(entry.first.second * region_size);
    json.key("x_high").value(
      std::min(width, (entry.first.first + 1) * region_size) - 1);
    json.key("y_high").value(
      std::min(height, (entry.first.second + 1) * region_size) - 1);
    write_usage(json, "chanx", region.used_x, region.cap_x);
    write_usage(json, "chany", region.used_y, region.cap_y);
    json.end_object();
  }
  json.end_array();
  if (hotspots > 0) {
    const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
    json.key("hotspots").begin_array();
    for (const Hotspot& spot : spots) {
      json.begin_object();
      json.key("channel").value(spot.horizontal ? "chanx" : "chany");
      json.key("x").value(spot.x);
      json.key("y").value(spot.y);
      json.key("used").value(spot.used);
      json.key("capacity").value(spot.capacity);
      json.key("occupancy").value(spot.occupancy);
      json.key("overused_nodes").value(spot.overused);
      json.key("num_nets").value(static_cast<int64_t>(spot.nets.size()));
      json.key("nets").begin_array();
      for (size_t i = 0; i < std::min(spot.nets.size(), HOTSPOT_NETS_MAX);
           ++i) {
        json.value(clb_nlist.net_name(spot.nets[i]));
      }
      json.end_array();
      json.end_object();
    }
    json.end_array();
  }
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

bool cluster_report_json(const std::string& query, const bool with_stats,
                         const size_t limit, std::string& json_text) {
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  if (0 == clb_nlist.blocks().size()) {
    VTR_LOG_ERROR("The design has no packing: run the stage PACKED first\n");
    return false;
  }
  if (limit < 1) {
    VTR_LOG_ERROR("The limit must be at least 1\n");
    return false;
  }
  const std::string pattern = query.empty() ? "*" : query;

  std::vector<ClusterBlockId> matching;
  struct TypeStats {
    size_t count = 0;
    double input = 0., output = 0.;
  };
  std::map<std::string, TypeStats> by_type;
  /* As the report of the packing pin usage of VPR: the pins of the block in
   * the netlist, the clock pins being inputs, over the pins of the type */
  auto input_utils = [&](const ClusterBlockId blk) {
    const t_pb_type* pb_type = clb_nlist.block_type(blk)->pb_type;
    return ratio(clb_nlist.block_input_pins(blk).size() +
                   clb_nlist.block_clock_pins(blk).size(),
                 pb_type->num_input_pins + pb_type->num_clock_pins);
  };
  auto output_utils = [&](const ClusterBlockId blk) {
    return ratio(clb_nlist.block_output_pins(blk).size(),
                 clb_nlist.block_type(blk)->pb_type->num_output_pins);
  };
  for (const ClusterBlockId blk : clb_nlist.blocks()) {
    if (glob_match(pattern, clb_nlist.block_name(blk))) {
      matching.push_back(blk);
    }
    TypeStats& stats = by_type[clb_nlist.block_type(blk)->name];
    ++stats.count;
    stats.input += input_utils(blk);
    stats.output += output_utils(blk);
  }

  JsonWriter json;
  begin_response(json, "cluster_report", CLUSTER_REPORT_SCHEMA_VERSION,
                 "PACKED");
  json.begin_object();
  json.key("query").value(pattern);
  const size_t returned = std::min(limit, matching.size());
  json.key("total").value(static_cast<int64_t>(clb_nlist.blocks().size()));
  json.key("matched").value(static_cast<int64_t>(matching.size()));
  json.key("returned").value(static_cast<int64_t>(returned));
  json.key("truncated").value(returned < matching.size());
  json.key("clusters").begin_array();
  for (size_t i = 0; i < returned; ++i) {
    const ClusterBlockId blk = matching[i];
    json.begin_object();
    json.key("name").value(clb_nlist.block_name(blk));
    json.key("type").value(clb_nlist.block_type(blk)->name);
    json.key("input_pin_utils").value(input_utils(blk));
    json.key("output_pin_utils").value(output_utils(blk));
    json.end_object();
  }
  json.end_array();
  if (with_stats) {
    json.key("stats").begin_object();
    json.key("num_clusters")
      .value(static_cast<int64_t>(clb_nlist.blocks().size()));
    json.key("by_type").begin_array();
    for (const auto& entry : by_type) {
      const TypeStats& stats = entry.second;
      json.begin_object();
      json.key("type").value(entry.first);
      json.key("count").value(static_cast<int64_t>(stats.count));
      json.key("average_input_pin_utils").value(stats.input / stats.count);
      json.key("average_output_pin_utils").value(stats.output / stats.count);
      json.end_object();
    }
    json.end_array();
    json.end_object();
  }
  json.end_object();
  end_response(json);
  json_text = json.str();
  return true;
}

} /* End namespace openfpga */
