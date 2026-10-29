/********************************************************************
 * Unit test of the tracking of inputs and stages by OpenFpgaSession:
 * allowed transitions, and invalidation of the stages that depend on
 * what is re-read or re-run
 *******************************************************************/
#include <cstdio>
#include <string>
#include <vector>

#include "openfpga_context.h"
#include "openfpga_session.h"

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

static const OpenFpgaInput all_inputs[] = {OpenFpgaInput::VPR_ARCH,
                                           OpenFpgaInput::OPENFPGA_ARCH,
                                           OpenFpgaInput::CIRCUIT};

/* All the stages, in the order they can be run along the main path */
static const OpenFpgaStage all_stages[] = {
  OpenFpgaStage::DEVICE,   OpenFpgaStage::PACKED,   OpenFpgaStage::PLACED,
  OpenFpgaStage::ROUTED,   OpenFpgaStage::ANALYZED, OpenFpgaStage::REPACKED,
  OpenFpgaStage::BITSTREAM};

static void read_all_inputs(OpenFpgaSession& session) {
  for (const OpenFpgaInput& input : all_inputs) {
    CHECK(session.read_input(input));
  }
}

/* Read the inputs, then run the stages up to the one given, in order */
static void run_up_to(OpenFpgaSession& session, const OpenFpgaStage& last) {
  read_all_inputs(session);
  for (const OpenFpgaStage& stage : all_stages) {
    CHECK(session.complete(stage));
    if (stage == last) {
      return;
    }
  }
}

/* Run the whole flow */
static void run_all(OpenFpgaSession& session) {
  run_up_to(session, OpenFpgaStage::BITSTREAM);
}

/* Return true if the stages valid are exactly the ones given */
static bool reached_are(const OpenFpgaSession& session,
                        const std::vector<OpenFpgaStage>& expected) {
  return session.reached_stages() == expected;
}

static void test_initial_state() {
  OpenFpgaSession session;
  CHECK(session.uninitialized());
  CHECK(session.reached(OpenFpgaStage::UNINIT));
  for (const OpenFpgaStage& stage : all_stages) {
    CHECK(!session.reached(stage));
    CHECK(!session.can_run(stage));
  }
  /* Nothing runs to get to UNINIT */
  CHECK(!session.can_run(OpenFpgaStage::UNINIT));
  CHECK(!session.complete(OpenFpgaStage::UNINIT));
  CHECK(session.reached_stages().empty());
  for (const OpenFpgaInput& input : all_inputs) {
    CHECK(!session.has_input(input));
  }
  CHECK(!session.has_all_inputs());
  /* The circuit has to wait for the VPR architecture */
  CHECK(session.can_read(OpenFpgaInput::VPR_ARCH));
  CHECK(session.can_read(OpenFpgaInput::OPENFPGA_ARCH));
  CHECK(!session.can_read(OpenFpgaInput::CIRCUIT));
}

static void test_inputs() {
  OpenFpgaSession session;
  CHECK(!session.read_input(OpenFpgaInput::CIRCUIT));
  CHECK(!session.has_input(OpenFpgaInput::CIRCUIT));
  CHECK(session.uninitialized());

  CHECK(session.read_input(OpenFpgaInput::VPR_ARCH));
  CHECK(!session.uninitialized());
  CHECK(session.can_read(OpenFpgaInput::CIRCUIT));
  CHECK(!session.has_all_inputs());
  CHECK(!session.can_run(OpenFpgaStage::DEVICE));

  CHECK(session.read_input(OpenFpgaInput::CIRCUIT));
  CHECK(!session.has_all_inputs());
  CHECK(!session.can_run(OpenFpgaStage::DEVICE));
  CHECK(!session.complete(OpenFpgaStage::DEVICE));

  CHECK(session.read_input(OpenFpgaInput::OPENFPGA_ARCH));
  CHECK(session.has_all_inputs());
  CHECK(session.can_run(OpenFpgaStage::DEVICE));
}

static void test_input_orders() {
  /* The OpenFPGA architecture can come first, middle or last */
  const OpenFpgaInput orders[][3] = {
    {OpenFpgaInput::VPR_ARCH, OpenFpgaInput::CIRCUIT,
     OpenFpgaInput::OPENFPGA_ARCH},
    {OpenFpgaInput::VPR_ARCH, OpenFpgaInput::OPENFPGA_ARCH,
     OpenFpgaInput::CIRCUIT},
    {OpenFpgaInput::OPENFPGA_ARCH, OpenFpgaInput::VPR_ARCH,
     OpenFpgaInput::CIRCUIT}};
  for (const auto& order : orders) {
    OpenFpgaSession session;
    for (const OpenFpgaInput& input : order) {
      CHECK(session.read_input(input));
    }
    CHECK(session.has_all_inputs());
    CHECK(session.complete(OpenFpgaStage::DEVICE));
  }
}

static void test_forward_flow() {
  OpenFpgaSession session;
  read_all_inputs(session);
  /* Only DEVICE can run once the inputs are read */
  for (const OpenFpgaStage& stage : all_stages) {
    CHECK(session.can_run(stage) == (stage == OpenFpgaStage::DEVICE));
  }
  for (const OpenFpgaStage& stage : all_stages) {
    CHECK(session.can_run(stage));
    CHECK(session.complete(stage));
    CHECK(session.reached(stage));
  }
  CHECK(reached_are(session, std::vector<OpenFpgaStage>(std::begin(all_stages),
                                                        std::end(all_stages))));
}

static void test_no_skipping() {
  OpenFpgaSession session;
  read_all_inputs(session);
  CHECK(!session.complete(OpenFpgaStage::PLACED));
  CHECK(session.reached_stages().empty());

  CHECK(session.complete(OpenFpgaStage::DEVICE));
  CHECK(!session.complete(OpenFpgaStage::PLACED));
  CHECK(!session.complete(OpenFpgaStage::ROUTED));
  CHECK(!session.complete(OpenFpgaStage::ANALYZED));
  CHECK(!session.complete(OpenFpgaStage::REPACKED));
  CHECK(!session.complete(OpenFpgaStage::BITSTREAM));
  /* A rejected stage leaves the session where it was */
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE}));

  CHECK(session.complete(OpenFpgaStage::PACKED));
  CHECK(!session.complete(OpenFpgaStage::ROUTED));
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED}));
}

static void test_branches() {
  OpenFpgaSession session;
  run_up_to(session, OpenFpgaStage::ROUTED);
  /* Both branches are open once routed */
  CHECK(session.can_run(OpenFpgaStage::ANALYZED));
  CHECK(session.can_run(OpenFpgaStage::REPACKED));
  CHECK(!session.can_run(OpenFpgaStage::BITSTREAM));

  /* Repacking does not wait for the analysis */
  CHECK(session.complete(OpenFpgaStage::REPACKED));
  CHECK(!session.reached(OpenFpgaStage::ANALYZED));
  CHECK(session.can_run(OpenFpgaStage::BITSTREAM));
  CHECK(session.complete(OpenFpgaStage::BITSTREAM));
  CHECK(!session.reached(OpenFpgaStage::ANALYZED));

  /* The analysis can come later and leaves the repacking alone */
  CHECK(session.complete(OpenFpgaStage::ANALYZED));
  CHECK(session.reached(OpenFpgaStage::REPACKED));
  CHECK(session.reached(OpenFpgaStage::BITSTREAM));

  /* Analysis first, then repacking */
  OpenFpgaSession other;
  run_up_to(other, OpenFpgaStage::ANALYZED);
  CHECK(other.complete(OpenFpgaStage::REPACKED));
  CHECK(other.reached(OpenFpgaStage::ANALYZED));
}

static void test_rerun_invalidates_dependents() {
  OpenFpgaSession session;
  run_all(session);

  /* Routing again drops both branches */
  CHECK(session.complete(OpenFpgaStage::ROUTED));
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED,
                              OpenFpgaStage::PLACED, OpenFpgaStage::ROUTED}));

  /* Placing again drops the routing and what follows */
  run_all(session);
  CHECK(session.complete(OpenFpgaStage::PLACED));
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED,
                              OpenFpgaStage::PLACED}));
  CHECK(session.can_run(OpenFpgaStage::ROUTED));
  CHECK(!session.can_run(OpenFpgaStage::ANALYZED));
  CHECK(!session.can_run(OpenFpgaStage::REPACKED));

  /* Packing again drops everything but the device */
  run_all(session);
  CHECK(session.complete(OpenFpgaStage::PACKED));
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED}));

  /* Running the device again drops all the stages and keeps the inputs */
  run_all(session);
  CHECK(session.complete(OpenFpgaStage::DEVICE));
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE}));
  CHECK(session.has_all_inputs());
}

static void test_rerun_sibling_branches() {
  OpenFpgaSession session;
  run_all(session);

  /* Repacking again drops the bitstream only */
  CHECK(session.complete(OpenFpgaStage::REPACKED));
  CHECK(session.reached(OpenFpgaStage::ANALYZED));
  CHECK(session.reached(OpenFpgaStage::ROUTED));
  CHECK(!session.reached(OpenFpgaStage::BITSTREAM));

  /* The analysis can be repeated and drops nothing */
  run_all(session);
  CHECK(session.complete(OpenFpgaStage::ANALYZED));
  CHECK(session.reached(OpenFpgaStage::REPACKED));
  CHECK(session.reached(OpenFpgaStage::BITSTREAM));

  /* The last stage can be repeated */
  CHECK(session.complete(OpenFpgaStage::BITSTREAM));
  CHECK(session.reached(OpenFpgaStage::BITSTREAM));
}

static void test_reread_inputs() {
  OpenFpgaSession session;
  run_all(session);

  /* Reading the OpenFPGA architecture again drops the stages, not the inputs */
  CHECK(session.read_input(OpenFpgaInput::OPENFPGA_ARCH));
  CHECK(session.reached_stages().empty());
  CHECK(session.has_all_inputs());
  CHECK(session.can_run(OpenFpgaStage::DEVICE));

  /* Reading the circuit again drops the stages */
  CHECK(session.complete(OpenFpgaStage::DEVICE));
  CHECK(session.read_input(OpenFpgaInput::CIRCUIT));
  CHECK(session.reached_stages().empty());
  CHECK(session.has_all_inputs());

  /* Reading the VPR architecture again drops the circuit as well */
  CHECK(session.complete(OpenFpgaStage::DEVICE));
  CHECK(session.read_input(OpenFpgaInput::VPR_ARCH));
  CHECK(session.reached_stages().empty());
  CHECK(session.has_input(OpenFpgaInput::VPR_ARCH));
  CHECK(session.has_input(OpenFpgaInput::OPENFPGA_ARCH));
  CHECK(!session.has_input(OpenFpgaInput::CIRCUIT));
  CHECK(!session.has_all_inputs());
  CHECK(!session.can_run(OpenFpgaStage::DEVICE));
  CHECK(session.can_read(OpenFpgaInput::CIRCUIT));

  /* The flow can then go on */
  CHECK(session.read_input(OpenFpgaInput::CIRCUIT));
  CHECK(session.complete(OpenFpgaStage::DEVICE));
}

static void test_failed_input() {
  OpenFpgaSession session;
  run_all(session);

  session.fail_input(OpenFpgaInput::OPENFPGA_ARCH);
  CHECK(!session.has_input(OpenFpgaInput::OPENFPGA_ARCH));
  CHECK(session.has_input(OpenFpgaInput::VPR_ARCH));
  CHECK(session.has_input(OpenFpgaInput::CIRCUIT));
  CHECK(session.reached_stages().empty());

  /* A failure on the VPR architecture takes the circuit with it */
  session.fail_input(OpenFpgaInput::VPR_ARCH);
  CHECK(!session.has_input(OpenFpgaInput::VPR_ARCH));
  CHECK(!session.has_input(OpenFpgaInput::CIRCUIT));
  CHECK(session.uninitialized());
}

static void test_failure() {
  OpenFpgaSession session;

  /* A failed stage and the ones that depend on it are dropped */
  run_all(session);
  session.fail(OpenFpgaStage::PLACED);
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED}));
  CHECK(session.can_run(OpenFpgaStage::PLACED));
  CHECK(!session.can_run(OpenFpgaStage::ROUTED));

  /* Only the branch that fails is dropped */
  run_all(session);
  session.fail(OpenFpgaStage::ANALYZED);
  CHECK(!session.reached(OpenFpgaStage::ANALYZED));
  CHECK(session.reached(OpenFpgaStage::REPACKED));
  CHECK(session.reached(OpenFpgaStage::BITSTREAM));

  run_all(session);
  session.fail(OpenFpgaStage::REPACKED);
  CHECK(session.reached(OpenFpgaStage::ANALYZED));
  CHECK(!session.reached(OpenFpgaStage::REPACKED));
  CHECK(!session.reached(OpenFpgaStage::BITSTREAM));

  run_all(session);
  session.fail(OpenFpgaStage::BITSTREAM);
  CHECK(session.reached(OpenFpgaStage::REPACKED));
  CHECK(!session.reached(OpenFpgaStage::BITSTREAM));

  /* A failure of the device drops all the stages, and keeps the inputs */
  run_all(session);
  session.fail(OpenFpgaStage::DEVICE);
  CHECK(session.reached_stages().empty());
  CHECK(session.has_all_inputs());
  CHECK(session.can_run(OpenFpgaStage::DEVICE));

  /* Failing a stage that was not reached changes nothing */
  run_up_to(session, OpenFpgaStage::PACKED);
  session.fail(OpenFpgaStage::ANALYZED);
  CHECK(reached_are(session, {OpenFpgaStage::DEVICE, OpenFpgaStage::PACKED}));
  OpenFpgaSession fresh;
  fresh.fail(OpenFpgaStage::ROUTED);
  fresh.fail(OpenFpgaStage::UNINIT);
  CHECK(fresh.uninitialized());

  /* The session recovers by running the stage again */
  run_up_to(session, OpenFpgaStage::ROUTED);
  session.fail(OpenFpgaStage::ANALYZED);
  CHECK(session.complete(OpenFpgaStage::ANALYZED));
}

static void test_reset() {
  OpenFpgaSession session;
  run_up_to(session, OpenFpgaStage::ROUTED);
  session.reset();
  CHECK(session.uninitialized());
  CHECK(session.reached_stages().empty());
  CHECK(!session.has_all_inputs());
  CHECK(!session.can_run(OpenFpgaStage::DEVICE));
  CHECK(!session.can_read(OpenFpgaInput::CIRCUIT));
}

static void test_names() {
  CHECK(openfpga::to_string(OpenFpgaStage::UNINIT) == "UNINIT");
  CHECK(openfpga::to_string(OpenFpgaStage::DEVICE) == "DEVICE");
  CHECK(openfpga::to_string(OpenFpgaStage::PACKED) == "PACKED");
  CHECK(openfpga::to_string(OpenFpgaStage::PLACED) == "PLACED");
  CHECK(openfpga::to_string(OpenFpgaStage::ROUTED) == "ROUTED");
  CHECK(openfpga::to_string(OpenFpgaStage::ANALYZED) == "ANALYZED");
  CHECK(openfpga::to_string(OpenFpgaStage::REPACKED) == "REPACKED");
  CHECK(openfpga::to_string(OpenFpgaStage::BITSTREAM) == "BITSTREAM");
  CHECK(openfpga::to_string(OpenFpgaInput::VPR_ARCH) == "VPR_ARCH");
  CHECK(openfpga::to_string(OpenFpgaInput::OPENFPGA_ARCH) == "OPENFPGA_ARCH");
  CHECK(openfpga::to_string(OpenFpgaInput::CIRCUIT) == "CIRCUIT");
}

static void test_vpr_generation() {
  OpenFpgaSession session;
  CHECK(0 == session.vpr_generation());
  session.set_vpr_generation(7);
  CHECK(7 == session.vpr_generation());
  /* Resetting forgets the inputs and the stages, not the generation */
  session.reset();
  CHECK(7 == session.vpr_generation());
}

static void test_linked_generation() {
  OpenFpgaSession session;
  CHECK(0 == session.linked_generation());
  session.set_linked_generation(3);
  CHECK(3 == session.linked_generation());
  /* What was linked is forgotten with the flow */
  session.reset();
  CHECK(0 == session.linked_generation());
}

static void test_required_stage() {
  CHECK(openfpga::required_stage(OpenFpgaStage::UNINIT) ==
        OpenFpgaStage::UNINIT);
  CHECK(openfpga::required_stage(OpenFpgaStage::DEVICE) ==
        OpenFpgaStage::UNINIT);
  CHECK(openfpga::required_stage(OpenFpgaStage::PACKED) ==
        OpenFpgaStage::DEVICE);
  CHECK(openfpga::required_stage(OpenFpgaStage::PLACED) ==
        OpenFpgaStage::PACKED);
  CHECK(openfpga::required_stage(OpenFpgaStage::ROUTED) ==
        OpenFpgaStage::PLACED);
  CHECK(openfpga::required_stage(OpenFpgaStage::ANALYZED) ==
        OpenFpgaStage::ROUTED);
  CHECK(openfpga::required_stage(OpenFpgaStage::REPACKED) ==
        OpenFpgaStage::ROUTED);
  CHECK(openfpga::required_stage(OpenFpgaStage::BITSTREAM) ==
        OpenFpgaStage::REPACKED);
}

static void test_context_has_session() {
  OpenfpgaContext context;
  const OpenfpgaContext& const_context = context;
  CHECK(const_context.session().uninitialized());
  CHECK(context.mutable_session().read_input(OpenFpgaInput::VPR_ARCH));
  CHECK(const_context.session().has_input(OpenFpgaInput::VPR_ARCH));
}

int main() {
  test_initial_state();
  test_inputs();
  test_input_orders();
  test_forward_flow();
  test_no_skipping();
  test_branches();
  test_rerun_invalidates_dependents();
  test_rerun_sibling_branches();
  test_reread_inputs();
  test_failed_input();
  test_failure();
  test_reset();
  test_names();
  test_vpr_generation();
  test_linked_generation();
  test_required_stage();
  test_context_has_session();

  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All OpenFpgaSession tests passed\n");
  return 0;
}
