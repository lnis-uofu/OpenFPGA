class openfpga_api {
 public:
  openfpga_api();
  ~openfpga_api();
  void version();
  void start_tcl_shell();
  void read_arch (std::string opt, std::string path);
};

