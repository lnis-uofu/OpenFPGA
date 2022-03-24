#include <stdio.h>
#include <string>
#include "openfpga_api.h"

/********************************************************************
 * Build the OpenFPGA shell interface to be used in TCL
 *******************************************************************/
/* Header file from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Header file from libopenfpgashell library */
#include "command_parser.h"
#include "command_echo.h"
#include "shell.h"

/* Header file from openfpga */
#include "vpr_command.h"
#include "openfpga_setup_command.h"
#include "openfpga_verilog_command.h"
#include "openfpga_bitstream_command.h"
#include "openfpga_spice_command.h"
#include "openfpga_sdc_command.h"
#include "basic_command.h"

#include "openfpga_title.h"
#include "openfpga_context.h"

// TODO  this is duplication of main function, call it in main to avoid duplication
openfpga::Shell<OpenfpgaContext> initialize_mini_shell (){

  /* Create a shell object
   * Add two commands, which are
   * 1. exit
   * 2. help. This must the last to add
   */
  openfpga::Shell<OpenfpgaContext> shell("OpenFPGA");

  shell.add_title(create_openfpga_title().c_str());

  /* Add vpr commands */
  openfpga::add_vpr_commands(shell);

  /* Add openfpga setup commands */
  openfpga::add_openfpga_setup_commands(shell);

  /* Add openfpga verilog commands */
  openfpga::add_openfpga_verilog_commands(shell);

  /* Add openfpga bitstream commands */
  openfpga::add_openfpga_bitstream_commands(shell);

  /* Add openfpga SPICE commands */
  openfpga::add_openfpga_spice_commands(shell);

  /* Add openfpga sdc commands */
  openfpga::add_openfpga_sdc_commands(shell);

  /* Add basic commands: exit, help, etc. 
   * Note:
   * This MUST be the last command group to be added! 
   */
  openfpga::add_basic_commands(shell);
 
   return shell;

}

openfpga_api::openfpga_api(){
  // call OpenFPGA shell initializer once
  shell = initialize_mini_shell ();
  tcl_comnd = "call_title";
  shell.run_tcl_mode(openfpga_context,tcl_comnd);
  //printf("Constructor is called, object created!!\n");
}

openfpga_api::~openfpga_api(){
  printf("Destructor is called, object distroyed!!\n");
}

void openfpga_api::read_openfpga_arch (std::string flag , std::string path){
    
  tcl_comnd = flag + " " + path;  
  /* call tcl mode of mini shell */
  shell.run_tcl_mode(openfpga_context,tcl_comnd);
  status = shell.exit_code();
  // TODO make this status as data member and access it in TCL class to decide whether to exit or not
  //VTR_LOG("exit status is '%d'...\n", status);
}

void openfpga_api::version(){
  print_openfpga_version_info();
}