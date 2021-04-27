/********************************************************************
 * This file includes functions that output io mapping information 
 * to files in XML format
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

/* Headers from archopenfpga library */
#include "openfpga_naming.h"

#include "openfpga_version.h"

#include "build_io_mapping_info.h"
#include "write_xml_io_mapping.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information to an I/O mapping file
 *******************************************************************/
static 
void write_io_mapping_xml_file_head(std::fstream& fp) {
  valid_file_stream(fp);
 
  auto end = std::chrono::system_clock::now(); 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  fp << "<!--" << std::endl;
  fp << "\t- I/O mapping" << std::endl;
  fp << "\t- Version: " << openfpga::VERSION << std::endl;
  fp << "\t- Date: " << std::ctime(&end_time) ;
  fp << "-->" << std::endl;
  fp << std::endl;
}

/********************************************************************
 * Write an io mapping pair to an XML file 
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static 
int write_io_mapping_pair_to_xml_file(std::fstream& fp,
                                      const IoMap& io_map,
                                      const IoMapId& io_map_id,
                                      int xml_hierarchy_depth) {
  if (false == valid_file_stream(fp)) {
    return 1;
  }

  write_tab_to_file(fp, xml_hierarchy_depth);

  BasicPort io_port = io_map.io_port(io_map_id);
  fp << "<io ";
  fp << "name=\"" << io_port.get_name().c_str() << "[" << io_port.get_lsb() << ":" << io_port.get_msb() << "]" << "\"";
   
  VTR_ASSERT(1 == io_map.io_net(io_map_id).get_width());
  fp << "net=\"" << io_map.io_net(io_map_id).get_name().c_str() << "\"";

  if (io_map.is_io_input(io_map_id)) {
    fp << "dir=\"" << "input" << "\"";
  } else {
    VTR_ASSERT_SAFE(io_map.is_io_output(io_map_id));
    fp << "dir=\"" << "output" << "\"";
  }

  fp << "/>\n";

  return 0;
}

/********************************************************************
 * Write the io mapping information to an XML file 
 * Notes: 
 *   - This file is designed for users to learn 
 *     - what nets are mapped to each I/O is mapped, io[0] -> netA
 *     - what directionality is applied to each I/O, io[0] -> input
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
int write_io_mapping_to_xml_file(const IoMap& io_map,
                                 const std::string& fname,
                                 const bool& verbose) {
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR("Received empty file name to output io_mapping!\n\tPlease specify a valid file name.\n");
    return 1;
  }

  std::string timer_message = std::string("Write I/O mapping into xml file '") + fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(fname.c_str(), fp);

  /* Write XML head */
  write_io_mapping_xml_file_head(fp);

  int xml_hierarchy_depth = 0;
  fp << "<io_mapping>\n";

  /* Output fabric bitstream to the file */
  int status = 0;
  int io_map_cnt = 0;
  for (const auto& io_map_id : io_map.io_map()) {
    status = write_io_mapping_pair_to_xml_file(fp,
                                               io_map, io_map_id,
                                               xml_hierarchy_depth + 1);
    io_map_cnt++;
    if (1 == status) {
      break;
    }
  }

  /* Print an end to the file here */
  fp << "</io_mapping>\n";

  VTR_LOGV(verbose,
           "Outputted %d I/O mapping to file '%s'\n",
           io_map,
           fname.c_str());

  /* Close file handler */
  fp.close();

  return status;
}

} /* end namespace openfpga */
