
#include "csv_reader.h"
#include "pcf_reader.h"
#include "xml_reader.h"
#include "blif_reader.h"
#include "cmd_line.h"
#include "pin_location.h"
#include <algorithm>
#include <set>
#include "vtr_log.h"

const string USAGE_MSG = "usage options: --xml PINMAP_XML --pcf PCF --blif BLIF --csv CSV_FILE --output OUTPUT";
const cmd_line & pin_location::get_cmd() const
{
    return cl_;
}

bool pin_location::reader_and_writer()
{
    cmd_line cmd = cl_;
    string xml_name = cmd.get_param("--xml");    
    string csv_name = cmd.get_param("--csv");
    string pcf_name = cmd.get_param("--pcf");
    string blif_name = cmd.get_param("--blif");
    string output_name = cmd.get_param("--output");
    if ((xml_name == "") || (csv_name == "") || (pcf_name == "") ||  (blif_name == "")|| (output_name == "") )
    {
        VTR_LOG_ERROR("%s\n %s\n", error_messages[MISSING_IN_OUT_FILES].c_str(), USAGE_MSG.c_str());
        return false;
    }
  
    XmlReader rd_xml;
    if (!rd_xml.read_xml(xml_name))
    {
        VTR_LOG_ERROR("%s.\n", error_messages[PIN_LOC_XML_PARSE_ERROR].c_str());
        return false;
    }
    std::map<std::string, PinMappingData>  xml_port_map = rd_xml.get_port_map();

    CvsReader rd_csv;
    if (!rd_csv.read_cvs(csv_name))
    {
        VTR_LOG_ERROR("%s.\n", error_messages[PIN_MAP_CSV_PARSE_ERROR].c_str());
        return false;
    }
    map<string, string> csv_port_map = rd_csv.get_port_map();

    PcfReader rd_pcf;
    if (!rd_pcf.read_pcf(pcf_name))
    {
        VTR_LOG_ERROR("%s.\n", error_messages[PIN_CONSTRAINT_PARSE_ERROR].c_str());
        return false;
    }

    // read port info from blif file
    BlifReader rd_blif;
    if (!rd_blif.read_blif(blif_name)) {
        VTR_LOG_ERROR("%s.\n", error_messages[INPUT_DESIGN_PARSE_ERROR].c_str());
        return false;
    }
    std::vector<std::string> inputs = rd_blif.get_inputs();
    std::vector<std::string> outputs = rd_blif.get_outputs();
   
    std::ofstream out_file;
    out_file.open(output_name);
    out_file << "#Block Name   x   y   z\n";
    out_file << "#------------ --  --  -\n";

    vector<vector<string>> pcf_pin_cstr = rd_pcf.get_commands();
    std::set<std::string> constrained_ports, constrained_pins;
    for (auto pin_cstr_v : pcf_pin_cstr)
    {
        if ((pin_cstr_v[0] != "set_io") && (pin_cstr_v[0] != "set_clk"))
            continue;
        
        string pin_name = pin_cstr_v[1];
        string cstr_name = pin_cstr_v[2];
        auto found_in = std::find(inputs.begin(), inputs.end(), pin_name);
        bool valid = false, is_out = false;
       
        if (found_in != inputs.end())
            valid = true;
        else
        {
            auto found_out = std::find(outputs.begin(), outputs.end(), pin_name);
            if (found_out != outputs.end())
            {
                valid = true;
                is_out = true;
            }
            else
            {
                VTR_LOG_ERROR("%s: <%s>.\n", error_messages[CONSTRAINED_PORT_NOT_FOUND].c_str(), pin_name.c_str());
                out_file.close();
                return false;
            }
        }
        if (constrained_ports.find(pin_name) == constrained_ports.end()) {
            constrained_ports.insert(pin_name);
        } else {
            VTR_LOG_ERROR("%s: <%s>.\n", error_messages[RE_CONSTRAINED_PORT].c_str(), pin_name.c_str());
            out_file.close();
            return false;
        }
        if (!valid)
            continue;

        std::string content_to_write;  
 
        // Get the the intternal port from the CSV file
        auto element_cstr = csv_port_map.find(cstr_name);
        if (element_cstr != csv_port_map.end())
        {

            if (constrained_pins.find(cstr_name) == constrained_pins.end()) {
                constrained_pins.insert(cstr_name);
            } else {
                VTR_LOG_ERROR("%s: <%s>.\n", error_messages[OVERLAP_PIN_IN_CONSTRAINT].c_str(), cstr_name.c_str());
                out_file.close();
                return false;
            }

            // Get the (x, y, z) from the XML reader 
            PinMappingData pinMapData = xml_port_map.at(element_cstr->second);
            if (is_out)
            {
                content_to_write += "out:";
           }
            content_to_write += pin_name + "        " +  std::to_string(pinMapData.get_x()) + "   " + std::to_string(pinMapData.get_y()) + "   " + std::to_string(pinMapData.get_z()) + "\n";
        } else {
           VTR_LOG_ERROR("%s: <%s>.\n", error_messages[CONSTRAINED_PIN_NOT_FOUND].c_str(),  cstr_name.c_str());
           out_file.close();
           return false;
        }

        out_file << content_to_write;
    }
    out_file.close();

    return true;
}
