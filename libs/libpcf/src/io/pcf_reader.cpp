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
#include "pcf_reader.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Constants
 *************************************************/
constexpr const char COMMENT = '#';

/********************************************************************
 * A writer to output a repack pin constraint object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are serious errors when parsing data
 * Return 2 if fail when opening files
 *******************************************************************/
int read_pcf(const char* fname, PcfData& pcf_data,
             bool reduce_error_to_warning) {
  vtr::ScopedStartFinishTimer timer("Read " + std::string(fname));

  /* Create a file handler */
  std::ifstream fp(fname);
  if (!fp.is_open()) {
    VTR_LOG_ERROR("Fail to open pcf file '%s'!", fname);
    return 2;
  }

  int num_err = 0;

  /* Get line by line */
  std::string line;
  while (std::getline(fp, line)) {
    std::stringstream ss(line);
    while (ss) {
      std::string word;
      /* TODO: Use command parser */
      if (ss >> word) {
        if (word.find("set_io") == 0) {
          std::string net_name;
          std::string pin_name;
          ss >> net_name >> pin_name;
          /* Decode data */
          PcfIoConstraintId io_id = pcf_data.create_io_constraint();
          pcf_data.set_io_net(io_id, net_name);
          pcf_data.set_io_pin(io_id, pin_name);
        } else if (word[0] == COMMENT) {  // if it's a comment
          break;  // or ignore the full line comment and move on
        } else {
          if (reduce_error_to_warning) {
            VTR_LOG_WARN("Bypass unknown command '%s' !\n", word.c_str());
            break;
          } else {
            /* Reach unknown command for OpenFpga, error out */
            VTR_LOG_ERROR("Unknown command '%s'!\n", word.c_str());
            num_err++;
            break;  // and move onto next line. without this, it will accept
                    // more following values on this line
          }
        }
      }
    }
  }

  if (num_err) {
    return 1;
  }
  return 0;
}

} /* end namespace openfpga */
