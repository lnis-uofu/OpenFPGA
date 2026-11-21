#include "app_options.h"

#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>

/* Begin namespace openfpga */
namespace openfpga {

namespace {

std::string lower_case(const std::string& text) {
  std::string lower = text;
  for (char& c : lower) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  return lower;
}

/* The shortest text that gives the same number back */
std::string format_double(const double value) {
  char buffer[64];
  for (int precision = 6; precision <= 17; ++precision) {
    std::snprintf(buffer, sizeof(buffer), "%.*g", precision, value);
    if (std::strtod(buffer, nullptr) == value) {
      break;
    }
  }
  return std::string(buffer);
}

std::string join(const std::vector<std::string>& items) {
  std::string joined;
  for (const std::string& item : items) {
    joined += (joined.empty() ? "" : ", ") + item;
  }
  return joined;
}

}  // End anonymous namespace

AppOptionValue AppOptionValue::make_string(const std::string& value,
                                           const std::string& help_message) {
  AppOptionValue option;
  option.type = STRING;
  option.string_value = value;
  option.help_message = help_message;
  return option;
}

AppOptionValue AppOptionValue::make_int(const int value,
                                        const std::string& help_message) {
  AppOptionValue option;
  option.type = INT;
  option.int_value = value;
  option.help_message = help_message;
  return option;
}

AppOptionValue AppOptionValue::make_float(const double value,
                                          const std::string& help_message) {
  AppOptionValue option;
  option.type = FLOAT;
  option.float_value = value;
  option.help_message = help_message;
  return option;
}

AppOptionValue AppOptionValue::make_boolean(const bool value,
                                            const std::string& help_message) {
  AppOptionValue option;
  option.type = BOOLEAN;
  option.bool_value = value;
  option.help_message = help_message;
  return option;
}

AppOptionValue AppOptionValue::make_selection(
  const std::string& value, const std::map<std::string, int>& selection_values,
  const std::string& help_message) {
  AppOptionValue option;
  option.type = SELECTION;
  option.string_value = value;
  option.selection_values = selection_values;
  option.help_message = help_message;
  const auto selected = selection_values.find(value);
  if (selected != selection_values.end()) {
    option.int_value = selected->second;
  }
  return option;
}

int AppOptionValue::to_enum() const {
  if (SELECTION != type) {
    return 0;
  }
  const auto selected = selection_values.find(string_value);
  return selection_values.end() == selected ? 0 : selected->second;
}

std::string AppOptionValue::to_string() const {
  switch (type) {
    case STRING:
    case SELECTION:
      return string_value;
    case INT:
      return std::to_string(int_value);
    case FLOAT:
      return format_double(float_value);
    case BOOLEAN:
      return bool_value ? "true" : "false";
    default:
      break;
  }
  return std::string();
}

std::string AppOptionValue::type_name() const {
  switch (type) {
    case STRING:
      return "string";
    case INT:
      return "int";
    case FLOAT:
      return "float";
    case BOOLEAN:
      return "bool";
    case SELECTION:
      return "selection";
    default:
      break;
  }
  return "unknown";
}

std::vector<std::string> AppOptionValue::choices() const {
  std::vector<std::string> names;
  for (const auto& selection : selection_values) {
    names.push_back(selection.first);
  }
  return names;
}

bool AppOptionValue::set_from_string(const std::string& text,
                                     std::string& error) {
  switch (type) {
    case STRING:
      string_value = text;
      return true;

    case INT: {
      errno = 0;
      char* end = nullptr;
      const long parsed = std::strtol(text.c_str(), &end, 10);
      if (text.empty() || *end != '\0' ||
          std::isspace(static_cast<unsigned char>(text[0]))) {
        error = "'" + text + "' is not a whole number";
        return false;
      }
      if (ERANGE == errno || parsed < INT_MIN || parsed > INT_MAX) {
        error = "'" + text + "' is out of the range of an int";
        return false;
      }
      int_value = static_cast<int>(parsed);
      return true;
    }

    case FLOAT: {
      errno = 0;
      char* end = nullptr;
      const double parsed = std::strtod(text.c_str(), &end);
      if (text.empty() || *end != '\0' ||
          std::isspace(static_cast<unsigned char>(text[0]))) {
        error = "'" + text + "' is not a number";
        return false;
      }
      if (!std::isfinite(parsed)) {
        error = "'" + text + "' is not a finite number";
        return false;
      }
      float_value = parsed;
      return true;
    }

    case BOOLEAN: {
      const std::string lower = lower_case(text);
      if ("true" == lower || "on" == lower || "yes" == lower || "1" == lower) {
        bool_value = true;
        return true;
      }
      if ("false" == lower || "off" == lower || "no" == lower || "0" == lower) {
        bool_value = false;
        return true;
      }
      error = "'" + text +
              "' is not a boolean: use true, false, on, off, yes, no, 1 or 0";
      return false;
    }

    case SELECTION: {
      const auto selected = selection_values.find(text);
      if (selection_values.end() == selected) {
        error = "'" + text + "' is not one of: " + join(choices());
        return false;
      }
      string_value = text;
      int_value = selected->second;
      return true;
    }

    default:
      break;
  }
  error = "unknown type of option";
  return false;
}

AppOptions::AppOptions() : values_(app_option_defaults()) {}

bool AppOptions::has(const std::string& name) const {
  return values_.end() != values_.find(name);
}

const AppOptionValue* AppOptions::find(const std::string& name) const {
  const auto found = values_.find(name);
  return values_.end() == found ? nullptr : &found->second;
}

std::vector<std::string> AppOptions::names() const {
  std::vector<std::string> names;
  for (const auto& option : values_) {
    names.push_back(option.first);
  }
  return names;
}

std::vector<std::string> AppOptions::names(const std::string& group) const {
  std::vector<std::string> names;
  const std::string prefix = group + ".";
  for (const auto& option : values_) {
    if (0 == option.first.compare(0, prefix.size(), prefix)) {
      names.push_back(option.first);
    }
  }
  return names;
}

const AppOptionValue& AppOptions::default_of(const std::string& name) const {
  return app_option_defaults().at(name);
}

bool AppOptions::is_default(const std::string& name) const {
  const AppOptionValue* option = find(name);
  return nullptr != option &&
         option->to_string() == default_of(name).to_string();
}

bool AppOptions::set(const std::string& name, const std::string& text,
                     std::string& error) {
  const auto found = values_.find(name);
  if (values_.end() == found) {
    error = "Application option '" + name + "' does not exist";
    return false;
  }
  std::string reason;
  if (!found->second.set_from_string(text, reason)) {
    error = "Invalid value for the " + found->second.type_name() +
            " application option '" + name + "': " + reason;
    return false;
  }
  return true;
}

bool AppOptions::reset(const std::string& name) {
  const auto found = values_.find(name);
  if (values_.end() == found) {
    return false;
  }
  found->second = default_of(name);
  return true;
}

void AppOptions::reset_all() { values_ = app_option_defaults(); }

} /* End namespace openfpga */
