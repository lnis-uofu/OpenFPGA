#pragma once

#include <cstdio>
#include <string>

namespace openfpga {

/********************************************************************
 * Capture everything written to stdout and stderr (file descriptors
 * 1 and 2) while the object is active.
 * The capture happens at file-descriptor level, so it includes output
 * from VTR_LOG, printf, std::cout and std::cerr. Both streams are written
 * to the same buffer.
 * Notes:
 * - stdout is fully buffered when redirected, while stderr is not. Text on
 *   stderr may therefore appear ahead of earlier text on stdout. Use the
 *   text of the messages, not their position, to tell them apart.
 * - Capture is process-wide and not thread-safe. Only one capture may
 *   be active at a time.
 * - Capture is not supported on Windows. The object is then inactive
 *   and stop() returns an empty string.
 * - The text is held until stop(): nothing reaches the terminal while the
 *   capture is active. Print the text again if it should still be seen.
 *
 * Example: run one command of the shell and get its output as text, as
 * OpenfpgaShell::execute() does for the Python binding
 *
 *   OutputCapture capture;
 *   const int status = shell.execute_command(line.c_str(), openfpga_ctx);
 *   const std::string log = capture.stop();
 *
 * Example: an option '--log <file>' of the shell, which writes the output of
 * each command to a file and still shows it on the terminal
 *
 *   std::ofstream log_file(log_path, std::ios::app);
 *   for (const std::string& line : commands) {
 *     OutputCapture capture;
 *     const int status = shell.execute_command(line.c_str(), openfpga_ctx);
 *     const std::string text = capture.stop();
 *     log_file << text;
 *     std::fwrite(text.data(), 1, text.size(), stdout);
 *     if (CMD_EXEC_SUCCESS != status) break;
 *   }
 *
 * The output of a command shows when the command ends, not while it runs.
 * For a log that follows a long command as it runs, vtr::set_log_file()
 * copies what VTR_LOG writes to a file (not printf, std::cout or the
 * output of other libraries).
 *******************************************************************/
class OutputCapture {
 public: /* Constructors */
  /* Start capturing right away */
  OutputCapture();
  /* Restore the streams if stop() was not called, e.g., on exceptions */
  ~OutputCapture();
  OutputCapture(const OutputCapture&) = delete;
  OutputCapture& operator=(const OutputCapture&) = delete;

 public: /* Accessors */
  /* Return true if the streams are currently being captured */
  bool active() const;

 public: /* Mutators */
  /* Restore stdout and stderr, and return all the text captured. Return an
   * empty string if the capture is not active */
  std::string stop();

 private: /* Internal data */
  int saved_stdout_ = -1;
  int saved_stderr_ = -1;
  std::FILE* storage_ = nullptr;
};

} /* End namespace openfpga */
