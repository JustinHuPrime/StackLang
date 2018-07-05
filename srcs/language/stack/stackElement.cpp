#include "language/stack/stackElement.h"

#include "language/exceptions/parserException.h"
#include "language/stack/stackElements/booleanElement.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/numberElement.h"
#include "language/stack/stackElements/stringElement.h"
#include "language/stack/stackElements/substackElement.h"
#include "language/stack/stackElements/typeElement.h"
#include "utils/stringUtils.h"

#include <algorithm>
#include <csignal>
#include <stack>
#include <string>

namespace StackLang
{
using Exceptions::ParserException;
using StackElements::BooleanElement;
using StackElements::CommandElement;
using StackElements::NumberElement;
using StackElements::StringElement;
using StackElements::SubstackElement;
using StackElements::TypeElement;
using std::any_of;
using std::find;
using std::stack;
using std::stod;
using std::to_string;
using Util::ends_with;
using Util::findImproperEscape;
using Util::removeChar;
using Util::starts_with;
using Util::trim;
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
    if (s.empty ())
    {
        throw ParserException ("Given input is empty.", s, 0);
    }

    else if (isdigit (s[0]) || NumberElement::NUMBER_SIGNS.find (s[0]) != string::npos) //looks like a number.
    {
        if (s.find_first_not_of (NumberElement::ALLOWED_NUMBER) == string::npos) //has allowed chars
        {
            if (count (s.begin (), s.end (), '.') + count (s.begin (), s.end (), '/') <= 1)
            {
                if (s.find_last_of (NumberElement::NUMBER_SIGNS) == 0 || s.find_last_of (NumberElement::NUMBER_SIGNS) == string::npos)
                {
                    if (removeChar (s, '\'').find_first_of ('/') != removeChar (s, '\'').length () - 1)
                    {
                        return new NumberElement (removeChar (s, '\''));
                    }
                    else
                    {
                        throw ParserException ("Looks like a fraction, but has an empty denominator.", s, s.find_last_of ('/'));
                    }
                }
                else
                {
                    throw ParserException ("Looks like a number, but has a sign in the middle.", s, s.find_first_of (NumberElement::NUMBER_SIGNS));
                }
            }
            else
            {
                if (count (s.begin (), s.end (), '.') > 1) //more than one dot
                {
                    throw ParserException ("Looks like a number, but has more than one deminal point.", s, s.find ('.', s.find ('.') + 1));
                }
                else if (count (s.begin (), s.end (), '/') > 1) //has more than one slash
                {
                    throw ParserException ("Looks like a number, but has more than one fraction bar.", s, s.find ('/', s.find ('/') + 1));
                }
                else //dot and slash
                {
                    throw ParserException ("Looks like a number, but has a decimal point in a fraction.", s, s.find_first_of ("."));
                }
            }
        }
        else
        {
            throw ParserException ("Looks like a number, but has an unexpected character.", s, s.find_first_not_of (NumberElement::ALLOWED_NUMBER));
        }
    }
    else if (starts_with (s, "\"")) //starts with a quote
    {
        if (ends_with (s, "\"")) //has quotes on either end
        {
            if (findImproperEscape (s.substr (1, s.length () - 2)) == string::npos)
            {
                return new StringElement (unescape (s.substr (1, s.length () - 2)));
            }
            else
            {
                throw ParserException ("Looks like a string, but has an invalid escape sequence", s, findImproperEscape (s));
            }
        }
        else
        {
            throw ParserException ("Looks like a string, but is missing a closing quote.", s, s.length ());
        }
    }
    else if (starts_with (s, "<<") && ends_with (s, ">>")) //has a pair of substack delmiters on either end - must be a substack.
    {
        int parseLevel = 0;
        bool inString = false;
        string accumulator = "";
        char lastChar = '\0';
        Stack buffer;

        for (auto iter = s.begin () + 2; iter != s.end () - 2; iter++) //loop between the << and >>'s
        {
            accumulator += *iter;

            if (!inString && *iter == ',' && parseLevel == 0) //found a comma at top level - our responsibility
            {
                accumulator.erase (accumulator.end () - 1); //remove the comma
                buffer.push (parse (trim (accumulator)));
                accumulator = "";
            }
            else if (*iter == '"' && lastChar != '\\') //found an unescaped quote
            {
                inString = !inString;
            }
            else if (!inString && *iter == '<' && lastChar == '<') //start of substack
            {
                parseLevel++;
            }
            else if (!inString && *iter == '>' && lastChar == '>') //end of substack
            {
                parseLevel--;
                if (parseLevel < 0)
                {
                    throw ParserException ("Missing at least one matching opening substack delimiter.", s, iter - s.begin () - 1);
                }
            }

            lastChar = *iter;
        }

        if (parseLevel != 0)
        {
            throw ParserException ("Missing " + to_string (parseLevel) + " closing substack delimiter" + (parseLevel == 1 ? "" : "s") + ".", s, s.length () - 1);
        }
        else if (trim (accumulator) != "") //allow empty lists and terminal commas
        {
            buffer.push (parse (trim (accumulator)));
        }

        SubstackElement* elm = new SubstackElement ();
        Stack& elmData = elm->getData ();

        while (!buffer.empty ())
        {
            elmData.push (buffer.pop ());
        }

        return elm;
    }
    else if (s == BooleanElement::TSTR || s == BooleanElement::FSTR) //it's either true or false
    {
        return new BooleanElement (s == "true");
    }
    else if (find (TypeElement::TYPES, TypeElement::TYPES + 7, s) != TypeElement::TYPES + 7) //exists in TYPES
    {                                                                                        //is a type
        return new TypeElement (DataType (find (TypeElement::TYPES, TypeElement::TYPES + 7, s) - TypeElement::TYPES));
    }
    else if (isalpha (s[0])) //starts with a character
    {
        if (s.find_first_not_of (CommandElement::ALLOWED_COMMAND) == string::npos) //has only allowed characters
        {
            return new CommandElement (s);
        }
        else
        {
            unsigned badIndex = s.find_first_not_of (CommandElement::ALLOWED_COMMAND);
            if (s[badIndex] == ' ') //has a space
            {
                throw ParserException ("Input looks like a command, but has a space.", s, badIndex);
            }
            else
            {
                throw ParserException ("Input looks like a command, but has a symbol that is not `-`, `?`, or `*`.", s, badIndex);
            }
        }
    }
    else //error case
    {
        throw ParserException ("Input doesn't look like any type - does it begin with a symbol?", s, 0);
    }
}
} // namespace StackLang