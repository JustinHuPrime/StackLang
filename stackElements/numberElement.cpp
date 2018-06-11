#include "stackElements/numberElement.h"
#include <string>
using namespace StackElements;
using std::string;
using std::to_string;

NumberElement::NumberElement () : StackElement (StackElement::DataType::Number)
{}

NumberElement::operator const char* () const
{
    return to_string (data).c_str ();
}