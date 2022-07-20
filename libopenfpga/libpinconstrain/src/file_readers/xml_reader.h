#ifndef XML_READER_H
#define XML_READER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "pugixml.hpp"

class PinMappingData 
{
    public:
        PinMappingData(std::string p_name, std::string map_pin, int x, int y, int z):port_name_(p_name), mapped_pin_(map_pin), x_(x), y_(y), z_(z){}
        std::string get_port_name () { return port_name_; }
        std::string get_mapped_pin () { return mapped_pin_; }
        int get_x () { return x_; }
        int get_y () { return y_; }
        int get_z () { return z_; }
    private:
        std::string port_name_;
        std::string mapped_pin_;
        int x_;
        int y_;
        int z_;
    
};

class XmlReader
{
    std::map<std::string, PinMappingData> port_map_;
public:
  XmlReader() {}
  bool read_xml(const std::string &f);
  const std::map<std::string, PinMappingData>& get_port_map()const { return port_map_;}
  std::vector<std::string> vec_to_scalar(std::string str);
  bool parse_io_cell (const pugi::xml_node xml_orient_io, const int row_or_col, const int io_per_cell, std::map<std::string, PinMappingData> *port_map);
  bool parse_io (const pugi::xml_node xml_io, const int width, const int height, const int io_per_cell, std::map<std::string, PinMappingData> *port_map);
};

#endif
