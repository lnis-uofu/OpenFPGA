/********************************************************************
 * Unit test of the translation of the application options to the options
 * of VPR:
 * - Every field of VPR's t_options has an application option or a reason
 *   not to have one, so that a change of VTR cannot go unnoticed
 * - The defaults of the table are the defaults of VPR
 * - Every option reaches the field of VPR it stands for, with its value
 * - The command line given to VPR follows its rules
 *******************************************************************/
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#ifndef _WIN32
#include <sys/wait.h>
#include <unistd.h>
#endif

#include "app_options.h"
#include "output_capture.h"
#include "read_options.h"
#include "show_setup.h"
#include "vpr_api.h"
#include "vpr_app_options.h"
#include "vpr_option_table.h"

using openfpga::AppOptions;
using openfpga::AppOptionValue;

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

/* Words of a text */
static std::vector<std::string> words(const std::string& text) {
  std::vector<std::string> list;
  std::istringstream stream(text);
  std::string word;
  while (stream >> word) {
    list.push_back(word);
  }
  return list;
}

static bool is_number(const std::string& text, double& number) {
  char* end = nullptr;
  number = std::strtod(text.c_str(), &end);
  return !text.empty() && *end == '\0';
}

/* Return true if the value read from VPR is the one of the option */
static bool same_value(const AppOptionValue& option,
                       const AppOptionValue& vpr) {
  switch (option.type) {
    case AppOptionValue::BOOLEAN:
      return option.bool_value == vpr.bool_value;
    case AppOptionValue::INT:
      return option.int_value == vpr.int_value;
    case AppOptionValue::FLOAT:
      /* VPR keeps floats */
      return static_cast<float>(option.float_value) ==
             static_cast<float>(vpr.float_value);
    case AppOptionValue::SELECTION:
      return option.to_enum() == vpr.int_value;
    default:
      break;
  }
  /* Text, or a list of values: the numbers are compared as numbers */
  const std::vector<std::string> left = words(option.string_value);
  const std::vector<std::string> right = words(vpr.string_value);
  if (left.size() != right.size()) {
    return false;
  }
  for (size_t i = 0; i < left.size(); ++i) {
    double a = 0;
    double b = 0;
    if (is_number(left[i], a) && is_number(right[i], b)) {
      if (static_cast<float>(a) != static_cast<float>(b)) {
        return false;
      }
    } else if (left[i] != right[i]) {
      return false;
    }
  }
  return true;
}

static std::string describe(const AppOptionValue& value) {
  switch (value.type) {
    case AppOptionValue::BOOLEAN:
      return value.bool_value ? "true" : "false";
    case AppOptionValue::INT:
      return std::to_string(value.int_value);
    case AppOptionValue::FLOAT:
      return std::to_string(value.float_value);
    case AppOptionValue::SELECTION:
      return value.string_value + " (" + std::to_string(value.to_enum()) + ")";
    default:
      return "'" + value.string_value + "'";
  }
}

/* VPR exits the program when it rejects a command line. Say which option it
 * was reading, so that it can be found */
static std::string reading;
static void report_exit() {
  if (!reading.empty()) {
    std::fprintf(stderr, "VPR exited while reading the option: %s\n",
                 reading.c_str());
  }
}

static std::string join_args(const std::vector<std::string>& args) {
  std::string line;
  for (const std::string& arg : args) {
    line += (line.empty() ? "" : " ") + arg;
  }
  return line;
}

static std::string read_file(const std::string& path) {
  std::ifstream file(path);
  std::stringstream content;
  content << file.rdbuf();
  return content.str();
}

/* The command line that gives the architecture and the circuit only */
static std::vector<std::string> minimal_args() {
  return {"vpr", "arch.xml", "circuit"};
}

/* The options of VPR, by name */
static std::map<std::string, const openfpga::VprOptionSpec*> specs_by_option() {
  std::map<std::string, const openfpga::VprOptionSpec*> specs;
  for (const openfpga::VprOptionSpec& spec : openfpga::vpr_option_specs()) {
    specs[std::string(spec.group) + "." + spec.name] = &spec;
  }
  return specs;
}

static void test_every_field_is_accounted_for() {
  const std::string header =
    read_file(std::string(OPENFPGA_SOURCE_DIR) +
              "/vtr-verilog-to-routing/vpr/src/base/read_options.h");
  CHECK(!header.empty());

  /* The fields of t_options, now */
  std::set<std::string> fields;
  const std::regex field_pattern("ArgValue<(.+?)>\\s+(\\w+);");
  for (auto it =
         std::sregex_iterator(header.begin(), header.end(), field_pattern);
       it != std::sregex_iterator(); ++it) {
    fields.insert((*it)[2]);
  }
  CHECK(fields.size() > 200);

  /* The fields that the mapping knows about */
  std::set<std::string> known;
  for (const openfpga::VprOptionSpec& spec : openfpga::vpr_option_specs()) {
    CHECK(known.insert(spec.field).second); /* an option for each field */
  }
  for (const openfpga::VprFieldNotInTable& field :
       openfpga::vpr_fields_not_in_table()) {
    CHECK(known.insert(field.field).second); /* mapped or skipped, not both */
    CHECK(std::string(field.reason).size() > 0);
  }

  for (const std::string& field : fields) {
    if (known.count(field) == 0) {
      std::fprintf(stderr,
                   "  The VPR option '%s' has no application option and no "
                   "reason: run "
                   "openfpga_flow/scripts/generate_vpr_option_mapping.py\n",
                   field.c_str());
      ++num_failures;
    }
  }
  for (const std::string& field : known) {
    if (fields.count(field) == 0) {
      std::fprintf(
        stderr,
        "  The mapping has '%s', which is not a VPR option any "
        "more: run openfpga_flow/scripts/generate_vpr_option_mapping.py\n",
        field.c_str());
      ++num_failures;
    }
  }
}

static void test_specs_match_the_table() {
  const AppOptions options;
  const auto specs = specs_by_option();
  const std::set<std::string> openfpga_only = {
    "build_fabric.reorder_incoming_edges",
    "build_fabric.sort_gsb_chan_node_in_edges"};

  std::set<std::string> flags;
  for (const auto& entry : specs) {
    /* The option exists, and the flag is used once */
    CHECK(options.has(entry.first));
    CHECK(flags.insert(entry.second->flag).second);
    /* The type of the option is the one of its description */
    CHECK(options.find(entry.first)->type == entry.second->type);
    /* A selection comes with the values it accepts */
    CHECK((entry.second->type == AppOptionValue::SELECTION) ==
          (entry.second->choices != nullptr));
  }
  /* Every option of the table is an option of VPR, but the ones of OpenFPGA */
  for (const std::string& name : options.names()) {
    CHECK((specs.count(name) == 1) != (openfpga_only.count(name) == 1));
  }
  CHECK(options.names().size() == specs.size() + openfpga_only.size());
  /* The positional arguments are the file of the architecture and the circuit
   */
  CHECK(std::string(specs.at("filename.arch_file")->flag) == "architecture");
  CHECK(std::string(specs.at("filename.circuit_name")->flag) == "circuit");
}

static void test_defaults_are_the_defaults_of_vpr() {
  const AppOptions options;
  reading = "(the defaults)";
  const t_options vpr = openfpga::read_vpr_options(minimal_args());
  reading.clear();

  size_t compared = 0;
  for (const openfpga::VprOptionSpec& spec : openfpga::vpr_option_specs()) {
    const std::string name = std::string(spec.group) + "." + spec.name;
    const AppOptionValue& table = options.default_of(name);
    if (spec.derived_default ||
        std::string(spec.flag).compare(0, 2, "--") != 0) {
      /* VPR names them after the circuit, or they are the positional
       * arguments: the table leaves them empty */
      CHECK(table.string_value.empty());
      continue;
    }
    AppOptionValue read = table; /* the same type */
    spec.get(vpr, read);
    ++compared;
    if (!same_value(table, read)) {
      std::fprintf(stderr, "  default of %s: the table has %s, VPR has %s\n",
                   name.c_str(), describe(table).c_str(),
                   describe(read).c_str());
      ++num_failures;
    }
  }
  CHECK(compared > 150);
}

/* A value for an option that is not its default, and that VPR accepts */
static bool other_value(const std::string& name, const AppOptionValue& option,
                        const openfpga::VprOptionSpec& spec,
                        std::string& text) {
  switch (option.type) {
    case AppOptionValue::BOOLEAN:
      /* A switch cannot be turned off */
      if (spec.store_true && option.bool_value) {
        return false;
      }
      text = option.bool_value ? "false" : "true";
      return true;
    case AppOptionValue::INT:
      text = std::to_string(option.int_value + 1);
      return true;
    case AppOptionValue::FLOAT:
      text =
        std::to_string(option.float_value > 0.5 ? option.float_value - 0.25
                                                : option.float_value + 0.125);
      return true;
    case AppOptionValue::SELECTION:
      for (const std::string& choice : option.choices()) {
        if (choice != option.string_value) {
          text = choice;
          return true;
        }
      }
      return false;
    case AppOptionValue::STRING:
      /* Free text is only safe for names of files: VPR reads the other ones */
      if (0 == name.compare(0, 9, "filename.")) {
        text = "other_file";
        return true;
      }
      return false;
    default:
      break;
  }
  return false;
}

/* The options that VPR wants with an option: it refuses the option alone */
static std::vector<std::pair<std::string, std::string>> companions(
  const std::string& name) {
  if ("filename.read_rr_graph_file" == name) {
    return {{"router.route_chan_width", "100"}};
  }
  return {};
}

/* Give an option to VPR and check that VPR has its value. Return 0 if it has,
 * 1 if not. VPR exits the program if it rejects the command line */
static int try_option(const std::string& name, const std::string& text,
                      const std::vector<std::string>& args,
                      const AppOptionValue& option,
                      const openfpga::VprOptionSpec& spec) {
  /* VPR throws when options need each other, and exits when it rejects one */
  t_options vpr;
  try {
    vpr = openfpga::read_vpr_options(args);
  } catch (const std::exception& error) {
    std::fprintf(stderr, "  %s = %s: VPR refuses the command line (%s): %s\n",
                 name.c_str(), text.c_str(), join_args(args).c_str(),
                 error.what());
    return 1;
  }
  AppOptionValue read = option;
  spec.get(vpr, read);
  if (!same_value(option, read)) {
    std::fprintf(stderr, "  %s = %s: VPR has %s (%s)\n", name.c_str(),
                 text.c_str(), describe(read).c_str(), join_args(args).c_str());
    return 1;
  }
  return 0;
}

static void test_every_option_reaches_vpr() {
  const auto specs = specs_by_option();
  size_t tested = 0;

  for (const auto& entry : specs) {
    const std::string& name = entry.first;
    const openfpga::VprOptionSpec& spec = *entry.second;
    /* The positional arguments are not flags */
    if (0 != std::string(spec.flag).compare(0, 2, "--")) {
      continue;
    }
    AppOptions options;
    std::string error;
    std::string text;
    if (!other_value(name, *options.find(name), spec, text)) {
      continue;
    }
    CHECK(options.set(name, text, error));
    CHECK(!options.is_default(name));
    /* VPR wants some options to come with others */
    for (const auto& companion : companions(name)) {
      CHECK(options.set(companion.first, companion.second, error));
    }
    CHECK(options.set("filename.arch_file", "arch.xml", error));
    CHECK(options.set("filename.circuit_name", "circuit", error));
    std::vector<std::string> args;
    CHECK(openfpga::vpr_args_from_app_options(options, args, error));
    ++tested;

#ifndef _WIN32
    /* VPR exits the program when it rejects a command line: ask it in a child
     * process, so that all the options can be tried */
    std::fflush(stdout);
    std::fflush(stderr);
    const pid_t child = fork();
    if (0 == child) {
      /* Say what is read, in case VPR exits */
      reading = name + " = " + text + "  (" + join_args(args) + ")";
      _exit(try_option(name, text, args, *options.find(name), spec));
    }
    int status = 0;
    waitpid(child, &status, 0);
    if (!WIFEXITED(status) || 0 != WEXITSTATUS(status)) {
      ++num_failures;
    }
#else
    reading = name + " = " + text + "  (" + join_args(args) + ")";
    num_failures += try_option(name, text, args, *options.find(name), spec);
    reading.clear();
#endif
  }
  /* Almost all the options can be tried */
  CHECK(tested > 150);
  std::printf("%zu options tried against VPR\n", tested);
}

static void test_command_line_needs_architecture_and_circuit() {
  AppOptions options;
  std::vector<std::string> args;
  std::string error;
  CHECK(!openfpga::vpr_args_from_app_options(options, args, error));
  CHECK(error.find("filename.arch_file") != std::string::npos);

  CHECK(options.set("filename.arch_file", "path/arch.xml", error));
  CHECK(!openfpga::vpr_args_from_app_options(options, args, error));
  CHECK(error.find("circuit") != std::string::npos);

  /* The circuit is a name, or a file whose name is used */
  CHECK(options.set("filename.circuit_file", "dir/and2.blif", error));
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  /* The file is given as well, as VPR takes the positional as a name then */
  CHECK((args == std::vector<std::string>{"vpr", "path/arch.xml", "and2",
                                          "--circuit_file", "dir/and2.blif"}));
  CHECK(options.set("filename.circuit_name", "named", error));
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  CHECK((args == std::vector<std::string>{"vpr", "path/arch.xml", "named",
                                          "--circuit_file", "dir/and2.blif"}));
}

static void test_command_line_has_only_what_was_changed() {
  AppOptions options;
  std::string error;
  CHECK(options.set("filename.arch_file", "arch.xml", error));
  CHECK(options.set("filename.circuit_name", "c", error));
  std::vector<std::string> args;
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  CHECK((args == std::vector<std::string>{"vpr", "arch.xml", "c"}));

  /* A number, a boolean, a selection */
  CHECK(options.set("place.seed", "7", error));
  CHECK(options.set("atom.sweep_dangling_nets", "false", error));
  CHECK(options.set("place.place_algorithm", "bounding_box", error));
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  const std::set<std::string> given(args.begin() + 3, args.end());
  CHECK(args.size() == 3 + 6);
  CHECK(given.count("--seed") == 1 && given.count("7") == 1);
  CHECK(given.count("--sweep_dangling_nets") == 1 && given.count("off") == 1);
  CHECK(given.count("--place_algorithm") == 1 &&
        given.count("bounding_box") == 1);

  /* Back to the default: it is not given any more */
  CHECK(options.set("place.seed", "1", error));
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  CHECK(args.size() == 3 + 4);
}

static void test_switches_and_lists() {
  AppOptions options;
  std::string error;
  CHECK(options.set("filename.arch_file", "arch.xml", error));
  CHECK(options.set("filename.circuit_name", "c", error));
  std::vector<std::string> args;

  /* A switch has no value, and can only be turned on */
  const auto specs = specs_by_option();
  std::string a_switch;
  for (const auto& entry : specs) {
    if (entry.second->store_true) {
      a_switch = entry.first;
      break;
    }
  }
  CHECK(!a_switch.empty());
  if (!a_switch.empty()) {
    const bool is_on = options.find(a_switch)->bool_value;
    CHECK(!is_on);
    CHECK(options.set(a_switch, "true", error));
    CHECK(openfpga::vpr_args_from_app_options(options, args, error));
    CHECK((args == std::vector<std::string>{"vpr", "arch.xml", "c",
                                            specs.at(a_switch)->flag}));
  }

  /* A list gives one argument for each word */
  CHECK(options.reset(a_switch));
  CHECK(
    options.set("cluster.target_external_pin_util", "clb:0.8 io:1.0", error));
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  CHECK(args.size() == 3 + 3);
  CHECK(args[3] == specs.at("cluster.target_external_pin_util")->flag);
  CHECK(args[4] == "clb:0.8" && args[5] == "io:1.0");

  /* A text with spaces stays one argument when VPR takes one value */
  CHECK(options.reset("cluster.target_external_pin_util"));
  CHECK(options.set("filename.write_vpr_constraints_file", "a b.xml", error));
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  CHECK(args.size() == 3 + 2);
  CHECK(args[4] == "a b.xml");
}

static void test_options_only_in_openfpga_are_not_given() {
  AppOptions options;
  std::string error;
  CHECK(options.set("filename.arch_file", "arch.xml", error));
  CHECK(options.set("filename.circuit_name", "c", error));
  CHECK(
    options.set("build_fabric.sort_gsb_chan_node_in_edges", "false", error));
  CHECK(options.set("build_fabric.reorder_incoming_edges", "false", error));
  std::vector<std::string> args;
  CHECK(openfpga::vpr_args_from_app_options(options, args, error));
  CHECK((args == std::vector<std::string>{"vpr", "arch.xml", "c"}));
}

/* Run the setup of VPR for a command line, and return what it shows of it */
static std::string vpr_setup_shown(const std::vector<std::string>& args) {
  std::vector<const char*> argv;
  for (const std::string& arg : args) {
    argv.push_back(arg.c_str());
  }
  t_options vpr_options =
    read_options(static_cast<int>(argv.size()), argv.data());
  t_arch arch;
  t_vpr_setup setup;
  vpr_init_with_options(&vpr_options, &setup, &arch);

  openfpga::OutputCapture capture;
  show_setup(setup);
  const std::string shown = capture.stop();

  vpr_free_all(arch, setup);
  return shown;
}

/* The setup that the options give is the one that VPR builds for the same
 * command line, on a real architecture and circuit */
static void test_setup_is_the_one_of_vpr() {
  const std::string flow = std::string(OPENFPGA_SOURCE_DIR) + "/openfpga_flow";
  const std::string arch = flow + "/vpr_arch/k6_frac_N10_tileable_40nm.xml";
  const std::string blif = flow + "/benchmarks/ubm/simple_gates/and2/and2.blif";
  if (read_file(arch).empty() || read_file(blif).empty()) {
    std::printf("skip the setup test: %s is missing\n", blif.c_str());
    return;
  }

  /* The options, as a user sets them */
  AppOptions options;
  std::string error;
  CHECK(options.set("filename.arch_file", arch, error));
  CHECK(options.set("filename.circuit_file", blif, error));
  CHECK(options.set("general.device_layout", "2x2", error));
  CHECK(options.set("router.route_chan_width", "20", error));
  CHECK(options.set("general.clock_modeling", "route", error));
  CHECK(options.set("place.seed", "3", error));
  CHECK(options.set("place.place_algorithm", "bounding_box", error));
  CHECK(options.set("cluster.timing_driven_clustering", "false", error));
  CHECK(options.set("router.max_router_iterations", "20", error));
  std::vector<std::string> from_options;
  CHECK(openfpga::vpr_args_from_app_options(options, from_options, error));

  /* The same, as a user would type them for VPR */
  const std::vector<std::string> typed = {"vpr",
                                          arch,
                                          "and2",
                                          "--circuit_file",
                                          blif,
                                          "--device",
                                          "2x2",
                                          "--route_chan_width",
                                          "20",
                                          "--clock_modeling",
                                          "route",
                                          "--seed",
                                          "3",
                                          "--place_algorithm",
                                          "bounding_box",
                                          "--timing_driven_clustering",
                                          "off",
                                          "--max_router_iterations",
                                          "20"};

  const std::string expected = vpr_setup_shown(typed);
  const std::string given = vpr_setup_shown(from_options);
  CHECK(expected.size() > 1000); /* the setup has been shown */
  CHECK(given == expected);

  /* The test is able to tell setups apart */
  std::vector<std::string> other = typed;
  other[8] = "22"; /* the width of the channels */
  CHECK(vpr_setup_shown(other) != expected);
}

int main() {
  std::atexit(report_exit);

  test_every_field_is_accounted_for();
  test_specs_match_the_table();
  test_command_line_needs_architecture_and_circuit();
  test_command_line_has_only_what_was_changed();
  test_switches_and_lists();
  test_options_only_in_openfpga_are_not_given();
  test_defaults_are_the_defaults_of_vpr();
  test_every_option_reaches_vpr();
  test_setup_is_the_one_of_vpr();

  reading.clear();
  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All VPR application option tests passed\n");
  return 0;
}
