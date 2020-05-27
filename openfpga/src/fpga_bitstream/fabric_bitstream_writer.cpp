/********************************************************************
 * This file includes functions that output a fabric-dependent 
 * bitstream database to files in different formats
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_naming.h"

#include "bitstream_manager_utils.h"
#include "fabric_bitstream_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Write the fabric bitstream to a plain text file 
 * Notes: 
 *   - This is the final bitstream which is loadable to the FPGA fabric
 *     (Verilog netlists etc.)
 *   - Do NOT include any comments or other characters that the 0|1 bitstream content
 *     in this file
 *******************************************************************/
void write_fabric_bitstream_to_text_file(const BitstreamManager& bitstream_manager,
                                         const FabricBitstream& fabric_bitstream,
                                         const std::string& fname) {
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR("Received empty file name to output bitstream!\n\tPlease specify a valid file name.\n");
  }

  std::string timer_message = std::string("Write ") + std::to_string(fabric_bitstream.bits().size()) + std::string(" fabric bitstream into plain text file '") + fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(fname.c_str(), fp);

  /* Put down pure 0|1 bitstream here */
  for (const FabricBitId& fabric_bit : fabric_bitstream.bits()) {
    fp << bitstream_manager.bit_value(fabric_bitstream.config_bit(fabric_bit));
  }
  /* Print an end to the file here */
  fp << std::endl;

  /* Close file handler */
  fp.close();
}

} /* end namespace openfpga */
