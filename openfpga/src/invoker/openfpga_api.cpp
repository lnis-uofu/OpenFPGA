#include <stdio.h>
#include <vector>
#include <string>
#include "openfpga_title.h"
#include "openfpga_invoker.h"

void openfpga_api(std::string opt){
  
  /* actual command with all the command line opts */
  std::vector<std::string> tcl_cmd_opts;
  tcl_cmd_opts.push_back(opt);

  /* call mode invoker function with TCL mode true */
  int return_status = mode_invoker(tcl_cmd_opts,true);

}