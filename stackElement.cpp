#include "stackElement.h"
#include "stackElements/numberElement.h"
#include "stackElements/stringElement.h"
#include "stackElements/booleanElement.h"
#include "stackElements/substackElement.h"
#include "stackElements/typeElement.h"
#include "stackElements/commandElement.h"
#include "stringUtils.h"
#include <algorithm>
#include <string>
#include <stdexcept>
using namespace StackElements;
using namespace BooleanConstants;
using namespace TypeNameConstants;
using std::invalid_argument;
using std::all_of;
using std::find;
using std::count;
using std::stod;

StackElement::StackElement (DataType type) : dataType (type)
{}

StackElement* StackElement::parse (string s)
{
    if (all_of (s.begin (), s.end (), [] (char c) {return isdigit (c) || c == '.';})) // looks like a number!
    {
        return new NumberElement (stod (s));
    }
    else if (starts_with (s, "\"") && ends_with (s, "\"") && [] (string str) //all quotes are escaped.
    {
        for (unsigned i = 0; i < str.length (); i++)
        {
            if (str[i] == '\\' && (i + 1 >= str.length () || (str[i + 1] != 'n' && str[i + 1] != '"' && str[i + 1] != '\\')))
            {
                return false;
            }
            else if (str[i] == '\\')
            {
                i++;
            }
            else if (str[i] == '"')
            {
                return false;
            }
        }

        return true;
    } (s.substr (1, s.length () - 2))) // looks like a string!
    {
        return new StringElement (unescape (s.substr (1, s.length () - 2)));
    }
    else if (s == TSTR || s == FSTR)
    {
        return new BooleanElement (s == "true");
    }
    else if (s.length () > 0 && isalpha(s[0]) && all_of (s.begin (), s.end (), [] (char c) {return isalnum (c) || c == '-' || c == '?';}))
    {
        return new CommandElement (s);
    }
    else
    {
        throw invalid_argument ("`" + s + "` cannot be parsed as anything!");
    }
}

/*
Number,
String,
Boolean,
Substack,
Type,
Command,
*/