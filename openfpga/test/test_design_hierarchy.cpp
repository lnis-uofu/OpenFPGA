/********************************************************************
 * Unit test of the hierarchy of a design from the JSON of Yosys
 * (design_hierarchy.h): the names of the derived modules, the kinds, the
 * counts of the instances and the limit of the tree
 *******************************************************************/
#include <cstdio>
#include <stdexcept>
#include <string>

#include "design_hierarchy.h"

using json = nlohmann::json;
using openfpga::hierarchy_of;
using openfpga::module_name;

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

static void test_module_names() {
  CHECK(module_name("top")["module"] == "top");
  CHECK(module_name("\\top")["module"] == "top");
  CHECK(module_name("top")["parameters"].empty());
  const auto sub =
    module_name("$paramod\\sub\\W=s32'00000000000000000000000000001000");
  CHECK(sub["module"] == "sub");
  CHECK(sub["parameters"]["W"] == 8);
  const auto two = module_name("$paramod\\sub\\A=s4'1111\\B=2'10\\NAME=\"x\"");
  CHECK(two["parameters"]["A"] == -1);
  CHECK(two["parameters"]["B"] == 2);
  CHECK(two["parameters"]["NAME"] == "\"x\"");
  /* The names that are too long are a hash: no parameters */
  const auto hashed = module_name("$paramod$a1b2c3\\sub");
  CHECK(hashed["module"] == "sub");
  CHECK(hashed["parameters"].empty());
}

static json yosys() {
  return json::parse(R"({
    "modules": {
      "top": {
        "attributes": {"top": "00000000000000000000000000000001",
                       "src": "top.v:9.1-14.10"},
        "cells": {
          "u_b": {"type": "sub", "attributes": {"src": "top.v:11.7-11.40"}},
          "u_a": {"type": "sub", "attributes": {"src": "top.v:10.7-10.40"}},
          "u_m": {"type": "mult_8", "attributes": {}},
          "u_x": {"type": "vendor_ip", "attributes": {}},
          "$and$top.v:12$1": {"type": "$and", "attributes": {}}
        }
      },
      "sub": {
        "attributes": {"src": "top.v:4.1-8.10"},
        "cells": {
          "u_0": {"type": "leaf", "attributes": {}},
          "u_1": {"type": "leaf", "attributes": {}},
          "u_2": {"type": "leaf", "attributes": {}}
        }
      },
      "leaf": {
        "attributes": {"src": "top.v:1.1-3.10"},
        "cells": {"$xor$top.v:2$2": {"type": "$xor"},
                  "$not$top.v:2$3": {"type": "$not"}}
      },
      "mult_8": {"attributes": {"blackbox": "00000000000000000000000000000001"},
                 "cells": {}}
    }
  })");
}

static void test_tree() {
  const auto answer = hierarchy_of(yosys(), "top", 500);
  CHECK(answer["top"] == "top");
  CHECK(answer["nodes"] == 11);
  CHECK(answer["instances"] == 11);
  CHECK(answer["truncated"] == false);
  const auto& tree = answer["tree"];
  CHECK(tree["instance"].is_null());
  CHECK(tree["children"].size() == 4);
  /* By the name of the instance; the cells of Yosys are not instances */
  CHECK(tree["children"][0]["instance"] == "u_a");
  CHECK(tree["children"][1]["instance"] == "u_b");
  CHECK(tree["children"][0]["src"]["line"] == 10);
  CHECK(tree["children"][0]["src"]["file"] == "top.v");
  CHECK(tree["children"][0]["children"].size() == 3);
  CHECK(tree["children"][2]["kind"] == "library");
  CHECK(tree["children"][3]["kind"] == "unknown");
  CHECK(tree["children"][3]["module"] == "vendor_ip");
  const auto& modules = answer["modules"];
  CHECK(modules[0]["module"] == "leaf");
  CHECK(modules[0]["instances"] == 6);
  CHECK(modules[0]["cells"] == 2);
  CHECK(modules[0]["src"]["line"] == 1);
  CHECK(modules[1]["module"] == "sub");
  CHECK(modules[1]["instances"] == 2);
  bool unknown = false;
  for (const auto& module : modules) {
    if ("vendor_ip" == module["module"]) {
      unknown = "unknown" == module["kind"] && module["src"].is_null();
    }
  }
  CHECK(unknown);
}

static void test_limit() {
  /* Breadth first: the top and its four instances, then one leaf */
  const auto answer = hierarchy_of(yosys(), "top", 6);
  CHECK(answer["nodes"] == 6);
  CHECK(answer["instances"] == 11);
  CHECK(answer["truncated"] == true);
  const auto& first = answer["tree"]["children"][0];
  CHECK(first["children"].size() == 1);
  CHECK(first["more"] == 2);
  CHECK(answer["tree"]["children"][1]["more"] == 3);
  CHECK(!answer["tree"].contains("more"));
}

static void test_no_top() {
  bool thrown = false;
  try {
    hierarchy_of(yosys(), "nope", 10);
  } catch (const std::runtime_error&) {
    thrown = true;
  }
  CHECK(thrown);
}

int main() {
  test_module_names();
  test_tree();
  test_limit();
  test_no_top();

  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All design hierarchy tests passed\n");
  return 0;
}
