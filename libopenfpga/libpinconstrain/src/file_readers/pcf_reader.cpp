#include "pcf_reader.h"
#include "vtr_log.h"

bool PcfReader::read_pcf(const std::string &f)
{
    std::ifstream infile(f);
    if (!infile.is_open())
    {
        VTR_LOG_ERROR("ERROR: cound not open the file %s.\n", f.c_str());
        return false;
    }
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string a, b, c;
        if (!(iss >> a >> b >> c))
        {
            break;
        } // error
        commands.push_back(std::vector<std::string>());
        commands.back().push_back(a);
        commands.back().push_back(b);
        commands.back().push_back(c);
    }
    return true;
}
