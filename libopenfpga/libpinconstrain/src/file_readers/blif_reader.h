#ifndef BLIF_READER_H
#define BLIF_READER_H

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
class BlifReader
{
  vector<string> inputs;
  vector<string> outputs;

public:
  BlifReader() {}
  BlifReader(const std::string &f)
  {
    read_blif(f);
  }
  bool read_blif(const std::string &f);
  const vector<string>& get_inputs()const { return inputs;}
  const vector<string>& get_outputs()const { return outputs;}
};

#endif
