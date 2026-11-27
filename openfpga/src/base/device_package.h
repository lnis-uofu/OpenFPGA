#ifndef DEVICE_PACKAGE_H
#define DEVICE_PACKAGE_H
/********************************************************************
 * Device packages: one directory with everything that targets one device
 * (docs/spec_device_package.md).
 *
 * load_device_package() reads the manifest of a package and checks what a
 * session needs before it uses the package: the format version, the name,
 * the license and its LICENSE file, the checksums, that every path is a file
 * inside the package, the type of each file, one architecture of VPR and one
 * of OpenFPGA, the files that the manifest lists, the layout of the device
 * in the VPR architecture, the options of the flow, and whether the cached
 * files of the device (R-27) can be read. It never writes into a package.
 *******************************************************************/
#include <filesystem>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

/* begin namespace openfpga */
namespace openfpga {

/* A package that cannot be used: the message says why */
class DevicePackageError : public std::runtime_error {
 public:
  explicit DevicePackageError(const std::string& message)
    : std::runtime_error(message) {}
};

struct DevicePackageFile {
  std::string type;   /* R-20, or one that starts with x_ */
  std::string path;   /* inside the package, with '/' */
  std::string corner; /* empty for a file of every corner */
};

/* A value of flow_defaults.vpr_options: text, a number or true/false */
struct DevicePackageValue {
  enum Kind { TEXT, INTEGER, REAL, BOOLEAN } kind = TEXT;
  std::string text;    /* the value as an option takes it */
  std::string literal; /* the value as the manifest has it, for messages */
  bool boolean = false;
  double number = 0;
};

struct DevicePackage {
  std::string name;
  std::string version;
  std::string description;
  std::string license;
  std::filesystem::path directory; /* absolute */
  std::string layout;       /* device.layout: a fixed layout of the VPR arch */
  long long grid_width = 0; /* the size of that layout, from the VPR arch */
  long long grid_height = 0;
  std::filesystem::path vpr_arch;  /* absolute */
  std::filesystem::path openfpga_arch;
  std::optional<long long> lut_size;
  std::optional<long long> channel_width;
  std::vector<std::string> tags;
  bool flat_routing = false;
  /* flow_defaults.vpr_options, by the flag of VPR without its dashes, in the
   * order of the names */
  std::vector<std::pair<std::string, DevicePackageValue>> vpr_options;
  std::vector<DevicePackageFile> files;
  /* flow_defaults.yosys_options: the options of read_verilog and of the DSP
   * mapping that the script of the package takes, as the manifest gives them.
   * valid is false when yosys_options is not a map of lists */
  std::vector<std::string> yosys_read_verilog;
  std::vector<std::string> yosys_dsp_map;
  bool yosys_options_valid = true;
  /* device.bitstream, and flow_defaults.bitstream (spec_bitstream.md B-10,
   * B-11): the flags of link_openfpga_arch and build_fabric, and the fixups.
   * has_bitstream_recipe is false when the manifest has none (the defaults) */
  bool bitstream = true;
  bool has_bitstream_recipe = false;
  std::vector<std::string> bitstream_link_options;
  std::vector<std::string> bitstream_fixups;
  std::vector<std::string> bitstream_fabric_options;
  /* The corners (spec R-16 to R-24): the names of corners.list, and the corner
   * in use, which is the one asked for or corners.default. corner is empty
   * when the package has none. timing_values holds the variables of
   * vpr_arch.xml for that corner, already in the units of VPR (seconds,
   * farads, ohms). has_variables is true when vpr_arch.xml has any ${name} */
  std::vector<std::string> corners;
  std::string corner;
  std::map<std::string, double> timing_values;
  bool has_variables = false;
  /* The JSON text of the reference of the manifest, or "{}" */
  std::string reference;
  /* The cached files of the device (R-27) for the corner in use, by type
   * (rr_graph, router_lookahead, place_delay_lookup), as whole paths. They
   * are read only when cache_usable: cache.vtr_version is the version of VTR
   * of this build and cache.key is the key of the files they were made from
   * (cache_key). cache_reason says why not, when there are any */
  std::map<std::string, std::filesystem::path> cached;
  std::string cache_vtr_version;
  std::string cache_key_given;
  std::string cache_key;
  bool cache_usable = false;
  std::string cache_reason;

  /* The files of a type, as whole paths */
  std::vector<std::filesystem::path> files_of(const std::string& type) const;
};

/* The package in a directory, checked. corner is the corner to use ("" for
 * corners.default). Throws DevicePackageError with the first problem */
DevicePackage load_device_package(const std::filesystem::path& directory,
                                  const bool verify_checksums = true,
                                  const std::string& corner = "");

/* The types of the files that hold the device that VPR makes, which a
 * package may cache (R-27), and the option of VPR that reads each */
const std::vector<std::pair<std::string, std::string>>& cached_file_types();

/* The version of VTR of this build, which the cached files must have */
std::string vtr_build_version();

/* The VPR architecture of the package for its corner: the text of vpr_arch.xml
 * with each ${name} replaced by its value (R-22). The file of the package is
 * not changed. Throws DevicePackageError with the name of a variable that has
 * no value */
std::string render_vpr_arch(const DevicePackage& package);

/* What a package sets when it is read into a session: the application options
 * (by name, with their value as text) and the entries of
 * flow_defaults.vpr_options that it does not set, with the reason. flags maps
 * the flags of VPR ("--seed") to the names of the options ("place.seed") */
struct DevicePackageOptions {
  std::vector<std::pair<std::string, DevicePackageValue>> set;
  std::vector<std::pair<std::string, std::string>> ignored;
};
DevicePackageOptions device_package_options(
  const DevicePackage& package,
  const std::vector<std::pair<std::string, std::string>>& flags);

/* The data of the answer report_device_package (JSON): the package, with its
 * files, and when given the options it set and did not set */
std::string device_package_json(const DevicePackage& package,
                                const DevicePackageOptions* applied);

} /* end namespace openfpga */

#endif
