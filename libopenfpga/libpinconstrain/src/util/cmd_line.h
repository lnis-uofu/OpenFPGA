#ifndef CMD_LINE
#define CMD_LINE
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;
class cmd_line
{
    unordered_map<string, string> params;
    unordered_set<string> flags;

public:
    cmd_line(int argc, const char *argv[]);
    const unordered_set<string> &get_flag_set() const { return flags; }
    const unordered_map<string, string> get_param_map() const { return params; }
    bool is_flag_set(string &fl);
    string get_param(const string &key);
    void set_flag(string &fl);
    void set_param_value(string &key, string &val);
    void print_options() const;
};

#endif
