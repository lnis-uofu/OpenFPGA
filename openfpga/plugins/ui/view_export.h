#ifndef VIEW_EXPORT_H
#define VIEW_EXPORT_H

/********************************************************************
 * The data of the views of a design (docs/plan_design_views.md, 3.1), from
 * what VPR holds, as the tables of a view file (view_file.h):
 *
 * Whole exports (export_view), for what is small at any size of device or
 * is asked by scope:
 * - device: the tile types and their sub-tiles, the tiles, the width of
 *   each channel
 * - placement: the blocks on their slots, the regions of the constraints
 * - pins: the I/O sites and the ports of the design on them
 * - routing: the route of each net (its nodes), the use of each segment of
 *   the channels
 * - timing: the slack and the criticality of each net, the worst paths
 * - atoms: the atoms of a scope (a module, a cone, or all of a small
 *   design), their pins and nets, and the module of each atom
 * - clusters: the clusters of a scope, their atoms and the nets between them
 *
 * Tiles (export_tile), for what grows with the device: the square of
 * VIEW_TILE_SIZE << level grid tiles at (x, y). Level 0 has the objects
 * (tiles, blocks, route nodes, the wires of the channels, the slack of the
 * blocks); a coarser level has one value per cell of 2^level grid tiles
 *******************************************************************/
#include <cstddef>
#include <string>
#include <vector>

#include "openfpga_session.h"
#include "view_file.h"
#include "vpr_run.h"

/* begin namespace openfpga */
namespace openfpga {

constexpr int VIEW_EXPORT_SCHEMA_VERSION = 1;
/* The side of a tile at level 0, in grid tiles */
constexpr int VIEW_TILE_SIZE = 16;
/* The most atoms or clusters of an export with no scope */
constexpr size_t VIEW_WHOLE_LIMIT = 100000;

struct ViewRequest {
  std::string kind;
  /* atoms: the atoms of a module and of the modules in it ("" is the top) */
  std::string module;
  bool has_module = false;
  /* atoms: the atoms around an atom or a net, depth nets away */
  std::string cone;
  int depth = 2;
  /* clusters: these clusters only */
  std::vector<std::string> blocks;
  /* atoms, clusters: the most that an export with no scope has */
  size_t limit = VIEW_WHOLE_LIMIT;
  /* timing: the number of worst paths */
  size_t paths = 20;
};

struct TileRequest {
  std::string kind;
  int level = 0;
  int x = 0;
  int y = 0;
};

/* The stages whose data a kind reads: the first one is needed, the others
 * are read when they are reached */
std::vector<OpenFpgaStage> view_stages(const std::string& kind, bool tile);

/* The version of the data of a kind: it changes when a stage it reads runs
 * again */
std::string view_version(const OpenFpgaSession& session,
                         const std::string& kind, bool tile);

/* Fill the file with the data of the view. False, and the reason in error,
 * if it cannot be made */
bool export_view(const vpr::VprRun& run, const OpenFpgaSession& session,
                 const ViewRequest& request, ViewFile& file,
                 std::string& error);

bool export_tile(const vpr::VprRun& run, const OpenFpgaSession& session,
                 const TileRequest& request, ViewFile& file,
                 std::string& error);

/* The module of the RTL that the name of an atom or a net gives: the part
 * before its last '.', "" for the top, or false for a name that synthesis
 * made ($abc$..., $auto$...) */
bool module_of_name(const std::string& name, std::string& module);

} /* end namespace openfpga */

#endif
