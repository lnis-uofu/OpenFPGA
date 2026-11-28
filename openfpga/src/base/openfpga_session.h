#ifndef OPENFPGA_SESSION_H
#define OPENFPGA_SESSION_H

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "app_options.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Inputs of an OpenFPGA flow, which are all read upfront
 *******************************************************************/
enum class OpenFpgaInput : int {
  VPR_ARCH = 0, /* VPR architecture file */
  OPENFPGA_ARCH,
  CIRCUIT /* Netlist. It is read against the VPR architecture */
};

/********************************************************************
 * Stages of an OpenFPGA flow.
 * They form a tree, not a line:
 *
 *   UNINIT - DEVICE - PACKED - PLACED - ROUTED -+- ANALYZED
 *                                               |
 *                                               +- REPACKED - BITSTREAM
 *
 * The stage before another one in the tree is the one it requires
 *******************************************************************/
enum class OpenFpgaStage : int {
  UNINIT = 0, /* Nothing done yet */
  DEVICE,     /* Inputs read, device created */
  PACKED,
  PLACED,
  ROUTED,
  ANALYZED, /* Runs on a routed design */
  REPACKED, /* Runs on a routed design, without ANALYZED */
  BITSTREAM
};

/* The stage that a stage requires: its parent in the tree of stages. UNINIT
 * for UNINIT and DEVICE */
OpenFpgaStage required_stage(const OpenFpgaStage& stage);

/* Name of an input or a stage, to be used in messages */
std::string to_string(const OpenFpgaInput& input);
std::string to_string(const OpenFpgaStage& stage);

/********************************************************************
 * How build_bitstream makes the bitstream of a device: the defaults, or the
 * recipe that a plugin sets for the architectures it read
 *******************************************************************/
struct BitstreamRecipe {
  /* The device can make a bitstream */
  bool supported = true;
  /* Who set the recipe (e.g., the name of a device package), empty for the
   * defaults */
  std::string owner;
  std::vector<std::string> link_options = {"--sort_gsb_chan_node_in_edges"};
  /* Commands run after the link, e.g., check_netlist_naming_conflict */
  std::vector<std::string> fixups;
  std::vector<std::string> fabric_options = {"--compress_routing"};
};

/* The files that build_bitstream wrote, and the steps it ran and kept */
struct BitstreamOutput {
  std::string file;
  std::string format;
  std::string arch_file;
  std::vector<std::string> settings;
  std::vector<std::string> ran;
  std::vector<std::string> kept;
};

/********************************************************************
 * What the constraints of the session give to the commands of the core: the
 * synthesis, and the OpenFPGA commands that take a pin constraints file. A
 * plugin sets them, e.g., from a constraints file that it reads
 *******************************************************************/
struct SessionConstraints {
  /* The Yosys commands for the line ${CONSTRAINT_SYNTHESIS} of a script */
  std::string synthesis_script;
  /* The top module and the defines of the synthesis */
  std::string synthesis_top;
  std::vector<std::pair<std::string, std::string>> synthesis_defines;
  /* The pin constraints XML of the pins without a location, for the commands
   * that take --pin_constraints_file and are not given one */
  std::string pin_constraints_file;
  /* The file the constraints were read from, for messages */
  std::string source;
  /* The constraints give the SDC of VPR (filename.sdc_file), and the VPR
   * constraints (filename.read_vpr_constraints_file): another file is not
   * given for them (e.g., the SDC of a design directory) */
  bool timing = false;
  bool vpr_constraints = false;
};

/********************************************************************
 * The data of a flow that is run stage by stage, together with the
 * inputs that have been read and the stages whose results are valid.
 *
 * Rules, which are strict:
 * - Inputs: the circuit can only be read once the VPR architecture has
 *   been read. Reading an input again drops the results of all the
 *   stages. Reading the VPR architecture again also drops the circuit,
 *   which was read against the previous one
 * - DEVICE can only be run once all the inputs have been read
 * - Any other stage can only be run once the stage it requires is valid
 * - Running a stage invalidates the results of all the stages that
 *   depend on it, i.e., the ones after it in the tree. The stages on the
 *   other branches stay valid: re-running ANALYZED does not touch
 *   REPACKED. Re-running a stage that is valid is allowed
 * - When a stage fails, its results and the ones of the stages that
 *   depend on it are dropped. The stages it requires stay valid
 *
 * Notes:
 * - This class only tracks what has been done. It does not run VPR nor
 *   release the VPR data, e.g., with vpr_free_all(): the data is in the live
 *   run of VPR (see vpr_run.h)
 * - The application options are kept when the session is reset. Changing one
 *   drops the stage it tunes, and the ones that depend on it
 * - Commands such as link_openfpga_arch and build_fabric are prerequisites
 *   of REPACKED, but they are not stages. They are checked by the shell
 *   command dependencies
 * - VPR keeps most of its data in global contexts (g_vpr_ctx), which are
 *   shared by the whole process. Only one session can be live in a process
 * - The generation of the VPR run that the session has been working with
 *   is recorded, so that the session can tell that another command, e.g.,
 *   'vpr', has replaced the VPR data
 *******************************************************************/
class OpenFpgaSession {
 public: /* Public accessors */
  /* Return true if nothing has been read nor run */
  bool uninitialized() const;
  /* Return true if an input has been read */
  bool has_input(const OpenFpgaInput& input) const;
  /* Return true if all the inputs have been read */
  bool has_all_inputs() const;
  /* Return true if both architectures have been read: the device is made from
   * them, before the circuit */
  bool has_architectures() const;
  /* Return true if an input can be read now */
  bool can_read(const OpenFpgaInput& input) const;
  /* Return true if the results of a stage are valid. UNINIT is always valid */
  bool reached(const OpenFpgaStage& stage) const;
  /* Return true if a stage can be run now */
  bool can_run(const OpenFpgaStage& stage) const;
  /* All the stages that are valid, in the order of the enum, UNINIT excluded */
  std::vector<OpenFpgaStage> reached_stages() const;
  const AppOptions& app_options() const { return app_options_; }
  uint64_t vpr_generation() const { return vpr_generation_; }
  /* The generation of the VPR run that the architecture was linked to, see
   * set_linked_generation(). 0 if it has not been linked */
  uint64_t linked_generation() const { return linked_generation_; }
  /* The file that the results of a stage were read from, see
   * set_imported(). Empty if the stage was run, or is not valid */
  std::string imported_file(const OpenFpgaStage& stage) const;
  /* Why a stage failed, see fail(). Null if it did not, or if the failure was
   * forgotten */
  const std::string* failure(const OpenFpgaStage& stage) const;
  /* What a stage that was run printed, and the seconds it took. Null if the
   * stage was not run by a command of this session (e.g., its results were
   * read from a file), or was dropped, see set_stage_log() */
  const std::string* stage_log(const OpenFpgaStage& stage) const;
  /* A number that changes each time a stage completes, and only then: the
   * version of what was made from its data (e.g., what a plugin exports of
   * it). 0 if the stage is not reached */
  uint64_t stage_generation(const OpenFpgaStage& stage) const;
  const double* stage_seconds(const OpenFpgaStage& stage) const;
  /* Return true if the placer is the analytical placement of VPR (the option
   * place.placer): it makes the packing and the placement at once, and they
   * are one result. A change that drops the placement drops the packing */
  bool analytical_placer() const;
  /* The file that an input was read from, as it was given to read_input().
   * Empty if it is not known, or the input is not read */
  std::string input_file(const OpenFpgaInput& input) const;
  /* The data that a plugin keeps with the architectures of the session, as
   * JSON, under a key that starts with the name of the plugin (e.g.,
   * "weft.device_package"); "" if there is none. Reading an architecture,
   * failing to, and reset() clear them all. Checkpoints keep them, and
   * report_session lists them (data.attachments) */
  const std::string& attachment(const std::string& key) const;
  std::vector<std::string> attachment_keys() const;
  /* Why an option cannot be changed, or empty if it can: the channel width
   * (router.route_chan_width) is fixed while it has an owner (see
   * set_fixed_channel_width_owner()) */
  std::string fixed_option(const std::string& name) const;
  /* The files of the simulation settings ("simulation_setting") or of the
   * bitstream settings ("bitstream_setting") that were read, in order */
  std::vector<std::string> settings_files(const std::string& kind) const;
  /* The generation of the link that the fabric was built on (see
   * linked_generation()), 0 if it was not built or the link was dropped */
  uint64_t fabric_generation() const { return fabric_generation_; }
  /* True if the link is made on the data of VPR of the session, and the
   * fabric on that link */
  bool link_current() const;
  bool fabric_current() const;
  const BitstreamRecipe& bitstream_recipe() const { return bitstream_recipe_; }
  /* What build_bitstream wrote; null when BITSTREAM is not reached or was
   * made by the commands by hand */
  const BitstreamOutput* bitstream_output() const {
    return bitstream_output_.file.empty() ? nullptr : &bitstream_output_;
  }
  /* The constraints of the session, see SessionConstraints. They are kept
   * when the session is reset, as the options are */
  const SessionConstraints& constraints() const { return constraints_; }
  /* Who fixed the channel width, empty when it can be changed */
  const std::string& fixed_channel_width_owner() const {
    return fixed_channel_width_owner_;
  }
  /* The options that read cached files of the device, see
   * set_cache_options() */
  const std::vector<std::string>& cache_options() const {
    return cache_options_;
  }

 public: /* Public mutators */
  /* Mark an input as read. Return false, and leave the session untouched, if
   * the input cannot be read now */
  bool read_input(const OpenFpgaInput& input, const std::string& file = "");
  /* Mark an input as failed to read: it is dropped, with the inputs that
   * depend on it, and so are the results of all the stages */
  void fail_input(const OpenFpgaInput& input);
  /* Drop an input that is no longer good, e.g., the circuit when the
   * synthesis constraints change: as fail_input() */
  void drop_input(const OpenFpgaInput& input) { fail_input(input); }
  void set_constraints(const SessionConstraints& constraints) {
    constraints_ = constraints;
  }
  /* Mark a stage as done. Return false, and leave the session untouched, if
   * the stage cannot be run now */
  bool complete(const OpenFpgaStage& stage);
  /* Mark a stage as failed: it is dropped, with the stages that depend on it.
   * The message says why. The failure is remembered until the stage is run
   * again, or until it is dropped for another reason (an option or an input
   * changed), or the session is reset */
  void fail(const OpenFpgaStage& stage, const std::string& message = "");
  /* Forget the inputs and the stages. The VPR data is not released */
  void reset();
  /* Set an application option from text, see AppOptions::set(). If the value
   * changes, the stage that the option tunes and the ones that depend on it
   * are dropped, see app_option_stage(). Setting the value it already has
   * keeps the stages. Return false, and leave everything untouched, if the
   * option cannot be set, or is fixed (fixed_option()) and would change */
  bool set_app_option(const std::string& name, const std::string& text,
                      std::string& error);
  /* Give an option its default value back, with the same effect on the
   * stages. Return false if it does not exist, or is fixed (fixed_option())
   * and would change */
  bool reset_app_option(const std::string& name);
  /* Give all the options their default value back */
  void reset_app_options();
  /* Add the options that plugins registered since the session was made */
  void add_registered_app_options() { app_options_.add_new_defaults(); }
  /* Record that the results of a valid stage were read from a file. They are
   * read again when the stages before another stage are run again. Running
   * the stage, or dropping it, forgets the file */
  void set_imported(const OpenFpgaStage& stage, const std::string& file);
  /* Record what a stage printed, and the time it took, when the command that
   * ran it ends. They are forgotten with the stage */
  void set_stage_log(const OpenFpgaStage& stage, const std::string& log);
  void set_stage_seconds(const OpenFpgaStage& stage, const double seconds);
  void set_vpr_generation(uint64_t generation) { vpr_generation_ = generation; }
  /* Keep, or drop, the data of a plugin with the architectures, see
   * attachment() */
  void set_attachment(const std::string& key, const std::string& json);
  void clear_attachment(const std::string& key);
  /* The channel width that was just set belongs to an owner (a description
   * for messages, e.g., "the device package 'k6_2x2'"): it cannot be changed
   * (see fixed_option()) until an architecture is read. An empty owner lets
   * it be changed again */
  void set_fixed_channel_width_owner(const std::string& owner) {
    fixed_channel_width_owner_ = owner;
  }
  /* The options that were just set to read cached files of the device (e.g.,
   * the routing resource graph): reading an architecture gives them their
   * default value back, as the files are of that device only */
  void set_cache_options(const std::vector<std::string>& names) {
    cache_options_ = names;
  }
  /* Give the options of set_cache_options() their default back now */
  void reset_cache_options();
  /* Record the generation of the VPR run that the OpenFPGA architecture was
   * linked to, by link_openfpga_arch. What the link makes refers to the data
   * of that run, and it is not valid for another one */
  void set_linked_generation(uint64_t generation) {
    linked_generation_ = generation;
    fabric_generation_ = 0;
  }
  /* build_fabric was built on the current link */
  void set_fabric_built() { fabric_generation_ = linked_generation_; }
  /* Settings were read: the list of their files is replaced, or appended to;
   * the link and the fabric are dropped, and with the bitstream settings
   * REPACKED and BITSTREAM */
  void read_settings(const std::string& kind, const std::string& file,
                     const bool append);
  /* The recipe of the bitstream of the architectures that were just read,
   * set by a plugin. Reading an architecture gives the defaults back */
  void set_bitstream_recipe(const BitstreamRecipe& recipe) {
    bitstream_recipe_ = recipe;
  }
  void set_bitstream_output(const BitstreamOutput& output) {
    if (reached(OpenFpgaStage::BITSTREAM)) {
      bitstream_output_ = output;
    }
  }

 private: /* Internal functions */
  void invalidate_stage_and_dependents(const OpenFpgaStage& stage);
  void invalidate_all_stages();
  void invalidate_for_option(const std::string& name);
  /* An architecture was read, or failed to be: what came with the previous
   * one goes (the attachments, the owner of the channel width, the cache
   * options, the recipe of the bitstream) */
  void forget_architecture_state();
  /* The stage that is dropped to drop a stage: the packing with the
   * placement, when the analytical placer made them */
  OpenFpgaStage dropped_with(const OpenFpgaStage& stage) const;

 private: /* Internal data */
  /* One bit per input and per stage, from the value of their enum */
  unsigned inputs_ = 0;
  unsigned stages_ = 0;
  AppOptions app_options_;
  uint64_t vpr_generation_ = 0;
  uint64_t linked_generation_ = 0;
  uint64_t fabric_generation_ = 0;
  std::map<std::string, std::vector<std::string>> settings_files_;
  BitstreamRecipe bitstream_recipe_;
  BitstreamOutput bitstream_output_;
  std::map<OpenFpgaStage, std::string> imported_;
  std::map<OpenFpgaStage, std::string> failures_;
  std::map<OpenFpgaStage, std::string> stage_logs_;
  std::map<OpenFpgaStage, double> stage_seconds_;
  std::map<OpenFpgaInput, std::string> input_files_;
  std::map<OpenFpgaStage, uint64_t> stage_generations_;
  uint64_t completions_ = 0;
  std::map<std::string, std::string> attachments_;
  std::string fixed_channel_width_owner_;
  std::vector<std::string> cache_options_;
  SessionConstraints constraints_;
};

} /* End namespace openfpga */

#endif
