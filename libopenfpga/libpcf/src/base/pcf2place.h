#ifndef PCF2PLACE_H
#define PCF2PLACE_H
/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include "pcf_data.h"
#include "blif_head_reader.h"
#include "io_pin_table.h"
#include "io_location_map.h"
#include "io_net_place.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Generate a .place file with the following input files
 * - A Pin Constraint File (.pcf)
 * - A netlist (.blif)
 * - A chip I/O pin table file (.csv)
 * - An FPGA I/O location file (.xml)
 */
int pcf2place(const PcfData& pcf_data,
              const blifparse::BlifHeadReader& blif_head,
              const IoPinTable& io_pin_table,
              const IoLocationMap& io_location_map,
              IoNetPlace& io_net_place);

} /* End namespace openfpga*/

#endif
