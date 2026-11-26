/********************************************************************
 * The binary file of the data of a view (see view_file.h)
 *******************************************************************/
#include "view_file.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include "json_writer.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

size_t aligned(size_t size) { return (size + 7) / 8 * 8; }

void put_u32(std::vector<uint8_t>& out, uint32_t value) {
  for (int i = 0; i < 4; ++i) {
    out.push_back(static_cast<uint8_t>(value >> (8 * i)));
  }
}

}  // namespace

void ViewTable::check_rows(const std::string& column, size_t count) const {
  if (count != rows_) {
    throw std::logic_error("The column '" + column + "' of the table '" +
                           name_ + "' has " + std::to_string(count) +
                           " values, not " + std::to_string(rows_));
  }
}

ViewTable& ViewTable::strings(const std::string& name,
                              const std::vector<std::string>& values) {
  check_rows(name, values.size());
  Column column;
  column.name = name;
  column.dtype = "str";
  column.count = values.size() + 1;
  std::vector<uint32_t> offsets;
  offsets.reserve(values.size() + 1);
  offsets.push_back(0);
  for (const std::string& value : values) {
    column.blob.insert(column.blob.end(), value.begin(), value.end());
    offsets.push_back(static_cast<uint32_t>(column.blob.size()));
  }
  const auto* bytes = reinterpret_cast<const uint8_t*>(offsets.data());
  column.data.assign(bytes, bytes + offsets.size() * sizeof(uint32_t));
  columns_.push_back(std::move(column));
  return *this;
}

ViewTable& ViewFile::table(const std::string& name, size_t rows) {
  tables_.emplace_back(name, rows);
  return tables_.back();
}

std::vector<uint8_t> ViewFile::bytes() const {
  /* The place of each column in the data */
  size_t offset = 0;
  JsonWriter json;
  json.begin_object();
  json.key("format").value("openfpga_view");
  json.key("format_version").value(VIEW_FORMAT_VERSION);
  json.key("kind").value(kind_);
  json.key("version").value(version_);
  json.key("tables").begin_array();
  for (const ViewTable& table : tables_) {
    json.begin_object();
    json.key("name").value(table.name());
    json.key("rows").value(static_cast<int64_t>(table.rows()));
    json.key("columns").begin_array();
    for (const ViewTable::Column& column : table.columns_) {
      json.begin_object();
      json.key("name").value(column.name);
      json.key("dtype").value(column.dtype);
      json.key("offset").value(static_cast<int64_t>(offset));
      json.key("count").value(static_cast<int64_t>(column.count));
      offset += aligned(column.data.size());
      if ("str" == column.dtype) {
        json.key("blob_offset").value(static_cast<int64_t>(offset));
        json.key("blob_bytes").value(static_cast<int64_t>(column.blob.size()));
        offset += aligned(column.blob.size());
      }
      json.end_object();
    }
    json.end_array();
    json.end_object();
  }
  json.end_array();
  json.end_object();
  /* The metadata goes in as it is: the writer has no raw value */
  std::string header = json.str();
  header.insert(header.size() - 1, ",\"meta\":" + meta_);

  std::vector<uint8_t> out;
  out.reserve(16 + header.size() + offset);
  const char magic[8] = {'O', 'F', 'V', 'I', 'E', 'W', '1', '\0'};
  out.insert(out.end(), magic, magic + 8);
  put_u32(out, static_cast<uint32_t>(header.size()));
  out.insert(out.end(), header.begin(), header.end());
  out.resize(aligned(out.size()), 0);
  for (const ViewTable& table : tables_) {
    for (const ViewTable::Column& column : table.columns_) {
      out.insert(out.end(), column.data.begin(), column.data.end());
      out.resize(aligned(out.size()), 0);
      if ("str" == column.dtype) {
        out.insert(out.end(), column.blob.begin(), column.blob.end());
        out.resize(aligned(out.size()), 0);
      }
    }
  }
  return out;
}

bool ViewFile::write(const std::string& path, std::string& error) const {
  const std::vector<uint8_t> data = bytes();
  /* Written beside, then renamed: a reader never sees half a file */
  const std::string partial = path + ".partial";
  {
    std::ofstream out(partial, std::ios::binary | std::ios::trunc);
    if (!out) {
      error = "Cannot write the file '" + partial + "'";
      return false;
    }
    out.write(reinterpret_cast<const char*>(data.data()),
              static_cast<std::streamsize>(data.size()));
    if (!out) {
      error = "Cannot write the file '" + partial + "'";
      return false;
    }
  }
  if (0 != std::rename(partial.c_str(), path.c_str())) {
    error = "Cannot rename '" + partial + "' to '" + path + "'";
    return false;
  }
  return true;
}

} /* end namespace openfpga */
