#include "stackElements/typeElement.h"
#include <string>
using namespace StackElements;
using namespace TypeNameConstants;
using std::string;

TypeElement::TypeElement (DataType type) : StackElement (StackElement::DataType::Type), data (type)
{}

TypeElement::operator const string () const
{
    return TYPES[(int) data];
}