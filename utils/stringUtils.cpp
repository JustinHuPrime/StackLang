#include "stringUtils.h"

bool starts_with (const string& s, const string& s1)
{
    return 0 == s.find (s1);
}

bool ends_with (const string& s, const string& s1)
{
    return s.length () - s1.length () == s.find (s1, s.length () - s1.length ());
}

string escape (string s)
{
    for (unsigned i = 0; i < s.length (); i++)
    {
        if (s[i] == '"') //escape quotes
        {
            s.erase (i, 1);
            s.insert (i, "\\\"");
            i++;
        }
        else if (s[i] == '\n') //newlines
        {
            s.erase (i, 1);
            s.insert (i, "\\n");
            i++;
        }
        else if (s[i] == '\\') //escaped backslashes
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
        if (s[i] == '"' && prevBackslash && !prevPrevBackslash) // escaped quote
        {
            i--;
            s.erase (i, 2);
            s.insert (i, "\"");
            prevPrevBackslash = false;
            prevBackslash = false;
        }
        else if (s[i] == 'n' && prevBackslash && !prevPrevBackslash) // escaped n
        {
            i--;
            s.erase (i, 2);
            s.insert (i, "\n");
            prevPrevBackslash = false;
            prevBackslash = false;
        }
        else if (s[i] == '\\' && prevBackslash && !prevPrevBackslash) // escaped backslash
        {
            i--;
            s.erase (i, 2);
            s.insert (i, "\\");
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

size_t findImproperEscape (const string& str)
{
    for (unsigned i = 0; i < str.length (); i++)
    {
        if ((str[i] == '\\' && i + 1 >= str.length ()) || //if the last char's a backslash
            (str[i] == '\\' && str[i + 1] != 'n' && str[i + 1] != '"' && str[i + 1] != '\\') || //make sure that we only escape \, n, and "
            (str[i] == '"')) //it's an unescaped quote!
        {
            return i;
        }
        else if (str[i] == '\\') //it's a valid escape - skip next char
        {
            i++;
        }
    }

    return string::npos;
}

string spaces (int n)
{
    string acc = "";
    acc.resize (n, ' ');
    return acc;
}

string removeChar (string s, char c)
{
    for (unsigned i = 0; i < s.length (); i++)
    {
        if (s[i] == c)
        {
            s.erase (i, 1);
            i--;
        }
    }

    return s;
}