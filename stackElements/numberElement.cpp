#include "stackElements/numberElement.h"
#include <string>
using namespace StackElements;
using std::string;
using std::to_string;

NumberElement::NumberElement (double d) : StackElement (StackElement::DataType::Number), data (d)
{}

NumberElement::operator const char* () const
{
    return to_string (data).c_str ();
}