#ifndef DESIGN_SYNTHESIS_H
#define DESIGN_SYNTHESIS_H
/********************************************************************
 * Synthesis with Yosys, and the design directories
 * (docs/spec_design_directory.md).
 *
 * A Verilog design becomes a BLIF netlist of the LUTs of one device, with a
 * script made from the template of the device package of the session (role
 * yosys_script) or from a generic one. What goes into a script is checked:
 * plain file names, the top module, and a few values of the package, as a
 * script can run commands and write anywhere. A source that takes a file from
 * outside its folder (`include "/etc/passwd"`) is refused.
 *******************************************************************/
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "device_package.h"

/* begin namespace openfpga */
namespace openfpga {

/* What is refused before Yosys runs: the arguments, the sources, the design.
 * The message says why */
class SynthesisRefused : public std::runtime_error {
 public:
  explicit SynthesisRefused(const std::string& message)
    : std::runtime_error(message) {}
};

/* What a synthesis is asked for */
struct SynthesisRequest {
  std::string top;
  std::vector<std::filesystem::path> files;
  std::optional<int> lut_size; /* when there is no device package */
  std::vector<std::filesystem::path> include_dirs;
  std::vector<std::pair<std::string, std::string>> defines;
  std::vector<std::string> read_options;
  std::filesystem::path folder; /* where the script and the netlist go */
};

/* What one synthesis runs: the script, and where its result goes */
struct SynthesisPlan {
  std::string top;
  std::string script;
  std::filesystem::path script_file;
  std::filesystem::path netlist; /* .blif, or .eblif for extended BLIF */
  int lut_size = 0;
  std::vector<std::filesystem::path> files;
  std::string package; /* the name of the device package, or empty */
};

/* The only script of a folder: each synthesis writes it again */
extern const char* const SYNTHESIS_SCRIPT_NAME;

/* The plan of a synthesis, with the package of the session or none. Throws
 * SynthesisRefused */
SynthesisPlan make_synthesis_plan(const SynthesisRequest& request,
                                  const DevicePackage* package);

/* A design directory, read and checked (docs/spec_design_directory.md) */
struct DesignDirectory {
  std::string name;
  std::filesystem::path folder;
  std::string top;
  std::vector<std::filesystem::path> rtl; /* empty for a design of a netlist */
  std::vector<std::filesystem::path> include_dirs;
  std::vector<std::pair<std::string, std::string>> defines;
  std::vector<std::string> read_options;
  /* The netlist to read instead of synthesizing: of a design with no RTL, or
   * the one of the label that was asked for. Empty otherwise */
  std::filesystem::path netlist;
  /* The SDC of the stage implementation (or of both), or empty */
  std::filesystem::path sdc;
};

/* Read the manifest of a design directory and check its files. label asks
 * for a netlist by its label. Throws SynthesisRefused */
DesignDirectory read_design_directory(const std::filesystem::path& folder,
                                      const std::string& label);

} /* end namespace openfpga */

#endif
