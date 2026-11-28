/********************************************************************
 * Unit test of the writer of JSON text and of the envelope of the answers of
 * the queries
 *******************************************************************/
#include <cmath>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>

#include "app_options.h"
#include "app_options_json.h"
#include "json_response.h"
#include "json_writer.h"

using openfpga::JsonWriter;

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

#define CHECK_EQ(actual, expected)                                       \
  do {                                                                   \
    const std::string a = (actual);                                      \
    const std::string e = (expected);                                    \
    if (a != e) {                                                        \
      std::fprintf(stderr, "%s:%d: got [%s], expected [%s]\n", __FILE__, \
                   __LINE__, a.c_str(), e.c_str());                      \
      ++num_failures;                                                    \
    }                                                                    \
  } while (false)

#define CHECK_THROWS(statement)         \
  do {                                  \
    bool threw = false;                 \
    try {                               \
      statement;                        \
    } catch (const std::logic_error&) { \
      threw = true;                     \
    }                                   \
    CHECK(threw);                       \
  } while (false)

static void test_scalars() {
  CHECK_EQ(JsonWriter().value("a").str(), "\"a\"");
  CHECK_EQ(JsonWriter().value(std::string("a b")).str(), "\"a b\"");
  CHECK_EQ(JsonWriter().value(42).str(), "42");
  CHECK_EQ(JsonWriter().value(-7).str(), "-7");
  CHECK_EQ(JsonWriter().value(int64_t(9007199254740993LL)).str(),
           "9007199254740993");
  CHECK_EQ(JsonWriter().value(true).str(), "true");
  CHECK_EQ(JsonWriter().value(false).str(), "false");
  CHECK_EQ(JsonWriter().null().str(), "null");
  const char* none = nullptr;
  CHECK_EQ(JsonWriter().value(none).str(), "null");
}

static void test_numbers() {
  CHECK_EQ(JsonWriter::number(0.5), "0.5");
  CHECK_EQ(JsonWriter::number(1.0), "1");
  CHECK_EQ(JsonWriter::number(-2.25), "-2.25");
  CHECK_EQ(JsonWriter::number(0.1), "0.1");
  CHECK_EQ(JsonWriter::number(1e-9), "1e-09");
  CHECK_EQ(JsonWriter::number(0.0), "0");
  /* The text gives the same number back */
  for (const double value : {0.1, 1.0 / 3.0, 8.85948e-10, 1e300, 5e-324,
                             123456789.125, std::numeric_limits<double>::max(),
                             std::numeric_limits<double>::min()}) {
    CHECK(std::strtod(JsonWriter::number(value).c_str(), nullptr) == value);
  }
  /* Not finite: null, as JSON cannot say it */
  CHECK_EQ(JsonWriter::number(std::nan("")), "null");
  CHECK_EQ(JsonWriter::number(std::numeric_limits<double>::infinity()), "null");
  CHECK_EQ(JsonWriter::number(-std::numeric_limits<double>::infinity()),
           "null");
  CHECK_EQ(JsonWriter().value(std::nan("")).str(), "null");
}

static void test_escaping() {
  CHECK_EQ(JsonWriter::quote(""), "\"\"");
  CHECK_EQ(JsonWriter::quote("a\"b"), "\"a\\\"b\"");
  CHECK_EQ(JsonWriter::quote("a\\b"), "\"a\\\\b\"");
  CHECK_EQ(JsonWriter::quote("a\nb\r\t\b\f"), "\"a\\nb\\r\\t\\b\\f\"");
  /* Control characters without a short form */
  CHECK_EQ(JsonWriter::quote(std::string("a\x01"
                                         "b",
                                         3)),
           "\"a\\u0001b\"");
  CHECK_EQ(JsonWriter::quote(std::string("\x1f", 1)), "\"\\u001f\"");
  CHECK_EQ(JsonWriter::quote(std::string("a\0b", 3)), "\"a\\u0000b\"");
  /* '/' and DEL need no escape */
  CHECK_EQ(JsonWriter::quote("a/b\x7f"), "\"a/b\x7f\"");
}

static void test_utf8() {
  /* Valid sequences of 2, 3 and 4 bytes are kept */
  CHECK_EQ(JsonWriter::quote("\xC3\xA9"), "\"\xC3\xA9\"");
  CHECK_EQ(JsonWriter::quote("\xE2\x82\xAC"), "\"\xE2\x82\xAC\"");
  CHECK_EQ(JsonWriter::quote("\xF0\x9F\x98\x80"), "\"\xF0\x9F\x98\x80\"");
  /* What is not valid UTF-8 becomes U+FFFD, one byte at a time */
  CHECK_EQ(JsonWriter::quote("\xFF"), "\"\\ufffd\"");
  CHECK_EQ(JsonWriter::quote("a\x80"
                             "b"),
           "\"a\\ufffdb\"");
  CHECK_EQ(JsonWriter::quote("\xC3"), "\"\\ufffd\"");            /* cut */
  CHECK_EQ(JsonWriter::quote("\xC0\x80"), "\"\\ufffd\\ufffd\""); /* overlong */
  CHECK_EQ(JsonWriter::quote("\xED\xA0\x80"),
           "\"\\ufffd\\ufffd\\ufffd\""); /* surrogate */
  CHECK_EQ(JsonWriter::quote("\xF4\x90\x80\x80"),
           "\"\\ufffd\\ufffd\\ufffd\\ufffd\""); /* > U+10FFFF */
}

static void test_structures() {
  {
    JsonWriter json;
    json.begin_object().end_object();
    CHECK_EQ(json.str(), "{}");
  }
  {
    JsonWriter json;
    json.begin_array().end_array();
    CHECK_EQ(json.str(), "[]");
  }
  {
    JsonWriter json;
    json.begin_object();
    json.key("name").value("place.seed");
    json.key("value").value(1);
    json.key("list").begin_array();
    json.value(1).value("two").null().value(false);
    json.begin_object().key("k").value(0.5).end_object();
    json.begin_array().end_array();
    json.end_array();
    json.key("nested").begin_object();
    json.key("empty").begin_object().end_object();
    json.end_object();
    json.end_object();
    CHECK_EQ(json.str(),
             "{\"name\":\"place.seed\",\"value\":1,\"list\":[1,\"two\",null,"
             "false,{\"k\":0.5},[]],\"nested\":{\"empty\":{}}}");
  }
  {
    /* The keys keep the order they were written in */
    JsonWriter json;
    json.begin_object();
    json.key("b").value(1);
    json.key("a").value(2);
    json.key("c").value(3);
    json.end_object();
    CHECK_EQ(json.str(), "{\"b\":1,\"a\":2,\"c\":3}");
  }
  {
    /* The keys are escaped as well */
    JsonWriter json;
    json.begin_object().key("a\"b").value(1).end_object();
    CHECK_EQ(json.str(), "{\"a\\\"b\":1}");
  }
}

static void test_structure_errors() {
  /* A value in an object needs a key */
  CHECK_THROWS(JsonWriter().begin_object().value(1));
  /* A key needs an object, and a value after it */
  CHECK_THROWS(JsonWriter().key("a"));
  CHECK_THROWS(JsonWriter().begin_array().key("a"));
  CHECK_THROWS(JsonWriter().begin_object().key("a").key("b"));
  CHECK_THROWS(JsonWriter().begin_object().key("a").end_object());
  /* The wrong end */
  CHECK_THROWS(JsonWriter().begin_object().end_array());
  CHECK_THROWS(JsonWriter().begin_array().end_object());
  CHECK_THROWS(JsonWriter().end_object());
  CHECK_THROWS(JsonWriter().end_array());
  /* One value only at the top */
  CHECK_THROWS(JsonWriter().value(1).value(2));
  CHECK_THROWS(JsonWriter().begin_object().end_object().begin_object());
  /* The text of an incomplete value */
  CHECK_THROWS(JsonWriter().str());
  CHECK_THROWS(JsonWriter().begin_object().str());
  CHECK_THROWS(JsonWriter().begin_array().value(1).str());
  CHECK(!JsonWriter().begin_array().complete());
  CHECK(JsonWriter().begin_array().end_array().complete());
}

static void test_response() {
  {
    JsonWriter json;
    openfpga::begin_response(json, "thing", 3);
    json.begin_object().key("n").value(1).end_object();
    openfpga::end_response(json);
    CHECK_EQ(json.str(),
             "{\"schema_version\":3,\"kind\":\"thing\",\"stage\":null,"
             "\"data\":{\"n\":1}}");
  }
  {
    JsonWriter json;
    openfpga::begin_response(json, "thing", 1, "ROUTED");
    json.begin_array().end_array();
    openfpga::end_response(json);
    CHECK_EQ(json.str(),
             "{\"schema_version\":1,\"kind\":\"thing\",\"stage\":\"ROUTED\","
             "\"data\":[]}");
  }
  {
    /* The data must be written */
    JsonWriter json;
    openfpga::begin_response(json, "thing", 1);
    CHECK_THROWS(openfpga::end_response(json));
  }
}

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
  CHECK(seed.find("\"choices\":[],\"stage\":\"PLACED\"}") != std::string::npos);

  /* A changed value is not the default any more */
  std::string error;
  CHECK(options.set("place.seed", "5", error));
  const std::string changed =
    openfpga::app_options_json(options, {"place.seed"});
  CHECK(changed.find("\"value\":5,\"default\":1,\"is_default\":false") !=
        std::string::npos);
}

int main() {
  test_scalars();
  test_numbers();
  test_escaping();
  test_utf8();
  test_structures();
  test_structure_errors();
  test_response();
  test_app_options_json();

  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All JSON writer tests passed\n");
  return 0;
}
