#ifndef CVS_READER_H
#define CVS_READER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ctype.h>
#include <map>

using namespace std;

class CvsReader
{
  vector<vector<string>> entries;
  map<string, string> port_map;
public:
  CvsReader() {}
  bool read_cvs(const std::string &f); 
  const vector<vector<string>>& get_entries()const { return entries;}
  const map<string, string>& get_port_map()const { return port_map;}
};

#endif
