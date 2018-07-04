#include "language/stack/stackElements/booleanElement.h"

namespace StackLang
{
namespace StackElements
{

BooleanElement::BooleanElement (bool b) :
    StackElement (StackElement::DataType::Boolean), data (b)
{}

BooleanElement* BooleanElement::clone () const
{
    return new BooleanElement (*this);
}

BooleanElement::operator const string () const
{
    return data ? TSTR : FSTR;
}

bool& BooleanElement::getData ()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang