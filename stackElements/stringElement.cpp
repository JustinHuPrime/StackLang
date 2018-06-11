#include "stackElements/stringElement.h"
#include <string>
using namespace StackElements;
using std::string;

StringElement::StringElement () : StackElement (StackElement::DataType::String)
{}

StringElement::operator const char* () const
{
    return data.c_str ();
}