#include "stackElements/typeElement.h"
#include <string>
using namespace StackElements;
using std::string;

namespace StackElements
{
    namespace TypeNameConstants
    {
        const char* TYPES [] = {"Number", "String", "Boolean", "Substack", "Type", "Command"};
    }
}

using namespace TypeNameConstants;

TypeElement::TypeElement () : StackElement (StackElement::DataType::Type)
{}

TypeElement::operator const char* () const
{
    return TYPES[(int) data];
}