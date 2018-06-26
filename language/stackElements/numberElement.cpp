#include "language/stackElements/numberElement.h"
#include <cmath>
#include <string>
using namespace StackLang::StackElements;
using std::to_string;

NumberElement::NumberElement (double d) : StackElement (StackElement::DataType::Number), data (d)
{}

NumberElement::operator const string () const
{
    return to_string (data);
}

double NumberElement::getData ()
{
    return data;
}