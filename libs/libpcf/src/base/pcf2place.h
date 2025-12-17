#ifndef PCF2PLACE_H
#define PCF2PLACE_H
/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>
#include <vector>

#include "bitstream_setting.h"
#include "io_location_map.h"
#include "io_net_place.h"
#include "io_pin_table.h"
#include "pcf_data_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Generate a .place file with the following input files
 * - A Pin Constraint File (.pcf)
 * - Input and output lists from a netlist
 * - A chip I/O pin table file (.csv)
 * - An FPGA I/O location file (.xml)
 */
int pcf2place(const PcfData& pcf_data,
              const std::vector<std::string>& input_nets,
              const std::vector<std::string>& output_nets,
              const IoPinTable& io_pin_table,
              const IoLocationMap& io_location_map, IoNetPlace& io_net_place);
int pcf2bitstream_setting(const PcfData& pcf_data,
                          BitstreamSetting& bitstream_setting);

} /* End namespace openfpga*/

#endif
