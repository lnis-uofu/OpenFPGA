#include "openfpga_gz_xml_reader.h"

#include <zlib.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace openfpga {

// Helper to decompress a GZIP file into an in-memory buffer using zlib
static bool load_gz_to_buffer(const std::filesystem::path& path,
                              std::vector<char>& buffer) {
  gzFile file = gzopen(path.string().c_str(), "rb");
  if (!file) return false;

  char tmp[4096];  // Efficient 4KB chunk buffer

  while (true) {
    int bytes_read = gzread(file, tmp, sizeof(tmp));
    if (bytes_read > 0) {
      buffer.insert(buffer.end(), tmp, tmp + bytes_read);
    } else if (bytes_read == 0) {
      break;  // Finished decompression successfully
    } else {
      gzclose(file);
      return false;  // Decompression error
    }
  }

  gzclose(file);
  return true;
}

// Modernized: Automatically detects regular XML vs GZ via magic bytes
pugiutil::loc_data load_xml(pugi::xml_document& doc,
                            std::string_view filename) {
  std::filesystem::path file_path(filename);

  if (!std::filesystem::exists(file_path)) {
    throw pugiutil::XmlError(
      "XML file does not exist: " + std::string(filename),
      std::string(filename).c_str(), 0);
  }

  auto location_data = pugiutil::loc_data(std::string(filename));
  pugi::xml_parse_result load_result;
  std::vector<char> xml_data;

  // 1. Inspect the file's first two bytes (magic numbers) safely
  {
    std::ifstream file(file_path, std::ios::binary);
    std::array<std::byte, 2> magic{};

    if (file.read(reinterpret_cast<char*>(magic.data()), magic.size())) {
      // Check for GZIP identification signature (0x1F, 0x8B)
      if (magic[0] == std::byte{0x1F} && magic[1] == std::byte{0x8B}) {
        if (!load_gz_to_buffer(file_path, xml_data)) {
          throw pugiutil::XmlError(
            "Failed to decompress GZ file: " + std::string(filename),
            std::string(filename).c_str(), 0);
        }
        // Fast in-place DOM generation (destructive to xml_data)
        load_result = doc.load_buffer_inplace(xml_data.data(), xml_data.size());
      }
    }
  }  // File stream closes here automatically

  // 2. Fallback: Parse as a regular uncompressed XML file if the buffer is
  // empty
  if (xml_data.empty()) {
    load_result = doc.load_file(file_path.string().c_str());
  }

  // 3. Error diagnostics and exception handling
  if (!load_result) {
    std::string msg = load_result.description();
    auto line = location_data.line(load_result.offset);
    auto col = location_data.col(load_result.offset);

    throw pugiutil::XmlError("Unable to load XML file '" +
                               std::string(filename) + "', " + msg +
                               " (line: " + std::to_string(line) +
                               " col: " + std::to_string(col) + ")",
                             std::string(filename).c_str(), line);
  }

  return location_data;
}

}  // namespace openfpga
