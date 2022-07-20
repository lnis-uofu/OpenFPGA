#include "pcf_reader.h"

bool PcfReader::read_pcf(const std::string &f)
{
    std::ifstream infile(f);
    if (!infile.is_open())
    {
        cout << "ERROR: cound not open the file " << f << endl;
        return false;
    }
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        string a, b, c;
        if (!(iss >> a >> b >> c))
        {
            break;
        } // error
        commands.push_back(vector<string>());
        commands.back().push_back(a);
        commands.back().push_back(b);
        commands.back().push_back(c);
        //pcf_pin_map.insert(std::pair<std::string, string>(b, c));
    }
    return true;
}
