/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
#include <fstream>
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "openfpga_digest.h"
#include "openfpga_gz_xml_writer.h"
#include "read_xml_arch_bitstream.h"
#include "report_arch_bitstream_distribution.h"
#include "write_xml_arch_bitstream.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two or 3 argument */
  VTR_ASSERT((2 == argc) || (3 == argc) || (4 == argc) || (5 == argc));

  /* Parse the bitstream from an XML file */
  openfpga::BitstreamManager test_bitstream =
    openfpga::read_xml_architecture_bitstream(argv[1]);
  VTR_LOG("Read the bitstream from an XML file: %s.\n", argv[1]);

  /* Output the bitstream database to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) {
    openfpga::write_xml_architecture_bitstream(test_bitstream, argv[2], true);
    VTR_LOG("Echo the bitstream (with time stamp) to an XML file: %s.\n",
            argv[2]);
    openfpga::write_xml_architecture_bitstream(test_bitstream, argv[2], false);
    VTR_LOG("Echo the bitstream (w/o time stamp) to an XML file: %s.\n",
            argv[2]);
  }
  /* Output the bitstream distribution to an XML file
   * This is optional only used when there is a third argument
   */
  if (4 <= argc) {
    std::string fname(argv[3]);
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("bitstream_distribution");

    openfpga::report_architecture_bitstream_distribution(root, test_bitstream,
                                                         1, 0);
    VTR_LOG("Echo the bitstream distribution to an XML file: %s.\n", argv[3]);
    /* Output to xml file */
    bool output_success = false;
    if (openfpga::file_require_gz(fname)) {
      openfpga::GzXmlWriter writer(fname.c_str());
      if (writer.isValid()) {
        doc.save(writer);
        output_success = true;
      }
    } else {
      output_success = doc.save_file(fname.c_str());
    }
    if (output_success) {
      VTR_LOG("Succeed to output XML file: %s\n", fname.c_str());
    } else {
      VTR_LOG_ERROR("Failed to output XML file: %s\n", fname.c_str());
    }
    return output_success ? 0 : 1;
  }
}
