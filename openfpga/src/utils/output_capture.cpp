#include "output_capture.h"

#include <iostream>

#ifndef _WIN32
#include <unistd.h>
#endif

namespace openfpga {

#ifndef _WIN32

/* Push out everything buffered by the C and C++ streams, so that text is
 * attributed to the right side of a redirection */
static void flush_all_streams() {
  std::cout.flush();
  std::cerr.flush();
  std::fflush(nullptr);
}

OutputCapture::OutputCapture() {
  flush_all_streams();

  storage_ = std::tmpfile();
  if (nullptr == storage_) {
    return;
  }

  saved_stdout_ = ::dup(STDOUT_FILENO);
  saved_stderr_ = ::dup(STDERR_FILENO);
  if (saved_stdout_ < 0 || saved_stderr_ < 0 ||
      ::dup2(::fileno(storage_), STDOUT_FILENO) < 0 ||
      ::dup2(::fileno(storage_), STDERR_FILENO) < 0) {
    /* Fail to redirect: undo whatever has been done and stay inactive */
    if (saved_stdout_ >= 0) {
      ::dup2(saved_stdout_, STDOUT_FILENO);
      ::close(saved_stdout_);
    }
    if (saved_stderr_ >= 0) {
      ::dup2(saved_stderr_, STDERR_FILENO);
      ::close(saved_stderr_);
    }
    saved_stdout_ = -1;
    saved_stderr_ = -1;
    std::fclose(storage_);
    storage_ = nullptr;
  }
}

OutputCapture::~OutputCapture() { stop(); }

bool OutputCapture::active() const { return nullptr != storage_; }

std::string OutputCapture::stop() {
  if (!active()) {
    return std::string();
  }

  flush_all_streams();

  /* Give the streams back first, so that nothing else is lost in the file */
  ::dup2(saved_stdout_, STDOUT_FILENO);
  ::dup2(saved_stderr_, STDERR_FILENO);
  ::close(saved_stdout_);
  ::close(saved_stderr_);
  saved_stdout_ = -1;
  saved_stderr_ = -1;

  std::string captured;
  std::rewind(storage_);
  char buffer[4096];
  std::size_t num_read = 0;
  while ((num_read = std::fread(buffer, 1, sizeof(buffer), storage_)) > 0) {
    captured.append(buffer, num_read);
  }
  std::fclose(storage_);
  storage_ = nullptr;

  return captured;
}

#else /* Windows: capture is not supported */

OutputCapture::OutputCapture() {}
OutputCapture::~OutputCapture() {}
bool OutputCapture::active() const { return false; }
std::string OutputCapture::stop() { return std::string(); }

#endif

} /* End namespace openfpga */
