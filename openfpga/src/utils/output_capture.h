#ifndef OUTPUT_CAPTURE_H
#define OUTPUT_CAPTURE_H

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

#endif
