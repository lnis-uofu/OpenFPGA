#include <stdio.h>
#include <vector>
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
  /* call title of OpenFPGA shell */
  start_tcl_shell();
  //printf("Constructor is called, object created!!\n");
}

openfpga_api::~openfpga_api(){
  printf("Destructor is called, object distroyed!!\n");
}

void openfpga_api::call_openfpga_shell (std::string& tcl_comnd){
    
  /* call tcl mode in of mini shell */
  shell.run_tcl_mode(openfpga_context,tcl_comnd);
  int status = shell.exit_code();
  VTR_LOG("And its exit status is '%d'...\n",
          status);

}

void openfpga_api::start_tcl_shell(){
  std::string to_send = "call_title";
  call_openfpga_shell(to_send);
}

void openfpga_api::version(){
  print_openfpga_version_info();
}

void openfpga_api::read_arch(std::string opt, std::string path){
  
  std::string to_send = "read_openfpga_arch ";
  to_send = to_send + opt + " " +path;
  
  VTR_LOG("to send command is '%s'...\n",to_send.c_str());
  call_openfpga_shell(to_send);
}
void openfpga_api::write_arch(std::string opt, std::string path){
  
  std::string to_send = "write_openfpga_arch ";
  to_send = to_send + opt + " " +path;
  
  VTR_LOG("to send command is '%s'...\n",to_send.c_str());
  call_openfpga_shell(to_send);
}