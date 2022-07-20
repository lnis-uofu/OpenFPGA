#ifndef PCF_READER_H
#define PCF_READER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>

/*
Supported PCF commands:

* set_io  <net> <pad> - constrain a given <net> to a given physical <pad> in eFPGA pinout.
* set_clk <pin> <net> - constrain a given global clock <pin> to a given <net>

  Every tile where <net> is present will be constrained to use a given global clock.
*/
class PcfReader
{
  std::vector<std::vector<std::string>> commands;

public:
  PcfReader() {}
  PcfReader(const std::string &f)
  {
    read_pcf(f);
  }
  bool read_pcf(const std::string &f);
  const std::vector<std::vector<std::string>>& get_commands()const { return commands;}
};

#endif
