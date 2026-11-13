#ifndef QUERY_DESIGN_H
#define QUERY_DESIGN_H

#include <cstddef>
#include <string>

#include "vpr_run.h"

/********************************************************************
 * The answers of the queries about the design, which are read from the data
 * of VPR: the packing, the placement and the routing. Each one names the
 * stage it reads in the envelope, see json_response.h. They return false, and
 * log why, when the answer cannot be made. The schema of each kind is
 * openfpga_flow/schemas/<kind>.schema.json
 *
 * device (stage DEVICE): the device that the flow works on, which is there as
 * soon as the inputs are read
 *   grid                   {width, height, layers, layout: its name}
 *   channel_width          {x, y, fixed}: the widest channel of the routing
 *                          resource graph that VPR made with the device. It
 *                          is the width that is set (fixed true), or else the
 *                          width that VPR estimates the delays of the
 *                          placement with (place.place_chan_width, 100 by
 *                          default). null if there is no graph
 *   tile_types[]           the types of the tiles that are on the grid: name,
 *                          width, height (in tiles), capacity (the blocks that
 *                          one tile holds, all its sub tiles), is_io, the
 *                          number of input_pins, output_pins and clock_pins,
 *                          count (the tiles of this type on the grid), slots
 *                          (the places for blocks that they make: count times
 *                          capacity, as VPR counts them)
 *                          sub_tiles[]: name, capacity, equivalent_blocks[]
 *                          (the types of blocks that fit)
 *   io_sides               {left, right, bottom, top}: the I/O tiles on each
 *                          side of the perimeter (a corner is not counted)
 *   locations              only if a tile type was asked for: type, total,
 *                          returned, truncated, tiles[]: x, y, layer, the
 *                          lower left corner of each tile of the type
 *
 * utilization (stage PACKED): the use of the blocks of the device
 *   device_utilization     as VPR reports it
 *   grid                   {width, height}
 *   block_types[]          name, is_io, used (blocks of the netlist),
 *                          available (places for blocks: a tile has as many as
 *                          its capacity, as in the resource usage that VPR
 *                          reports), utilization
 *   limiting_resources[]   the block types that limit the size of the device
 *
 * placement (stage PLACED): where the blocks are
 *   grid, filter {type, prefix}, total (blocks that match), returned,
 *   truncated, blocks[]: name, type, x, y, sub_tile, layer
 *
 * net_route (stage ROUTED): the routing tree of a net, as the nodes of the
 *   routing resource graph in the order of a depth-first walk of the tree
 *   net, num_sinks, nodes[]: index, parent (the index of the node that drives
 *   it, null for the source), rr_node, type, xlow, ylow, xhigh, yhigh, ptc,
 *   layer, sink_pin (the index of the pin of the net for a sink, else null)
 *
 * congestion (stage ROUTED): the use of the routing channels, aggregated in
 *   regions of region_size by region_size tiles. A channel is used by the
 *   wires of the nets that cross it
 *   grid, channel_width {x, y}, region_size, summary {chanx, chany,
 *   overused_nodes}, regions[]: x_low, y_low, x_high, y_high, chanx, chany.
 *   chanx and chany are {used, capacity, occupancy}, and only the regions
 *   with a wire in use are listed. With hotspots: hotspots[], the fullest
 *   segments of the channels, the fullest first: channel (chanx or chany), x, y
 *   (the tile it is beside), used (wires that cross it), capacity (wires of the
 *   channel there), occupancy, overused_nodes (the wires that cross it which
 *   more than one net uses), num_nets, nets[] (the names of the nets that cross
 *   it, at most 50)
 *
 * cluster_report (stage PACKED): the clusters of the packing that match a
 *   shell-style pattern of their name
 *   query, total, matched, returned, truncated, clusters[]: name, type,
 *   input_pin_utils, output_pin_utils (the pins of the cluster over the pins
 *   of its type, as in the packing pin usage report of VPR: the clock pins are
 *   input pins). With stats: stats {num_clusters, by_type[]: type, count,
 *   average_input_pin_utils, average_output_pin_utils}, over all the clusters
 *
 * find (stage PACKED): the blocks and the nets whose name matches a
 *   shell-style pattern
 *   pattern, kind (null: both), blocks and nets: null if not looked for, else
 *   {total, matched, returned, truncated, items[]}. A block item: name, type,
 *   placed, x, y, sub_tile, layer (null until the design is placed). A net
 *   item: name, fanout (the number of sinks), ignored (VPR does not route it,
 *   e.g., a clock)
 *
 * block_info (stage PACKED): one cluster
 *   name, type, placed, location {x, y, sub_tile, layer} (null until placed),
 *   pins[]: port, bit, direction (input, output or clock), net (the pins of the
 *   cluster that are connected), atoms[]: the blocks of the circuit that the
 *   cluster holds: name, model, path (where in the cluster: the type of the
 *   cluster, then the type and index of each level down to the primitive, with
 *   the mode of each in brackets, as the packed netlist file writes them)
 *
 * net_info (stage PACKED): one net between clusters
 *   name, fanout, ignored, driver {block, port, bit}, sinks[] likewise,
 *   bounding_box {xmin, ymin, xmax, ymax, half_perimeter} of the blocks that it
 *   connects (null until placed), routing {nodes, wires, wirelength} (null
 *   until routed: the wires are the nodes of the channels, and the wirelength
 *   counts the tiles that they span, as VPR does), timing {criticality,
 *   slack_ns} (null until routed or if the timing analysis is off: the
 *   criticality of the worst connection of the net, and its worst setup slack)
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

constexpr int DEVICE_SCHEMA_VERSION = 1;
constexpr int UTILIZATION_SCHEMA_VERSION = 1;
constexpr int PLACEMENT_SCHEMA_VERSION = 1;
constexpr int NET_ROUTE_SCHEMA_VERSION = 1;
constexpr int CONGESTION_SCHEMA_VERSION = 1;
constexpr int CLUSTER_REPORT_SCHEMA_VERSION = 1;
constexpr int FIND_SCHEMA_VERSION = 1;
constexpr int BLOCK_INFO_SCHEMA_VERSION = 1;
constexpr int NET_INFO_SCHEMA_VERSION = 1;

/* The most blocks or clusters a query lists, unless a limit is given */
constexpr size_t QUERY_LIST_DEFAULT_LIMIT = 1000;

/* The most nets that a hot spot of the congestion lists */
constexpr size_t HOTSPOT_NETS_MAX = 50;

/* An empty type lists no locations */
bool device_json(const bool fixed_channel_width, const std::string& type,
                 const size_t limit, std::string& json);

bool utilization_json(std::string& json);

/* An empty type or prefix does not filter */
bool placement_json(const std::string& type, const std::string& prefix,
                    const size_t limit, std::string& json);

bool net_route_json(const std::string& net, std::string& json);

/* No hot spots if hotspots is 0 */
bool congestion_json(const int region_size, const size_t hotspots,
                     std::string& json);

bool cluster_report_json(const std::string& query, const bool with_stats,
                         const size_t limit, std::string& json);

/* An empty kind looks for blocks and nets, else it is "block" or "net" */
bool find_json(const std::string& pattern, const std::string& kind,
               const size_t limit, const bool has_placement, std::string& json);

/* has_placement and has_routing say that the stages are reached: what the
 * answer gives of them is null if not */
bool block_info_json(const std::string& block, const bool has_placement,
                     std::string& json);

bool net_info_json(const vpr::VprRun& run, const std::string& net,
                   const bool has_placement, const bool has_routing,
                   std::string& json);

} /* End namespace openfpga */

#endif
