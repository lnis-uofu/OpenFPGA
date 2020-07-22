/******************************************************************************
 * Memember functions for data structure FabricSpiceOption
 ******************************************************************************/
#include "vtr_assert.h"

#include "fabric_spice_options.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
FabricSpiceOption::FabricSpiceOption() {
  output_directory_.clear();
  explicit_port_mapping_ = false;
  compress_routing_ = false;
  verbose_output_ = false;
}

/**************************************************
 * Public Accessors 
 *************************************************/
std::string FabricSpiceOption::output_directory() const {
  return output_directory_;
}

bool FabricSpiceOption::explicit_port_mapping() const {
  return explicit_port_mapping_;
}

bool FabricSpiceOption::compress_routing() const {
  return compress_routing_;
}

bool FabricSpiceOption::verbose_output() const {
  return verbose_output_;
}

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
void FabricSpiceOption::set_output_directory(const std::string& output_dir) {
  output_directory_ = output_dir;
}

void FabricSpiceOption::set_explicit_port_mapping(const bool& enabled) {
  explicit_port_mapping_ = enabled;
}

void FabricSpiceOption::set_compress_routing(const bool& enabled) {
  compress_routing_ = enabled;
}

void FabricSpiceOption::set_verbose_output(const bool& enabled) {
  verbose_output_ = enabled;
}

} /* end namespace openfpga */
