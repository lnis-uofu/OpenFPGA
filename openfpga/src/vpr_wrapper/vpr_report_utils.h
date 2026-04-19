#pragma once

#include <fnmatch.h>

#include <string>
#include <vector>

#include "clustered_netlist.h"

struct cluster_netlist_report {
  const ClusteredNetlist& clb_nlist;

  explicit cluster_netlist_report(const ClusteredNetlist& clb_nlist_in)
    : clb_nlist(clb_nlist_in) {}

  // Write simple tokenizer to filter based on query string (e.g. "clb*") using
  // fnmatch
  std::vector<ClusterBlockId> filter_cluster_netlist(
    const std::string& filter_string) {
    std::vector<ClusterBlockId> filtered_blocks;
    for (auto blk_id : clb_nlist.blocks()) {
      std::string name = clb_nlist.block_name(blk_id);
      if (fnmatch(filter_string.c_str(), name.c_str(), 0) == 0) {
        filtered_blocks.push_back(blk_id);
      }
    }
    return filtered_blocks;
  }
};
