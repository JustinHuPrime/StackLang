#include "stringUtils.h"
#include <string>
using std::string;

bool starts_with (string s, string s1)
{
    return 0 == s.find (s1);
}

bool ends_with (string s, string s1)
{
    return s.length () - s1.length () == s.find (s1, s.length () - s1.length ());
}

string escape (string s)
{
    for (unsigned i = 0; i < s.length (); i++)
    {
        if (s[i] == '"')
        {
            s.erase (i, 1);
            s.insert (i, "\\\"");
            i++;
        }
        else if (s[i] == '\n')
        {
            s.erase (i, 1);
            s.insert (i, "\\n");
            i++;
        }
        else if (s[i] == '\\')
        {
            s.erase (i, 1);
            s.insert (i, "\\\\");
            i++;
        }
    }

    return s;
}

string unescape (string s)
{
    bool prevBackslash = false;
    bool prevPrevBackslash = false;

    for (unsigned i = 0; i < s.length (); i++)
    {
        if (s[i] == '"' && prevBackslash && !prevPrevBackslash)
        {
            i--;
            s.erase (i, 2);
            s.insert (i, "\"");
            prevPrevBackslash = false;
            prevBackslash = false;
        }
        else if (s[i] == 'n' && prevBackslash && !prevPrevBackslash)
        {
            i--;
            s.erase (i, 2);
            s.insert (i, "\n");
            prevPrevBackslash = false;
            prevBackslash = false;
        }
        else if (s[i] == '\\' && prevBackslash && !prevPrevBackslash)
        {
            i--;
            s.erase (i, 2);
            s.insert (i, "\\\\");
            prevPrevBackslash = false;
            prevBackslash = false;
        }
        else
        {
            prevPrevBackslash = prevBackslash;
            prevBackslash = s[i] == '\\';
        }
    }

    return s;
}