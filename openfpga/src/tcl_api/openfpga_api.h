#include "openfpga_context.h"
#include "shell.h"

class openfpga_api {
 public:
  // data member
  /* Create the data base for the shell */
  OpenfpgaContext openfpga_context;
  
  // function member
  openfpga_api();
  ~openfpga_api();
  void version();
  void start_tcl_shell();
  void read_arch (std::string opt, std::string path);
  void write_arch (std::string opt, std::string path);

  private:
  // declare dummy 
  openfpga::Shell<OpenfpgaContext> shell {"OpenFPGA"};
  // method call to call OpenFPGA mini shell
  void call_openfpga_shell (std::string& tcl_comnd);
  
};

