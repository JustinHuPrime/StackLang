#include "language/stackElements/numberElement.h"
using namespace StackLang::StackElements;

NumberElement::NumberElement (BigDecimal d) : StackElement (StackElement::DataType::Number), data (d)
{}

NumberElement::operator const string () const
{
    return string (data);
}

BigDecimal NumberElement::getData ()
{
    return data;
}