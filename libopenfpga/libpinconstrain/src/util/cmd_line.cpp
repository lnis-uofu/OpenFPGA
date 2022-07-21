#include "cmd_line.h"

cmd_line::cmd_line(int argc, const char *argv[])
{
    bool needVal = false;
    string key;
    for (int i = 1; i < argc; ++i)
    {
        string s(argv[i]);
        if (s.size() < 2)
        {
            cout << "Warning: Not a valid flag  \"" << s << "\" discarding" << endl;
            continue;
        }
        if ('-' == s[0])
        {
            if ('-' == s[1])
            { // param key
                if (needVal)
                    cout << "Warning: Key " << key << " did not get a value" << endl;
                needVal = true;
                key = s;
            }
            else
            { // flag
                flags.insert(s);
                if (needVal)
                    cout << "Warning: Key " << key << " did not get a value" << endl;
                needVal = false;
            }
        }
        else
        { // param value
            if (needVal)
            {
                params[key] = s;
                needVal = false;
            }
            else
            {
                cout << "Warning: No key for value " << s << endl;
            }
        }
    }
}

bool cmd_line::is_flag_set(string &fl)
{
    return (flags.find(fl) != end(flags));
}

string cmd_line::get_param(const string &key)
{
    if (params.find(key) != end(params))
        return params[key];
    return "";
}

void cmd_line::set_flag(string &fl)
{
    flags.insert(fl);
}

void cmd_line::set_param_value(string &key, string &val)
{
    params[key] = val;
}

void cmd_line::print_options() const
{
    cout << "Flags :\n";
    for (auto &f : flags)
        cout << "\t" << f << endl;
    cout << "Params :\n";
    for (auto &p : params)
        cout << "\t" << p.first << "\t" << p.second << endl;
}
