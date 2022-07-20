#include "xml_reader.h"
#include <vector>
#include <fstream>

//======================================================================
std::vector<std::string> XmlReader::vec_to_scalar(std::string str) 
{
    auto open_bracket_pos = str.find("[");
    auto close_bracket_pos = str.find("]");
    auto colon_pos = str.find(":");
    std::vector<std::string> scalar_ports;

    //Parse checks
    if (open_bracket_pos == std::string::npos && close_bracket_pos != std::string::npos) {
        //Close brace only
        std::string msg = "near '" + str + "', missing '['";
        std::cerr << " ERROR: " << msg << std::endl;
    }

    if (open_bracket_pos != std::string::npos && close_bracket_pos == std::string::npos) {
        //Open brace only
        std::string msg = "near '" + str + "', missing ']'";
        std::cerr << " ERROR: " << msg << std::endl;
    }

    if (open_bracket_pos != std::string::npos && close_bracket_pos != std::string::npos) {
        //Have open and close braces, close must be after open
        if (open_bracket_pos > close_bracket_pos) {
            std::string msg = "near '" + str + "', '[' after ']'";
            std::cerr << " ERROR: " << msg << std::endl;
        }
    }

    if (colon_pos != std::string::npos) {
        //Have a colon, it must be between open/close braces
        if (colon_pos > close_bracket_pos || colon_pos < open_bracket_pos) {
            std::string msg = "near '" + str + "', found ':' but not between '[' and ']'";
            std::cerr << " ERROR: " << msg << std::endl;
        }
    }

    std::string name = str.substr(0, open_bracket_pos);
    std::string first_idx_str;
    std::string second_idx_str;

    if (colon_pos == std::string::npos && open_bracket_pos == std::string::npos && close_bracket_pos == std::string::npos) {
    } else if (colon_pos == std::string::npos) {
        first_idx_str = str.substr(open_bracket_pos + 1, close_bracket_pos);
        second_idx_str = first_idx_str;
    } else {
        first_idx_str = str.substr(open_bracket_pos + 1, colon_pos);
        second_idx_str = str.substr(colon_pos + 1, close_bracket_pos);
    }

    int first_idx = std::stoi(first_idx_str);
    int second_idx = std::stoi(second_idx_str);

    if (first_idx < second_idx)
    {
        for(int i=first_idx; i < second_idx+1; i++)
        {
            std::string curr_port_name = name + '[' + std::to_string(i) + ']';
            scalar_ports.push_back(curr_port_name);
        }
    }
    else
    {
        for(int i=first_idx; i >= second_idx ; i--)
        {
            std::string curr_port_name = name + '[' + std::to_string(i) + ']';
            scalar_ports.push_back(curr_port_name);
        }
    }
    return scalar_ports;
}
//======================================================================
bool XmlReader::parse_io_cell (const pugi::xml_node xml_orient_io, const int row_or_col, const int io_per_cell, std::map<std::string, PinMappingData> *port_map)
{
    pugi::xpath_node_set cells = xml_orient_io.select_nodes("CELL");
    for (pugi::xpath_node_set::const_iterator it = cells.begin(); it != cells.end(); ++it)
    {
       pugi::xpath_node node = *it;
       int startx, starty, endx, endy, i, j, x, y;
       std::string port_name = node.node().attribute("port_name").as_string();
       std::string mapped_name = node.node().attribute("mapped_name").as_string();
       std::vector<std::string> scalar_mapped_pins = vec_to_scalar(mapped_name);
       i = 0;

       if (node.node().attribute("startx") && node.node().attribute("endx"))
       {
           startx = node.node().attribute("startx").as_int();
           endx = node.node().attribute("endx").as_int();
           y = row_or_col;
           if (startx < endx)
           {
               for (x=startx; x < endx+1; x++)
               {
                   for (j=0; j < io_per_cell; j++)
                   {
                       std::string mapped_pin = scalar_mapped_pins[i];
                       PinMappingData pinMapData = PinMappingData(port_name, mapped_pin, x, y, j);
                       port_map->insert(std::pair<std::string, PinMappingData>(mapped_pin, pinMapData));
                       i++;
                   }
               }
           }
           else
           {
                for (x=startx; x >= endx; x--)
                {
                   for (j=0; j < io_per_cell; j++)
                   {
                       std::string mapped_pin = scalar_mapped_pins[i];
                       PinMappingData pinMapData = PinMappingData(port_name, mapped_pin, x, y, j);
                       port_map->insert(std::pair<std::string, PinMappingData>(mapped_pin, pinMapData));
                       i++;
                   }
                }
          }
       }
       if (node.node().attribute("starty") && node.node().attribute("endy").value())
       {
           starty = node.node().attribute("starty").as_int();
           endy = node.node().attribute("endy").as_int();
           x = row_or_col;
           if (starty < endy)
           {
               for (y=starty; y < endy+1; y++)
               {
                   for (j=0; j < io_per_cell; j++)
                   {
                       std::string mapped_pin = scalar_mapped_pins[i];
                       PinMappingData pinMapData = PinMappingData(port_name, mapped_pin, x, y, j);
                       port_map->insert(std::pair<std::string, PinMappingData>(mapped_pin, pinMapData));
                       i++;
                   }
               }
           }
           else
           {
                for (y=starty; y >= endy; y--)
                {
                   for (j=0; j < io_per_cell; j++)
                   {
                       std::string mapped_pin = scalar_mapped_pins[i];
                       PinMappingData pinMapData = PinMappingData(port_name, mapped_pin, x, y, j);
                       port_map->insert(std::pair<std::string, PinMappingData>(mapped_pin, pinMapData));
                       i++;
                   }
                }
          }
       }
        
    }
    return true;
}

//======================================================================
bool XmlReader::parse_io (const pugi::xml_node xml_io, const int width, const int height, const int io_per_cell, std::map<std::string, PinMappingData> *port_map)
{
    pugi::xml_node xml_top_io = xml_io.child("TOP_IO");
    if (!xml_top_io)
    {
        return false;
    }
    int io_row_top = height - 1;
    if (xml_top_io.attribute("y"))
        io_row_top = xml_top_io.attribute("y").as_int();

    pugi::xml_node xml_bottom_io = xml_io.child("BOTTOM_IO");
    if (!xml_bottom_io)
    {
        return false;
    }
    int io_row_bottom = 0;
    if (xml_bottom_io.attribute("y"))
        io_row_bottom = xml_bottom_io.attribute("y").as_int();

    pugi::xml_node xml_left_io = xml_io.child("LEFT_IO");
    if (!xml_left_io)
    {
        return false;
    }
    int io_col_left = 0;
    if (xml_left_io.attribute("x"))
        io_col_left = xml_left_io.attribute("x").as_int();
    pugi::xml_node xml_right_io = xml_io.child("RIGHT_IO");
    if (!xml_right_io)
    {
        return false;
    }
    int io_col_right = width - 1;
    if (xml_right_io.attribute("x"))
        io_col_right = xml_right_io.attribute("x").as_int();
    
    
    if (!parse_io_cell (xml_top_io, io_row_top, io_per_cell, port_map))
        return false;
    if (!parse_io_cell (xml_bottom_io, io_row_bottom, io_per_cell, port_map))
        return false;
    if (!parse_io_cell (xml_right_io, io_col_right, io_per_cell, port_map))
        return false;
    if (!parse_io_cell (xml_left_io, io_col_left, io_per_cell, port_map))
        return false;

    return true;
}
//======================================================================

bool XmlReader::read_xml(const std::string &f)
{
    pugi::xml_document doc;
    std::ifstream infile(f);
    if (!infile.is_open())
    {
        std::cerr << "ERROR: cound not open the file " << f << std::endl;
        return false;
    }

    pugi::xml_parse_result result = doc.load_file(f.c_str());

    if (!result)
        return false;

    pugi::xml_node device = doc.child("DEVICE");
    if (!device)
        return false;

    int width = device.attribute("width").as_int();
    int height = device.attribute("height").as_int();
    int z = device.attribute("z").as_int();
    if (z <= 0)
       return false;
    
    pugi::xml_node xml_io = device.child("IO");
    if (!xml_io)
        return false;

    if (!parse_io (xml_io, width, height, z, &port_map_))
        return false;
    
    return true;
}
