/********************************************************************
 * Unit tests of libodc (docs/spec_odc.md): the format, the queries, the
 * locations, the checks against a netlist and a device, and the writers
 *******************************************************************/
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "odc.h"

namespace {

int failures = 0;

void check(const bool condition, const std::string& what) {
  if (!condition) {
    std::cerr << "FAIL: " << what << "\n";
    ++failures;
  }
}

odc::Constraints read(const std::string& text, const bool lenient = false) {
  odc::ReadOptions options;
  options.lenient = lenient;
  return odc::read_odc_text(text, "t.odc", options);
}

/* The first diagnostic of a level, or an empty one */
odc::Diagnostic first(const std::vector<odc::Diagnostic>& diagnostics,
                      const odc::Diagnostic::Level level) {
  for (const odc::Diagnostic& diagnostic : diagnostics) {
    if (level == diagnostic.level) {
      return diagnostic;
    }
  }
  return {};
}

bool has(const std::string& text, const std::string& part) {
  return std::string::npos != text.find(part);
}

std::filesystem::path temp_dir() {
  const std::filesystem::path dir =
    std::filesystem::temp_directory_path() / "test_odc";
  std::filesystem::remove_all(dir);
  std::filesystem::create_directories(dir);
  return dir;
}

void write(const std::filesystem::path& path, const std::string& text) {
  std::ofstream(path) << text;
}

/* T-1: the format */
void test_format() {
  odc::Constraints c = read(
    "odc_version 1\n"
    "# a comment\n"
    "create_clock -period 5 \\\n"
    "  -name clk [get_ports clk]   # a comment after\n"
    "\n"
    "set_input_delay -clock clk -max 1 [get_ports {a[0] b c}]\n"
    "set_property PACKAGE_PIN \"LED \\\"0\\\"\" [get_ports out[0]]\n");
  check(!c.has_errors(), "a valid file has no error");
  check(3 == c.commands.size(), "three commands");
  check(3 == c.commands[0].line, "a continued line has its first line");
  check(c.commands[0].words.size() == 6, "the continued line is one command");
  const odc::Query& q = c.commands[1].words.back().query;
  check(3 == q.patterns.size() && "a[0]" == q.patterns[0],
        "a braced list in a query, with a[0] as one name");
  check("LED \"0\"" == c.commands[2].value, "a quoted word with \\\"");
  check("out[0]" == c.commands[2].words[3].query.patterns[0],
        "out[0] is one bare word");

  c = read("create_clock -period 5 [get_ports clk]\n");
  check(!c.has_errors(), "no header is not an error");
  check(odc::Diagnostic::WARNING == c.diagnostics.at(0).level &&
          has(c.diagnostics[0].message, "odc_version 1"),
        "no header is a warning");

  c = read("odc_version 2\n");
  check(c.has_errors() && has(c.diagnostics[0].message, "newer"),
        "a higher version is refused");

  c = read("odc_version 1\nfrobnicate 1\n");
  const odc::Diagnostic e = first(c.diagnostics, odc::Diagnostic::ERROR);
  check("t.odc" == e.file && 2 == e.line && has(e.message, "frobnicate"),
        "an unknown command is an error with its file and line");
  c = read("odc_version 1\ncreate_clok -period 2 [get_ports clk]\n");
  check(has(c.diagnostics.at(0).message, "the nearest: create_clock"),
        "an unknown command names the nearest command");
  c = read("odc_version 1\nfrobnicate 1\n", true);
  check(!c.has_errors() && c.commands.empty(),
        "an unknown command is skipped with --lenient");

  c = read("odc_version 1\nset_property LOC {X1Y1 [get_cells a]\n");
  check(c.has_errors() && has(c.diagnostics[0].message, "not closed"),
        "an unbalanced brace");
  c = read("odc_version 1\ncreate_clock -period 1 [get_ports a]b\n");
  check(c.has_errors(), "text glued to a query");
  c = read("odc_version 1\nodc_version 1\n");
  check(c.has_errors(), "odc_version only first");
}

/* T-1 and the table: arguments of each kind */
void test_commands() {
  odc::Constraints c = read(
    "odc_version 1\n"
    "set_property KEEP true [get_nets debug]\n"
    "set_property DONT_TOUCH true [get_cells u0]\n"
    "set_property MAX_FANOUT 8 [get_nets n]\n"
    "set_property RAM_STYLE BLOCK [get_cells mem]\n"
    "set_synth_option -define WIDTH=8\n"
    "set_synth_option -flatten\n"
    "create_pblock pb\n"
    "resize_pblock pb -add X1Y1:X2Y3\n"
    "add_cells_to_pblock pb [get_cells -hierarchical ctrl_*]\n"
    "set_property LOC X2Y3 [get_cells r0]\n"
    "set_global_signal -route_model ideal [get_nets clk]\n"
    "set_property LOC X0Y2/4 [get_ports clk]\n"
    "set_property PACKAGE_PIN LED0 [get_ports out]\n"
    "set_property PIN_DEFAULT 1 [get_ports rst]\n");
  check(!c.has_errors(), "every command of version 1");
  check(odc::Kind::SYNTHESIS == c.commands[0].kind &&
          odc::Stage::CIRCUIT == c.commands[0].stage,
        "KEEP is synthesis, stage CIRCUIT");
  check("block" == c.commands[3].value, "RAM_STYLE value in lower case");
  check(odc::Kind::FLOORPLAN == c.commands[9].kind, "LOC of a cell");
  check(odc::Kind::PIN == c.commands[11].kind, "LOC of a port");
  check(odc::Kind::ROUTING == c.commands[10].kind, "set_global_signal");

  const char* refused[] = {
    "set_property KEEP maybe [get_nets a]\n",
    "set_property KEEP true [get_ports a]\n",
    "set_property RAM_STYLE ultra [get_cells a]\n",
    "set_property MAX_FANOUT 0 [get_nets a]\n",
    "set_property LOC X1Y1:X2Y2 [get_cells a]\n",
    "set_property IOSTANDARD LVCMOS33 [get_ports a]\n",
    "set_property KEEP true [get_nets -regexp a.*]\n",
    "resize_pblock nope -add X1Y1\n",
    "create_pblock pb\ncreate_pblock pb\n",
    "set_global_signal -route_model dedicated_network [get_nets clk]\n",
    "set_global_signal -route_model wire [get_nets clk]\n",
    "set_synth_option -top \"a b\"\n",
    "create_clock -name c [get_ports clk]\n",
    "set_input_delay 1 [get_pblocks pb]\n",
    "set_property LOC X1Y1 [get_nets a]\n",
  };
  for (const char* text : refused) {
    check(read(std::string("odc_version 1\n") + text).has_errors(),
          std::string("refused: ") + text);
  }
}

/* T-2: locations */
void test_locations() {
  odc::Location l;
  std::string error;
  check(odc::parse_location("X3Y2", l, error) && 3 == l.x_low && 2 == l.y_low &&
          -1 == l.subtile && l.is_tile(),
        "a tile");
  check(odc::parse_location("X3Y2/1", l, error) && 1 == l.subtile, "a subtile");
  check(odc::parse_location("X4Y6:X1Y1", l, error) && 1 == l.x_low &&
          4 == l.x_high && 1 == l.y_low && 6 == l.y_high && !l.is_tile(),
        "a rectangle, corners in any order");
  check(!odc::parse_location("3,2", l, error) && has(error, "not a location"),
        "not a location");
}

std::vector<odc::Object> names(const std::vector<std::string>& list) {
  std::vector<odc::Object> objects;
  for (const std::string& name : list) {
    objects.push_back({name, {}});
  }
  return objects;
}

std::vector<std::string> matched(const std::string& query_text,
                                 const std::vector<odc::Object>& objects) {
  odc::Constraints c =
    read("odc_version 1\nset_false_path -from [" + query_text + "]\n");
  check(!c.has_errors(), "query " + query_text);
  std::vector<const odc::Object*> found;
  std::string error;
  odc::match_query(c.commands.at(0).words.back().query, objects, found, error);
  std::vector<std::string> out;
  for (const odc::Object* object : found) {
    out.push_back(object->name);
  }
  return out;
}

/* T-3: queries */
void test_queries() {
  const auto objects = names({"OUT0", "OUT1", "out:OUT0", "out[0]", "data[3]",
                              "u0.state", "u0.sub.state", "ab"});
  check(2 == matched("get_ports OUT?", objects).size(), "? is one character");
  check(1 == matched("get_ports out[0]", objects).size(), "out[0] is a name");
  check(1 == matched("get_ports data[*]", objects).size(), "data[*]");
  check(matched("get_ports {OUT[0-2]}", objects).empty(),
        "[ ] are not special in a glob");
  check(2 == matched("get_ports -regexp {OUT[0-2]}", objects).size(),
        "-regexp is matched whole: out:OUT0 is not taken");
  check(matched("get_cells *state", objects).empty(),
        "* does not cross the hierarchy");
  check(2 == matched("get_cells -hierarchical *state", objects).size(),
        "-hierarchical");
  check(1 == matched("get_cells -nocase AB", objects).size(), "-nocase");

  std::vector<odc::Object> ports = {{"a", {{"direction", "in"}}},
                                    {"b", {{"direction", "out"}}}};
  check(std::vector<std::string>{"b"} ==
          matched("get_ports -filter {direction == out}", ports),
        "-filter direction");
  check(std::vector<std::string>{"a"} == matched("all_inputs", ports),
        "all_inputs");
  check(read("odc_version 1\nset_false_path -from [get_ports -filter {color "
             "== red}]\n")
          .has_errors(),
        "an unknown property of a filter");
  std::vector<odc::Object> sites = {
    {"X0Y1/0", {{"side", "TOP"}, {"pin_type", "GPIO"}}},
    {"X0Y1/1", {{"side", "TOP"}, {"pin_type", "UNDEF"}}},
    {"X1Y0/0", {{"side", "LEFT"}, {"pin_type", "GPIO"}}}};
  odc::Query q;
  q.kind = "sites";
  q.patterns = {"*"};
  q.hierarchical = true;
  q.filter = "side == TOP && pin_type == GPIO || side =~ L*";
  std::vector<const odc::Object*> found;
  std::string error;
  check(odc::match_query(q, sites, found, error) && 2 == found.size(),
        "a filter of sites with && and ||");

  check("^out\\[0\\]$" == odc::exact_regex("out[0]"), "exact regex");
  check("^OUT[^./]*$" == odc::glob_regex("OUT*", false), "glob regex");
  check(odc::nearest_names("stat", objects, 1).at(0) == "u0.state" ||
          odc::nearest_names("stat", objects, 1).at(0) == "ab",
        "nearest names");
}

/* A netlist and a device for the checks */
const char* BLIF =
  ".model top\n"
  ".inputs clk a b rst\n"
  ".outputs q y\n"
  ".names a b n1\n11 1\n"
  ".names b n2\n1 1\n"
  ".latch n1 q re clk 0\n"
  ".subckt mult_16 a=a b=b out=y\n"
  ".end\n"
  ".model mult_16\n.inputs a b\n.outputs out\n.blackbox\n.end\n";

odc::Device make_device() {
  /* 4x4: I/O ring at x=0, clb at x=1..2, a mult column at x=3 two tiles tall */
  odc::Device d;
  d.width = 4;
  d.height = 4;
  d.clock_modeling = "ideal";
  d.tiles.resize(16);
  for (int x = 0; x < 4; ++x) {
    for (int y = 0; y < 4; ++y) {
      odc::Tile& t = d.tiles[x * 4 + y];
      t.root_x = x;
      t.root_y = y;
      if (0 == x) {
        t.type = "io";
        t.capacity = 8;
        t.io = true;
      } else if (3 == x) {
        t.type = "mult";
        t.capacity = 1;
        t.root_y = y - y % 2;
      } else {
        t.type = "clb";
        t.capacity = 1;
      }
    }
  }
  d.type_models["io"] = {{"input", 8}, {"output", 8}};
  d.type_models["clb"] = {{"names", 1}, {"latch", 1}};
  d.type_models["mult"] = {{"mult_16", 1}};
  return d;
}

std::vector<odc::Site> make_sites() {
  std::vector<odc::Site> sites;
  auto add = [&sites](const char* direction, const char* type, int y, int s,
                      const char* name, const char* alias) {
    odc::Site site;
    site.line = sites.size() + 2;
    site.side = "LEFT";
    site.pin_direction = direction;
    site.pin_type = type;
    site.has_location = y >= 0;
    site.x = 0;
    site.y = y;
    site.subtile = s;
    site.netlist_name = name;
    site.customer_pin_alias = alias;
    sites.push_back(site);
  };
  add("input", "GPIO", 1, 0, "pad_in[0]", "P0");
  add("output", "GPIO", 1, 1, "pad_out[0]", "P0");
  add("input", "UNDEF", 2, 0, "pad_in[1]", "P1");
  add("input", "CONTROL", -1, 0, "op_reset[0]", "RST");
  return sites;
}

/* T-6, T-7, T-8: the checks */
void test_resolve() {
  const std::filesystem::path dir = temp_dir();
  write(dir / "top.blif", BLIF);
  const odc::Netlist netlist = odc::read_blif_netlist(dir / "top.blif");
  check("top" == netlist.top && 6 == netlist.ports.size(), "the ports");
  check(
    netlist.port("y") && "out" == netlist.port("y")->properties.at("direction"),
    "an output");
  bool mult = false;
  for (const odc::Object& cell : netlist.cells) {
    mult |= "y" == cell.name && "mult_16" == cell.properties.at("type");
  }
  check(mult, "a .subckt is named by the net of its first output");
  check(1 == netlist.clock_nets.count("clk"), "the clock of a latch");

  const odc::Device device = make_device();
  const std::vector<odc::Site> sites = make_sites();
  odc::Design design;
  design.netlist = &netlist;
  design.device = &device;
  design.sites = &sites;
  design.io_map = true;

  odc::Constraints c = read(
    "odc_version 1\n"
    "create_clock -period 5 [get_ports clk]\n"
    "set_input_delay -clock clk -max 1 [get_ports {a nope}]\n"
    "create_pblock pb\n"
    "resize_pblock pb -add X1Y1:X2Y2\n"
    "add_cells_to_pblock pb [get_cells {n1 q}]\n"
    "set_property LOC X3Y2 [get_cells y]\n"
    "set_property PACKAGE_PIN P0 [get_ports q]\n"
    "set_property PACKAGE_PIN P0 [get_ports a]\n"
    "set_property PACKAGE_PIN RST [get_ports rst]\n"
    "set_property PIN_DEFAULT 1 [get_ports rst]\n"
    "set_property LOC X0Y3/5 [get_ports b]\n"
    "set_global_signal -route_model ideal [get_nets clk]\n");
  check(!c.has_errors(), "the constraints read");
  odc::Resolution r = odc::resolve(c, design);
  check(!r.has_errors(), "the constraints fit: " +
                           first(r.diagnostics, odc::Diagnostic::ERROR).text());
  const odc::Diagnostic w = first(r.diagnostics, odc::Diagnostic::WARNING);
  check(3 == w.line && has(w.message, "'nope'"),
        "an object of a list that matches nothing is reported: " + w.text());
  bool nope = false;
  for (const odc::QueryMatch& m : r.queries) {
    nope |= 1 == m.command && 1 == m.matched.size();
  }
  check(nope, "the query matched a only");
  check(r.timing.checked && 1 == r.timing.clocks.size() &&
          "5" == r.timing.clocks[0].period,
        "the clocks");
  check(r.timing.outputs_without_delay.size() == 2, "outputs without delay");
  odc::Resolution bare = odc::resolve(
    read("odc_version 1\ncreate_clock -period 5 -waveform {0 2.5} clk\n"),
    design);
  check(1 == bare.timing.clocks.size() && "clk" == bare.timing.clocks[0].name &&
          bare.timing.unconstrained_clocks.empty(),
        "a clock named by its bare target, as in an SDC, is the clock of it");
  const std::string xml = odc::write_vpr_constraints(r);
  check(
    has(xml, "<add_atom name_pattern=\"out:q\"/>") && has(xml, "subtile=\"1\""),
    "an output port on its pin, with the atom out:");
  check(has(xml, "<add_atom name_pattern=\"a\"/>") && has(xml, "subtile=\"0\""),
        "an input port on the input of the pin");
  check(has(xml, "name=\"pblock_pb\"") && has(xml, "name_pattern=\"n1\""),
        "the pblock");
  check(has(xml, "<set_global_signal name=\"clk\" route_model=\"ideal\"/>"),
        "the global signal");
  const std::string pins = odc::write_pin_constraints(r);
  check(has(pins,
            "<set_io pin=\"op_reset[0]\" net=\"rst\" "
            "default_value=\"1\"/>"),
        "a pin without a location is a set_io");

  /* The errors */
  struct Case {
    const char* text;
    const char* message;
  } cases[] = {
    {"set_property LOC X9Y1 [get_cells n1]\n", "outside the grid"},
    {"set_property LOC X3Y3 [get_cells y]\n", "inside the tile mult"},
    {"set_property LOC X1Y1/3 [get_cells n1]\n", "has 1 subtiles"},
    {"set_property LOC X1Y1 [get_cells y]\n", "cannot hold the cell 'y'"},
    {"create_pblock p\nresize_pblock p -add X1Y1\nadd_cells_to_pblock p "
     "[get_cells y]\n",
     "can hold the cell 'y'"},
    {"create_pblock p\nresize_pblock p -add X1Y1\nadd_cells_to_pblock p "
     "[get_cells {n1 n2}]\n",
     "has room for"},
    {"create_pblock p\nresize_pblock p -add X1Y1\nadd_cells_to_pblock p "
     "[get_cells n1]\nset_property LOC X2Y2 [get_cells n1]\n",
     "outside its pblock"},
    {"set_global_signal -route_model route [get_nets clk]\n",
     "general.clock_modeling"},
    {"set_global_signal -route_model dedicated_network -network gclk "
     "[get_nets clk]\n",
     "no clock network 'gclk'"},
    {"set_property PACKAGE_PIN P1 [get_ports y]\n", "is an output"},
    {"set_property PACKAGE_PIN P9 [get_ports a]\n", "has no pin 'P9'"},
    {"set_property LOC X0Y1/0 [get_ports a]\nset_property PACKAGE_PIN P0 "
     "[get_ports b]\n",
     "on the same pin"},
    {"set_property PIN_DEFAULT 1 [get_ports a]\n", "without a location"},
    {"set_property LOC X1Y1/0 [get_ports a]\n", "holds no port"},
  };
  for (const Case& test : cases) {
    c = read(std::string("odc_version 1\n") + test.text);
    check(!c.has_errors(), std::string("read: ") + test.text);
    r = odc::resolve(c, design);
    const odc::Diagnostic e = first(r.diagnostics, odc::Diagnostic::ERROR);
    check(has(e.message, test.message), std::string("error '") + test.message +
                                          "' for " + test.text +
                                          " got: " + e.message);
  }
  c = read("odc_version 1\nset_property PACKAGE_PIN P1 [get_ports a]\n");
  r = odc::resolve(c, design);
  check(!r.has_errors() &&
          has(first(r.diagnostics, odc::Diagnostic::WARNING).message, "UNDEF"),
        "a pin of type UNDEF is a warning");

  /* Without the netlist nor the device: nothing is checked, and the patterns
   * go to VPR anchored */
  c = read(
    "odc_version 1\ncreate_pblock pb\nresize_pblock pb -add X1Y1\n"
    "add_cells_to_pblock pb [get_cells OUT*]\n"
    "set_property LOC X0Y1/2 [get_ports out[0]]\n");
  r = odc::resolve(c, odc::Design());
  check(!r.has_errors() && !r.checked[2], "nothing to check yet");
  const std::string unchecked = odc::write_vpr_constraints(r);
  check(has(unchecked, "name_pattern=\"^OUT[^./]*$\" is_regex=\"true\""),
        "a glob as an anchored regex");
  check(has(unchecked, "name_pattern=\"^(out:)?out\\[0\\]$\""),
        "a port as an anchored regex, input or output");
}

/* R-46: the defaults of the package, overridden by the user */
void test_defaults() {
  const std::filesystem::path dir = temp_dir();
  write(dir / "top.blif", BLIF);
  const odc::Netlist netlist = odc::read_blif_netlist(dir / "top.blif");
  const odc::Device device = make_device();
  const std::vector<odc::Site> sites = make_sites();
  odc::Design design{&netlist, &device, &sites, true};
  const odc::Constraints defaults =
    read("odc_version 1\nset_property LOC X0Y3/1 [get_ports a]\n");
  const odc::Constraints user =
    read("odc_version 1\nset_property PACKAGE_PIN P0 [get_ports a]\n");
  const odc::Constraints merged = odc::merge_defaults(defaults, user);
  const odc::Resolution r = odc::resolve(merged, design);
  check(!r.has_errors() && 1 == r.pins.size() && 1 == r.pins[0].site.y,
        "the user's pin wins over the defaults'");
  check(!has(odc::write_odc(merged), "X0Y3/1"),
        "write_odc leaves the defaults out");
}

/* T-9, T-12: write_odc reads back, includes */
void test_files() {
  const std::filesystem::path dir = temp_dir();
  write(dir / "timing.odc",
        "odc_version 1\ncreate_clock -period 2.5 -name c [get_ports clk]\n");
  std::filesystem::create_directories(dir / "sub");
  write(dir / "sub" / "pins.odc",
        "set_property LOC X0Y1/2 [get_ports {a b}]\ninclude ../timing.odc\n");
  write(dir / "main.odc",
        "odc_version 1\ninclude sub/pins.odc\n"
        "set_property KEEP true [get_nets {x y}]  # keep\n");
  odc::Constraints c = odc::read_odc_file(dir / "main.odc", {});
  check(!c.has_errors(), "includes are relative to the file that includes");
  check(3 == c.files.size() && 3 == c.commands.size(),
        "three files, three commands");
  check(c.commands[1].file == (dir / "sub" / ".." / "timing.odc").string() ||
          has(c.commands[1].file, "timing.odc"),
        "a command has the file it is in");
  const std::string text = odc::write_odc(c);
  write(dir / "back.odc", text);
  const odc::Constraints back = odc::read_odc_file(dir / "back.odc", {});
  check(!back.has_errors() && back.commands.size() == c.commands.size(),
        "write_odc reads back");
  for (size_t i = 0; i < back.commands.size() && i < c.commands.size(); ++i) {
    check(back.commands[i].text() == c.commands[i].text(),
          "the same command: " + c.commands[i].text());
  }
  write(dir / "loop.odc", "odc_version 1\ninclude loop.odc\n");
  c = odc::read_odc_file(dir / "loop.odc", {});
  check(c.has_errors() && has(c.diagnostics[0].message, "includes itself"),
        "a loop of includes");
  c = odc::read_odc_file(dir / "missing.odc", {});
  check(c.has_errors(), "a missing file");

  /* The SDC is the timing commands as they are written */
  c = read(
    "odc_version 1\ncreate_clock -period 5 -name clk [get_ports {clk}]\n"
    "set_property KEEP true [get_nets n]\n");
  const std::string sdc = odc::write_sdc(c);
  check(has(sdc, "create_clock -period 5 -name clk [get_ports {clk}]\n") &&
          !has(sdc, "KEEP"),
        "the SDC");
  const std::string yosys = odc::write_yosys(
    read("odc_version 1\nset_property KEEP true [get_nets n*]\n"
         "set_property DONT_TOUCH true [get_cells u0]\n"
         "set_property RAM_STYLE block [get_cells mem]\n"
         "set_synth_option -define W=8\nset_synth_option -top t\n"));
  check(has(yosys, "setattr -set keep 1 w:n*\n") &&
          has(yosys, "setattr -set keep_hierarchy 1 c:u0") &&
          has(yosys, "setattr -set ram_style \"block\" m:mem c:mem") &&
          !has(yosys, "W=8"),
        "the Yosys commands: " + yosys);
  const odc::SynthesisOptions options = odc::synthesis_options(read(
    "odc_version 1\nset_synth_option -define W=8\nset_synth_option -top t\n"));
  check("t" == options.top && 1 == options.defines.size() &&
          "8" == options.defines[0].second,
        "the options of the synthesis");
}

}  // namespace

int main() {
  test_format();
  test_commands();
  test_locations();
  test_queries();
  test_resolve();
  test_defaults();
  test_files();
  if (failures) {
    std::cerr << failures << " checks failed\n";
    return 1;
  }
  std::cout << "test_odc: all checks passed\n";
  return 0;
}
