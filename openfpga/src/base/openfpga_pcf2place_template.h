#ifndef OPENFPGA_PCF2PLACE_TEMPLATE_H
#define OPENFPGA_PCF2PLACE_TEMPLATE_H
/********************************************************************
 * This file includes functions to build bitstream database
 *******************************************************************/
#include "atom_netlist_utils.h"
#include "blif_head_reader.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "io_net_place.h"
#include "openfpga_digest.h"
#include "pcf2place.h"
#include "pcf_reader.h"
#include "read_blif.h"
#include "read_blif_clock_info.h"
#include "read_csv_io_pin_table.h"
#include "read_interchange_netlist.h"
#include "read_xml_arch_file.h"
#include "read_xml_boundary_timing.h"
#include "read_xml_io_location_map.h"
#include "vtr_log.h"
#include "vtr_path.h"
#include "vtr_time.h"
/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to convert a .pcf file to a .place file which
 * which VPR can force I/O placement
 *******************************************************************/
template <class T>
int pcf2bitstream_setting_wrapper_template(T& openfpga_context,
                                           const Command& cmd,
                                           const CommandContext& cmd_context) {
  /* todo: create a factory to produce this in the future*/
  CommandOptionId opt_pcf = cmd.option("pcf");
  CommandOptionId opt_pcf_config = cmd.option("config");
  CommandOptionId opt_reduce_error_to_warning =
    cmd.option("reduce_error_to_warning");
  CommandOptionId opt_pin_table = cmd.option("pin_table");
  CommandOptionId opt_pin_table_dir_convention =
    cmd.option("pin_table_direction_convention");
  CommandOptionId opt_verbose = cmd.option("verbose");

  std::string pcf_fname = cmd_context.option_value(cmd, opt_pcf);
  std::string pcf_config_fname = cmd_context.option_value(cmd, opt_pcf_config);
  std::string pin_table_fname = cmd_context.option_value(cmd, opt_pin_table);
  /* FIXME: This part is dirty. Should have function to handle the legalization
   */
  e_pin_table_direction_convention pin_table_dir_convention =
    e_pin_table_direction_convention::EXPLICIT;
  if (cmd_context.option_enable(cmd, opt_pin_table_dir_convention)) {
    std::string pin_table_dir_convention_str =
      cmd_context.option_value(cmd, opt_pin_table_dir_convention);
    if (pin_table_dir_convention_str ==
        std::string(PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::EXPLICIT))) {
      pin_table_dir_convention = e_pin_table_direction_convention::EXPLICIT;
    } else if (pin_table_dir_convention_str ==
               std::string(PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
                 e_pin_table_direction_convention::QUICKLOGIC))) {
      pin_table_dir_convention = e_pin_table_direction_convention::QUICKLOGIC;
    } else {
      VTR_LOG_ERROR(
        "Invalid pin naming convention ('%s') to identify port direction for "
        "pin table! Expect ['%s'|'%s'].\n",
        pin_table_dir_convention_str.c_str(),
        PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::EXPLICIT),
        PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::QUICKLOGIC));
    }
  }

  /* Parse the input files */
  openfpga::PcfCustomCommand pcf_custom_command;
  openfpga::read_pcf_config(pcf_config_fname, pcf_custom_command);

  openfpga::PcfData pcf_data;
  openfpga::read_pcf(
    pcf_fname.c_str(), pcf_data, pcf_custom_command,
    cmd_context.option_enable(cmd, opt_reduce_error_to_warning),
    cmd_context.option_enable(cmd, opt_verbose));
  VTR_LOG("Read the design constraints from a pcf file: %s.\n",
          pcf_fname.c_str());

  IoPinTable io_pin_table =
    read_csv_io_pin_table(pin_table_fname.c_str(), pin_table_dir_convention);
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n",
          pin_table_fname.c_str());

  int status = pcf2bitstream_setting(
    pcf_data, openfpga_context.mutable_bitstream_setting(), io_pin_table,
    cmd_context.option_enable(cmd, opt_verbose));
  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Top-level function to convert a .pcf file to a .place file which
 * which VPR can force I/O placement
 *******************************************************************/
template <class T>
int pcf2place_wrapper_template(const Command& cmd,
                               const CommandContext& cmd_context) {
  /* todo: create a factory to produce this in the future*/
  CommandOptionId opt_pcf = cmd.option("pcf");
  CommandOptionId opt_blif = cmd.option("blif");
  CommandOptionId opt_fpga_io_map = cmd.option("fpga_io_map");
  CommandOptionId opt_pin_table = cmd.option("pin_table");
  CommandOptionId opt_fpga_fix_pins = cmd.option("fpga_fix_pins");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_pin_table_dir_convention =
    cmd.option("pin_table_direction_convention");
  CommandOptionId opt_reduce_error_to_warning =
    cmd.option("reduce_error_to_warning");
  CommandOptionId opt_verbose = cmd.option("verbose");

  std::string pcf_fname = cmd_context.option_value(cmd, opt_pcf);
  std::string blif_fname = cmd_context.option_value(cmd, opt_blif);
  std::string fpga_io_map_fname =
    cmd_context.option_value(cmd, opt_fpga_io_map);
  std::string pin_table_fname = cmd_context.option_value(cmd, opt_pin_table);
  std::string fpga_fix_pins_fname =
    cmd_context.option_value(cmd, opt_fpga_fix_pins);
  e_pin_table_direction_convention pin_table_dir_convention =
    e_pin_table_direction_convention::EXPLICIT;
  if (cmd_context.option_enable(cmd, opt_pin_table_dir_convention)) {
    std::string pin_table_dir_convention_str =
      cmd_context.option_value(cmd, opt_pin_table_dir_convention);
    if (pin_table_dir_convention_str ==
        std::string(PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::EXPLICIT))) {
      pin_table_dir_convention = e_pin_table_direction_convention::EXPLICIT;
    } else if (pin_table_dir_convention_str ==
               std::string(PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
                 e_pin_table_direction_convention::QUICKLOGIC))) {
      pin_table_dir_convention = e_pin_table_direction_convention::QUICKLOGIC;
    } else {
      VTR_LOG_ERROR(
        "Invalid pin naming convention ('%s') to identify port direction for "
        "pin table! Expect ['%s'|'%s'].\n",
        pin_table_dir_convention_str.c_str(),
        PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::EXPLICIT),
        PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::QUICKLOGIC));
    }
  }

  /* Parse the input files */
  openfpga::PcfData pcf_data;
  openfpga::PcfCustomCommand pcf_cust_cmd;
  openfpga::read_pcf(
    pcf_fname.c_str(), pcf_data, pcf_cust_cmd,
    cmd_context.option_enable(cmd, opt_reduce_error_to_warning),
    cmd_context.option_enable(cmd, opt_verbose));
  VTR_LOG("Read the design constraints from a pcf file: %s.\n",
          pcf_fname.c_str());

  blifparse::BlifHeadReader callback;
  blifparse::blif_parse_filename(blif_fname.c_str(), callback);
  VTR_LOG("Read the blif from a file: %s.\n", blif_fname.c_str());
  if (callback.had_error()) {
    VTR_LOG_ERROR("Read the blif ends with errors\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  IoLocationMap io_location_map =
    read_xml_io_location_map(fpga_io_map_fname.c_str());
  VTR_LOG("Read the I/O location map from an XML file: %s.\n",
          fpga_io_map_fname.c_str());

  IoPinTable io_pin_table =
    read_csv_io_pin_table(pin_table_fname.c_str(), pin_table_dir_convention);
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n",
          pin_table_fname.c_str());

  /* Convert */
  IoNetPlace io_net_place;
  int status =
    pcf2place(pcf_data, callback.input_pins(), callback.output_pins(),
              io_pin_table, io_location_map, io_net_place);
  if (status) {
    return status;
  }

  /* Output */
  status = io_net_place.write_to_place_file(
    fpga_fix_pins_fname.c_str(),
    cmd_context.option_enable(cmd, opt_no_time_stamp),
    cmd_context.option_enable(cmd, opt_verbose));
  if (status) {
    return status;
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Top-level function to convert a .pcf file to a .sdc file which
 *******************************************************************/
template <class T>
int pcf2sdc_wrapper_template(const Command& cmd,
                             const CommandContext& cmd_context) {
  /* todo: create a factory to produce this in the future*/
  CommandOptionId opt_pcf = cmd.option("pcf");
  CommandOptionId opt_blif = cmd.option("blif");
  CommandOptionId opt_ckt_fmt = cmd.option("circuit_format");
  CommandOptionId opt_pin_table = cmd.option("pin_table");
  CommandOptionId opt_input_sdc_file = cmd.option("input_sdc");
  CommandOptionId opt_sdc_file = cmd.option("output_sdc");
  CommandOptionId opt_boundary_timing_file = cmd.option("boundary_timing");
  CommandOptionId opt_arch_file = cmd.option("vpr_arch_file");
  CommandOptionId opt_pin_table_dir_convention =
    cmd.option("pin_table_direction_convention");
  CommandOptionId opt_reduce_error_to_warning =
    cmd.option("reduce_error_to_warning");
  CommandOptionId opt_verbose = cmd.option("verbose");

  std::string pcf_fname = cmd_context.option_value(cmd, opt_pcf);
  std::string blif_fname = cmd_context.option_value(cmd, opt_blif);
  std::string ckt_fmt = "auto";
  if (cmd_context.option_enable(cmd, opt_ckt_fmt)) {
    ckt_fmt = cmd_context.option_value(cmd, opt_ckt_fmt);
  }
  std::string arch_fname = cmd_context.option_value(cmd, opt_arch_file);
  std::string sdc_fname = cmd_context.option_value(cmd, opt_sdc_file);
  std::string input_sdc = "";
  if (cmd_context.option_enable(cmd, opt_input_sdc_file)) {
    input_sdc = cmd_context.option_value(cmd, opt_input_sdc_file);
  }
  std::string boundary_timing_fname =
    cmd_context.option_value(cmd, opt_boundary_timing_file);
  std::string pin_table_fname = cmd_context.option_value(cmd, opt_pin_table);

  e_pin_table_direction_convention pin_table_dir_convention =
    e_pin_table_direction_convention::EXPLICIT;
  if (cmd_context.option_enable(cmd, opt_pin_table_dir_convention)) {
    std::string pin_table_dir_convention_str =
      cmd_context.option_value(cmd, opt_pin_table_dir_convention);
    if (pin_table_dir_convention_str ==
        std::string(PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::EXPLICIT))) {
      pin_table_dir_convention = e_pin_table_direction_convention::EXPLICIT;
    } else if (pin_table_dir_convention_str ==
               std::string(PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
                 e_pin_table_direction_convention::QUICKLOGIC))) {
      pin_table_dir_convention = e_pin_table_direction_convention::QUICKLOGIC;
    } else {
      VTR_LOG_ERROR(
        "Invalid pin naming convention ('%s') to identify port direction for "
        "pin table! Expect ['%s'|'%s'].\n",
        pin_table_dir_convention_str.c_str(),
        PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::EXPLICIT),
        PIN_TABLE_DIRECTION_CONVENTION_STRING.at(
          e_pin_table_direction_convention::QUICKLOGIC));
    }
  }

  /* Parse the input files */
  openfpga::PcfData pcf_data;
  openfpga::PcfCustomCommand pcf_cust_cmd;
  openfpga::read_pcf(
    pcf_fname.c_str(), pcf_data, pcf_cust_cmd,
    cmd_context.option_enable(cmd, opt_reduce_error_to_warning),
    cmd_context.option_enable(cmd, opt_verbose));
  VTR_LOG("Read the design constraints from a pcf file: %s.\n",
          pcf_fname.c_str());

  VTR_LOG("Read the boundary timing file: %s.\n",
          boundary_timing_fname.c_str());
  openfpga::BoundaryTiming boundary_timing;
  openfpga::read_xml_boundary_timing(boundary_timing_fname.c_str(),
                                     boundary_timing);

  IoPinTable io_pin_table =
    read_csv_io_pin_table(pin_table_fname.c_str(), pin_table_dir_convention);
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n",
          pin_table_fname.c_str());

  /*get clk info from blif or eblfi file*/
  std::vector<std::string> clock_names = read_blif_clock_info(
    arch_fname.c_str(), blif_fname.c_str(), ckt_fmt.c_str());

  if (clock_names.size() == 0) {
    VTR_LOG(
      "Skip generating sdc file from pcf file as there is no clock in the "
      "current design!\n");
    /*generated an empty sdc file or append to input_sdc*/
    if (!input_sdc.empty()) {
      std::ifstream ifs(input_sdc);
      if (!ifs.is_open()) {
        VTR_LOG_ERROR("Failed to open input SDC file %s\n", input_sdc.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }

      {
        std::ofstream ofs(sdc_fname);
        if (!ofs.is_open()) {
          VTR_LOG_ERROR("Failed to generate file %s\n", sdc_fname.c_str());
          return CMD_EXEC_FATAL_ERROR;
        }
        ofs << ifs.rdbuf();
      }
    }
    std::ofstream ofs(sdc_fname, std::ios::app);
    if (!ofs.is_open()) {
      VTR_LOG_ERROR("Failed to generate file %s \n", sdc_fname.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    return CMD_EXEC_SUCCESS;
  }

  if (!input_sdc.empty()) {
    std::ifstream ifs(input_sdc);
    if (!ifs.is_open()) {
      VTR_LOG_ERROR("Failed to open input SDC file %s\n", input_sdc.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    {
      std::ofstream ofs(sdc_fname);
      if (!ofs.is_open()) {
        VTR_LOG_ERROR("Failed to generate file %s\n", sdc_fname.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      ofs << ifs.rdbuf();
    }
  }

  std::ofstream ofs(sdc_fname, std::ios::app);
  if (!ofs.is_open()) {
    VTR_LOG_ERROR("Failed to open sdc file %s \n", sdc_fname.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  ofs << "\n";
  std::string clock_name;
  clock_name = clock_names[0];
  if (clock_names.size() > 1) {
    if (!cmd_context.option_enable(cmd, opt_reduce_error_to_warning)) {
      VTR_LOG_ERROR(
        "Multiple (%lu) clocks are detected in the design, which is not fully "
        "support yet. Will consider a virtual clock for all the inputs and "
        "outputs, resulting in timing loss\n",
        clock_names.size());
      return CMD_EXEC_FATAL_ERROR;
    }
    VTR_LOG_WARN(
      "Multiple (%lu) clocks are detected in the design, which is not fully "
      "support yet. Will consider a virtual clock for all the inputs and "
      "outputs, resulting in timing loss\n",
      clock_names.size());
    clock_name = "virtual_clock";
  }
  /*force clock to be virtual_clock*/
  double clock_period = 10;
  /* Convert */
  int status =
    pcf2sdc_from_boundary_timing(pcf_data, boundary_timing, io_pin_table,
                                 clock_name, clock_period, ofs, true);

  if (status) {
    return status;
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
