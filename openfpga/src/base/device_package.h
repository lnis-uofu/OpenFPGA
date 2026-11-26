#ifndef DEVICE_PACKAGE_H
#define DEVICE_PACKAGE_H
/********************************************************************
 * Device packages: one directory with everything that targets one device
 * (docs/spec_device_package.md).
 *
 * load_device_package() reads the manifest of a package and checks what a
 * session needs before it uses the package: the format version, the name,
 * the license, the checksums, that every path is a file inside the package,
 * one architecture of VPR and one of OpenFPGA, the files that the manifest
 * lists, the grid and the layout of that name in the VPR architecture, and
 * the options of the flow. It never writes into a package.
 *******************************************************************/
#include <filesystem>
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
  std::string role;
  std::string path; /* inside the package, with '/' */
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
  std::string layout;              /* "<width>x<height>" of device.grid */
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
  /* The JSON text of the reference of the manifest, or "{}" */
  std::string reference;

  /* The files of a role, as whole paths */
  std::vector<std::filesystem::path> files_of(const std::string& role) const;
};

/* The package in a directory, checked. Throws DevicePackageError with the
 * first problem */
DevicePackage load_device_package(const std::filesystem::path& directory,
                                  const bool verify_checksums = true);

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
