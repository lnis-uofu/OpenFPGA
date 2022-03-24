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
  // method call to call OpenFPGA mini shell
  //void call_openfpga_shell (std::string tcl_comnd);
  // read_arch command
  void read_openfpga_arch (std::string flag , std::string path);
  void write_openfpga_arch (std::string flag , std::string path);
  void read_openfpga_simulation_setting (std::string flag , std::string path);
  void write_openfpga_simulation_setting (std::string flag , std::string path);
  //void read_openfpga_arch (std::string flag std::string path);

  private:
  // declare dummy 
  openfpga::Shell<OpenfpgaContext> shell {"OpenFPGA"};
  std::string tcl_comnd;
  int status;
  
};

