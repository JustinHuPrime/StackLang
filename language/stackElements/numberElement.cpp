#include "language/stackElements/numberElement.h"
using namespace StackLang::StackElements;

NumberElement::NumberElement (double d) : StackElement (StackElement::DataType::Number), data (d)
{}

NumberElement::operator const string () const
{
    return to_string (data);
}

double& NumberElement::getData ()
{
    return data;
}