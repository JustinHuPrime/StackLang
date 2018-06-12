#include "stackElements/stringElement.h"
#include <string>
using namespace StackElements;
using std::string;

StringElement::StringElement (string s) : StackElement (StackElement::DataType::String), data (s)
{}

StringElement::operator const char* () const
{
    string buffer = "\"";

    for (const char c : data)
    {
        switch (c)
        {
            case '\t': buffer += "\\t"; break;
            case '\\': buffer += "\\\\"; break;
            case '"': buffer += "\\\""; break;
            default: buffer += c;
        }
    }

    buffer += "\"";

    return buffer.c_str ();
}