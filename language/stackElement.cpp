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
using namespace StackLang::StackElements;
using namespace BooleanConstants;
using std::invalid_argument;
using std::all_of;
using std::find;
using std::count;
using std::stod;
using std::to_string;

namespace StackLang
{
    StackElement::DataType StackElement::getType () const
    {
        return dataType;
    }

    StackElement::StackElement (DataType type) : dataType (type)
    {}

    //ASSUME: s is not empty
    StackElement* StackElement::parse (const string& s)
    {
        if (all_of (s.begin (), s.end (), [] (char c) {return isdigit (c) || c == '.';}) && count (s.begin (), s.end (), '.') <= 1) //is a number
        {
            return new NumberElement (stod (s));
        }
        else if (starts_with (s, "\"") && ends_with (s, "\"") && properlyEscaped (s)) //is a string
        {
            return new StringElement (unescape (s.substr (1, s.length () - 2)));
        }
        else if (s == TSTR || s == FSTR) //it's either true or false
        {
            return new BooleanElement (s == "true");
        }
        else if (isalpha(s[0]) && all_of (s.begin (), s.end (), [] (char c) {return isalnum (c) || c == '-' || c == '?';})) //it's a valid command name
        {
            return new CommandElement (s);
        }
        else
        {
            if (isdigit (s[0])) //kinda looks like a number.
            {
                if (!all_of (s.begin (), s.end (), [] (char c) {return isdigit (c) || c == '.';})) //doesn't have all numbers
                {
                    throw invalid_argument ("Parser error: input looks like a number, but has non-number characters.");
                }
                else if (!(count (s.begin (), s.end (), '.') <= 1)) //has more than one dot
                {
                    throw invalid_argument ("Parser error: input looks like a number, but has more than one decimal point.");
                }
            }
            else if (s[0] == '"') //starts with a quote
            {
                throw invalid_argument ("Parser error: input looks like a string, but has invalid escape sequences.");
            }
            else if (isalpha (s[0])) //starts with an alphabetical character
            {
                throw invalid_argument ("Parser error: input looks like a command, but has invalid characters.");
            }
            else //Starts with a symbol, I guess?
            {
                throw invalid_argument ("Parser error: input doesn't look like any type.");
            }
        }
    }
}