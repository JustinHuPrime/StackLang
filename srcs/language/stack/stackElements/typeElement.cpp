#include "language/stack/stackElements/typeElement.h"

namespace StackLang
{
namespace StackElements
{
using StackLang::StackElement;

const string TypeElement::TYPES[] = {"Number", "String", "Boolean", "Substack", "Type", "Command", "Any"};

TypeElement::TypeElement (DataType type) :
    StackElement (StackElement::DataType::Type), data (type)
{}

TypeElement* TypeElement::clone () const
{
    return new TypeElement (*this);
}

TypeElement::operator const string () const
{
    return to_string (data);
}

StackElement::DataType& TypeElement::getData ()
{
    return data;
}

string TypeElement::to_string (StackElement::DataType type)
{
    return TYPES[int(type)];
}
} // namespace StackElements
} // namespace StackLang