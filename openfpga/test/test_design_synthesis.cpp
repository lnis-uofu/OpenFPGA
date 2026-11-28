/********************************************************************
 * Unit test of the plan of a synthesis (design_synthesis.h) with a
 * synthesis target, as a plugin gives one: its script and values, its LUT
 * size, extended BLIF, the checks of its values, the line of the
 * constraints, and the cell libraries of the hierarchy. Yosys does not run
 *******************************************************************/
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "design_synthesis.h"

namespace fs = std::filesystem;
using openfpga::make_hierarchy_script;
using openfpga::make_synthesis_plan;
using openfpga::SynthesisPlan;
using openfpga::SynthesisRefused;
using openfpga::SynthesisRequest;
using openfpga::SynthesisTarget;

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

static bool has(const std::string& text, const std::string& part) {
  return std::string::npos != text.find(part);
}

/* The message of the SynthesisRefused of a plan, or "" */
static std::string refusal(const SynthesisRequest& request,
                           const SynthesisTarget* target) {
  try {
    make_synthesis_plan(request, target);
  } catch (const SynthesisRefused& error) {
    return error.what();
  }
  return "";
}

static void write(const fs::path& path, const std::string& text) {
  std::ofstream(path) << text;
}

int main() {
  const fs::path dir = fs::temp_directory_path() / "test_design_synthesis";
  fs::remove_all(dir);
  fs::create_directories(dir);
  write(dir / "and2.v",
        "module and2(input a, input b, output y);\n  assign y = a & b;\n"
        "endmodule\n");
  write(dir / "cells.v", "module cell(input a, output y);\nendmodule\n");
  write(dir / "synth.ys",
        "read_verilog ${READ_VERILOG_OPTIONS} ${VERILOG_FILES}\n"
        "hierarchy -top ${TOP_MODULE}\n${CONSTRAINT_SYNTHESIS}\n"
        "read_verilog -lib ${YOSYS_CELL_SIM_VERILOG}\n"
        "techmap ${YOSYS_DSP_MAP_PARAMETERS}\n"
        "abc -lut ${LUT_SIZE}\nwrite_blif ${OUTPUT_BLIF}\n");

  SynthesisRequest request;
  request.top = "and2";
  request.files = {dir / "and2.v"};
  request.folder = dir / "synth";

  /* No target: the generic script, with the LUT size of the request */
  CHECK(has(refusal(request, nullptr), "lut_size (2 to 8) is needed"));
  request.lut_size = 4;
  SynthesisPlan plan = make_synthesis_plan(request, nullptr);
  CHECK(plan.target.empty() && 4 == plan.lut_size);
  CHECK(has(plan.script, "abc -lut 4") && !has(plan.script, "${"));
  CHECK(".blif" == plan.netlist.extension());

  /* A target without a script: the generic one, with its LUT size */
  SynthesisTarget target;
  target.name = "dev";
  target.lut_size = 6;
  plan = make_synthesis_plan(request, &target);
  CHECK("dev" == plan.target && 6 == plan.lut_size);
  CHECK(has(plan.script, "abc -lut 6"));

  /* Its script and its values; extended BLIF from its VPR options */
  target.yosys_script = dir / "synth.ys";
  target.values = {{"READ_VERILOG_OPTIONS", "-nolatches"},
                   {"YOSYS_CELL_SIM_VERILOG", (dir / "cells.v").string()},
                   {"YOSYS_DSP_MAP_PARAMETERS", "-D A=8 -D B=8"}};
  target.vpr_options = {{"circuit_format", "eblif"}};
  plan = make_synthesis_plan(request, &target);
  CHECK(
    has(plan.script, "read_verilog -nolatches " + (dir / "and2.v").string()));
  CHECK(has(plan.script, "read_verilog -lib " + (dir / "cells.v").string()));
  CHECK(has(plan.script, "techmap -D A=8 -D B=8"));
  CHECK(".eblif" == plan.netlist.extension());

  /* The constraints go on their line; a script without it refuses them */
  request.constraints = "setattr -mod -set keep 1 and2\n";
  plan = make_synthesis_plan(request, &target);
  CHECK(plan.constraints && has(plan.script, "setattr -mod -set keep 1 and2"));
  SynthesisTarget plain = target;
  write(dir / "plain.ys", "read_verilog ${VERILOG_FILES}\n");
  plain.yosys_script = dir / "plain.ys";
  CHECK(has(refusal(request, &plain), "${CONSTRAINT_SYNTHESIS}"));
  request.constraints.clear();

  /* What a target may not give */
  SynthesisTarget bad = target;
  bad.values["YOSYS_DSP_MAP_PARAMETERS"] = "-D A=8\nshell rm -rf /";
  CHECK(has(refusal(request, &bad), "characters that a Yosys script"));
  bad = target;
  bad.values["READ_VERILOG_OPTIONS"] = "-nolatches x.v";
  CHECK(has(refusal(request, &bad), "options like -nolatches"));
  bad = target;
  bad.values["YOSYS_CELL_SIM_VERILOG"] = "cells,x.v";
  CHECK(has(refusal(request, &bad), "characters that a Yosys script"));
  bad = target;
  bad.yosys_script = dir / "missing.ys";
  CHECK(has(refusal(request, &bad), "does not exist"));
  bad = target;
  bad.lut_size = 12;
  CHECK(has(refusal(request, &bad), "lut_size"));

  /* The cell libraries of the target are blackboxes of the hierarchy */
  target.cell_libraries = {dir / "cells.v"};
  const std::string hierarchy =
    make_hierarchy_script(request, &target, dir / "h.json");
  CHECK(
    has(hierarchy, "read_verilog -lib -specify " + (dir / "cells.v").string()));
  CHECK(has(hierarchy, "read_verilog -nolatches"));
  CHECK(!has(make_hierarchy_script(request, nullptr, dir / "h.json"), "-lib"));

  /* The provider of the session, and what it throws */
  OpenfpgaContext ctx;
  SynthesisTarget given;
  CHECK(!openfpga::synthesis_target(ctx, given));
  openfpga::set_synthesis_target_provider(
    [](const OpenfpgaContext&, SynthesisTarget& out) {
      out.name = "from_plugin";
      return true;
    });
  CHECK(openfpga::synthesis_target(ctx, given) && "from_plugin" == given.name);
  openfpga::set_synthesis_target_provider(
    [](const OpenfpgaContext&, SynthesisTarget&) -> bool {
      throw std::runtime_error("bad manifest");
    });
  bool thrown = false;
  try {
    openfpga::synthesis_target(ctx, given);
  } catch (const std::exception& error) {
    thrown = has(error.what(), "bad manifest");
  }
  CHECK(thrown);
  openfpga::set_synthesis_target_provider(nullptr);
  CHECK(!openfpga::synthesis_target(ctx, given));

  openfpga::set_design_constraints_command("read_mine");
  CHECK("read_mine" == openfpga::design_constraints_command());
  openfpga::set_design_constraints_command("");

  fs::remove_all(dir);
  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All design synthesis tests passed\n");
  return 0;
}
