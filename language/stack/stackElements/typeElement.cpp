#include "language/stack/stackElements/typeElement.h"
using namespace StackLang;
using namespace StackElements;
using namespace TypeNameConstants;

TypeElement::TypeElement (DataType type) :
    StackElement (StackElement::DataType::Type), data (type)
{}

TypeElement::operator const string () const
{
    return to_string (data);
}

StackElement::DataType TypeElement::getData ()
{
    return data;
}

string TypeElement::to_string (StackElement::DataType type)
{
    return TYPES[int(type)];
}