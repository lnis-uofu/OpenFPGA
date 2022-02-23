#include <stdio.h>
#include <vector>
#include <string>
#include "openfpga_api.h"
#include "openfpga_title.h"
#include "openfpga_invoker.h"

openfpga_api::openfpga_api(){
  version();
  //printf("Constructor is called, object created!!\n");
}

openfpga_api::~openfpga_api(){
  printf("Destructor is called, object distroyed!!\n");
}

void openfpga_api::version(){

  print_openfpga_version_info();
}

void openfpga_api::read_arch(std::string opt, std::string path){
  
  /* actual command with all the command line opts */
  std::vector<std::string> tcl_cmd_opts;
  tcl_cmd_opts.push_back("read_openfpga_arch ");
  tcl_cmd_opts.push_back(opt);
  tcl_cmd_opts.push_back(" ");
  tcl_cmd_opts.push_back(path);

  /* call mode invoker function with TCL mode true */
  int return_status = mode_invoker(tcl_cmd_opts,true);

}