#pragma once

#include <zlib.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace openfpga {

class dynamic_streambuf : public std::streambuf {
 private:
  std::string base_filename;
  gzFile gz_file = nullptr;
  std::ofstream raw_file;
  bool is_compressed = false;
  std::vector<char> buffer;

  bool flush_buffer() {
    ptrdiff_t num_bytes = pptr() - pbase();
    if (num_bytes <= 0) return true;

    if (is_compressed && gz_file) {
      int written =
        gzwrite(gz_file, pbase(), static_cast<unsigned int>(num_bytes));
      if (written != num_bytes) return false;
    } else if (!is_compressed && raw_file.is_open()) {
      raw_file.write(pbase(), num_bytes);
      if (!raw_file) return false;
    } else {
      return false;
    }

    setp(buffer.data(), buffer.data() + buffer.size());
    return true;
  }

 public:
  dynamic_streambuf(const std::string& filename, bool initial_compress,
                    size_t buf_size = 1024)
    : base_filename(filename),
      is_compressed(initial_compress),
      buffer(buf_size) {
    setp(buffer.data(), buffer.data() + buffer.size());
    set_compression(initial_compress);
  }

  ~dynamic_streambuf() override { close(); }

  void close() {
    flush_buffer();
    if (gz_file) {
      gzclose(gz_file);
      gz_file = nullptr;
    }
    if (raw_file.is_open()) {
      raw_file.close();
    }
  }

  void set_compression(bool compress) {
    flush_buffer();

    if (compress == is_compressed && (gz_file || raw_file.is_open())) {
      return;
    }

    close();
    is_compressed = compress;

    if (is_compressed) {
      std::string gz_name = base_filename + ".gz";
      gz_file = gzopen(gz_name.c_str(), "wb6m");
    } else {
      raw_file.open(base_filename, std::ios::out | std::ios::binary);
    }
  }

  bool is_open() const {
    return is_compressed ? (gz_file != nullptr) : raw_file.is_open();
  }

 protected:
  int_type overflow(int_type ch) override {
    if (!flush_buffer()) return traits_type::eof();
    if (ch != traits_type::eof()) {
      *pptr() = traits_type::to_char_type(ch);
      pbump(1);
    }
    return ch;
  }

  int sync() override { return flush_buffer() ? 0 : -1; }
};

class mmostream : public std::ostream {
 private:
  std::unique_ptr<dynamic_streambuf> sbuf;

 public:
  mmostream(const std::string& filename, bool initial_compress = false)
    : std::ostream(nullptr) {
    sbuf = std::make_unique<dynamic_streambuf>(filename, initial_compress);
    init(sbuf.get());

    if (!sbuf->is_open()) {
      setstate(std::ios_base::badbit);
    }
  }

  void set_compression(bool compress) {
    sbuf->set_compression(compress);
    if (!sbuf->is_open()) {
      setstate(std::ios_base::badbit);
    }
  }

  void close() {
    if (sbuf) {
      sbuf->close();
    }
  }

  bool is_open() const { return sbuf && sbuf->is_open(); }

  // Explicit overload to catch standard stream manipulators like std::endl or
  // std::flush
  mmostream& operator<<(std::ostream& (*pf)(std::ostream&)) {
    // 1. Pass the manipulator down to the base class execution engine
    static_cast<std::ostream&>(*this) << pf;

    // 2. Perform your standard zlib error health tracking check
    if (this->bad() || this->fail()) {
      // Already flagged
    } else if (sbuf && sbuf->pubsync() != 0) {
      setstate(std::ios_base::badbit);
    }
    return *this;
  }

  template <typename T>
  mmostream& operator<<(const T& val) {
    static_cast<std::ostream&>(*this) << val;
    if (this->bad() || this->fail()) {
      // State already flagged
    } else if (sbuf &&
               sbuf->pubsync() != 0) {  // FIXED: Use pubsync() from streambuf
      setstate(std::ios_base::badbit);
    }
    return *this;
  }
};

inline mmostream& use_gzip(mmostream& os) {
  os.set_compression(true);
  return os;
}

inline mmostream& use_raw(mmostream& os) {
  os.set_compression(false);
  return os;
}

}  // namespace openfpga
