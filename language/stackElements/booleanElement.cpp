#include "language/stackElements/booleanElement.h"
#include <string>
using namespace StackElements;
using namespace BooleanConstants;
using std::string;

BooleanElement::BooleanElement (bool b) : StackElement (StackElement::DataType::Boolean), data (b)
{}

BooleanElement::operator const string () const
{
    return data ? TSTR : FSTR;
}

bool& BooleanElement::getData ()
{
    return data;
}