/**
 * This is a wrapper file for Yosys executable.
 */

#include "yosys_main.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <string>
#include <vector>

#include "command_exit_codes.h"
#include "vtr_log.h"

namespace openfpga {

static std::string resolve_yosys_binary() {
  /* Prefer the in-tree build binary, and fall back to PATH lookup. */
  constexpr const char* kInTreeBinary = "./build/yosys/bin/yosys";
  if (0 == access(kInTreeBinary, X_OK)) {
    return std::string(kInTreeBinary);
  }
  return std::string("yosys");
}

static int run_yosys(int argc, char** argv) {
  std::string yosys_binary = resolve_yosys_binary();

  std::vector<char*> exec_argv;
  exec_argv.reserve(static_cast<size_t>(argc) + 2);
  exec_argv.push_back(const_cast<char*>(yosys_binary.c_str()));
  for (int iarg = 1; iarg < argc; ++iarg) {
    exec_argv.push_back(argv[iarg]);
  }
  exec_argv.push_back(nullptr);

  pid_t pid = fork();
  if (pid < 0) {
    VTR_LOG_ERROR("Failed to fork for Yosys execution: %s\n", strerror(errno));
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  if (0 == pid) {
    execvp(exec_argv[0], exec_argv.data());
    VTR_LOG_ERROR("Failed to execute Yosys binary '%s': %s\n", exec_argv[0],
                  strerror(errno));
    _exit(127);
  }

  int wait_status = 0;
  if (waitpid(pid, &wait_status, 0) < 0) {
    VTR_LOG_ERROR("Failed to wait for Yosys process: %s\n", strerror(errno));
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  if (WIFEXITED(wait_status) && (0 == WEXITSTATUS(wait_status))) {
    return openfpga::CMD_EXEC_SUCCESS;
  }

  if (WIFEXITED(wait_status)) {
    VTR_LOG_ERROR("Yosys exited with status code %d\n",
                  WEXITSTATUS(wait_status));
  } else if (WIFSIGNALED(wait_status)) {
    VTR_LOG_ERROR("Yosys terminated by signal %d\n", WTERMSIG(wait_status));
  } else {
    VTR_LOG_ERROR("Yosys terminated unexpectedly\n");
  }

  return openfpga::CMD_EXEC_FATAL_ERROR;
}

int yosys_wrapper(int argc, char** argv) { return run_yosys(argc, argv); }

} /* End namespace openfpga */
