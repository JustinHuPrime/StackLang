#include "language/stack/stackElement.h"

#include "language/exceptions/parserError.h"
#include "language/stack/stackElements/booleanElement.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/numberElement.h"
#include "language/stack/stackElements/stringElement.h"
#include "language/stack/stackElements/substackElement.h"
#include "language/stack/stackElements/typeElement.h"
#include "utils/stringUtils.h"

#include <algorithm>
#include <string>

namespace StackLang
{
using Exceptions::ParserError;
using StackElements::ALLOWED_COMMAND;
using StackElements::ALLOWED_NUMBER;
using StackElements::BooleanElement;
using StackElements::CommandElement;
using StackElements::FSTR;
using StackElements::NumberElement;
using StackElements::StringElement;
using StackElements::SubstackElement;
using StackElements::TSTR;
using std::stod;
using Util::ends_with;
using Util::findImproperEscape;
using Util::removeChar;
using Util::starts_with;
using Util::unescape;

StackElement::DataType
        StackElement::getType () const
{
    return dataType;
}

StackElement::StackElement (DataType type) :
    dataType (type)
{}

StackElement::~StackElement ()
{}

//ASSUME: s is not empty
StackElement* StackElement::parse (const string& s)
{
    if (s[0] != '\'' && s.find_first_not_of (ALLOWED_NUMBER) == string::npos && count (s.begin (), s.end (), '.') <= 1) //is a number
    {
        return new NumberElement (removeChar (s, '\''));
    }
    else if (starts_with (s, "\"") && ends_with (s, "\"") && findImproperEscape (s.substr (1, s.length () - 2)) == string::npos) //is a string
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
    else if (isalpha (s[0]) && s.find_first_not_of (ALLOWED_COMMAND) == string::npos) //it's a valid command name - starts with an alpha, had all
    {
        return new CommandElement (s);
    }
    else
    {
        if (isdigit (s[0])) //kinda looks like a number.
        {
            if (!all_of (s.begin (), s.end (), [](char c) { return isdigit (c) || c == '.'; })) //doesn't have all numbers
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
            unsigned badIndex = s.find_first_not_of (ALLOWED_COMMAND);
            if (s[badIndex] == ' ') //has a space
            {
                throw ParserError ("Input looks like a command, but has a space.", s, badIndex);
            }
            else
            {
                throw ParserError ("Input looks like a command, but has a symbol that is not a `-`, `?`, or `*`.", s, badIndex);
            }
        }
        else //Starts with a symbol, I guess?
        {
            throw ParserError ("Input doesn't look like any type - does it begin with a symbol?", s, 0);
        }
    }

    return nullptr; //shouldn't get here - keep the compiler happy
}
} // namespace StackLang