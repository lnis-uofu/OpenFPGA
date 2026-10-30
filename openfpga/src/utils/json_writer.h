#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <cstdint>
#include <string>
#include <vector>

/********************************************************************
 * A writer of JSON text, for the answers of the queries.
 *
 * It only writes, as OpenFPGA never reads JSON: queries take their
 * parameters as command options.
 *
 * The text is compact, and keys stay in the order they are written in, so
 * that the same answer is always the same text, which golden files can be
 * compared with.
 *
 * - Strings are escaped. UTF-8 is written as it is, and a byte that is not
 *   valid UTF-8 is replaced by U+FFFD, so the text is always valid JSON
 * - A number that is not finite (NaN, infinity) is written as null, which is
 *   the only way JSON can say it
 * - A double is written as the shortest text that gives the same number back
 *
 * The writer checks the structure as it goes: it throws std::logic_error when
 * a value is written where a key is expected or the other way around, when a
 * container is closed with the wrong end, or when the text is asked for before
 * it is complete
 *
 * Example:
 *   JsonWriter json;
 *   json.begin_object();
 *   json.key("name").value("place.seed");
 *   json.key("value").value(1);
 *   json.end_object();
 *   json.str();  // {"name":"place.seed","value":1}
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

class JsonWriter {
 public: /* Mutators */
  JsonWriter& begin_object();
  JsonWriter& end_object();
  JsonWriter& begin_array();
  JsonWriter& end_array();
  /* The name of the next member, inside an object */
  JsonWriter& key(const std::string& name);
  JsonWriter& value(const std::string& text);
  JsonWriter& value(const char* text);
  JsonWriter& value(int num);
  JsonWriter& value(int64_t num);
  JsonWriter& value(double num);
  JsonWriter& value(bool flag);
  JsonWriter& null();

 public: /* Accessors */
  /* The text, once every container is closed and a value was written */
  std::string str() const;
  /* True once a complete value was written */
  bool complete() const;

 public: /* Static functions */
  /* A string as it is written in JSON, with its quotes */
  static std::string quote(const std::string& text);
  /* A double as it is written in JSON: null if it is not finite */
  static std::string number(double value);

 private: /* Internal functions */
  /* Check a value can be written here, and write what comes before it */
  void before_value();
  void after_value();

 private: /* Internal data */
  enum class Context { OBJECT, ARRAY };
  struct Level {
    Context context;
    bool empty;
    bool key_written;
  };
  std::string text_;
  std::vector<Level> levels_;
  bool done_ = false;
};

} /* End namespace openfpga */

#endif
