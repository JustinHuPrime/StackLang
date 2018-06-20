#include "language/stackElement.h"
#include "language/stackElements/numberElement.h"
#include "language/stackElements/stringElement.h"
#include "language/stackElements/booleanElement.h"
#include "language/stackElements/commandElement.h"
#include "language/stackElements/typeElement.h"
#include "language/stackElements/substackElement.h"
#include "language/exceptions/parserError.h"
#include "utils/stringUtils.h"
#include <algorithm>
#include <string>
using namespace StackLang;
using namespace StackElements;
using namespace Exceptions;
using namespace BooleanConstants;
using namespace CharSet;

StackElement::DataType StackElement::getType () const
{
    return dataType;
}

StackElement::StackElement (DataType type) : dataType (type)
{}

StackElement::~StackElement ()
{}

//ASSUME: s is not empty
StackElement* StackElement::parse (const string& s)
{
    if (s[0] != '\'' && s.find_first_not_of (ALLOWED_NUMBER) == string::npos && count (s.begin (), s.end (), '.') <= 1) //is a number
    {
        return new NumberElement (stod (removeChar (s, '\'')));
    }
    else if (starts_with (s, "\"") && ends_with (s, "\"") && findImproperEscape (s) == string::npos) //is a string
    {
        return new StringElement (unescape (s.substr (1, s.length () - 2)));
    }
    else if (s == TSTR || s == FSTR) //it's either true or false
    {
        return new BooleanElement (s == "true");
    }
    //else if (/* types contains this string */false)
    //{
    //    return new TypeElement ()
    //}
    else if (isalpha(s[0]) && s.find_first_not_of (ALLOWED_COMMAND) == string::npos) //it's a valid command name - starts with an alpha, had all
    {
        return new CommandElement (s);
    }
    else
    {
        if (isdigit (s[0])) //kinda looks like a number.
        {
            if (!all_of (s.begin (), s.end (), [] (char c) {return isdigit (c) || c == '.';})) //doesn't have all numbers
            {
                throw ParserError ("Input looks like a number, but has non-numeric characters.", s, s.find_first_not_of (ALLOWED_NUMBER));
            }
            else if (!(count (s.begin (), s.end (), '.') <= 1)) //has more than one dot
            {
                throw ParserError ("Input looks like a number, but has more than one deminal point.", s, s.find ('.', s.find ('.') + 1));
            }
        }
        else if (s[0] == '"') //starts with a quote
        {
            throw ParserError ("Input looks like a string, but has an invalid escape sequence", s, findImproperEscape (s));
        }
        else if (isalpha (s[0])) //starts with an alphabetical character
        {
            throw ParserError ("Input looks like a command, but has a special character that is not -, ?, or *.", s, s.find_first_not_of (ALLOWED_COMMAND));
        }
        else //Starts with a symbol, I guess?
        {
            throw ParserError ("Input doesn't look like any type - does it begin with a special character?", s, 0);
        }
    }

    return nullptr; //shouldn't get here - keep the compiler happy
}