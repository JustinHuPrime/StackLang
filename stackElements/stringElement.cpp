#include "stackElements/stringElement.h"
#include "stringUtils.h"
#include <string>
using namespace StackElements;
using std::string;

StringElement::StringElement (string s) : StackElement (StackElement::DataType::String), data (s)
{}

StringElement::operator const string () const
{
    return "\"" + escape (data) + "\"";
}

string& StringElement::getData ()
{
    return data;
}