#ifndef VIEW_FILE_H
#define VIEW_FILE_H

/********************************************************************
 * The binary file of the data of a view (docs/plan_design_views.md, 3.1):
 * tables of typed columns that the page maps as typed arrays, with no parse.
 *
 * Layout, little-endian:
 * - 8 bytes: the magic "OFVIEW1" and a 0
 * - 4 bytes: the length H of the header
 * - H bytes: the header, JSON (UTF-8)
 * - the data, from the first multiple of 8 after the header: each column
 *   starts on a multiple of 8, at its offset from the start of the data
 *
 * The header:
 *   {"format": "openfpga_view", "format_version": 1, "kind": ...,
 *    "version": ..., "meta": {...},
 *    "tables": [{"name": ..., "rows": n,
 *                "columns": [{"name", "dtype", "offset", "count"}, ...]}]}
 * dtype is u8, i8, u16, i16, u32, i32, f32 or f64; count is the number of
 * values (the rows). A column of text has dtype "str": its offsets are
 * rows + 1 u32 (count) into a blob of UTF-8 at blob_offset, blob_bytes long
 *******************************************************************/
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

/* begin namespace openfpga */
namespace openfpga {

constexpr int VIEW_FORMAT_VERSION = 1;

class ViewTable {
 public:
  ViewTable(const std::string& name, size_t rows) : name_(name), rows_(rows) {}

  /* A column of numbers, one a row */
  template <typename T>
  ViewTable& column(const std::string& name, const std::vector<T>& values) {
    static_assert(std::is_arithmetic<T>::value, "a column holds numbers");
    check_rows(name, values.size());
    Column column;
    column.name = name;
    column.dtype = dtype_of<T>();
    column.count = values.size();
    const auto* bytes = reinterpret_cast<const uint8_t*>(values.data());
    column.data.assign(bytes, bytes + values.size() * sizeof(T));
    columns_.push_back(std::move(column));
    return *this;
  }

  /* A column of text, one string a row */
  ViewTable& strings(const std::string& name,
                     const std::vector<std::string>& values);

  const std::string& name() const { return name_; }
  size_t rows() const { return rows_; }

 private:
  friend class ViewFile;
  struct Column {
    std::string name;
    std::string dtype;
    size_t count = 0;
    std::vector<uint8_t> data; /* the values, or the offsets of text */
    std::vector<uint8_t> blob; /* the text */
  };

  template <typename T>
  static const char* dtype_of() {
    if (std::is_same<T, uint8_t>::value) return "u8";
    if (std::is_same<T, int8_t>::value) return "i8";
    if (std::is_same<T, uint16_t>::value) return "u16";
    if (std::is_same<T, int16_t>::value) return "i16";
    if (std::is_same<T, uint32_t>::value) return "u32";
    if (std::is_same<T, int32_t>::value) return "i32";
    if (std::is_same<T, float>::value) return "f32";
    if (std::is_same<T, double>::value) return "f64";
    static_assert(sizeof(T) <= 8, "a type of a column");
    return "?";
  }

  void check_rows(const std::string& column, size_t count) const;

  std::string name_;
  size_t rows_;
  std::vector<Column> columns_;
};

class ViewFile {
 public:
  ViewFile(const std::string& kind, const std::string& version)
    : kind_(kind), version_(version) {}

  /* A new table; the reference stays valid until the next one */
  ViewTable& table(const std::string& name, size_t rows);

  /* A table made for another file */
  void add_table(const ViewTable& table) { tables_.push_back(table); }

  /* The metadata of the header, as the text of a JSON object ({} if none) */
  void set_meta(const std::string& json_object) { meta_ = json_object; }

  /* The bytes of the file */
  std::vector<uint8_t> bytes() const;

  /* Write it. False, and the reason in error, if it cannot */
  bool write(const std::string& path, std::string& error) const;

  /* The rows of each table, for the answer of the command */
  const std::vector<ViewTable>& tables() const { return tables_; }

 private:
  std::string kind_;
  std::string version_;
  std::string meta_ = "{}";
  std::vector<ViewTable> tables_;
};

} /* end namespace openfpga */

#endif
