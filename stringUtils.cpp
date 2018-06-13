#include "stringUtils.h"
#include <string>
using std::string;

bool starts_with (string s, string s1)
{
    return 0 == s.find (s1);
}

bool ends_with (string s, string s1)
{
    return s.length () - s1.length () == s.find (s1);
}

string escape (string s)
{
    for (int i = 0; i < s.length (); i++)
    {
        if (s[i] == '"')
        {
            s.erase (i, 1);
            s.insert (i, "\\\"");
        }
        else if (s[i] == '\n')
        {
            s.erase (i, 1);
            s.insert (i, "\\n");
        }
        else if (s[i] == '\\')
        {
            
        }
    }

    return s;
}

string unescape (string s)
{
    bool prevBackslash = false;
    bool prevPrevBackslash = false;

    for (int i = 0; i < s.length (); i++)
    {
        if (s[i] == '"' && prevBackslash && !prevPrevBackslash)
        {
            s.erase (i, 1);
        }
    }

    return s;
}