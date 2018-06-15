#include "language/stackElement.h"
#include "language/stackElements/numberElement.h"
#include "language/stackElements/stringElement.h"
#include "language/stackElements/booleanElement.h"
#include "language/stackElements/commandElement.h"
#include "language/stackElements/typeElement.h"
#include "language/stackElements/substackElement.h"
#include "utils/stringUtils.h"
#include <algorithm>
#include <string>
#include <stdexcept>
using namespace StackElements;
using namespace BooleanConstants;
using std::invalid_argument;
using std::all_of;
using std::find;
using std::count;
using std::stod;
using std::to_string;

StackElement::DataType StackElement::getType () const
{
    return dataType;
}

StackElement::StackElement (DataType type) : dataType (type)
{}

// ASSUME: s is not empty
StackElement* StackElement::parse (string s)
{
    if (all_of (s.begin (), s.end (), [] (char c) {return isdigit (c) || c == '.';}) && s.find ('.', s.find ('.') + 1) == string::npos) // looks like a number!
    {
        return new NumberElement (stod (s));
    }
    else if (isdigit (s [0]) || s[0] == '.')
    {
        throw invalid_argument ("`" + s + "` cannot be parsed as a number - second deminal point at position " + to_string (s.find ('.', s.find ('.') + 1)) + ".");
    }
    else if (starts_with (s, "\"") && ends_with (s, "\"") && [] (string str) //all quotes are escaped.
    {
        for (unsigned i = 0; i < str.length (); i++)
        {
            if (str[i] == '\\' && i + 1 >= str.length ())
            {
                throw invalid_argument ("`\"" + str + "\"` cannot be parsed as a string - escaped ending quote.");
            }
            if (str[i] == '\\' && str[i + 1] != 'n' && str[i + 1] != '"' && str[i + 1] != '\\')
            {
                throw invalid_argument ("`\"" + str + "\"` cannot be parsed as a string - invalid escape sequence \\" + str [i + 1] + " at position " + to_string (i) + ".");
            }
            else if (str[i] == '\\')
            {
                i++;
            }
            else if (str[i] == '"')
            {
                throw invalid_argument ("`\"" + str + "\"` cannot be parsed as a string - unescaped quote at position " + to_string (i) + ".");
            }
        }

        return true;
    } (s.substr (1, s.length () - 2))) // looks like a string!
    {
        return new StringElement (unescape (s.substr (1, s.length () - 2)));
    }
    else if (s == TSTR || s == FSTR) //it's either true or false
    {
        return new BooleanElement (s == "true");
    }
    else if (isalpha(s[0]) && all_of (s.begin (), s.end (), [] (char c) {return isalnum (c) || c == '-' || c == '?';})) // it's a valid symbol
    {
        return new CommandElement (s);
    }
    else
    {
        throw invalid_argument ("`" + s + "` cannot be parsed as anything.");
    }
}