#include "openfpga_mif_bitstream.h"

#include "aggregate_mif.h"
#include "mif_vpr_placement.h"
#include "read_mif.h"

namespace openfpga {

int aggregate_mif_storage(MifStorage& mif_storage,
                          const BitstreamSetting& bitstream_setting,
                          MifStorage& aggregated_mif_storage) {
  return aggregate_mif(mif_storage, bitstream_setting, aggregated_mif_storage,
                       [](const std::string& model_name,
                          const MifEblifPortConnections& port_connections) {
                         return get_mif_pb_type_from_vpr(model_name,
                                                         port_connections);
                       });
}

} /* namespace openfpga */
