#include "vpr_app_options.h"

#include <algorithm>
#include <map>
#include <sstream>

#include "vpr_option_table.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* The name of a file without its directory and its extension */
std::string file_stem(const std::string& path) {
  const size_t slash = path.find_last_of("/\\");
  std::string name =
    (std::string::npos == slash) ? path : path.substr(slash + 1);
  const size_t dot = name.find_last_of('.');
  if (std::string::npos != dot && dot > 0) {
    name = name.substr(0, dot);
  }
  return name;
}

std::vector<std::string> split_words(const std::string& text) {
  std::vector<std::string> words;
  std::istringstream stream(text);
  std::string word;
  while (stream >> word) {
    words.push_back(word);
  }
  return words;
}

}  // End anonymous namespace

bool vpr_args_from_app_options(const AppOptions& options,
                               std::vector<std::string>& args,
                               std::string& error) {
  args.clear();
  args.push_back("vpr");

  /* The positional arguments */
  const AppOptionValue* arch = options.find("filename.arch_file");
  if (nullptr == arch || arch->string_value.empty()) {
    error = "The architecture is not set: set the option filename.arch_file";
    return false;
  }
  std::string circuit;
  const AppOptionValue* circuit_name = options.find("filename.circuit_name");
  const AppOptionValue* circuit_file = options.find("filename.circuit_file");
  if (nullptr != circuit_name && !circuit_name->string_value.empty()) {
    circuit = circuit_name->string_value;
  } else if (nullptr != circuit_file && !circuit_file->string_value.empty()) {
    circuit = file_stem(circuit_file->string_value);
  } else {
    error =
      "The circuit is not set: set the option filename.circuit_file or "
      "filename.circuit_name";
    return false;
  }
  args.push_back(arch->string_value);
  args.push_back(circuit);

  /* The options that are not at their default value */
  for (const VprOptionSpec& spec : vpr_option_specs()) {
    const std::string name = std::string(spec.group) + "." + spec.name;
    const std::string flag = spec.flag;
    /* The positional arguments are done */
    if (0 != flag.compare(0, 2, "--")) {
      continue;
    }
    const AppOptionValue* option = options.find(name);
    if (nullptr == option || options.is_default(name)) {
      continue;
    }

    if (spec.store_true) {
      /* A switch can only be turned on. The default is off */
      if (option->bool_value) {
        args.push_back(flag);
      }
      continue;
    }

    args.push_back(flag);
    switch (option->type) {
      case AppOptionValue::BOOLEAN:
        args.push_back(option->bool_value ? "on" : "off");
        break;
      default:
        if (spec.multi_value) {
          for (const std::string& word : split_words(option->to_string())) {
            args.push_back(word);
          }
        } else {
          args.push_back(option->to_string());
        }
        break;
    }
  }
  return true;
}

t_options read_vpr_options(const std::vector<std::string>& args) {
  std::vector<const char*> argv;
  argv.reserve(args.size());
  for (const std::string& arg : args) {
    argv.push_back(arg.c_str());
  }
  return read_options(static_cast<int>(argv.size()), argv.data());
}

} /* End namespace openfpga */
