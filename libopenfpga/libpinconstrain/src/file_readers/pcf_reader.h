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
using namespace std;
class PcfReader
{
  vector<vector<string>> commands;
  //std::map<string, string> pcf_pin_map;

public:
  PcfReader() {}
  PcfReader(const std::string &f)
  {
    read_pcf(f);
  }
  bool read_pcf(const std::string &f);
  const vector<vector<string>>& get_commands()const { return commands;}
  //const unordered_map<string, string>& get_pcf_pin_map()const { return pcf_pin_map;}
};

#endif
