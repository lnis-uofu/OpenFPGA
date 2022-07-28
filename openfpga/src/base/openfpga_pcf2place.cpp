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

  /* Parse the input files */
  openfpga::PcfData pcf_data;
  openfpga::read_pcf(pcf_fname, pcf_data);
  VTR_LOG("Read the design constraints from a pcf file: %s.\n",
          pcf_fname.c_str());

  blifparse::BlifHeadReader callback;
  blifparse::blif_parse_filename(argv[2], callback);
  VTR_LOG("Read the blif from a file: %s.\n",
          argv[2]);
  if (callback.had_error()) {
    VTR_LOG("Read the blif ends with errors\n",
            argv[2]);
    return 1;
  }

  openfpga::IoLocationMap io_location_map = openfpga::read_xml_io_location_map(argv[3]);
  VTR_LOG("Read the I/O location map from an XML file: %s.\n",
          argv[3]);

  openfpga::IoPinTable io_pin_table = openfpga::read_csv_io_pin_table(argv[4]);
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n",
          argv[4]);

  /* Convert */
  openfpga::IoNetPlace io_net_place;
  int status = pcf2place(pcf_data, callback.input_pins(), callback.output_pins(), io_pin_table, io_location_map, io_net_place);
  if (status) {
    return status;
  }

  /* Output */
  status = io_net_place.write_to_place_file(argv[5], true, true);


    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
