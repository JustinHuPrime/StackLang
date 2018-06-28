#include "language/stack/stackElements/booleanElement.h"

namespace StackLang
{
namespace StackElements
{

BooleanElement::BooleanElement (bool b) :
    StackElement (StackElement::DataType::Boolean), data (b)
{}

BooleanElement::operator const string () const
{
    return data ? TSTR : FSTR;
}

bool BooleanElement::getData ()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang