/********************************************************************
 * Unit test of the file of the data of a view (the ui plugin, view_file.h):
 * every type of column, text, an empty table, the alignment and the header
 *******************************************************************/
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "view_file.h"

using json = nlohmann::json;
using openfpga::ViewFile;

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

template <typename T>
static T at(const std::vector<uint8_t>& bytes, size_t offset) {
  T value;
  std::memcpy(&value, bytes.data() + offset, sizeof(T));
  return value;
}

static void test_file() {
  ViewFile file("test", "v1-2");
  file.table("numbers", 3)
    .column("u8", std::vector<uint8_t>{1, 2, 255})
    .column("i8", std::vector<int8_t>{-1, 0, 127})
    .column("u16", std::vector<uint16_t>{1, 2, 65535})
    .column("i16", std::vector<int16_t>{-300, 0, 300})
    .column("u32", std::vector<uint32_t>{7, 8, 4000000000u})
    .column("i32", std::vector<int32_t>{-5, 0, 5})
    .column("f32", std::vector<float>{0.5f, -1.f, INFINITY})
    .column("f64", std::vector<double>{1e-12, 2., -3.})
    .strings("name", {"a", "", "ünï"});
  file.table("empty", 0).column("x", std::vector<uint16_t>{});
  file.set_meta("{\"grid\":{\"width\":4}}");

  const std::vector<uint8_t> bytes = file.bytes();
  CHECK(0 == std::memcmp(bytes.data(), "OFVIEW1\0", 8));
  const uint32_t length = at<uint32_t>(bytes, 8);
  const json header = json::parse(
    std::string(reinterpret_cast<const char*>(bytes.data()) + 12, length));
  const size_t start = (12 + length + 7) / 8 * 8;
  CHECK(header["format"] == "openfpga_view");
  CHECK(header["format_version"] == 1);
  CHECK(header["kind"] == "test");
  CHECK(header["version"] == "v1-2");
  CHECK(header["meta"]["grid"]["width"] == 4);
  CHECK(header["tables"].size() == 2);
  const json& columns = header["tables"][0]["columns"];
  CHECK(header["tables"][0]["rows"] == 3);
  CHECK(columns.size() == 9);
  for (const json& column : columns) {
    CHECK(0 == column["offset"].get<size_t>() % 8);
  }
  auto offset = [&](size_t i) {
    return start + columns[i]["offset"].get<size_t>();
  };
  CHECK(columns[0]["dtype"] == "u8");
  CHECK(255 == at<uint8_t>(bytes, offset(0) + 2));
  CHECK(-1 == at<int8_t>(bytes, offset(1)));
  CHECK(65535 == at<uint16_t>(bytes, offset(2) + 4));
  CHECK(-300 == at<int16_t>(bytes, offset(3)));
  CHECK(4000000000u == at<uint32_t>(bytes, offset(4) + 8));
  CHECK(-5 == at<int32_t>(bytes, offset(5)));
  CHECK(std::isinf(at<float>(bytes, offset(6) + 8)));
  CHECK(-3. == at<double>(bytes, offset(7) + 16));
  /* Text: rows + 1 offsets into the blob */
  const json& name = columns[8];
  CHECK(name["dtype"] == "str");
  CHECK(name["count"] == 4);
  CHECK(0 == name["blob_offset"].get<size_t>() % 8);
  const size_t blob = start + name["blob_offset"].get<size_t>();
  const uint32_t a = at<uint32_t>(bytes, offset(8) + 8);
  const uint32_t b = at<uint32_t>(bytes, offset(8) + 12);
  CHECK(1 == at<uint32_t>(bytes, offset(8) + 4));
  CHECK(1 == a);
  CHECK(std::string(reinterpret_cast<const char*>(bytes.data()) + blob + a,
                    b - a) == "ünï");
  CHECK(name["blob_bytes"] == b);
  /* The empty table has its column */
  CHECK(header["tables"][1]["rows"] == 0);
  CHECK(header["tables"][1]["columns"][0]["count"] == 0);
  CHECK(0 == bytes.size() % 8);
}

static void test_rows_are_checked() {
  ViewFile file("test", "v");
  bool thrown = false;
  try {
    file.table("t", 2).column("x", std::vector<int32_t>{1});
  } catch (const std::logic_error&) {
    thrown = true;
  }
  CHECK(thrown);
}

int main() {
  test_file();
  test_rows_are_checked();
  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All view file tests passed\n");
  return 0;
}
