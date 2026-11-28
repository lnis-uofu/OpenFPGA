#ifndef DESIGN_SYNTHESIS_H
#define DESIGN_SYNTHESIS_H
/********************************************************************
 * Synthesis with Yosys, and the design directories (see the page "Design
 * directories" of the manual).
 *
 * A Verilog design becomes a BLIF netlist of the LUTs of one device, with a
 * script made from a generic template, or from the one of the synthesis
 * target that a plugin gives for the session (set_synthesis_target_provider).
 * What goes into a script is checked: plain file names, the top module, and
 * the values of the target, as a script can run commands and write anywhere.
 * A source that takes a file from outside its folder (`include "/etc/passwd"`)
 * is refused.
 *******************************************************************/
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "openfpga_context.h"

/* begin namespace openfpga */
namespace openfpga {

/* The device that a synthesis is for, as a plugin describes it */
struct SynthesisTarget {
  std::string name;                   /* the device (package), "" for none */
  std::optional<int> lut_size;        /* the size of its LUTs, if it says */
  std::filesystem::path yosys_script; /* script template; "" = the generic */
  std::map<std::string, std::string> values; /* ${KEY} values of the script */
  /* Read as blackboxes by report_design_hierarchy (the cells of the device) */
  std::vector<std::filesystem::path> cell_libraries;
  /* The options of VPR of the device, by flag without dashes: the netlist is
   * extended BLIF (.eblif) when ("circuit_format", "eblif") is one */
  std::vector<std::pair<std::string, std::string>> vpr_options;
};

/* Fills the target of the session; returns false when there is none (the
 * generic synthesis). It may throw: synthesize and report_design_hierarchy
 * refuse with the message of a std::exception */
using SynthesisTargetProvider =
  std::function<bool(const OpenfpgaContext&, SynthesisTarget&)>;

/* Set by a plugin when it registers; an empty function takes it away */
void set_synthesis_target_provider(SynthesisTargetProvider provider);

/* The target of the session: false when there is none. Throws what the
 * provider throws */
bool synthesis_target(const OpenfpgaContext& ctx, SynthesisTarget& target);

/* The command that read_design reads the constraints file of a design with
 * (role constraints), e.g., a command of a plugin; "" for none, and a design
 * with a constraints file is then refused. Set by a plugin at register */
void set_design_constraints_command(const std::string& name);
const std::string& design_constraints_command();

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
  std::optional<int> lut_size; /* when the target does not give it */
  std::vector<std::filesystem::path> include_dirs;
  std::vector<std::pair<std::string, std::string>> defines;
  std::vector<std::string> read_options;
  std::filesystem::path folder; /* where the script and the netlist go */
  /* The Yosys commands of the synthesis constraints of the session, for the
   * line ${CONSTRAINT_SYNTHESIS} of the script, or empty */
  std::string constraints;
};

/* What one synthesis runs: the script, and where its result goes */
struct SynthesisPlan {
  std::string top;
  std::string script;
  std::filesystem::path script_file;
  std::filesystem::path netlist; /* .blif, or .eblif for extended BLIF */
  int lut_size = 0;
  std::vector<std::filesystem::path> files;
  std::string target; /* the name of the synthesis target, or empty */
  /* The script has the synthesis commands of the constraints */
  bool constraints = false;
};

/* The only script of a folder: each synthesis writes it again */
extern const char* const SYNTHESIS_SCRIPT_NAME;

/* The plan of a synthesis, with the target of the session or none (null).
 * Throws SynthesisRefused */
SynthesisPlan make_synthesis_plan(const SynthesisRequest& request,
                                  const SynthesisTarget* target);

/* The script of report_design_hierarchy: the RTL of a request read as its
 * synthesis reads it (with the cell libraries of the target, as blackboxes),
 * elaborated from its top, and written as the JSON of Yosys in json_file.
 * Throws SynthesisRefused */
std::string make_hierarchy_script(const SynthesisRequest& request,
                                  const SynthesisTarget* target,
                                  const std::filesystem::path& json_file);

/* A design directory, read and checked */
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
  /* The constraints file of the design (role constraints), or empty */
  std::filesystem::path constraints;
};

/* Read the manifest of a design directory and check its files. label asks
 * for a netlist by its label. Throws SynthesisRefused */
DesignDirectory read_design_directory(const std::filesystem::path& folder,
                                      const std::string& label);

} /* end namespace openfpga */

#endif
