#include "read_mif.h"

#include "mif_openfpga_format.h"

namespace openfpga {

int read_mif(const std::string& file_path, MifStorage& mif_storage) {
  return read_init_hex(file_path, mif_storage);
}

} /* namespace openfpga */
