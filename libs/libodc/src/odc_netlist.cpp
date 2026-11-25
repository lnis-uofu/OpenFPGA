/********************************************************************
 * The names of a BLIF netlist, as VPR names its blocks: a top input is a
 * block of its name, a top output the block "out:<name>", a .names or a
 * .latch the block of the net it drives, a .subckt the block of its .cname
 * or else of the net of its first output.
 *******************************************************************/
#include <algorithm>
#include <cctype>
#include <set>

#include "blifparse.hpp"
#include "odc.h"

/* begin namespace odc */
namespace odc {

namespace {

struct Subckt {
  std::string model;
  std::vector<std::string> ports;
  std::vector<std::string> nets;
  std::string cname;
};

class NetlistReader : public blifparse::Callback {
 public:
  explicit NetlistReader(
    const std::map<std::string, std::vector<std::string>>& model_outputs)
    : model_outputs_(model_outputs) {}

  void start_parse() override {}
  void filename(std::string) override {}
  void lineno(int line) override { line_ = line; }
  void begin_model(std::string name) override {
    ++models_;
    model_ = name;
    if (1 == models_) {
      netlist.top = name;
    }
  }
  void inputs(std::vector<std::string> names) override {
    if (1 == models_) {
      for (const std::string& name : names) {
        netlist.ports.push_back({name, {{"direction", "in"}}});
        net(name);
      }
    } else {
      model_inputs_[model_] = names;
    }
  }
  void outputs(std::vector<std::string> names) override {
    if (1 == models_) {
      for (const std::string& name : names) {
        netlist.ports.push_back({name, {{"direction", "out"}}});
        net(name);
      }
    } else {
      defined_outputs_[model_] = names;
    }
  }
  void names(std::vector<std::string> nets,
             std::vector<std::vector<blifparse::LogicValue>>) override {
    if (1 != models_ || nets.empty()) {
      return;
    }
    for (const std::string& name : nets) {
      net(name);
    }
    cell(nets.back(), "names");
    last_subckt_ = false;
  }
  void latch(std::string input, std::string output, blifparse::LatchType,
             std::string control, blifparse::LogicValue) override {
    if (1 != models_) {
      return;
    }
    net(input);
    net(output);
    if (!control.empty() && "NIL" != control) {
      net(control);
      netlist.clock_nets.insert(control);
    }
    cell(output, "latch");
    last_subckt_ = false;
  }
  void subckt(std::string model, std::vector<std::string> ports,
              std::vector<std::string> nets) override {
    if (1 != models_) {
      return;
    }
    for (const std::string& name : nets) {
      net(name);
    }
    subckts_.push_back({model, ports, nets, ""});
    last_subckt_ = true;
  }
  void blackbox() override {}
  void end_model() override {}
  void conn(std::string src, std::string dst) override {
    if (1 == models_) {
      net(src);
      net(dst);
    }
  }
  void cname(std::string name) override {
    if (1 == models_ && last_subckt_ && !subckts_.empty()) {
      subckts_.back().cname = name;
    } else if (1 == models_ && !netlist.cells.empty()) {
      /* .cname after a .names or a .latch renames its block */
      netlist.cells.back().name = name;
    }
  }
  void attr(std::string, std::string) override {}
  void param(std::string, std::string) override {}
  void finish_parse() override {
    for (const Subckt& subckt : subckts_) {
      std::set<std::string> outputs;
      const auto defined = defined_outputs_.find(subckt.model);
      const auto given = model_outputs_.find(subckt.model);
      if (defined_outputs_.end() != defined) {
        outputs.insert(defined->second.begin(), defined->second.end());
      } else if (model_outputs_.end() != given) {
        outputs.insert(given->second.begin(), given->second.end());
      }
      std::string name = subckt.cname;
      for (size_t i = 0; name.empty() && i < subckt.ports.size(); ++i) {
        if (outputs.count(port_name(subckt.ports[i]))) {
          name = subckt.nets[i];
        }
      }
      if (name.empty() && !subckt.nets.empty()) {
        name = subckt.nets.back();
      }
      cell(name, subckt.model);
      for (size_t i = 0; i < subckt.ports.size(); ++i) {
        const bool out = outputs.count(port_name(subckt.ports[i])) > 0;
        netlist.pins.push_back(
          {name + "/" + subckt.ports[i], {{"direction", out ? "out" : "in"}}});
        std::string lower = port_name(subckt.ports[i]);
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (!out && ("clk" == lower || "clock" == lower || "c" == lower)) {
          netlist.clock_nets.insert(subckt.nets[i]);
        }
      }
    }
  }
  void parse_error(const int line, const std::string& near,
                   const std::string& message) override {
    throw OdcError("line " + std::to_string(line) + " near '" + near +
                   "': " + message);
  }

  Netlist netlist;

 private:
  /* The port of a connection a[3]=net is a */
  static std::string port_name(const std::string& port) {
    const size_t bracket = port.find('[');
    return std::string::npos == bracket ? port : port.substr(0, bracket);
  }

  void net(const std::string& name) {
    if ("unconn" != name && nets_.insert(name).second) {
      netlist.nets.push_back({name, {}});
    }
  }

  void cell(const std::string& name, const std::string& type) {
    netlist.cells.push_back({name, {{"type", type}}});
  }

  const std::map<std::string, std::vector<std::string>>& model_outputs_;
  int line_ = 0;
  int models_ = 0;
  std::string model_;
  bool last_subckt_ = false;
  std::set<std::string> nets_;
  std::vector<Subckt> subckts_;
  std::map<std::string, std::vector<std::string>> model_inputs_;
  std::map<std::string, std::vector<std::string>> defined_outputs_;
};

}  // namespace

Netlist read_blif_netlist(
  const std::filesystem::path& file,
  const std::map<std::string, std::vector<std::string>>& model_outputs) {
  NetlistReader reader(model_outputs);
  try {
    blifparse::blif_parse_filename(file.string(), reader);
  } catch (const OdcError& error) {
    throw OdcError("cannot read the netlist '" + file.string() +
                   "': " + error.what());
  }
  reader.netlist.file = file.string();
  return reader.netlist;
}

std::string port_atom(const Object& port) {
  const auto found = port.properties.find("direction");
  if (port.properties.end() != found && "out" == found->second) {
    return "out:" + port.name;
  }
  return port.name;
}

} /* end namespace odc */
