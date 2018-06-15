#include "language/stackElements/stringElement.h"
using namespace StackLang::StackElements;

StringElement::StringElement (string s) : StackElement (StackElement::DataType::String), data (s)
{}

StringElement::operator const string () const
{
    return "\"" + escape (data) + "\"";
}

const string& StringElement::getData ()
{
    return data;
}