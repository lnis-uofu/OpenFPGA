/******************************************************************************
 * Inspired from https://github.com/genbtc/VerilogPCFparser
 ******************************************************************************/
#include <sstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
/* Headers from arch openfpga library */
#include "pcf_writer.h"
#include "write_xml_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Constants
 *************************************************/
constexpr const char* CMD_SET_IO = "set_io";

/********************************************************************
 * A writer to output a repack pin constraint object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are serious errors when parsing data
 * Return 2 if fail when opening files
 *******************************************************************/
int write_pcf(const char* fname, const PcfData& pcf_data) {
  vtr::ScopedStartFinishTimer timer("Write " + std::string(fname));

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Write from data */
  for (const PcfIoConstraintId& io_id : pcf_data.io_constraints()) {
    fp << CMD_SET_IO << " ";
    fp << pcf_data.io_net(io_id).c_str() << " ";
    fp << generate_xml_port_name(pcf_data.io_pin(io_id)).c_str() << "\n";
  }

  return 0;
}

} /* end namespace openfpga */
