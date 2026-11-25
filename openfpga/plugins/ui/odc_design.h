#ifndef ODC_DESIGN_H
#define ODC_DESIGN_H
/********************************************************************
 * What ODC (libodc) needs to know of the flow of a session, from the data of
 * VPR and the device package: the grid of the device and the blocks each of
 * its tiles can hold, its clock networks, the pins of its I/O map, the
 * netlist of the circuit.
 *******************************************************************/
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "odc.h"
#include "openfpga_session.h"

/* begin namespace openfpga */
namespace openfpga {

/* What is known: each part is null while its input is not read */
struct OdcDesignData {
  std::shared_ptr<odc::Netlist> netlist;
  std::shared_ptr<odc::Device> device;
  std::shared_ptr<std::vector<odc::Site>> sites;
  bool io_map = false;
  /* The I/O map and the default constraints of the device package, or
   * empty */
  std::string io_map_file;
  std::string defaults_file;
  /* Why a part could not be read, for the report */
  std::vector<std::string> problems;

  odc::Design design() const;
};

/* Read what is known of a session. netlist_file replaces the circuit of the
 * session, for the circuit about to be read; empty for the session's own */
OdcDesignData odc_design_data(const OpenFpgaSession& session,
                              const std::string& netlist_file = "");

} /* end namespace openfpga */

#endif
