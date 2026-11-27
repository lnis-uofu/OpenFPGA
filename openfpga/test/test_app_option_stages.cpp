/********************************************************************
 * Unit test of the stages that the application options tune: the stage of
 * every option, and the invalidation of the stages when an option changes
 *******************************************************************/
#include <cstdio>
#include <string>
#include <vector>

#include "app_option_stage.h"
#include "openfpga_session.h"

using openfpga::app_option_stage;
using openfpga::OpenFpgaInput;
using openfpga::OpenFpgaSession;
using openfpga::OpenFpgaStage;

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

using Stages = std::vector<OpenFpgaStage>;

static const Stages main_path = {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED,
                                 OpenFpgaStage::PLACED, OpenFpgaStage::ROUTED};

/* Run the whole flow: the inputs, then the stages down to BITSTREAM */
static void run_all(OpenFpgaSession& session) {
  CHECK(session.read_input(OpenFpgaInput::VPR_ARCH));
  CHECK(session.read_input(OpenFpgaInput::OPENFPGA_ARCH));
  CHECK(session.read_input(OpenFpgaInput::CIRCUIT));
  for (const OpenFpgaStage& stage : main_path) {
    CHECK(session.complete(stage));
  }
  CHECK(session.complete(OpenFpgaStage::ANALYZED));
  CHECK(session.complete(OpenFpgaStage::REPACKED));
  CHECK(session.complete(OpenFpgaStage::BITSTREAM));
}

static bool set(OpenFpgaSession& session, const std::string& name,
                const std::string& value) {
  std::string error;
  return session.set_app_option(name, value, error);
}

static OpenFpgaStage stage_of(const std::string& name) {
  OpenFpgaStage stage = OpenFpgaStage::UNINIT;
  CHECK(app_option_stage(name, stage));
  return stage;
}

static void test_every_option_has_a_stage() {
  OpenFpgaSession session;
  for (const std::string& name : session.app_options().names()) {
    OpenFpgaStage stage = OpenFpgaStage::UNINIT;
    if (!app_option_stage(name, stage)) {
      std::fprintf(stderr, "option %s has no stage\n", name.c_str());
      ++num_failures;
    }
    /* UNINIT is not a stage that can be run again */
    CHECK(OpenFpgaStage::UNINIT != stage);
  }
  OpenFpgaStage stage = OpenFpgaStage::PACKED;
  CHECK(!app_option_stage("nogroup.seed", stage));
  CHECK(OpenFpgaStage::PACKED == stage);
  CHECK(!app_option_stage("seed", stage));
  CHECK(!app_option_stage("", stage));
}

static void test_stages_of_groups() {
  CHECK(stage_of("filename.arch_file") == OpenFpgaStage::DEVICE);
  CHECK(stage_of("general.num_workers") == OpenFpgaStage::DEVICE);
  CHECK(stage_of("atom.sweep_dangling_nets") == OpenFpgaStage::DEVICE);
  CHECK(stage_of("cluster.cluster_seed_type") == OpenFpgaStage::PACKED);
  CHECK(stage_of("place.seed") == OpenFpgaStage::PLACED);
  CHECK(stage_of("tplace.timing_tradeoff") == OpenFpgaStage::PLACED);
  CHECK(stage_of("aplace.ap_analytical_solver") == OpenFpgaStage::PLACED);
  CHECK(stage_of("router.max_router_iterations") == OpenFpgaStage::ROUTED);
  CHECK(stage_of("crr.sb_maps") == OpenFpgaStage::DEVICE);
  CHECK(stage_of("trouter.astar_fac") == OpenFpgaStage::ROUTED);
  CHECK(stage_of("analysis.generate_post_synthesis_netlist") ==
        OpenFpgaStage::ANALYZED);
  CHECK(stage_of("power.act_file") == OpenFpgaStage::ANALYZED);
  CHECK(stage_of("build_fabric.reorder_incoming_edges") ==
        OpenFpgaStage::REPACKED);
}

static void test_stages_of_exceptions() {
  CHECK(stage_of("filename.net_file") == OpenFpgaStage::PACKED);
  CHECK(stage_of("filename.place_file") == OpenFpgaStage::PLACED);
  CHECK(stage_of("filename.route_file") == OpenFpgaStage::ROUTED);
  CHECK(stage_of("filename.read_rr_graph_file") == OpenFpgaStage::DEVICE);
  CHECK(stage_of("router.route_chan_width") == OpenFpgaStage::DEVICE);
  CHECK(stage_of("place.place_chan_width") == OpenFpgaStage::DEVICE);
  CHECK(stage_of("aplace.appack_max_dist_th") == OpenFpgaStage::PACKED);
}

/* An option of the stage PLACED, after ROUTED: PACKED stays */
static void test_place_option_after_routed() {
  OpenFpgaSession session;
  run_all(session);
  CHECK(set(session, "place.seed", "42"));
  CHECK(session.reached(OpenFpgaStage::DEVICE));
  CHECK(session.reached(OpenFpgaStage::PACKED));
  CHECK(!session.reached(OpenFpgaStage::PLACED));
  CHECK(!session.reached(OpenFpgaStage::ROUTED));
  CHECK(!session.reached(OpenFpgaStage::ANALYZED));
  CHECK(!session.reached(OpenFpgaStage::REPACKED));
  CHECK(!session.reached(OpenFpgaStage::BITSTREAM));
  /* The next stage command can run again, and uses the new value */
  CHECK(session.can_run(OpenFpgaStage::PLACED));
  CHECK(!session.can_run(OpenFpgaStage::ROUTED));
  CHECK(session.app_options().find("place.seed")->int_value == 42);
}

static void test_branches() {
  /* ROUTED options drop the two branches */
  OpenFpgaSession session;
  run_all(session);
  CHECK(set(session, "router.max_router_iterations", "10"));
  CHECK(session.reached(OpenFpgaStage::PLACED));
  CHECK(!session.reached(OpenFpgaStage::ROUTED));
  CHECK(!session.reached(OpenFpgaStage::ANALYZED));
  CHECK(!session.reached(OpenFpgaStage::REPACKED));
  CHECK(!session.reached(OpenFpgaStage::BITSTREAM));

  /* ANALYZED options leave the other branch */
  OpenFpgaSession analyzed;
  run_all(analyzed);
  CHECK(set(analyzed, "analysis.generate_post_synthesis_netlist", "true"));
  CHECK(!analyzed.reached(OpenFpgaStage::ANALYZED));
  CHECK(analyzed.reached(OpenFpgaStage::ROUTED));
  CHECK(analyzed.reached(OpenFpgaStage::REPACKED));
  CHECK(analyzed.reached(OpenFpgaStage::BITSTREAM));

  /* build_fabric options drop REPACKED and what follows, not ANALYZED */
  OpenFpgaSession fabric;
  run_all(fabric);
  CHECK(set(fabric, "build_fabric.reorder_incoming_edges", "false"));
  CHECK(fabric.reached(OpenFpgaStage::ANALYZED));
  CHECK(fabric.reached(OpenFpgaStage::ROUTED));
  CHECK(!fabric.reached(OpenFpgaStage::REPACKED));
  CHECK(!fabric.reached(OpenFpgaStage::BITSTREAM));
}

static void test_device_options_keep_inputs() {
  OpenFpgaSession session;
  run_all(session);
  CHECK(set(session, "general.num_workers", "2"));
  CHECK(session.reached_stages().empty());
  CHECK(session.has_all_inputs());
  CHECK(session.can_run(OpenFpgaStage::DEVICE));
}

/* An option of a stage that has not been reached changes nothing else */
static void test_option_of_stage_not_reached() {
  OpenFpgaSession session;
  CHECK(session.read_input(OpenFpgaInput::VPR_ARCH));
  CHECK(session.read_input(OpenFpgaInput::OPENFPGA_ARCH));
  CHECK(session.read_input(OpenFpgaInput::CIRCUIT));
  CHECK(session.complete(OpenFpgaStage::DEVICE));
  CHECK(session.complete(OpenFpgaStage::PACKED));
  const Stages before = session.reached_stages();
  CHECK(set(session, "router.max_router_iterations", "10"));
  CHECK(set(session, "analysis.generate_post_synthesis_netlist", "true"));
  CHECK(set(session, "build_fabric.reorder_incoming_edges", "false"));
  CHECK(session.reached_stages() == before);
  CHECK(session.has_all_inputs());

  /* Before any input */
  OpenFpgaSession empty;
  CHECK(set(empty, "place.seed", "5"));
  CHECK(empty.uninitialized());
}

static void test_same_value_keeps_stages() {
  OpenFpgaSession session;
  run_all(session);
  const Stages all = session.reached_stages();
  const int seed = session.app_options().find("place.seed")->int_value;
  CHECK(set(session, "place.seed", std::to_string(seed)));
  CHECK(session.reached_stages() == all);
  /* A different spelling of the same value is the same value */
  CHECK(set(session, "router.flat_routing", "false"));
  CHECK(session.reached_stages() == all);
  CHECK(set(session, "router.flat_routing", "FALSE"));
  CHECK(session.reached_stages() == all);
  CHECK(set(session, "router.flat_routing", "true"));
  CHECK(!session.reached(OpenFpgaStage::ROUTED));
}

static void test_refused_value_keeps_stages() {
  OpenFpgaSession session;
  run_all(session);
  const Stages all = session.reached_stages();
  std::string error;
  CHECK(!session.set_app_option("place.seed", "not_a_number", error));
  CHECK(!error.empty());
  CHECK(!session.set_app_option("place.nope", "1", error));
  CHECK(!session.set_app_option("place.place_algorithm", "bogus", error));
  CHECK(session.reached_stages() == all);
  CHECK(session.app_options().is_default("place.seed"));
}

static void test_reset_options() {
  OpenFpgaSession session;
  CHECK(set(session, "place.seed", "42"));
  CHECK(set(session, "router.max_router_iterations", "7"));
  run_all(session);

  /* An option that is at its default value changes nothing */
  CHECK(session.reset_app_option("trouter.astar_fac"));
  CHECK(session.reached(OpenFpgaStage::BITSTREAM));
  CHECK(!session.reset_app_option("place.nope"));

  CHECK(session.reset_app_option("place.seed"));
  CHECK(session.app_options().is_default("place.seed"));
  CHECK(session.reached(OpenFpgaStage::PACKED));
  CHECK(!session.reached(OpenFpgaStage::PLACED));

  /* All the options: the earliest stage among the ones that changed is
   * dropped */
  OpenFpgaSession other;
  CHECK(set(other, "router.max_router_iterations", "7"));
  run_all(other);
  other.reset_app_options();
  CHECK(other.reached(OpenFpgaStage::PLACED));
  CHECK(!other.reached(OpenFpgaStage::ROUTED));
}

int main() {
  test_every_option_has_a_stage();
  test_stages_of_groups();
  test_stages_of_exceptions();
  test_place_option_after_routed();
  test_branches();
  test_device_options_keep_inputs();
  test_option_of_stage_not_reached();
  test_same_value_keeps_stages();
  test_refused_value_keeps_stages();
  test_reset_options();

  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All application option stage tests passed\n");
  return 0;
}
