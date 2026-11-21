#ifdef OPENFPGA_INCLUDE_YOSYS_COMMAND

#include "yosys_main.h"

#if defined(__unix__) || defined(__APPLE__)
#include <dlfcn.h>
#endif
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "command_exit_codes.h"
#include "kernel/yosys.h"
#include "vtr_log.h"

namespace openfpga {

/* What Yosys reports through log_error(), which would otherwise end the whole
 * process with exit(1): the message that Yosys keeps of its last error */
struct YosysFailure : public std::runtime_error {
  explicit YosysFailure(const std::string& what) : std::runtime_error(what) {}
};

/* The folder of the file that holds this code: the openfpga executable, or the
 * Python module that was loaded. Yosys looks for its share/ folder and for
 * yosys-abc next to the running executable, which is python for the module */
static std::string folder_of_this_code() {
#if defined(__unix__) || defined(__APPLE__)
  Dl_info info;
  if (dladdr(reinterpret_cast<void*>(&folder_of_this_code), &info) == 0 ||
      info.dli_fname == nullptr) {
    return std::string();
  }
  std::string file(info.dli_fname);
  size_t slash = file.rfind('/');
  return slash == std::string::npos ? std::string(".") : file.substr(0, slash);
#else
  /* No dladdr: Yosys keeps looking next to the executable */
  return std::string();
#endif
}

/* Where Yosys finds what it needs when it is not in an executable of its own:
 * share/ and yosys-abc in the folder of this code (OpenFPGA's build links them
 * there). What Yosys found by itself (the environment variable ABC, its own
 * folders) is kept */
static void locate_yosys_files() {
  namespace fs = std::filesystem;
  std::string folder = folder_of_this_code();
  if (folder.empty()) {
    return;
  }
  std::error_code ec;
  if (!fs::is_directory(Yosys::yosys_share_dirname, ec) &&
      fs::is_directory(fs::path(folder) / "share", ec)) {
    Yosys::yosys_share_dirname = folder + "/share/";
  }
  if (!fs::is_regular_file(Yosys::yosys_abc_executable, ec) &&
      fs::is_regular_file(fs::path(folder) / "yosys-abc", ec)) {
    Yosys::yosys_abc_executable = folder + "/yosys-abc";
  }
}

static void throw_yosys_failure() { throw YosysFailure(Yosys::log_last_error); }

/* Yosys is set up once for the life of the process. It cannot be set up
 * again after yosys_shutdown(): its passes are not registered a second time.
 * The process ends and Yosys goes with it */
static void start_yosys_once() {
  static bool started = false;
  if (started) {
    return;
  }
  Yosys::yosys_setup();
  locate_yosys_files();
  Yosys::log_errfile = stderr;
  Yosys::log_streams.push_back(&std::cout);
  /* An error of a command (log_cmd_error) is an exception ... */
  Yosys::log_cmd_error_throw = true;
  /* ... and so is any other error (log_error): this hook runs before exit(1) */
  Yosys::log_error_atexit = throw_yosys_failure;
  started = true;
}

int yosys_script_mode_wrapper(const std::string& script_path) {
  start_yosys_once();
  /* A design of its own for each script: what an earlier script left is gone */
  std::unique_ptr<Yosys::RTLIL::Design> design(new Yosys::RTLIL::Design);
  try {
    VTR_LOG("Processing yosys script file: %s\n", script_path.c_str());
    Yosys::run_frontend(script_path, "script", design.get());
    design->check();
    VTR_LOG("Yosys script execution completed successfully.\n");
    return CMD_EXEC_SUCCESS;
  } catch (const YosysFailure& e) {
    VTR_LOG_ERROR("Yosys failed: %s", e.what());
  } catch (const Yosys::log_cmd_error_exception&) {
    VTR_LOG_ERROR("Yosys failed: %s", Yosys::log_last_error.c_str());
  } catch (const std::exception& e) {
    VTR_LOG_ERROR("Error during Yosys execution: %s\n", e.what());
  } catch (...) {
    VTR_LOG_ERROR("An unhandled Yosys exception occurred.\n");
  }
  /* The way that the Yosys shell recovers from an error of a command */
  while (design->selection_stack.size() > 1) {
    design->pop_selection();
  }
  Yosys::log_reset_stack();
  return CMD_EXEC_FATAL_ERROR;
}

}  // namespace openfpga

#endif
