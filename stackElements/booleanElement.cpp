#include "stackElements/booleanElement.h"
#include <string>
using namespace StackElements;
using namespace BooleanConstants;
using std::string;

BooleanElement::BooleanElement (bool b) : StackElement (StackElement::DataType::Boolean), data (b)
{}

BooleanElement::operator const char* () const
{
    return (data ? TSTR : FSTR).c_str ();
}