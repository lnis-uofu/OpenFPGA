#include <zlib.h>

#include <iostream>

#include "pugixml.hpp"

namespace openfpga {

// 1. Create a custom writer inheriting from pugi::xml_writer
struct GzXmlWriter : public pugi::xml_writer {
  gzFile file;

  GzXmlWriter(const char* path) {
    // Open the file in write-binary mode with compression level 6 (default)
    file = gzopen(path, "wb6");
  }

  ~GzXmlWriter() {
    if (file) {
      gzclose(file);
    }
  }

  // This method is triggered sequentially by pugixml during saving
  virtual void write(const void* data, size_t size) override {
    if (file && size > 0) {
      gzwrite(file, data, static_cast<unsigned int>(size));
    }
  }

  bool isValid() const { return file != nullptr; }
};

}  // namespace openfpga
