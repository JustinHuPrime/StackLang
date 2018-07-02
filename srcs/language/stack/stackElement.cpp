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
#include <csignal>
#include <string>

namespace StackLang
{
using Exceptions::ParserError;
using StackElements::ALLOWED_COMMAND;
using StackElements::ALLOWED_NUMBER;
using StackElements::BooleanElement;
using StackElements::CommandElement;
using StackElements::FSTR;
using StackElements::NUMBER_SIGNS;
using StackElements::NumberElement;
using StackElements::StringElement;
using StackElements::SubstackElement;
using StackElements::TSTR;
using StackElements::TypeElement;
using StackElements::TYPES;
using std::any_of;
using std::find;
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
    if ((isdigit (s[0]) || NUMBER_SIGNS.find (s[0]) != string::npos) &&                          //is digit or sign
        s.find_first_not_of (ALLOWED_NUMBER) == string::npos &&                                  //has only allowed chars
        count (s.begin (), s.end (), '.') + count (s.begin (), s.end (), '/') <= 1 &&            //has either a dot or a slash, and at most one of each
        (s.find_last_of (NUMBER_SIGNS) == 0 || s.find_last_of (NUMBER_SIGNS) == string::npos) && //sign is at the beginning or nonexistent
        s.find_first_of ('/') != s.length () - 1)                                                //slash is not at the end
    {                                                                                            //is a number
        return new NumberElement (removeChar (s, '\''));
    }
    else if (starts_with (s, "\"") && ends_with (s, "\"") &&                     //has quotes on either end
             findImproperEscape (s.substr (1, s.length () - 2)) == string::npos) //has no improper escapes
    {                                                                            //is a string
        return new StringElement (unescape (s.substr (1, s.length () - 2)));
    }
    else if (s == TSTR || s == FSTR) //it's either true or false
    {
        return new BooleanElement (s == "true");
    }
    else if (find (TYPES, TYPES + 7, s) != TYPES + 7) //exists in TYPES
    {                                                 //is a type
        return new TypeElement (DataType (find (TYPES, TYPES + 7, s) - TYPES));
    }
    else if (isalpha (s[0]) &&                                      //starts with a character
             s.find_first_not_of (ALLOWED_COMMAND) == string::npos) //has only allowed characters
    {                                                               //it's a valid command name
        return new CommandElement (s);
    }
    else //error case
    {
        if (isdigit (s[0]) || s[0] == '-' || s[0] == '+') //kinda looks like a number.
        {
            if (any_of (s.begin (), s.end (), [](char c) { return ALLOWED_NUMBER.find (c) == string::npos; })) //doesn't have all valid chars
            {
                throw ParserError ("Input looks like a number, but has non-numeric characters.", s, s.find_first_not_of (ALLOWED_NUMBER));
            }
            else if (count (s.begin (), s.end (), '.') > 1) //has more than one dot
            {
                throw ParserError ("Input looks like a number, but has more than one deminal point.", s, s.find ('.', s.find ('.') + 1));
            }
            else if (count (s.begin (), s.end (), '/') > 1) //has more than one slash
            {
                throw ParserError ("Input looks like a number, but has more than one fraction bar.", s, s.find ('/', s.find ('/') + 1));
            }
            else if (count (s.begin (), s.end (), '.') + count (s.begin (), s.end (), '/') > 1) //dot and slash
            {
                throw ParserError ("Input looks like a number, but has a decimal point in a fraction.", s, s.find_first_of ("."));
            }
            else if (s.find_last_of (NUMBER_SIGNS) != 0 && s.find_last_of (NUMBER_SIGNS) != string::npos) //sign isn't at the start or nowhere
            {
                throw ParserError ("Input looks like a number, but has a + or - in the middle.", s, s.find_first_of (NUMBER_SIGNS));
            }
            else
            {
                throw ParserError ("Input looks like a number, but has an unknown issue.", s, 0);
            }
        }
        else if (s[0] == '"') //starts with a quote
        {
            throw ParserError ("Input looks like a string, but has an invalid escape sequence", s, findImproperEscape (s));
        }
        else if (isalpha (s[0])) //starts with an alphabetical character - this catches all booleans and types as well.
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
}
} // namespace StackLang