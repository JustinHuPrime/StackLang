#include "stackElements/stringElement.h"
#include "stringUtils.h"
#include <string>
using namespace StackElements;
using std::string;

StringElement::StringElement (string s) : StackElement (StackElement::DataType::String), data (s)
{}

StringElement::operator const char* () const
{
    return ("\"" + escape (data) + "\"").c_str ();
}