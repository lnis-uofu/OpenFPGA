#include "json_writer.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* The length of the UTF-8 sequence that starts at text[pos], if it is valid,
 * 0 if not. Overlong forms, surrogates and values above U+10FFFF are not
 * valid */
size_t valid_utf8_length(const std::string& text, const size_t pos) {
  const unsigned char first = static_cast<unsigned char>(text[pos]);
  size_t length = 0;
  uint32_t code = 0;
  uint32_t minimum = 0;
  if (first < 0x80) {
    return 1;
  } else if (0xC0 == (first & 0xE0)) {
    length = 2;
    code = first & 0x1F;
    minimum = 0x80;
  } else if (0xE0 == (first & 0xF0)) {
    length = 3;
    code = first & 0x0F;
    minimum = 0x800;
  } else if (0xF0 == (first & 0xF8)) {
    length = 4;
    code = first & 0x07;
    minimum = 0x10000;
  } else {
    return 0;
  }
  if (pos + length > text.size()) {
    return 0;
  }
  for (size_t i = 1; i < length; ++i) {
    const unsigned char next = static_cast<unsigned char>(text[pos + i]);
    if (0x80 != (next & 0xC0)) {
      return 0;
    }
    code = (code << 6) | (next & 0x3F);
  }
  if (code < minimum || code > 0x10FFFF || (code >= 0xD800 && code <= 0xDFFF)) {
    return 0;
  }
  return length;
}

}  // End anonymous namespace

std::string JsonWriter::quote(const std::string& text) {
  std::string out = "\"";
  size_t pos = 0;
  while (pos < text.size()) {
    const unsigned char c = static_cast<unsigned char>(text[pos]);
    if (c >= 0x80) {
      const size_t length = valid_utf8_length(text, pos);
      if (0 == length) {
        out += "\\ufffd";
        ++pos;
      } else {
        out.append(text, pos, length);
        pos += length;
      }
      continue;
    }
    switch (c) {
      case '"':
        out += "\\\"";
        break;
      case '\\':
        out += "\\\\";
        break;
      case '\b':
        out += "\\b";
        break;
      case '\f':
        out += "\\f";
        break;
      case '\n':
        out += "\\n";
        break;
      case '\r':
        out += "\\r";
        break;
      case '\t':
        out += "\\t";
        break;
      default:
        if (c < 0x20) {
          char buffer[8];
          std::snprintf(buffer, sizeof(buffer), "\\u%04x", c);
          out += buffer;
        } else {
          out += static_cast<char>(c);
        }
        break;
    }
    ++pos;
  }
  out += "\"";
  return out;
}

std::string JsonWriter::number(const double value) {
  if (!std::isfinite(value)) {
    return "null";
  }
  char buffer[64];
  for (int precision = 1; precision <= 17; ++precision) {
    std::snprintf(buffer, sizeof(buffer), "%.*g", precision, value);
    if (std::strtod(buffer, nullptr) == value) {
      break;
    }
  }
  return std::string(buffer);
}

void JsonWriter::before_value() {
  if (done_) {
    throw std::logic_error("JsonWriter: a value is already complete");
  }
  if (levels_.empty()) {
    return;
  }
  Level& level = levels_.back();
  if (Context::OBJECT == level.context) {
    if (!level.key_written) {
      throw std::logic_error("JsonWriter: a value needs a key in an object");
    }
    level.key_written = false;
    return;
  }
  if (!level.empty) {
    text_ += ',';
  }
  level.empty = false;
}

void JsonWriter::after_value() {
  if (levels_.empty()) {
    done_ = true;
  }
}

JsonWriter& JsonWriter::begin_object() {
  before_value();
  text_ += '{';
  levels_.push_back({Context::OBJECT, true, false});
  return *this;
}

JsonWriter& JsonWriter::end_object() {
  if (levels_.empty() || Context::OBJECT != levels_.back().context ||
      levels_.back().key_written) {
    throw std::logic_error("JsonWriter: no object to end here");
  }
  levels_.pop_back();
  text_ += '}';
  after_value();
  return *this;
}

JsonWriter& JsonWriter::begin_array() {
  before_value();
  text_ += '[';
  levels_.push_back({Context::ARRAY, true, false});
  return *this;
}

JsonWriter& JsonWriter::end_array() {
  if (levels_.empty() || Context::ARRAY != levels_.back().context) {
    throw std::logic_error("JsonWriter: no array to end here");
  }
  levels_.pop_back();
  text_ += ']';
  after_value();
  return *this;
}

JsonWriter& JsonWriter::key(const std::string& name) {
  if (done_ || levels_.empty() || Context::OBJECT != levels_.back().context ||
      levels_.back().key_written) {
    throw std::logic_error("JsonWriter: a key belongs in an object");
  }
  Level& level = levels_.back();
  if (!level.empty) {
    text_ += ',';
  }
  level.empty = false;
  level.key_written = true;
  text_ += quote(name);
  text_ += ':';
  return *this;
}

JsonWriter& JsonWriter::value(const std::string& text) {
  before_value();
  text_ += quote(text);
  after_value();
  return *this;
}

JsonWriter& JsonWriter::value(const char* text) {
  return nullptr == text ? null() : value(std::string(text));
}

JsonWriter& JsonWriter::value(const int num) {
  return value(static_cast<int64_t>(num));
}

JsonWriter& JsonWriter::value(const int64_t num) {
  before_value();
  text_ += std::to_string(num);
  after_value();
  return *this;
}

JsonWriter& JsonWriter::value(const double num) {
  before_value();
  text_ += JsonWriter::number(num);
  after_value();
  return *this;
}

JsonWriter& JsonWriter::value(const bool flag) {
  before_value();
  text_ += flag ? "true" : "false";
  after_value();
  return *this;
}

JsonWriter& JsonWriter::null() {
  before_value();
  text_ += "null";
  after_value();
  return *this;
}

bool JsonWriter::complete() const { return done_; }

std::string JsonWriter::str() const {
  if (!done_) {
    throw std::logic_error("JsonWriter: the text is not complete");
  }
  return text_;
}

} /* End namespace openfpga */
