#ifndef OPENFPGA_SESSION_H
#define OPENFPGA_SESSION_H

#include <cstdint>
#include <map>
#include <string>
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

 public: /* Public mutators */
  /* Mark an input as read. Return false, and leave the session untouched, if
   * the input cannot be read now */
  bool read_input(const OpenFpgaInput& input);
  /* Mark an input as failed to read: it is dropped, with the inputs that
   * depend on it, and so are the results of all the stages */
  void fail_input(const OpenFpgaInput& input);
  /* Mark a stage as done. Return false, and leave the session untouched, if
   * the stage cannot be run now */
  bool complete(const OpenFpgaStage& stage);
  /* Mark a stage as failed: it is dropped, with the stages that depend on it */
  void fail(const OpenFpgaStage& stage);
  /* Forget the inputs and the stages. The VPR data is not released */
  void reset();
  /* Set an application option from text, see AppOptions::set(). If the value
   * changes, the stage that the option tunes and the ones that depend on it
   * are dropped, see app_option_stage(). Setting the value it already has
   * keeps the stages. Return false, and leave everything untouched, if the
   * option cannot be set */
  bool set_app_option(const std::string& name, const std::string& text,
                      std::string& error);
  /* Give an option its default value back, with the same effect on the
   * stages. Return false if it does not exist */
  bool reset_app_option(const std::string& name);
  /* Give all the options their default value back */
  void reset_app_options();
  /* Record that the results of a valid stage were read from a file. They are
   * read again when the stages before another stage are run again. Running
   * the stage, or dropping it, forgets the file */
  void set_imported(const OpenFpgaStage& stage, const std::string& file);
  void set_vpr_generation(uint64_t generation) { vpr_generation_ = generation; }
  /* Record the generation of the VPR run that the OpenFPGA architecture was
   * linked to, by link_openfpga_arch. What the link makes refers to the data
   * of that run, and it is not valid for another one */
  void set_linked_generation(uint64_t generation) {
    linked_generation_ = generation;
  }

 private: /* Internal functions */
  void invalidate_stage_and_dependents(const OpenFpgaStage& stage);
  void invalidate_all_stages();
  void invalidate_for_option(const std::string& name);

 private: /* Internal data */
  /* One bit per input and per stage, from the value of their enum */
  unsigned inputs_ = 0;
  unsigned stages_ = 0;
  AppOptions app_options_;
  uint64_t vpr_generation_ = 0;
  uint64_t linked_generation_ = 0;
  std::map<OpenFpgaStage, std::string> imported_;
};

} /* End namespace openfpga */

#endif
