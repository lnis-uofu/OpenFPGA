/********************************************************************
 * Unit test of the answer of report_app_option (the ui plugin): the
 * options as JSON
 *******************************************************************/
#include <cstdio>
#include <string>

#include "app_options.h"
#include "app_options_json.h"

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

static void test_app_options_json() {
  openfpga::AppOptions options;
  const std::string all = openfpga::app_options_json(options, options.names());
  CHECK(all.find("{\"schema_version\":1,\"kind\":\"app_options\","
                 "\"stage\":null,\"data\":{\"options\":[") == 0);
  CHECK(all.size() > 1000);

  /* One option: typed value and default, the stage and no choice */
  const std::string seed = openfpga::app_options_json(options, {"place.seed"});
  CHECK(seed.find("\"name\":\"place.seed\",\"type\":\"int\",\"value\":1,"
                  "\"default\":1,\"is_default\":true") != std::string::npos);
  CHECK(
    seed.find("\"choices\":[],\"stage\":\"PLACED\",\"vpr_flag\":\"--seed\"}") !=
    std::string::npos);

  /* A changed value is not the default any more */
  std::string error;
  CHECK(options.set("place.seed", "5", error));
  const std::string changed =
    openfpga::app_options_json(options, {"place.seed"});
  CHECK(changed.find("\"value\":5,\"default\":1,\"is_default\":false") !=
        std::string::npos);
}

int main() {
  test_app_options_json();
  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All app options JSON tests passed\n");
  return 0;
}
