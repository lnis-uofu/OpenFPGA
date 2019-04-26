/* Commands available in the shell */
t_shell_cmd shell_cmd[] = {
  {"vpr_setup", SETUP_CMD, setup_vpr_opts, &shell_execute_vpr_setup }, 
  {"vpr_pack", PACK_CMD, vpr_pack_opts, &shell_execute_vpr_pack }, 
  {"vpr_place_and_route", PLACE_CMD, vpr_place_and_route_opts, &shell_execute_vpr_place_and_route }, 
  {"vpr_versapower", ANALYSIS_CMD, vpr_versapower_opts, &shell_execute_vpr_versapower }, 
  {"fpga_x2p_setup", SETUP_CMD, fpga_x2p_setup_opts, &shell_execute_fpga_x2p_setup }, 
  {"fpga_spice", PRODUCTION_CMD, fpga_spice_opts, &shell_execute_fpga_spice }, 
  {"fpga_verilog", PRODUCTION_CMD, fpga_verilog_opts, &shell_execute_fpga_verilog }, 
  {"fpga_bitstream", PRODUCTION_CMD, fpga_bitstream_opts, &shell_execute_fpga_bitstream }, 
  {"help", BASIC_CMD, help_opts, &shell_execute_help }, 
  {"exit", BASIC_CMD, exit_opts, &shell_execute_exit }, 
  {"quit", BASIC_CMD, NULL, &shell_execute_exit }, 
  {LAST_CMD_NAME, BASIC_CMD, NULL, NULL}
};

/* Command category */
t_cmd_category cmd_category[] = {
  {BASIC_CMD, "Basic Commands"},
  {SETUP_CMD, "Commands to Setup Engines"},
  {PACK_CMD, "Packing Engines"},
  {PLACE_CMD, "Placement Engines"},
  {ROUTE_CMD, "Routing Engines"},
  {ANALYSIS_CMD, "Analysis Commands"},
  {PRODUCTION_CMD, "Production Commmands"},
  {LAST_CMD_CATEGORY, "END"}
};
