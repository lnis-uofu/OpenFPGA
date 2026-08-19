#pragma once

#include <map>
#include <string>

#include "atom_netlist_fwd.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "physical_types.h"
#include "vpr_types.h"

namespace openfpga {

/********************************************************************
 * Three MIF storages keyed by hierarchy (reviewer layout).
 *
 *   // Traceback logical mif from atom blocks
 *   std::map<AtomBlockId, MifStorage> logical_mifs_;
 *
 *   // Each (x, y, sub_tile, layer) may have several physical
 *   // pb_graph_nodes; each can hold a MIF aggregated from logical MIFs.
 *   // VPR type is t_pl_loc (x, y, sub_tile, layer).
 *   std::map<t_pl_loc, std::map<t_pb_graph_node*, MifStorage>> physical_mifs_;
 *
 *   // The one unified MIF for top-level fabric
 *   MifStorage top_mif_;
 *
 * hex_ is only the read_mif (source="others") input buffer, not a
 * hierarchy level.
 ********************************************************************/
class MifPipeline {
 public:
  void clear();

  const MifStorage& hex() const;
  MifStorage& mutable_hex();

  const std::map<AtomBlockId, MifStorage>& logical_mifs() const;
  std::map<AtomBlockId, MifStorage>& mutable_logical_mifs();

  const std::map<t_pl_loc, std::map<t_pb_graph_node*, MifStorage>>&
  physical_mifs() const;
  std::map<t_pl_loc, std::map<t_pb_graph_node*, MifStorage>>&
  mutable_physical_mifs();

  const MifStorage& top_mif() const;
  MifStorage& mutable_top_mif();

  /* physical_mifs_[phy_loc][pb_graph_node]; empty storage if missing. */
  const MifStorage& physical_mif(const t_pl_loc& phy_loc,
                                 t_pb_graph_node* pb_graph_node) const;

  /* Flatten every placed physical MIF into one storage (write_mif
   * fallback when top_mif_ was not built). */
  MifStorage copy_all_physical_mifs() const;

  /* Bind mif_source metadata and decode raw INIT / hex words in place. */
  int decode_storage(MifStorage& storage,
                     const BitstreamSetting& bitstream_setting) const;

  /* Remap every segment of logical through mif_address_map into dest.
   * Existing dest lines for the same des_pb_type are merged (OR). */
  int aggregate_logical_into_physical(const MifStorage& logical,
                                      const BitstreamSetting& bitstream_setting,
                                      MifStorage& dest) const;

 private:
  MifStorage hex_;
  std::map<AtomBlockId, MifStorage> logical_mifs_;
  std::map<t_pl_loc, std::map<t_pb_graph_node*, MifStorage>> physical_mifs_;
  MifStorage top_mif_;
};

} /* namespace openfpga */
