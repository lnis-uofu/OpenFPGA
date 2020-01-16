/********************************************************************
 * This file includes functions that handles the file outputting
 * in OpenFPGA framework
 *******************************************************************/
/* Headers from vtr util library */
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h" 

namespace openfpga {

/********************************************************************
 * A most utilized function to validate the file stream
 * This function will return true or false for a valid/invalid file stream 
 *******************************************************************/
bool valid_file_stream(std::fstream& fp) {
  /* Validate the file stream */
  if (!fp.is_open() || !fp.good()) {
    return false;
  }

  return true;
}

/********************************************************************
 * A most utilized function to validate the file stream
 * This function will error out for a valid/invalid file stream 
 *******************************************************************/
void check_file_stream(const char* fname, 
                       std::fstream& fp) {

  if (false == valid_file_stream(fp)) {
    VTR_LOG("Invalid file stream for file: %s\n",
            fname);
    exit(1);
  }
}

} /* namespace openfpga ends */
