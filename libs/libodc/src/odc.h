#ifndef ODC_H
#define ODC_H
/********************************************************************
 * ODC, the constraints file of OpenFPGA (docs/spec_odc.md).
 *
 * - read_odc_file() parses a file and checks each command against the table
 *   of commands (R-60): its arguments, its objects, its locations;
 * - resolve() matches the objects against a netlist and checks the
 *   commands against a device, as far as they are known: what matched, what
 *   did not, and the errors that VPR would not explain;
 * - the writers make the files of the tools: an SDC for VPR, the VPR
 *   constraints XML (partitions and global routing), the pin constraints XML
 *   of the OpenFPGA commands, and the Yosys commands of the synthesis.
 *******************************************************************/
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "odc_types.h"

/* begin namespace odc */
namespace odc {

/* Reading ----------------------------------------------------------------*/

struct ReadOptions {
  /* An unknown command is a warning, and is skipped (R-4) */
  bool lenient = false;
  /* The properties of the pcf_config of the device package (R-35). None
   * today */
  std::set<std::string> custom_properties;
};

/* Read a file, with the files it includes (R-37). The errors and warnings
 * are in the diagnostics; a file that cannot be opened is an error there */
Constraints read_odc_file(const std::filesystem::path& file,
                          const ReadOptions& options);

/* Read the text of a file. Includes are relative to the folder of file */
Constraints read_odc_text(const std::string& text, const std::string& file,
                          const ReadOptions& options);

/* The constraints of the package's odc_defaults first, then the user's
 * (R-46): the commands of the defaults are marked as such */
Constraints merge_defaults(const Constraints& defaults,
                           const Constraints& user);

/* Parse a location (R-6): X3Y2, X3Y2/1, X1Y1:X4Y6. Return false with the
 * reason if it is not one */
bool parse_location(const std::string& text, Location& location,
                    std::string& error);

/* Matching ---------------------------------------------------------------*/

/* The properties that -filter takes for a kind of object (R-13) */
const std::set<std::string>& filter_properties(const std::string& kind);

/* Check the expression of a -filter. Return false with the reason */
bool check_filter(const std::string& filter, const std::string& kind,
                  std::string& error);

/* The objects that a query matches, in their order. Return false with the
 * reason if a pattern or the filter is not valid */
bool match_query(const Query& query, const std::vector<Object>& objects,
                 std::vector<const Object*>& matched, std::string& error);

/* A glob of R-11 matched against a whole name. Without hierarchical, '*' and
 * '?' do not match the separators of the hierarchy ('.' and '/') */
bool glob_match(const std::string& pattern, const std::string& name,
                const bool hierarchical, const bool nocase = false);

/* The names closest to a pattern, at most count of them */
std::vector<std::string> nearest_names(const std::string& pattern,
                                       const std::vector<Object>& objects,
                                       const size_t count = 3);

/* A name as a regular expression that VPR, which searches its patterns
 * anywhere in a name, matches only on the whole name: anchored, with its
 * special characters escaped (R-11) */
std::string exact_regex(const std::string& name);

/* A glob of R-11 as an anchored regular expression */
std::string glob_regex(const std::string& pattern, const bool hierarchical);

/* The netlist ------------------------------------------------------------*/

/* Read the names of a BLIF netlist (.blif or .eblif). model_outputs gives
 * the output ports of the models that the file does not define, to name a
 * .subckt without .cname as VPR does: by the net of its first output.
 * Throws OdcError */
Netlist read_blif_netlist(
  const std::filesystem::path& file,
  const std::map<std::string, std::vector<std::string>>& model_outputs = {});

/* The name of the block that VPR makes for a top port */
std::string port_atom(const Object& port);

/* Checking ---------------------------------------------------------------*/

/* What is known of a design when the constraints are checked: any of these
 * may be missing, and the checks that need it wait */
struct Design {
  const Netlist* netlist = nullptr;
  const Device* device = nullptr;
  /* The pins of the device: the rows of the I/O map, or the I/O subtiles of
   * the grid. Without a map, PACKAGE_PIN cannot be used */
  const std::vector<Site>* sites = nullptr;
  bool io_map = false;
};

/* The objects that one query of a command matched */
struct QueryMatch {
  size_t command = 0; /* index in Constraints::commands */
  size_t word = 0;    /* index in Command::words */
  std::string text;   /* the query, as written */
  std::string kind;
  /* False while the objects are not known (no netlist yet, or the RTL that
   * Yosys reads) */
  bool checked = false;
  std::vector<std::string> matched;
  /* The names closest to the patterns, when nothing matched */
  std::vector<std::string> nearest;
};

/* A partition of the VPR constraints: a pblock, a fixed cell or a fixed port */
struct Partition {
  std::string name;
  size_t command = 0;
  /* The names of the blocks of VPR, when the netlist is known */
  std::vector<std::string> atoms;
  /* Otherwise, anchored regular expressions */
  std::vector<std::string> patterns;
  std::vector<Location> regions;
};

/* A set_global_signal (R-28) */
struct GlobalSignal {
  size_t command = 0;
  std::vector<std::string> nets;     /* when the netlist is known */
  std::vector<std::string> patterns; /* otherwise, regular expressions */
  std::string route_model;
  std::string network;
};

/* A port of the design on a pin of the device (R-31 to R-34) */
struct PinAssignment {
  size_t command = 0;
  std::string port;
  Site site;
  /* The default value of the pin constraints (R-34), or empty */
  std::string default_value;
};

/* A clock of create_clock */
struct Clock {
  size_t command = 0;
  std::string name;
  std::string period;
  std::vector<std::string> targets; /* the names of its objects */
};

/* The coverage of the timing (R-41), when the netlist is known */
struct TimingCoverage {
  bool checked = false;
  std::vector<Clock> clocks;
  /* The nets that clock a register and that no create_clock names */
  std::vector<std::string> unconstrained_clocks;
  std::vector<std::string> inputs_without_delay;
  std::vector<std::string> outputs_without_delay;
};

struct Resolution {
  std::vector<Diagnostic> diagnostics;
  std::vector<QueryMatch> queries;
  std::vector<Partition> partitions;
  std::vector<GlobalSignal> global_signals;
  std::vector<PinAssignment> pins;
  TimingCoverage timing;
  /* The commands that could be checked: their objects and locations were
   * known. One flag per command */
  std::vector<bool> checked;

  bool has_errors() const;
};

/* Match the objects of the commands, and check the commands as far as the
 * design is known (R-40 step 2) */
Resolution resolve(const Constraints& constraints, const Design& design);

/* Writing ----------------------------------------------------------------*/

/* The timing commands, as VPR reads them (R-20). Empty when there are none */
std::string write_sdc(const Constraints& constraints);

/* The floorplanning, the pins and the global routing, as a VPR constraints
 * XML (R-25, R-28, R-31). Empty when there is nothing */
std::string write_vpr_constraints(const Resolution& resolution);

/* The pins without a location, as the pin constraints XML of OpenFPGA
 * (R-32, R-34). Empty when there are none */
std::string write_pin_constraints(const Resolution& resolution);

/* The synthesis commands as Yosys commands, for the line ${ODC_SYNTHESIS} of
 * a script (R-22). Empty when there are none. The options -top and -define
 * are not in it: see synthesis_options() */
std::string write_yosys(const Constraints& constraints);

/* The options of set_synth_option that change what is synthesized */
struct SynthesisOptions {
  std::string top;
  std::vector<std::pair<std::string, std::string>> defines;
};
SynthesisOptions synthesis_options(const Constraints& constraints);

/* The constraints back as a file (R-42): the header, then each command of the
 * file and of its includes in their order, without the comments and without
 * the commands of the package's defaults */
std::string write_odc(const Constraints& constraints);

} /* end namespace odc */

#endif
