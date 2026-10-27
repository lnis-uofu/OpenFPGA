/********************************************************************
 * Unit test of the application options: the table of options, the
 * parsing of the values by type, and the errors on invalid input
 *******************************************************************/
#include <cstdio>
#include <set>
#include <string>
#include <vector>

#include "app_options.h"
#include "openfpga_context.h"

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

/* Return true if a text contains another one */
static bool contains(const std::string& text, const std::string& part) {
  return std::string::npos != text.find(part);
}

static void test_table() {
  const auto& defaults = openfpga::app_option_defaults();
  CHECK(defaults.size() > 200);

  const std::set<std::string> groups = {
    "filename", "general", "atom",        "aplace",  "cluster",
    "place",    "tplace",  "router",      "trouter", "analysis",
    "power",    "crr",     "build_fabric"};
  std::set<std::string> seen_groups;
  for (const auto& option : defaults) {
    /* Names are group.option, with a group that exists */
    const size_t dot = option.first.find('.');
    CHECK(std::string::npos != dot && dot > 0 && dot + 1 < option.first.size());
    seen_groups.insert(option.first.substr(0, dot));
    CHECK(groups.count(option.first.substr(0, dot)) == 1);
    /* Every option is documented */
    CHECK(!option.second.help_message.empty());
    if (option.second.help_message.empty()) {
      std::fprintf(stderr, "  no help for %s\n", option.first.c_str());
    }
    /* The default of a selection is one of its values */
    if (AppOptionValue::SELECTION == option.second.type) {
      CHECK(option.second.selection_values.count(option.second.string_value) ==
            1);
      if (option.second.selection_values.count(option.second.string_value) !=
          1) {
        std::fprintf(stderr, "  bad default for %s: %s\n", option.first.c_str(),
                     option.second.string_value.c_str());
      }
      CHECK(!option.second.selection_values.empty());
    }
  }
  CHECK(seen_groups == groups);
}

static void test_defaults_round_trip() {
  /* The text of every default gives the same value back */
  AppOptions options;
  for (const std::string& name : options.names()) {
    const AppOptionValue& value = *options.find(name);
    std::string error;
    AppOptions other;
    CHECK(other.set(name, value.to_string(), error));
    CHECK(other.find(name)->to_string() == value.to_string());
    if (!error.empty()) {
      std::fprintf(stderr, "  %s: %s\n", name.c_str(), error.c_str());
    }
    CHECK(other.is_default(name));
  }
}

static void test_names() {
  AppOptions options;
  const std::vector<std::string> names = options.names();
  CHECK(!names.empty());
  for (size_t i = 1; i < names.size(); ++i) {
    CHECK(names[i - 1] < names[i]); /* sorted, and no duplicates */
  }
  CHECK(options.has("place.seed"));
  CHECK(!options.has("place"));
  CHECK(!options.has("seed"));
  CHECK(options.find("nope") == nullptr);

  const std::vector<std::string> place = options.names("place");
  CHECK(place.size() > 10);
  for (const std::string& name : place) {
    CHECK(name.compare(0, 6, "place.") == 0);
  }
  /* 'place' does not take the options of 'tplace' */
  for (const std::string& name : place) {
    CHECK(name.compare(0, 7, "tplace.") != 0);
  }
  CHECK(options.names("nope").empty());
}

static void test_set_int() {
  AppOptions options;
  std::string error;
  CHECK(options.find("place.seed")->type == AppOptionValue::INT);
  CHECK(options.set("place.seed", "42", error));
  CHECK(options.find("place.seed")->int_value == 42);
  CHECK(options.find("place.seed")->to_string() == "42");
  CHECK(options.set("place.seed", "-7", error));
  CHECK(options.find("place.seed")->int_value == -7);
  CHECK(options.set("place.seed", "0", error));
  CHECK(options.find("place.seed")->int_value == 0);

  /* Invalid values change nothing */
  CHECK(options.set("place.seed", "5", error));
  const char* bad[] = {"abc", "1.5", "",     " 3",
                       "3 ",  "1e3", "0x10", "99999999999"};
  for (const char* text : bad) {
    error.clear();
    CHECK(!options.set("place.seed", text, error));
    CHECK(!error.empty());
    CHECK(options.find("place.seed")->int_value == 5);
  }
}

static void test_set_float() {
  AppOptions options;
  std::string error;
  CHECK(options.find("place.place_inner_num")->type == AppOptionValue::FLOAT);
  CHECK(options.set("place.place_inner_num", "0.7", error));
  CHECK(options.find("place.place_inner_num")->float_value == 0.7);
  CHECK(options.find("place.place_inner_num")->to_string() == "0.7");
  CHECK(options.set("place.place_inner_num", "1e-3", error));
  CHECK(options.find("place.place_inner_num")->float_value == 1e-3);
  CHECK(options.set("place.place_inner_num", "3", error));
  CHECK(options.find("place.place_inner_num")->to_string() == "3");
  CHECK(options.set("place.place_inner_num", "-0.25", error));
  CHECK(options.find("place.place_inner_num")->float_value == -0.25);
  /* The text keeps all the digits given */
  CHECK(options.set("place.place_inner_num", "0.1234567891", error));
  CHECK(options.find("place.place_inner_num")->to_string() == "0.1234567891");

  CHECK(options.set("place.place_inner_num", "0.5", error));
  const char* bad[] = {"abc",  "",      "nan",  "inf",
                       "-inf", "1.2.3", " 1.0", "1.0x"};
  for (const char* text : bad) {
    error.clear();
    CHECK(!options.set("place.place_inner_num", text, error));
    CHECK(!error.empty());
    CHECK(options.find("place.place_inner_num")->float_value == 0.5);
  }
}

static void test_set_bool() {
  AppOptions options;
  std::string error;
  CHECK(options.find("router.flat_routing")->type == AppOptionValue::BOOLEAN);
  CHECK(!options.find("router.flat_routing")->bool_value);

  const char* on[] = {"true", "TRUE", "True", "on", "yes", "1"};
  for (const char* text : on) {
    CHECK(options.reset("router.flat_routing"));
    CHECK(options.set("router.flat_routing", text, error));
    CHECK(options.find("router.flat_routing")->bool_value);
    CHECK(options.find("router.flat_routing")->to_string() == "true");
  }
  const char* off[] = {"false", "FALSE", "off", "no", "0"};
  for (const char* text : off) {
    CHECK(options.set("router.flat_routing", "true", error));
    CHECK(options.set("router.flat_routing", text, error));
    CHECK(!options.find("router.flat_routing")->bool_value);
    CHECK(options.find("router.flat_routing")->to_string() == "false");
  }

  CHECK(options.set("router.flat_routing", "true", error));
  const char* bad[] = {"maybe", "", "2", "t", "yes please"};
  for (const char* text : bad) {
    error.clear();
    CHECK(!options.set("router.flat_routing", text, error));
    CHECK(contains(error, "boolean"));
    CHECK(options.find("router.flat_routing")->bool_value);
  }
}

static void test_set_selection() {
  AppOptions options;
  std::string error;
  const AppOptionValue* algorithm = options.find("place.place_algorithm");
  CHECK(algorithm->type == AppOptionValue::SELECTION);
  CHECK(algorithm->string_value == "criticality_timing");
  const int before = algorithm->to_enum();

  CHECK(options.set("place.place_algorithm", "bounding_box", error));
  CHECK(algorithm->string_value == "bounding_box");
  /* The enumeration follows the selection */
  CHECK(algorithm->to_enum() == algorithm->selection_values.at("bounding_box"));
  CHECK(algorithm->to_enum() != before);
  CHECK(algorithm->int_value == algorithm->to_enum());

  /* A value outside of the selection is refused, and the error lists them */
  error.clear();
  CHECK(!options.set("place.place_algorithm", "not_an_algorithm", error));
  CHECK(contains(error, "not_an_algorithm"));
  CHECK(contains(error, "bounding_box"));
  CHECK(contains(error, "criticality_timing"));
  CHECK(contains(error, "place.place_algorithm"));
  CHECK(algorithm->string_value == "bounding_box");
  /* Selections are case sensitive, as the values of VPR */
  CHECK(!options.set("place.place_algorithm", "BOUNDING_BOX", error));

  CHECK(algorithm->choices().size() == algorithm->selection_values.size());
}

static void test_set_string() {
  AppOptions options;
  std::string error;
  CHECK(options.find("filename.circuit_file")->type == AppOptionValue::STRING);
  CHECK(options.set("filename.circuit_file", "path/to/and2.blif", error));
  CHECK(options.find("filename.circuit_file")->string_value ==
        "path/to/and2.blif");
  CHECK(options.set("filename.circuit_file", "", error));
  CHECK(options.find("filename.circuit_file")->string_value.empty());
}

static void test_unknown_option() {
  AppOptions options;
  std::string error;
  CHECK(!options.set("place.nope", "1", error));
  CHECK(contains(error, "place.nope"));
  CHECK(contains(error, "does not exist"));
  CHECK(!options.set("", "1", error));
  CHECK(!options.reset("place.nope"));
}

static void test_error_message() {
  AppOptions options;
  std::string error;
  CHECK(!options.set("place.seed", "abc", error));
  /* The message names the option, its type and the value refused */
  CHECK(contains(error, "place.seed"));
  CHECK(contains(error, "int"));
  CHECK(contains(error, "abc"));
}

static void test_reset() {
  AppOptions options;
  std::string error;
  CHECK(options.is_default("place.seed"));
  CHECK(options.set("place.seed", "9", error));
  CHECK(!options.is_default("place.seed"));
  CHECK(options.is_default("router.flat_routing"));
  CHECK(options.set("router.flat_routing", "true", error));

  CHECK(options.reset("place.seed"));
  CHECK(options.is_default("place.seed"));
  CHECK(options.find("place.seed")->int_value ==
        options.default_of("place.seed").int_value);
  /* Resetting one option leaves the others */
  CHECK(!options.is_default("router.flat_routing"));

  options.reset_all();
  CHECK(options.is_default("router.flat_routing"));
  /* Setting the default value is still the default */
  CHECK(options.set("place.seed", options.default_of("place.seed").to_string(),
                    error));
  CHECK(options.is_default("place.seed"));
  CHECK(!options.is_default("place.nope"));
}

static void test_copies_are_independent() {
  AppOptions first;
  std::string error;
  AppOptions second = first;
  CHECK(first.set("place.seed", "11", error));
  CHECK(second.find("place.seed")->int_value != 11);
  /* The defaults never change */
  CHECK(first.default_of("place.seed").int_value != 11);
  second = first;
  CHECK(second.find("place.seed")->int_value == 11);
}

static void test_session_owns_options() {
  OpenfpgaContext context;
  std::string error;
  CHECK(context.mutable_session().set_app_option("place.seed", "3", error));
  const OpenfpgaContext& const_context = context;
  CHECK(const_context.session().app_options().find("place.seed")->int_value ==
        3);
  /* The options are kept when the session is reset */
  context.mutable_session().reset();
  CHECK(const_context.session().app_options().find("place.seed")->int_value ==
        3);
}

int main() {
  test_table();
  test_defaults_round_trip();
  test_names();
  test_set_int();
  test_set_float();
  test_set_bool();
  test_set_selection();
  test_set_string();
  test_unknown_option();
  test_error_message();
  test_reset();
  test_copies_are_independent();
  test_session_owns_options();

  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All AppOptions tests passed\n");
  return 0;
}
