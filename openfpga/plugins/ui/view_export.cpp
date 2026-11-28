/********************************************************************
 * The data of the views of a design (see view_export.h)
 *******************************************************************/
#include "view_export.h"

#include <algorithm>
#include <cmath>
#include <deque>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_map>

#include "device_package.h"
#include "globals.h"
#include "json_writer.h"
#include "nlohmann/json.hpp"
#include "package_io_map.h"
#include "query_timing.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

constexpr float NO_SLACK = std::numeric_limits<float>::infinity();
/* The levels of LUTs that the module of an atom is looked for through */
constexpr int MODULE_SEARCH_DEPTH = 6;

/* The node types of the routing, as the codes of the column 'type' */
const char* const NODE_TYPES[] = {"SOURCE", "SINK",  "IPIN", "OPIN",
                                  "CHANX",  "CHANY", "OTHER"};

uint8_t node_type_code(const e_rr_type type) {
  switch (type) {
    case e_rr_type::SOURCE:
      return 0;
    case e_rr_type::SINK:
      return 1;
    case e_rr_type::IPIN:
      return 2;
    case e_rr_type::OPIN:
      return 3;
    case e_rr_type::CHANX:
      return 4;
    case e_rr_type::CHANY:
      return 5;
    default:
      return 6;
  }
}

/* The meta of a file: what the page needs besides the tables */
class Meta {
 public:
  Meta() { json_.begin_object(); }
  JsonWriter& key(const std::string& name) { return json_.key(name); }
  void grid() {
    const auto& grid = g_vpr_ctx.device().grid;
    json_.key("grid").begin_object();
    json_.key("width").value(static_cast<int64_t>(grid.width()));
    json_.key("height").value(static_cast<int64_t>(grid.height()));
    json_.key("layers").value(static_cast<int64_t>(grid.get_num_layers()));
    json_.key("layout").value(grid.name());
    json_.end_object();
  }
  std::string str() {
    json_.end_object();
    return json_.str();
  }

 private:
  JsonWriter json_;
};

bool device_made(std::string& error) {
  const auto& grid = g_vpr_ctx.device().grid;
  if (0 == grid.width() || 0 == grid.height()) {
    error = "The device has not been created: read the inputs first";
    return false;
  }
  return true;
}

bool packed(std::string& error) {
  if (0 == g_vpr_ctx.clustering().clb_nlist.blocks().size()) {
    error = "The design has no packing: run the stage PACKED first";
    return false;
  }
  return true;
}

bool placed() {
  return 0 != g_vpr_ctx.clustering().clb_nlist.blocks().size() &&
         g_vpr_ctx.placement().block_locs().size() ==
           g_vpr_ctx.clustering().clb_nlist.blocks().size();
}

bool routed(std::string& error) {
  if (g_vpr_ctx.routing().route_trees.empty()) {
    error = "The design has no routing: run the stage ROUTED first";
    return false;
  }
  return true;
}

/* The routing of a net of the clusters (by its atom net when the routing is
 * flat), or null */
const RouteTree* net_route(const ClusterNetId net) {
  const auto& route_trees = g_vpr_ctx.routing().route_trees;
  size_t index = size_t(net);
  if (g_vpr_ctx.routing().is_flat) {
    const AtomNetId atom_net = g_vpr_ctx.atom().lookup().atom_net(net);
    if (!atom_net.is_valid()) {
      return nullptr;
    }
    index = size_t(atom_net);
  }
  if (index >= route_trees.size() || !route_trees[ParentNetId(index)]) {
    return nullptr;
  }
  return &(*route_trees[ParentNetId(index)]);
}

std::vector<std::string> block_type_names() {
  std::vector<std::string> names;
  for (const t_logical_block_type& type :
       g_vpr_ctx.device().logical_block_types) {
    names.push_back(type.name);
  }
  return names;
}

/* The tiles of a rectangle of the grid (all of it by default): the root
 * location of each, its size and its type */
void tiles_table(ViewFile& file, int x0, int y0, int x1, int y1) {
  const auto& grid = g_vpr_ctx.device().grid;
  std::vector<uint16_t> x, y, w, h, type;
  std::vector<uint8_t> layer;
  for (size_t l = 0; l < grid.get_num_layers(); ++l) {
    for (int i = x0; i < x1; ++i) {
      for (int j = y0; j < y1; ++j) {
        const t_physical_tile_loc loc(i, j, static_cast<int>(l));
        if (!grid.is_root_location(loc)) {
          continue;
        }
        const t_physical_tile_type_ptr tile = grid.get_physical_type(loc);
        if (tile->is_empty()) {
          continue;
        }
        x.push_back(static_cast<uint16_t>(i));
        y.push_back(static_cast<uint16_t>(j));
        w.push_back(static_cast<uint16_t>(tile->width));
        h.push_back(static_cast<uint16_t>(tile->height));
        type.push_back(static_cast<uint16_t>(tile->index));
        layer.push_back(static_cast<uint8_t>(l));
      }
    }
  }
  file.table("tiles", x.size())
    .column("x", x)
    .column("y", y)
    .column("w", w)
    .column("h", h)
    .column("type", type)
    .column("layer", layer);
}

void tile_types_table(ViewFile& file) {
  const auto& types = g_vpr_ctx.device().physical_tile_types;
  std::vector<std::string> name;
  std::vector<uint16_t> width, height, capacity;
  std::vector<uint8_t> is_io, is_empty;
  std::vector<uint16_t> sub_type, sub_capacity, sub_low;
  std::vector<std::string> sub_name;
  for (const t_physical_tile_type& tile : types) {
    name.push_back(tile.name);
    width.push_back(static_cast<uint16_t>(tile.width));
    height.push_back(static_cast<uint16_t>(tile.height));
    capacity.push_back(static_cast<uint16_t>(tile.capacity));
    is_io.push_back(tile.is_io() ? 1 : 0);
    is_empty.push_back(tile.is_empty() ? 1 : 0);
    for (const t_sub_tile& sub : tile.sub_tiles) {
      sub_type.push_back(static_cast<uint16_t>(tile.index));
      sub_name.push_back(sub.name);
      sub_low.push_back(static_cast<uint16_t>(sub.capacity.low));
      sub_capacity.push_back(
        static_cast<uint16_t>(sub.capacity.high - sub.capacity.low + 1));
    }
  }
  file.table("tile_types", name.size())
    .strings("name", name)
    .column("width", width)
    .column("height", height)
    .column("capacity", capacity)
    .column("is_io", is_io)
    .column("is_empty", is_empty);
  file.table("sub_tiles", sub_name.size())
    .column("tile_type", sub_type)
    .strings("name", sub_name)
    .column("slot_low", sub_low)
    .column("capacity", sub_capacity);
}

/* The use of each segment of the channels: the wires in use that cross the
 * place (x, y) of a channel, by row (y * width + x) */
struct ChannelUse {
  std::vector<uint16_t> used_x, used_y, cap_x, cap_y;
  std::vector<uint32_t> node_users; /* the nets that use each node */
};

ChannelUse channel_use() {
  const auto& device_ctx = g_vpr_ctx.device();
  const auto& rr_graph = device_ctx.rr_graph;
  const int width = static_cast<int>(device_ctx.grid.width());
  const int height = static_cast<int>(device_ctx.grid.height());
  ChannelUse use;
  use.used_x.assign(width * height, 0);
  use.used_y.assign(width * height, 0);
  use.cap_x.assign(width * height, 0);
  use.cap_y.assign(width * height, 0);
  use.node_users.assign(rr_graph.num_nodes(), 0);
  for (const auto& tree : g_vpr_ctx.routing().route_trees) {
    if (!tree) {
      continue;
    }
    for (const RouteTreeNode& node : tree->all_nodes()) {
      ++use.node_users[size_t(node.inode)];
    }
  }
  for (const RRNodeId inode : rr_graph.nodes()) {
    if (0 == use.node_users[size_t(inode)]) {
      continue;
    }
    const e_rr_type type = rr_graph.node_type(inode);
    if (type != e_rr_type::CHANX && type != e_rr_type::CHANY) {
      continue;
    }
    auto& used = e_rr_type::CHANX == type ? use.used_x : use.used_y;
    for (int x = rr_graph.node_xlow(inode); x <= rr_graph.node_xhigh(inode);
         ++x) {
      for (int y = rr_graph.node_ylow(inode); y <= rr_graph.node_yhigh(inode);
           ++y) {
        if (x < width && y < height && used[y * width + x] < 0xffff) {
          ++used[y * width + x];
        }
      }
    }
  }
  /* The channels there are, as the query congestion counts them */
  const auto& chan_width = device_ctx.chan_width;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (y <= height - 2 && x >= 1 && x <= width - 2) {
        use.cap_x[y * width + x] = static_cast<uint16_t>(chan_width.x_list[y]);
      }
      if (x <= width - 2 && y >= 1 && y <= height - 2) {
        use.cap_y[y * width + x] = static_cast<uint16_t>(chan_width.y_list[x]);
      }
    }
  }
  return use;
}

/* What the tiles of a version read again and again: made once a version */
struct TileCache {
  std::string version;
  /* routing: the nodes of every route, and the nodes of each level-0 tile */
  std::vector<uint32_t> node_net;
  std::vector<int32_t> node_index, node_parent;
  std::vector<uint8_t> node_type, node_layer, node_sink;
  std::vector<uint16_t> node_xlow, node_ylow, node_xhigh, node_yhigh, node_ptc;
  std::vector<std::vector<uint32_t>> route_buckets;
  /* channels: the wires of each level-0 tile */
  std::vector<std::vector<uint32_t>> wire_buckets;
  ChannelUse use;
  /* timing */
  std::vector<float> net_slack, net_criticality, block_slack;
  int tiles_x = 0;
  int tiles_y = 0;
};

TileCache& tile_cache(const std::string& kind) {
  static std::map<std::string, TileCache> caches;
  return caches[kind];
}

/* The level-0 tiles that a rectangle of grid tiles touches */
template <typename F>
void each_tile(int xlow, int ylow, int xhigh, int yhigh, int tiles_x,
               int tiles_y, F f) {
  const int t0x = std::max(0, xlow / VIEW_TILE_SIZE);
  const int t0y = std::max(0, ylow / VIEW_TILE_SIZE);
  const int t1x = std::min(tiles_x - 1, xhigh / VIEW_TILE_SIZE);
  const int t1y = std::min(tiles_y - 1, yhigh / VIEW_TILE_SIZE);
  for (int j = t0y; j <= t1y; ++j) {
    for (int i = t0x; i <= t1x; ++i) {
      f(j * tiles_x + i);
    }
  }
}

void make_routing_cache(TileCache& cache) {
  const auto& rr_graph = g_vpr_ctx.device().rr_graph;
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const auto& grid = g_vpr_ctx.device().grid;
  cache.tiles_x =
    (static_cast<int>(grid.width()) + VIEW_TILE_SIZE - 1) / VIEW_TILE_SIZE;
  cache.tiles_y =
    (static_cast<int>(grid.height()) + VIEW_TILE_SIZE - 1) / VIEW_TILE_SIZE;
  cache.route_buckets.assign(cache.tiles_x * cache.tiles_y, {});
  for (const ClusterNetId net : clb_nlist.nets()) {
    const RouteTree* tree = net_route(net);
    if (nullptr == tree) {
      continue;
    }
    std::map<const RouteTreeNode*, int32_t> index;
    for (const RouteTreeNode& node : tree->all_nodes()) {
      const int32_t here = static_cast<int32_t>(index.size());
      index[&node] = here;
      const RRNodeId inode = node.inode;
      const auto parent = node.parent();
      const uint32_t id = static_cast<uint32_t>(cache.node_net.size());
      cache.node_net.push_back(static_cast<uint32_t>(size_t(net)));
      cache.node_index.push_back(here);
      cache.node_parent.push_back(parent ? index.at(&(*parent)) : -1);
      cache.node_type.push_back(node_type_code(rr_graph.node_type(inode)));
      cache.node_xlow.push_back(rr_graph.node_xlow(inode));
      cache.node_ylow.push_back(rr_graph.node_ylow(inode));
      cache.node_xhigh.push_back(rr_graph.node_xhigh(inode));
      cache.node_yhigh.push_back(rr_graph.node_yhigh(inode));
      cache.node_ptc.push_back(
        static_cast<uint16_t>(rr_graph.node_ptc_num(inode)));
      cache.node_layer.push_back(
        static_cast<uint8_t>(rr_graph.node_layer_low(inode)));
      cache.node_sink.push_back(node.net_pin_index > 0 ? 1 : 0);
      each_tile(rr_graph.node_xlow(inode), rr_graph.node_ylow(inode),
                rr_graph.node_xhigh(inode), rr_graph.node_yhigh(inode),
                cache.tiles_x, cache.tiles_y,
                [&](int t) { cache.route_buckets[t].push_back(id); });
    }
  }
  cache.use = channel_use();
}

void make_channel_cache(TileCache& cache) {
  const auto& rr_graph = g_vpr_ctx.device().rr_graph;
  const auto& grid = g_vpr_ctx.device().grid;
  cache.tiles_x =
    (static_cast<int>(grid.width()) + VIEW_TILE_SIZE - 1) / VIEW_TILE_SIZE;
  cache.tiles_y =
    (static_cast<int>(grid.height()) + VIEW_TILE_SIZE - 1) / VIEW_TILE_SIZE;
  cache.wire_buckets.assign(cache.tiles_x * cache.tiles_y, {});
  for (const RRNodeId inode : rr_graph.nodes()) {
    const e_rr_type type = rr_graph.node_type(inode);
    if (type != e_rr_type::CHANX && type != e_rr_type::CHANY) {
      continue;
    }
    each_tile(
      rr_graph.node_xlow(inode), rr_graph.node_ylow(inode),
      rr_graph.node_xhigh(inode), rr_graph.node_yhigh(inode), cache.tiles_x,
      cache.tiles_y, [&](int t) {
        cache.wire_buckets[t].push_back(static_cast<uint32_t>(size_t(inode)));
      });
  }
  cache.use = channel_use();
}

/* The worst slack of each cluster: of the nets on its pins */
std::vector<float> block_slacks(const std::vector<float>& net_slack) {
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  std::vector<float> slack(clb_nlist.blocks().size(), NO_SLACK);
  for (const ClusterBlockId blk : clb_nlist.blocks()) {
    float worst = NO_SLACK;
    for (const ClusterPinId pin : clb_nlist.block_pins(blk)) {
      const ClusterNetId net = clb_nlist.pin_net(pin);
      if (net.is_valid() && size_t(net) < net_slack.size()) {
        worst = std::min(worst, net_slack[size_t(net)]);
      }
    }
    slack[size_t(blk)] = worst;
  }
  return slack;
}

bool make_timing_cache(const vpr::VprRun& run, const std::string& version,
                       TileCache& cache, std::string& error) {
  if (!net_slacks(run, version, cache.net_slack, cache.net_criticality)) {
    error = "The timing cannot be analyzed (see the log)";
    return false;
  }
  cache.block_slack = block_slacks(cache.net_slack);
  return true;
}

/* ----- whole exports ---------------------------------------------------- */

bool device_view(ViewFile& file, std::string& error) {
  if (!device_made(error)) {
    return false;
  }
  const auto& device_ctx = g_vpr_ctx.device();
  const auto& grid = device_ctx.grid;
  tile_types_table(file);
  tiles_table(file, 0, 0, static_cast<int>(grid.width()),
              static_cast<int>(grid.height()));
  /* The width of each channel: the horizontal ones by row, the vertical ones
   * by column */
  const auto& chan_width = device_ctx.chan_width;
  std::vector<uint16_t> x_width, y_width;
  for (const int w : chan_width.x_list) {
    x_width.push_back(static_cast<uint16_t>(std::max(0, w)));
  }
  for (const int w : chan_width.y_list) {
    y_width.push_back(static_cast<uint16_t>(std::max(0, w)));
  }
  file.table("chanx", x_width.size()).column("width", x_width);
  file.table("chany", y_width.size()).column("width", y_width);
  Meta meta;
  meta.grid();
  meta.key("channel_width").begin_object();
  meta.key("x").value(static_cast<int64_t>(chan_width.x_max));
  meta.key("y").value(static_cast<int64_t>(chan_width.y_max));
  meta.key("fixed").value(chan_width.x_max > 0).end_object();
  file.set_meta(meta.str());
  return true;
}

void blocks_table(ViewFile& file, const std::vector<ClusterBlockId>& blocks) {
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const bool has_locs = placed();
  const auto& block_locs = g_vpr_ctx.placement().block_locs();
  std::vector<std::string> name;
  std::vector<uint16_t> type;
  std::vector<int32_t> x, y, sub_tile;
  std::vector<uint8_t> layer, fixed;
  std::vector<uint32_t> id;
  for (const ClusterBlockId blk : blocks) {
    name.push_back(clb_nlist.block_name(blk));
    id.push_back(static_cast<uint32_t>(size_t(blk)));
    type.push_back(static_cast<uint16_t>(clb_nlist.block_type(blk)->index));
    if (has_locs) {
      const t_pl_loc& loc = block_locs[blk].loc;
      x.push_back(loc.x);
      y.push_back(loc.y);
      sub_tile.push_back(loc.sub_tile);
      layer.push_back(static_cast<uint8_t>(loc.layer));
      fixed.push_back(block_locs[blk].is_fixed ? 1 : 0);
    } else {
      x.push_back(-1);
      y.push_back(-1);
      sub_tile.push_back(-1);
      layer.push_back(0);
      fixed.push_back(0);
    }
  }
  file.table("blocks", name.size())
    .strings("name", name)
    .column("id", id)
    .column("type", type)
    .column("x", x)
    .column("y", y)
    .column("sub_tile", sub_tile)
    .column("layer", layer)
    .column("fixed", fixed);
}

bool placement_view(ViewFile& file, std::string& error) {
  if (!packed(error)) {
    return false;
  }
  if (!placed()) {
    error = "The design has no placement: run the stage PLACED first";
    return false;
  }
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  std::vector<ClusterBlockId> blocks(clb_nlist.blocks().begin(),
                                     clb_nlist.blocks().end());
  file.table("block_types", block_type_names().size())
    .strings("name", block_type_names());
  blocks_table(file, blocks);
  /* The regions of the constraints of the placement */
  const auto& constraints = g_vpr_ctx.floorplanning().constraints;
  std::vector<std::string> partition_name;
  std::vector<uint32_t> region_partition;
  std::vector<int32_t> xmin, ymin, xmax, ymax;
  for (int i = 0; i < constraints.get_num_partitions(); ++i) {
    const Partition& partition = constraints.get_partition(PartitionId(i));
    partition_name.push_back(partition.get_name());
    for (const Region& region : partition.get_part_region().get_regions()) {
      const vtr::Rect<int>& rect = region.get_rect();
      region_partition.push_back(static_cast<uint32_t>(i));
      xmin.push_back(rect.xmin());
      ymin.push_back(rect.ymin());
      xmax.push_back(rect.xmax());
      ymax.push_back(rect.ymax());
    }
  }
  file.table("partitions", partition_name.size())
    .strings("name", partition_name);
  file.table("regions", region_partition.size())
    .column("partition", region_partition)
    .column("xmin", xmin)
    .column("ymin", ymin)
    .column("xmax", xmax)
    .column("ymax", ymax);
  Meta meta;
  meta.grid();
  file.set_meta(meta.str());
  return true;
}

/* The pins of the package on each site, from the I/O map of the device
 * package of the session (role io_map): the aliases of the rows at the site,
 * by the index of the site. Empty when there is no map */
std::vector<std::string> package_pins(
  const OpenFpgaSession& session,
  const std::map<std::tuple<int, int, int, int>, int32_t>& site_of,
  size_t sites, std::string& map_file) {
  std::vector<std::string> pins(sites);
  const std::string data = session.device_package();
  if (data.empty()) {
    return pins;
  }
  try {
    const DevicePackage package = load_device_package(
      nlohmann::json::parse(data)["directory"].get<std::string>(), false);
    const std::vector<std::filesystem::path> maps = package.files_of("io_map");
    if (maps.empty()) {
      return pins;
    }
    const PackageIoMap map = read_package_io_map(maps[0]);
    map_file = maps[0].filename().string();
    for (const PackageIoMapRow& row : map.rows) {
      if (!row.has_location()) {
        continue;
      }
      const auto found = site_of.find(
        {row.col.value(), row.row.value(), row.subtile.value_or(0), 0});
      if (found == site_of.end()) {
        continue;
      }
      const std::string& name = row.customer_pin_alias.empty()
                                  ? row.netlist_name
                                  : row.customer_pin_alias;
      std::string& here = pins[found->second];
      if (!name.empty() &&
          std::string::npos == ("/" + here + "/").find("/" + name + "/")) {
        here += (here.empty() ? "" : "/") + name;
      }
    }
  } catch (const std::exception& error) {
    VTR_LOG_WARN("The I/O map of the device package cannot be read: %s\n",
                 error.what());
  }
  return pins;
}

bool pins_view(const OpenFpgaSession& session, ViewFile& file,
               std::string& error) {
  if (!device_made(error)) {
    return false;
  }
  const auto& grid = g_vpr_ctx.device().grid;
  const int width = static_cast<int>(grid.width());
  const int height = static_cast<int>(grid.height());
  /* The sites: a slot of an I/O tile */
  std::vector<uint16_t> sx, sy, slot;
  std::vector<uint8_t> side, slayer;
  std::map<std::tuple<int, int, int, int>, int32_t> site_of;
  for (size_t l = 0; l < grid.get_num_layers(); ++l) {
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        const t_physical_tile_loc loc(x, y, static_cast<int>(l));
        if (!grid.is_root_location(loc)) {
          continue;
        }
        const t_physical_tile_type_ptr tile = grid.get_physical_type(loc);
        if (!tile->is_io()) {
          continue;
        }
        const uint8_t which = 0 == x            ? 0
                              : width - 1 == x  ? 1
                              : 0 == y          ? 2
                              : height - 1 == y ? 3
                                                : 4;
        for (int s = 0; s < tile->capacity; ++s) {
          site_of[{x, y, s, static_cast<int>(l)}] =
            static_cast<int32_t>(sx.size());
          sx.push_back(static_cast<uint16_t>(x));
          sy.push_back(static_cast<uint16_t>(y));
          slot.push_back(static_cast<uint16_t>(s));
          side.push_back(which);
          slayer.push_back(static_cast<uint8_t>(l));
        }
      }
    }
  }
  std::string map_file;
  const std::vector<std::string> package_pin =
    package_pins(session, site_of, sx.size(), map_file);
  file.table("sites", sx.size())
    .strings("package_pin", package_pin)
    .column("x", sx)
    .column("y", sy)
    .column("sub_tile", slot)
    .column("side", side)
    .column("layer", slayer);
  /* The ports: the pads of the atom netlist, on their site once placed */
  const auto& netlist = g_vpr_ctx.atom().netlist();
  const auto& lookup = g_vpr_ctx.atom().lookup();
  const bool has_locs = placed();
  const auto& block_locs = g_vpr_ctx.placement().block_locs();
  std::vector<std::string> name, block;
  std::vector<uint8_t> dir, fixed;
  std::vector<int32_t> site, cluster;
  for (const AtomBlockId blk : netlist.blocks()) {
    const AtomBlockType type = netlist.block_type(blk);
    if (type != AtomBlockType::INPAD && type != AtomBlockType::OUTPAD) {
      continue;
    }
    std::string port = netlist.block_name(blk);
    if (AtomBlockType::OUTPAD == type && 0 == port.rfind("out:", 0)) {
      port = port.substr(4);
    }
    name.push_back(port);
    dir.push_back(AtomBlockType::INPAD == type ? 0 : 1);
    const ClusterBlockId clb = lookup.atom_clb(blk);
    cluster.push_back(clb.is_valid() ? static_cast<int32_t>(size_t(clb)) : -1);
    block.push_back(clb.is_valid()
                      ? g_vpr_ctx.clustering().clb_nlist.block_name(clb)
                      : std::string());
    int32_t where = -1;
    uint8_t is_fixed = 0;
    if (has_locs && clb.is_valid()) {
      const t_pl_loc& loc = block_locs[clb].loc;
      const auto found = site_of.find({loc.x, loc.y, loc.sub_tile, loc.layer});
      where = site_of.end() == found ? -1 : found->second;
      is_fixed = block_locs[clb].is_fixed ? 1 : 0;
    }
    site.push_back(where);
    fixed.push_back(is_fixed);
  }
  file.table("ports", name.size())
    .strings("name", name)
    .column("dir", dir)
    .column("site", site)
    .column("fixed", fixed)
    .column("cluster", cluster)
    .strings("block", block);
  Meta meta;
  meta.grid();
  meta.key("placed").value(has_locs);
  if (map_file.empty()) {
    meta.key("io_map_file").null();
  } else {
    meta.key("io_map_file").value(map_file);
  }
  JsonWriter& sides = meta.key("sides").begin_array();
  for (const char* s : {"left", "right", "bottom", "top", "inside"}) {
    sides.value(s);
  }
  sides.end_array();
  file.set_meta(meta.str());
  return true;
}

void node_types_meta(Meta& meta) {
  JsonWriter& types = meta.key("node_types").begin_array();
  for (const char* type : NODE_TYPES) {
    types.value(type);
  }
  types.end_array();
}

bool routing_view(ViewFile& file, std::string& error) {
  if (!packed(error) || !routed(error)) {
    return false;
  }
  TileCache cache;
  make_routing_cache(cache);
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  /* The nets with a route, and their nodes in a row */
  std::vector<std::string> name;
  std::vector<uint32_t> start, count, index;
  for (size_t i = 0; i < cache.node_net.size();) {
    const uint32_t net = cache.node_net[i];
    size_t j = i;
    while (j < cache.node_net.size() && cache.node_net[j] == net) {
      ++j;
    }
    name.push_back(clb_nlist.net_name(ClusterNetId(net)));
    index.push_back(net);
    start.push_back(static_cast<uint32_t>(i));
    count.push_back(static_cast<uint32_t>(j - i));
    i = j;
  }
  file.table("nets", name.size())
    .strings("name", name)
    .column("net", index)
    .column("node_start", start)
    .column("node_count", count);
  file.table("nodes", cache.node_net.size())
    .column("type", cache.node_type)
    .column("xlow", cache.node_xlow)
    .column("ylow", cache.node_ylow)
    .column("xhigh", cache.node_xhigh)
    .column("yhigh", cache.node_yhigh)
    .column("ptc", cache.node_ptc)
    .column("layer", cache.node_layer)
    .column("parent", cache.node_parent)
    .column("sink", cache.node_sink);
  file.table("chanx_use", cache.use.used_x.size())
    .column("used", cache.use.used_x)
    .column("capacity", cache.use.cap_x);
  file.table("chany_use", cache.use.used_y.size())
    .column("used", cache.use.used_y)
    .column("capacity", cache.use.cap_y);
  Meta meta;
  meta.grid();
  meta.key("flat_routing").value(g_vpr_ctx.routing().is_flat);
  node_types_meta(meta);
  file.set_meta(meta.str());
  return true;
}

bool timing_view(const vpr::VprRun& run, const std::string& version,
                 const size_t paths, ViewFile& file, std::string& error) {
  if (!packed(error) || !routed(error)) {
    return false;
  }
  if (paths < 1 || paths > CRITICAL_PATHS_MAX) {
    error = "The number of paths must be between 1 and " +
            std::to_string(CRITICAL_PATHS_MAX);
    return false;
  }
  /* The tables of the paths first: the file has the nets, the blocks, then
   * them */
  ViewFile paths_file("timing", "");
  std::vector<float> slack, criticality;
  if (!timing_view_tables(run, version, paths, paths_file, slack,
                          criticality)) {
    error = "The timing cannot be analyzed (see the log)";
    return false;
  }
  /* No names: the row of a net is its index, as the column 'net' of routing
   * and of clusters gives it; the row of a block is its index, as the column
   * 'id' of the blocks of placement and of clusters gives it */
  file.table("nets", slack.size())
    .column("slack_ns", slack)
    .column("criticality", criticality);
  const std::vector<float> block = block_slacks(slack);
  file.table("blocks", block.size()).column("slack_ns", block);
  for (const ViewTable& table : paths_file.tables()) {
    file.add_table(table);
  }
  Meta meta;
  meta.grid();
  meta.key("nets").value("by the index of the net (routing and clusters: net)");
  meta.key("atom_nets")
    .value("by the index of the net of the atoms (atoms: id)");
  meta.key("blocks").value(
    "by the index of the block (placement and clusters: id)");
  file.set_meta(meta.str());
  return true;
}

/* ----- the atoms and their modules -------------------------------------- */

struct AtomModules {
  std::vector<std::string> modules;     /* by index */
  std::vector<uint32_t> module_of_atom; /* by atom */
  std::vector<uint8_t> how;             /* 0 net, 1 connectivity, 2 common,
                                           3 none */
};

std::string common_module(const std::set<std::string>& found) {
  std::vector<std::vector<std::string>> parts;
  for (const std::string& m : found) {
    std::vector<std::string> words;
    std::stringstream stream(m);
    std::string word;
    while (std::getline(stream, word, '.')) {
      words.push_back(word);
    }
    parts.push_back(words);
  }
  std::string common;
  for (size_t level = 0;; ++level) {
    if (parts.empty() || level >= parts[0].size()) {
      break;
    }
    const std::string& word = parts[0][level];
    bool all = true;
    for (const auto& p : parts) {
      all = all && level < p.size() && p[level] == word;
    }
    if (!all) {
      break;
    }
    common += (common.empty() ? "" : ".") + word;
  }
  return common;
}

AtomModules atom_modules() {
  const auto& netlist = g_vpr_ctx.atom().netlist();
  const size_t count = netlist.blocks().size();
  AtomModules result;
  std::map<std::string, uint32_t> index;
  auto intern = [&](const std::string& m) {
    const auto found = index.find(m);
    if (found != index.end()) {
      return found->second;
    }
    const uint32_t i = static_cast<uint32_t>(result.modules.size());
    index[m] = i;
    result.modules.push_back(m);
    return i;
  };
  intern("");
  std::vector<int> named(count, -1);
  for (const AtomBlockId blk : netlist.blocks()) {
    std::string name = netlist.block_name(blk);
    if (0 == name.rfind("out:", 0)) {
      name = name.substr(4);
    }
    std::string module;
    if (module_of_name(name, module)) {
      named[size_t(blk)] = static_cast<int>(intern(module));
    }
  }
  result.module_of_atom.assign(count, 0);
  result.how.assign(count, 3);
  /* The modules of the named atoms that an atom reaches, level by level */
  auto reach = [&](const AtomBlockId start, const bool forward) {
    std::set<std::string> found;
    std::vector<AtomBlockId> frontier = {start};
    std::set<AtomBlockId> seen = {start};
    for (int level = 0; level < MODULE_SEARCH_DEPTH && found.empty(); ++level) {
      std::vector<AtomBlockId> next;
      for (const AtomBlockId blk : frontier) {
        for (const AtomPinId pin : netlist.block_pins(blk)) {
          const bool out = PinType::DRIVER == netlist.pin_type(pin);
          if (out != forward) {
            continue;
          }
          const AtomNetId net = netlist.pin_net(pin);
          if (!net.is_valid()) {
            continue;
          }
          std::vector<AtomBlockId> neighbours;
          if (forward) {
            for (const AtomPinId sink : netlist.net_sinks(net)) {
              neighbours.push_back(netlist.pin_block(sink));
            }
          } else {
            const AtomPinId driver = netlist.net_driver(net);
            if (driver.is_valid()) {
              neighbours.push_back(netlist.pin_block(driver));
            }
          }
          for (const AtomBlockId other : neighbours) {
            if (!seen.insert(other).second) {
              continue;
            }
            if (named[size_t(other)] >= 0) {
              found.insert(result.modules[named[size_t(other)]]);
            } else {
              next.push_back(other);
            }
          }
        }
      }
      frontier = std::move(next);
    }
    return found;
  };
  for (const AtomBlockId blk : netlist.blocks()) {
    const size_t i = size_t(blk);
    if (named[i] >= 0) {
      result.module_of_atom[i] = static_cast<uint32_t>(named[i]);
      result.how[i] = 0;
      continue;
    }
    std::set<std::string> found = reach(blk, true);
    if (found.empty()) {
      found = reach(blk, false);
    }
    if (found.empty()) {
      result.module_of_atom[i] = 0;
      result.how[i] = 3;
    } else if (1 == found.size()) {
      result.module_of_atom[i] = intern(*found.begin());
      result.how[i] = 1;
    } else {
      result.module_of_atom[i] = intern(common_module(found));
      result.how[i] = 2;
    }
  }
  return result;
}

bool atoms_view(const ViewRequest& request, ViewFile& file,
                std::string& error) {
  const auto& atom_ctx = g_vpr_ctx.atom();
  const auto& netlist = atom_ctx.netlist();
  if (0 == netlist.blocks().size()) {
    error = "There is no circuit: read one first";
    return false;
  }
  const AtomModules modules = atom_modules();
  /* The atoms of the scope */
  std::vector<AtomBlockId> scope;
  std::string scope_text = "all";
  if (request.has_module) {
    const std::string& m = request.module;
    scope_text = "module " + (m.empty() ? std::string("(top)") : m);
    for (const AtomBlockId blk : netlist.blocks()) {
      const std::string& mine =
        modules.modules[modules.module_of_atom[size_t(blk)]];
      if (m.empty() || mine == m || 0 == mine.rfind(m + ".", 0)) {
        scope.push_back(blk);
      }
    }
    if (scope.empty()) {
      error = "No atom is in the module '" + m + "'";
      return false;
    }
  } else if (!request.cone.empty()) {
    scope_text =
      "cone " + request.cone + " depth " + std::to_string(request.depth);
    std::set<AtomBlockId> in;
    std::vector<AtomBlockId> frontier;
    const AtomBlockId blk = netlist.find_block(request.cone);
    if (blk.is_valid()) {
      frontier.push_back(blk);
    } else {
      const AtomNetId net = netlist.find_net(request.cone);
      if (!net.is_valid()) {
        error = "There is no atom or net '" + request.cone + "'";
        return false;
      }
      for (const AtomPinId pin : netlist.net_pins(net)) {
        frontier.push_back(netlist.pin_block(pin));
      }
    }
    in.insert(frontier.begin(), frontier.end());
    for (int level = 0; level < request.depth; ++level) {
      std::vector<AtomBlockId> next;
      for (const AtomBlockId b : frontier) {
        for (const AtomPinId pin : netlist.block_pins(b)) {
          const AtomNetId net = netlist.pin_net(pin);
          if (!net.is_valid()) {
            continue;
          }
          for (const AtomPinId other : netlist.net_pins(net)) {
            const AtomBlockId o = netlist.pin_block(other);
            if (in.insert(o).second) {
              next.push_back(o);
            }
          }
        }
      }
      frontier = std::move(next);
    }
    scope.assign(in.begin(), in.end());
  } else {
    scope.assign(netlist.blocks().begin(), netlist.blocks().end());
  }
  if (scope.size() > request.limit) {
    error = "The scope has " + std::to_string(scope.size()) +
            " atoms, more than " + std::to_string(request.limit) +
            ": give --module or --cone (or a smaller one)";
    return false;
  }
  /* The tables */
  std::map<LogicalModelId, uint16_t> model_index;
  std::vector<std::string> model_names;
  const auto& models = g_vpr_ctx.device().arch->models;
  std::map<AtomBlockId, uint32_t> atom_index;
  for (const AtomBlockId blk : scope) {
    atom_index[blk] = static_cast<uint32_t>(atom_index.size());
  }
  std::map<AtomNetId, uint32_t> net_index;
  std::vector<AtomNetId> nets;
  std::vector<std::string> aname;
  std::vector<uint16_t> amodel;
  std::vector<uint32_t> amodule, pin_start, pin_count;
  std::vector<uint8_t> ahow;
  std::vector<int32_t> acluster;
  std::vector<uint32_t> patom, pnet;
  std::vector<std::string> pport;
  std::vector<uint16_t> pbit;
  std::vector<uint8_t> pdir;
  std::map<AtomPinId, uint32_t> pin_index;
  for (const AtomBlockId blk : scope) {
    aname.push_back(netlist.block_name(blk));
    const LogicalModelId model = netlist.block_model(blk);
    auto found = model_index.find(model);
    if (found == model_index.end()) {
      found =
        model_index.emplace(model, static_cast<uint16_t>(model_names.size()))
          .first;
      model_names.push_back(models.get_model(model).name);
    }
    amodel.push_back(found->second);
    amodule.push_back(modules.module_of_atom[size_t(blk)]);
    ahow.push_back(modules.how[size_t(blk)]);
    const ClusterBlockId clb = atom_ctx.lookup().atom_clb(blk);
    acluster.push_back(clb.is_valid() ? static_cast<int32_t>(size_t(clb)) : -1);
    pin_start.push_back(static_cast<uint32_t>(patom.size()));
    for (const AtomPinId pin : netlist.block_pins(blk)) {
      const AtomNetId net = netlist.pin_net(pin);
      if (!net.is_valid()) {
        continue;
      }
      auto n = net_index.find(net);
      if (n == net_index.end()) {
        n = net_index.emplace(net, static_cast<uint32_t>(nets.size())).first;
        nets.push_back(net);
      }
      pin_index[pin] = static_cast<uint32_t>(patom.size());
      patom.push_back(atom_index[blk]);
      pport.push_back(netlist.port_name(netlist.pin_port(pin)));
      pbit.push_back(static_cast<uint16_t>(netlist.pin_port_bit(pin)));
      const PortType type = netlist.pin_port_type(pin);
      pdir.push_back(PortType::OUTPUT == type  ? 1
                     : PortType::CLOCK == type ? 2
                                               : 0);
      pnet.push_back(n->second);
    }
    pin_count.push_back(static_cast<uint32_t>(patom.size()) - pin_start.back());
  }
  std::vector<std::string> nname;
  std::vector<int32_t> ndriver;
  std::vector<uint32_t> npins, ninside, nid;
  for (const AtomNetId net : nets) {
    nname.push_back(netlist.net_name(net));
    nid.push_back(static_cast<uint32_t>(size_t(net)));
    const AtomPinId driver = netlist.net_driver(net);
    const auto d = pin_index.find(driver);
    ndriver.push_back(d == pin_index.end() ? -1
                                           : static_cast<int32_t>(d->second));
    npins.push_back(static_cast<uint32_t>(netlist.net_pins(net).size()));
    uint32_t inside = 0;
    for (const AtomPinId pin : netlist.net_pins(net)) {
      inside += pin_index.count(pin) ? 1 : 0;
    }
    ninside.push_back(inside);
  }
  file.table("models", model_names.size()).strings("name", model_names);
  file.table("modules", modules.modules.size())
    .strings("name", modules.modules);
  file.table("atoms", aname.size())
    .strings("name", aname)
    .column("model", amodel)
    .column("module", amodule)
    .column("how", ahow)
    .column("cluster", acluster)
    .column("pin_start", pin_start)
    .column("pin_count", pin_count);
  file.table("pins", patom.size())
    .column("atom", patom)
    .strings("port", pport)
    .column("bit", pbit)
    .column("dir", pdir)
    .column("net", pnet);
  file.table("nets", nname.size())
    .strings("name", nname)
    .column("driver", ndriver)
    .column("pins", npins)
    .column("inside", ninside)
    .column("id", nid);
  /* How the modules are known, over the whole design */
  int64_t by[4] = {0, 0, 0, 0};
  for (const uint8_t h : modules.how) {
    ++by[h];
  }
  Meta meta;
  meta.key("scope").value(scope_text);
  meta.key("total_atoms").value(static_cast<int64_t>(netlist.blocks().size()));
  meta.key("module_by").begin_object();
  meta.key("net").value(by[0]);
  meta.key("connectivity").value(by[1]);
  meta.key("common").value(by[2]);
  meta.key("none").value(by[3]).end_object();
  file.set_meta(meta.str());
  return true;
}

bool clusters_view(const ViewRequest& request, ViewFile& file,
                   std::string& error) {
  if (!packed(error)) {
    return false;
  }
  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const auto& atom_ctx = g_vpr_ctx.atom();
  const auto& netlist = atom_ctx.netlist();
  std::vector<ClusterBlockId> scope;
  if (!request.blocks.empty()) {
    for (const std::string& name : request.blocks) {
      const ClusterBlockId blk = clb_nlist.find_block(name);
      if (!blk.is_valid()) {
        error = "There is no cluster '" + name + "'";
        return false;
      }
      scope.push_back(blk);
    }
  } else {
    scope.assign(clb_nlist.blocks().begin(), clb_nlist.blocks().end());
  }
  if (scope.size() > request.limit) {
    error = "The design has " + std::to_string(scope.size()) +
            " clusters, more than " + std::to_string(request.limit) +
            ": give --blocks";
    return false;
  }
  /* The atoms of each cluster */
  std::map<ClusterBlockId, std::vector<AtomBlockId>> atoms_of;
  std::set<ClusterBlockId> in(scope.begin(), scope.end());
  for (const AtomBlockId atom : netlist.blocks()) {
    const ClusterBlockId clb = atom_ctx.lookup().atom_clb(atom);
    if (clb.is_valid() && in.count(clb)) {
      atoms_of[clb].push_back(atom);
    }
  }
  std::map<ClusterBlockId, int32_t> block_index;
  for (const ClusterBlockId blk : scope) {
    block_index[blk] = static_cast<int32_t>(block_index.size());
  }
  file.table("block_types", block_type_names().size())
    .strings("name", block_type_names());
  blocks_table(file, scope);
  std::vector<uint32_t> atom_start, atom_count;
  std::vector<std::string> atom_name, model_names;
  std::vector<uint16_t> atom_model;
  std::map<LogicalModelId, uint16_t> model_index;
  const auto& models = g_vpr_ctx.device().arch->models;
  for (const ClusterBlockId blk : scope) {
    atom_start.push_back(static_cast<uint32_t>(atom_name.size()));
    for (const AtomBlockId atom : atoms_of[blk]) {
      atom_name.push_back(netlist.block_name(atom));
      const LogicalModelId model = netlist.block_model(atom);
      auto found = model_index.find(model);
      if (found == model_index.end()) {
        found =
          model_index.emplace(model, static_cast<uint16_t>(model_names.size()))
            .first;
        model_names.push_back(models.get_model(model).name);
      }
      atom_model.push_back(found->second);
    }
    atom_count.push_back(static_cast<uint32_t>(atom_name.size()) -
                         atom_start.back());
  }
  file.table("block_atoms", atom_start.size())
    .column("atom_start", atom_start)
    .column("atom_count", atom_count);
  file.table("models", model_names.size()).strings("name", model_names);
  file.table("atoms", atom_name.size())
    .strings("name", atom_name)
    .column("model", atom_model);
  /* The nets with a pin on a cluster of the scope */
  std::vector<std::string> nname;
  std::vector<int32_t> ndriver;
  std::vector<uint32_t> nid, sink_start, sink_count, fanout;
  std::vector<uint8_t> ignored;
  std::vector<uint32_t> sinks;
  for (const ClusterNetId net : clb_nlist.nets()) {
    bool touches = false;
    for (const ClusterPinId pin : clb_nlist.net_pins(net)) {
      touches = touches || block_index.count(clb_nlist.pin_block(pin));
    }
    if (!touches) {
      continue;
    }
    nname.push_back(clb_nlist.net_name(net));
    nid.push_back(static_cast<uint32_t>(size_t(net)));
    const ClusterPinId driver = clb_nlist.net_driver(net);
    const auto d = driver.is_valid()
                     ? block_index.find(clb_nlist.pin_block(driver))
                     : block_index.end();
    ndriver.push_back(d == block_index.end() ? -1 : d->second);
    sink_start.push_back(static_cast<uint32_t>(sinks.size()));
    for (const ClusterPinId pin : clb_nlist.net_sinks(net)) {
      const auto s = block_index.find(clb_nlist.pin_block(pin));
      if (s != block_index.end()) {
        sinks.push_back(static_cast<uint32_t>(s->second));
      }
    }
    sink_count.push_back(static_cast<uint32_t>(sinks.size()) -
                         sink_start.back());
    fanout.push_back(static_cast<uint32_t>(clb_nlist.net_sinks(net).size()));
    ignored.push_back(clb_nlist.net_is_ignored(net) ? 1 : 0);
  }
  file.table("nets", nname.size())
    .strings("name", nname)
    .column("net", nid)
    .column("driver", ndriver)
    .column("sink_start", sink_start)
    .column("sink_count", sink_count)
    .column("fanout", fanout)
    .column("ignored", ignored);
  file.table("sinks", sinks.size()).column("block", sinks);
  /* The pins of the clusters that are on a net: the block (its row), the
   * port, the bit of the port, the direction (0 in, 1 out, 2 clock) and the
   * net (its index, as the column 'net' of the nets) */
  std::vector<uint32_t> pblock, pnet;
  std::vector<std::string> pport;
  std::vector<uint16_t> pbit;
  std::vector<uint8_t> pdir;
  for (const ClusterBlockId blk : scope) {
    for (const ClusterPinId pin : clb_nlist.block_pins(blk)) {
      const ClusterNetId net = clb_nlist.pin_net(pin);
      if (!net.is_valid()) {
        continue;
      }
      const ClusterPortId port = clb_nlist.pin_port(pin);
      pblock.push_back(static_cast<uint32_t>(block_index[blk]));
      pport.push_back(clb_nlist.port_name(port));
      pbit.push_back(static_cast<uint16_t>(clb_nlist.pin_port_bit(pin)));
      pdir.push_back(PinType::DRIVER == clb_nlist.pin_type(pin)     ? 1
                     : PortType::CLOCK == clb_nlist.port_type(port) ? 2
                                                                    : 0);
      pnet.push_back(static_cast<uint32_t>(size_t(net)));
    }
  }
  file.table("pins", pblock.size())
    .column("block", pblock)
    .strings("port", pport)
    .column("bit", pbit)
    .column("dir", pdir)
    .column("net", pnet);
  Meta meta;
  meta.key("total_clusters")
    .value(static_cast<int64_t>(clb_nlist.blocks().size()));
  file.set_meta(meta.str());
  return true;
}

/* ----- tiles ------------------------------------------------------------ */

/* A coarse level: one value per cell of 2^level grid tiles, from a value per
 * grid tile; how merges two */
template <typename T, typename Merge>
std::vector<T> coarse(const std::vector<T>& per_tile, int width, int height,
                      int x0, int y0, int cell, T none, Merge merge) {
  std::vector<T> cells(VIEW_TILE_SIZE * VIEW_TILE_SIZE, none);
  for (int cy = 0; cy < VIEW_TILE_SIZE; ++cy) {
    for (int cx = 0; cx < VIEW_TILE_SIZE; ++cx) {
      T value = none;
      for (int y = y0 + cy * cell; y < std::min(height, y0 + (cy + 1) * cell);
           ++y) {
        for (int x = x0 + cx * cell; x < std::min(width, x0 + (cx + 1) * cell);
             ++x) {
          value = merge(value, per_tile[y * width + x]);
        }
      }
      cells[cy * VIEW_TILE_SIZE + cx] = value;
    }
  }
  return cells;
}

}  // namespace

bool module_of_name(const std::string& given, std::string& module) {
  const std::string name = given.substr(0, given.find('['));
  if (name.empty()) {
    return false;
  }
  const size_t dot = name.rfind('.');
  if (std::string::npos == dot) {
    if ('$' == name[0] || '\\' == name[0]) {
      return false;
    }
    module.clear();
    return true;
  }
  const std::string prefix = name.substr(0, dot);
  if (prefix.empty() || std::string::npos != prefix.find('$')) {
    return false;
  }
  module = prefix;
  return true;
}

std::vector<OpenFpgaStage> view_stages(const std::string& kind,
                                       const bool tile) {
  if ("device" == kind || "grid" == kind) {
    return {OpenFpgaStage::DEVICE};
  }
  if ("pins" == kind) {
    return {OpenFpgaStage::DEVICE, OpenFpgaStage::PLACED};
  }
  if ("atoms" == kind) {
    return {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED};
  }
  if ("clusters" == kind) {
    return {OpenFpgaStage::PACKED, OpenFpgaStage::PLACED};
  }
  if ("placement" == kind) {
    return {OpenFpgaStage::PLACED};
  }
  if ("channels" == kind) {
    return {OpenFpgaStage::ROUTED};
  }
  (void)tile;
  /* routing, timing */
  return {OpenFpgaStage::ROUTED};
}

std::string view_version(const OpenFpgaSession& session,
                         const std::string& kind, const bool tile) {
  std::string version = "v" + std::to_string(session.vpr_generation());
  for (const OpenFpgaStage stage : view_stages(kind, tile)) {
    version += "-" + std::to_string(session.stage_generation(stage));
  }
  return version;
}

bool export_view(const vpr::VprRun& run, const OpenFpgaSession& session,
                 const ViewRequest& request, ViewFile& file,
                 std::string& error) {
  (void)session;
  const std::string& kind = request.kind;
  if ("device" == kind) {
    return device_view(file, error);
  }
  if ("placement" == kind) {
    return placement_view(file, error);
  }
  if ("pins" == kind) {
    return pins_view(session, file, error);
  }
  if ("routing" == kind) {
    return routing_view(file, error);
  }
  if ("timing" == kind) {
    return timing_view(run, view_version(session, kind, false), request.paths,
                       file, error);
  }
  if ("atoms" == kind) {
    return atoms_view(request, file, error);
  }
  if ("clusters" == kind) {
    return clusters_view(request, file, error);
  }
  error = "There is no view '" + kind +
          "' (device, placement, pins, routing, timing, atoms, clusters)";
  return false;
}

bool export_tile(const vpr::VprRun& run, const OpenFpgaSession& session,
                 const TileRequest& request, ViewFile& file,
                 std::string& error) {
  const std::string& kind = request.kind;
  if ("grid" != kind && "placement" != kind && "routing" != kind &&
      "channels" != kind && "timing" != kind) {
    error = "There is no tile '" + kind +
            "' (grid, placement, routing, channels, timing)";
    return false;
  }
  if (!device_made(error)) {
    return false;
  }
  const auto& grid = g_vpr_ctx.device().grid;
  const int width = static_cast<int>(grid.width());
  const int height = static_cast<int>(grid.height());
  if (request.level < 0 || request.level > 16) {
    error = "The level is from 0 to 16";
    return false;
  }
  const int cell = 1 << request.level;
  const int span = VIEW_TILE_SIZE * cell;
  const int x0 = request.x * span;
  const int y0 = request.y * span;
  if (request.x < 0 || request.y < 0 || x0 >= width || y0 >= height) {
    error = "The tile (" + std::to_string(request.x) + ", " +
            std::to_string(request.y) + ") of level " +
            std::to_string(request.level) + " is outside the device";
    return false;
  }
  const int x1 = std::min(width, x0 + span);
  const int y1 = std::min(height, y0 + span);

  /* What the kind needs, made once a version */
  TileCache& cache = tile_cache(kind);
  const std::string version = view_version(session, kind, true);
  if ("placement" == kind && !placed()) {
    error = "The design has no placement: run the stage PLACED first";
    return false;
  }
  if (("routing" == kind || "channels" == kind || "timing" == kind) &&
      (!packed(error) || !routed(error))) {
    return false;
  }
  if (cache.version != version) {
    cache = TileCache();
    if ("routing" == kind) {
      make_routing_cache(cache);
    } else if ("channels" == kind) {
      make_channel_cache(cache);
    } else if ("timing" == kind &&
               !make_timing_cache(run, version, cache, error)) {
      return false;
    }
    cache.version = version;
  }

  Meta meta;
  meta.grid();
  meta.key("level").value(request.level);
  meta.key("x").value(request.x);
  meta.key("y").value(request.y);
  meta.key("size").value(VIEW_TILE_SIZE);
  meta.key("cell").value(cell);
  meta.key("x0").value(x0);
  meta.key("y0").value(y0);
  meta.key("x1").value(x1);
  meta.key("y1").value(y1);
  meta.key("tiles_x").value((width + span - 1) / span);
  meta.key("tiles_y").value((height + span - 1) / span);
  if ("routing" == kind || "channels" == kind) {
    node_types_meta(meta);
  }

  const auto& clb_nlist = g_vpr_ctx.clustering().clb_nlist;
  const auto& block_locs = g_vpr_ctx.placement().block_locs();
  auto blocks_in = [&]() {
    std::vector<ClusterBlockId> found;
    for (const ClusterBlockId blk : clb_nlist.blocks()) {
      const t_pl_loc& loc = block_locs[blk].loc;
      if (loc.x >= x0 && loc.x < x1 && loc.y >= y0 && loc.y < y1) {
        found.push_back(blk);
      }
    }
    return found;
  };

  if (0 == request.level) {
    if ("grid" == kind) {
      tiles_table(file, x0, y0, x1, y1);
    } else if ("placement" == kind) {
      blocks_table(file, blocks_in());
    } else if ("routing" == kind) {
      const int t = request.y * cache.tiles_x + request.x;
      std::map<uint32_t, uint32_t> net_row;
      std::vector<std::string> name;
      std::vector<uint32_t> net_id, nnet;
      std::vector<int32_t> nindex, nparent;
      std::vector<uint8_t> ntype, nlayer, nsink;
      std::vector<uint16_t> xl, yl, xh, yh, ptc;
      for (const uint32_t id : cache.route_buckets[t]) {
        const uint32_t net = cache.node_net[id];
        auto row = net_row.find(net);
        if (row == net_row.end()) {
          row = net_row.emplace(net, static_cast<uint32_t>(name.size())).first;
          name.push_back(clb_nlist.net_name(ClusterNetId(net)));
          net_id.push_back(net);
        }
        nnet.push_back(row->second);
        nindex.push_back(cache.node_index[id]);
        nparent.push_back(cache.node_parent[id]);
        ntype.push_back(cache.node_type[id]);
        nlayer.push_back(cache.node_layer[id]);
        nsink.push_back(cache.node_sink[id]);
        xl.push_back(cache.node_xlow[id]);
        yl.push_back(cache.node_ylow[id]);
        xh.push_back(cache.node_xhigh[id]);
        yh.push_back(cache.node_yhigh[id]);
        ptc.push_back(cache.node_ptc[id]);
      }
      file.table("nets", name.size())
        .strings("name", name)
        .column("net", net_id);
      file.table("nodes", nnet.size())
        .column("net", nnet)
        .column("index", nindex)
        .column("parent", nparent)
        .column("type", ntype)
        .column("xlow", xl)
        .column("ylow", yl)
        .column("xhigh", xh)
        .column("yhigh", yh)
        .column("ptc", ptc)
        .column("layer", nlayer)
        .column("sink", nsink);
    } else if ("channels" == kind) {
      const auto& rr_graph = g_vpr_ctx.device().rr_graph;
      const int t = request.y * cache.tiles_x + request.x;
      std::vector<uint8_t> type, used, capacity, layer;
      std::vector<uint16_t> xl, yl, xh, yh, ptc;
      for (const uint32_t id : cache.wire_buckets[t]) {
        const RRNodeId inode(id);
        type.push_back(node_type_code(rr_graph.node_type(inode)));
        xl.push_back(rr_graph.node_xlow(inode));
        yl.push_back(rr_graph.node_ylow(inode));
        xh.push_back(rr_graph.node_xhigh(inode));
        yh.push_back(rr_graph.node_yhigh(inode));
        ptc.push_back(static_cast<uint16_t>(rr_graph.node_ptc_num(inode)));
        layer.push_back(static_cast<uint8_t>(rr_graph.node_layer_low(inode)));
        used.push_back(static_cast<uint8_t>(
          std::min<uint32_t>(255, cache.use.node_users[id])));
        capacity.push_back(static_cast<uint8_t>(
          std::min<int>(255, rr_graph.node_capacity(inode))));
      }
      file.table("wires", type.size())
        .column("type", type)
        .column("xlow", xl)
        .column("ylow", yl)
        .column("xhigh", xh)
        .column("yhigh", yh)
        .column("ptc", ptc)
        .column("layer", layer)
        .column("used", used)
        .column("capacity", capacity);
    } else {
      const std::vector<ClusterBlockId> blocks = blocks_in();
      std::vector<std::string> name;
      std::vector<int32_t> x, y;
      std::vector<float> slack;
      for (const ClusterBlockId blk : blocks) {
        name.push_back(clb_nlist.block_name(blk));
        x.push_back(block_locs[blk].loc.x);
        y.push_back(block_locs[blk].loc.y);
        slack.push_back(cache.block_slack[size_t(blk)]);
      }
      file.table("blocks", name.size())
        .strings("name", name)
        .column("x", x)
        .column("y", y)
        .column("slack_ns", slack);
    }
  } else {
    /* A coarse level: a value per cell */
    const size_t n = static_cast<size_t>(width) * height;
    if ("grid" == kind) {
      /* The type of the most tiles of each cell: the type of each grid tile,
       * counted by cell */
      const size_t num_types = g_vpr_ctx.device().physical_tile_types.size();
      std::vector<uint16_t> type(VIEW_TILE_SIZE * VIEW_TILE_SIZE, 0xffff);
      for (int cy = 0; cy < VIEW_TILE_SIZE; ++cy) {
        for (int cx = 0; cx < VIEW_TILE_SIZE; ++cx) {
          std::vector<int> count(num_types, 0);
          for (int yy = y0 + cy * cell; yy < std::min(y1, y0 + (cy + 1) * cell);
               ++yy) {
            for (int xx = x0 + cx * cell;
                 xx < std::min(x1, x0 + (cx + 1) * cell); ++xx) {
              const t_physical_tile_type_ptr tile =
                grid.get_physical_type({xx, yy, 0});
              if (!tile->is_empty()) {
                ++count[tile->index];
              }
            }
          }
          const auto best = std::max_element(count.begin(), count.end());
          if (*best > 0) {
            type[cy * VIEW_TILE_SIZE + cx] =
              static_cast<uint16_t>(best - count.begin());
          }
        }
      }
      file.table("cells", type.size()).column("type", type);
    } else if ("placement" == kind) {
      std::vector<float> used(n, 0.f), slots(n, 0.f);
      for (const ClusterBlockId blk : clb_nlist.blocks()) {
        const t_pl_loc& loc = block_locs[blk].loc;
        used[loc.y * width + loc.x] += 1.f;
      }
      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          const t_physical_tile_loc loc(x, y, 0);
          if (grid.is_root_location(loc)) {
            slots[y * width + x] =
              static_cast<float>(grid.get_physical_type(loc)->capacity);
          }
        }
      }
      const auto sum = [](float a, float b) { return a + b; };
      const std::vector<float> u =
        coarse(used, width, height, x0, y0, cell, 0.f, sum);
      const std::vector<float> s =
        coarse(slots, width, height, x0, y0, cell, 0.f, sum);
      std::vector<float> share(u.size(), 0.f);
      for (size_t i = 0; i < u.size(); ++i) {
        share[i] = s[i] > 0 ? u[i] / s[i] : std::nanf("");
      }
      file.table("cells", u.size())
        .column("used", u)
        .column("slots", s)
        .column("value", share);
    } else if ("routing" == kind || "channels" == kind) {
      std::vector<float> used(n, 0.f), capacity(n, 0.f);
      for (size_t i = 0; i < n; ++i) {
        used[i] = static_cast<float>(cache.use.used_x[i] + cache.use.used_y[i]);
        capacity[i] =
          static_cast<float>(cache.use.cap_x[i] + cache.use.cap_y[i]);
      }
      const auto sum = [](float a, float b) { return a + b; };
      const auto peak = [](float a, float b) { return std::max(a, b); };
      std::vector<float> occupancy(n, 0.f);
      for (size_t i = 0; i < n; ++i) {
        occupancy[i] = capacity[i] > 0 ? used[i] / capacity[i] : 0.f;
      }
      const std::vector<float> u =
        coarse(used, width, height, x0, y0, cell, 0.f, sum);
      const std::vector<float> c =
        coarse(capacity, width, height, x0, y0, cell, 0.f, sum);
      const std::vector<float> worst =
        coarse(occupancy, width, height, x0, y0, cell, 0.f, peak);
      std::vector<float> value(u.size(), 0.f);
      for (size_t i = 0; i < u.size(); ++i) {
        value[i] = c[i] > 0 ? u[i] / c[i] : std::nanf("");
      }
      file.table("cells", u.size())
        .column("used", u)
        .column("capacity", c)
        .column("value", value)
        .column("peak", worst);
    } else {
      std::vector<float> slack(n, NO_SLACK);
      for (const ClusterBlockId blk : clb_nlist.blocks()) {
        const t_pl_loc& loc = block_locs[blk].loc;
        float& here = slack[loc.y * width + loc.x];
        here = std::min(here, cache.block_slack[size_t(blk)]);
      }
      const auto least = [](float a, float b) { return std::min(a, b); };
      file.table("cells", VIEW_TILE_SIZE * VIEW_TILE_SIZE)
        .column("value",
                coarse(slack, width, height, x0, y0, cell, NO_SLACK, least));
    }
  }
  file.set_meta(meta.str());
  return true;
}

} /* end namespace openfpga */
