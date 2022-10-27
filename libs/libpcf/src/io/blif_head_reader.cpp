#include "blif_head_reader.h"

#include <cassert>
#include <cstdio>

namespace blifparse {

void BlifHeadReader::start_parse() {
  // Pass
}

void BlifHeadReader::finish_parse() {
  // Pass
}

void BlifHeadReader::begin_model(std::string model_name) {
  // Pass
}

void BlifHeadReader::inputs(std::vector<std::string> input_conns) {
  input_pins_ = input_conns;
}

void BlifHeadReader::outputs(std::vector<std::string> output_conns) {
  output_pins_ = output_conns;
}

void BlifHeadReader::names(std::vector<std::string> nets,
                           std::vector<std::vector<LogicValue>> so_cover) {
  // Pass
}

void BlifHeadReader::latch(std::string input, std::string output,
                           LatchType type, std::string control,
                           LogicValue init) {
  // Pass
}

void BlifHeadReader::subckt(std::string model, std::vector<std::string> ports,
                            std::vector<std::string> nets) {
  // Pass
}

void BlifHeadReader::blackbox() {
  // Pass
}

void BlifHeadReader::end_model() {
  // Pass
}

void BlifHeadReader::conn(std::string src, std::string dst) {
  // Pass
}

void BlifHeadReader::cname(std::string cell_name) {
  // Pass
}

void BlifHeadReader::attr(std::string name, std::string value) {
  // Pass
}

void BlifHeadReader::param(std::string name, std::string value) {
  // Pass
}

void BlifHeadReader::filename(std::string fname) {
  // Pass
}

void BlifHeadReader::lineno(int line_num) {
  // Pass
}

}  // namespace blifparse
