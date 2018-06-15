#include "language/stackElements/typeElement.h"
using namespace StackLang;
using namespace StackElements;
using namespace TypeNameConstants;

TypeElement::TypeElement (DataType type) : StackElement (StackElement::DataType::Type), data (type)
{}

TypeElement::operator const string () const
{
    return TYPES[(int) data];
}

StackElement::DataType& TypeElement::getData ()
{
    return data;
}