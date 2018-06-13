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
    if (all_of (s.begin (), s.end (), [=] (char c) {return isdigit (c) || c == '.';})) // looks like a number!
    {
        return new NumberElement (stod (s));
    }
    else if (starts_with (s, "\"") && ends_with (s, "\"") && [=] (string s) //all quotes are matched.
    {
        char prev = '\0';
        for (char curr : s)
        {
            if (curr == '"' && prev != '\\')
            {
                return false;
            }
            if (curr == '\\' && prev == '\\')
            {
                prev = '\0';
            }
        }

        return true;
    } (s)) // looks like a string!
    {
        return nullptr;//return new StringElement (unescape (s));
    }
    else if (s == TSTR || s == FSTR)
    {
        return new BooleanElement (s == "true");
    }
    else if (s.length () > 0 && isalpha(s[0]) && all_of (s.begin (), s.end (), [=] (char c) {return isalnum (c) || c == '-' || c == '?';}))
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