#include "vpr_run.h"

#include <exception>
#include <memory>

#include "globals.h"
#include "vtr_log.h"

/* Begin namespace vpr */
namespace vpr {

namespace {

std::unique_ptr<VprRun> live_run;
uint64_t generation = 0;

}  // End anonymous namespace

VprRun& begin_vpr_run() {
  release_vpr_run(true);
  live_run = std::make_unique<VprRun>();
  ++generation;
  return *live_run;
}

VprRun* current_vpr_run() { return live_run.get(); }

void release_vpr_run(bool announce) {
  if (!live_run) {
    return;
  }
  if (announce) {
    VTR_LOG("Free the data of the previous VPR run\n");
  }
  try {
    vpr_free_all(live_run->arch, live_run->setup);
    /* vpr_free_all() leaves the packing of the circuit in the clustering
     * context: the next circuit would be checked against it */
    ClusteringContext& cluster_ctx = g_vpr_ctx.mutable_clustering();
    cluster_ctx.atoms_lookup.clear();
    cluster_ctx.post_routing_clb_pin_nets.clear();
    cluster_ctx.pre_routing_net_pin_mapping.clear();
  } catch (const std::exception& exc) {
    VTR_LOG_ERROR("Fail to free VPR data: %s\n", exc.what());
  }
  live_run.reset();
  ++generation;
}

uint64_t vpr_run_generation() { return generation; }

} /* End namespace vpr */
