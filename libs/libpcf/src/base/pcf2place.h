#ifndef PCF2PLACE_H
#define PCF2PLACE_H
/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>
#include <vector>

#include "bitstream_setting.h"
#include "boundary_timing.h"
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
                          BitstreamSetting& bitstream_setting,
                          const IoPinTable& io_pin_table,
                          const bool& verbose = false);
int pcf2sdc_from_boundary_timing(const PcfData& pcf_data,
                                 BoundaryTiming& boundary_timing,
                                 const IoPinTable& io_pin_table,
                                 const std::string& clock_name,
                                 const double& clock_period, std::ostream& ofs,
                                 const bool& verbose);
} /* End namespace openfpga*/

#endif
