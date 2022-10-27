#ifndef BLIF_HEAD_READER
#define BLIF_HEAD_READER
#include <cstdio>

#include "blifparse.hpp"
#include "vtr_log.h"

namespace blifparse {

// An example callback which pretty-prints to stdout
// the BLIF which is being parsed
class BlifHeadReader : public Callback {
 public:
  void start_parse() override;
  void filename(std::string fname) override;
  void lineno(int line_num) override;
  void begin_model(std::string model_name) override;
  void inputs(std::vector<std::string> inputs) override;
  void outputs(std::vector<std::string> outputs) override;

  void names(std::vector<std::string> nets,
             std::vector<std::vector<LogicValue>> so_cover) override;

  void latch(std::string input, std::string output, LatchType type,
             std::string control, LogicValue init) override;

  void subckt(std::string model, std::vector<std::string> ports,
              std::vector<std::string> nets) override;

  void blackbox() override;

  void end_model() override;

  // BLIF Extensions
  void conn(std::string src, std::string dst) override;
  void cname(std::string cell_name) override;
  void attr(std::string name, std::string value) override;
  void param(std::string name, std::string value) override;

  void finish_parse() override;

  void parse_error(const int curr_lineno, const std::string& near_text,
                   const std::string& msg) override {
    VTR_LOG_ERROR("Error when parsing .blif at line %d near '%s': %s\n",
                  curr_lineno, near_text.c_str(), msg.c_str());
    had_error_ = true;
  }

  bool had_error() { return had_error_; }
  std::vector<std::string> input_pins() { return input_pins_; }
  std::vector<std::string> output_pins() { return output_pins_; }

 private:
  std::vector<std::string> input_pins_;
  std::vector<std::string> output_pins_;
  bool had_error_ = false;
};

}  // namespace blifparse
#endif
