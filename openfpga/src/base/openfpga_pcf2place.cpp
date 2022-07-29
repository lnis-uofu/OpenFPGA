/********************************************************************
 * This file includes functions to build bitstream database
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from pcf library */
#include "pcf_reader.h"
#include "blif_head_reader.h"
#include "read_csv_io_pin_table.h"
#include "read_xml_io_location_map.h"
#include "io_net_place.h"
#include "pcf2place.h"

#include "openfpga_pcf2place.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to convert a .pcf file to a .place file which 
 * which VPR can force I/O placement
 *******************************************************************/
int pcf2place_wrapper(const OpenfpgaContext& openfpga_context,
                      const Command& cmd, const CommandContext& cmd_context) {

  /* todo: create a factory to produce this in the future*/
  CommandOptionId opt_pcf = cmd.option("pcf");
  CommandOptionId opt_blif = cmd.option("blif");
  CommandOptionId opt_fpga_io_map = cmd.option("fpga_io_map");
  CommandOptionId opt_pin_table = cmd.option("pin_table");
  CommandOptionId opt_fpga_fix_pins = cmd.option("fpga_fix_pins");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_verbose = cmd.option("verbose");

  std::string pcf_fname = cmd_context.option_value(cmd, opt_pcf);
  std::string blif_fname = cmd_context.option_value(cmd, opt_blif);
  std::string fpga_io_map_fname = cmd_context.option_value(cmd, opt_fpga_io_map);
  std::string pin_table_fname = cmd_context.option_value(cmd, opt_pin_table);
  std::string fpga_fix_pins_fname = cmd_context.option_value(cmd, opt_fpga_fix_pins);

  /* Parse the input files */
  openfpga::PcfData pcf_data;
  openfpga::read_pcf(pcf_fname.c_str(), pcf_data);
  VTR_LOG("Read the design constraints from a pcf file: %s.\n",
          pcf_fname.c_str());

  blifparse::BlifHeadReader callback;
  blifparse::blif_parse_filename(blif_fname.c_str(), callback);
  VTR_LOG("Read the blif from a file: %s.\n",
          blif_fname.c_str());
  if (callback.had_error()) {
    VTR_LOG_ERROR("Read the blif ends with errors\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  IoLocationMap io_location_map = read_xml_io_location_map(fpga_io_map_fname.c_str());
  VTR_LOG("Read the I/O location map from an XML file: %s.\n",
          fpga_io_map_fname.c_str());

  IoPinTable io_pin_table = read_csv_io_pin_table(pin_table_fname.c_str());
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n",
          pin_table_fname.c_str());

  /* Convert */
  IoNetPlace io_net_place;
  int status = pcf2place(pcf_data, callback.input_pins(), callback.output_pins(), io_pin_table, io_location_map, io_net_place);
  if (status) {
    return status;
  }

  /* Output */
  status = io_net_place.write_to_place_file(fpga_fix_pins_fname.c_str(),
                                            cmd_context.option_enable(cmd, opt_no_time_stamp),
                                            cmd_context.option_enable(cmd, opt_verbose));
  if (status) {
    return status;
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
