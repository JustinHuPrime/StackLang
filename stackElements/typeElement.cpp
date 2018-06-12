#include "stackElements/typeElement.h"
#include <string>
#include <stdexcept>
using namespace StackElements;
using std::string;
using std::to_string;
using std::invalid_argument;

namespace StackElements
{
    namespace TypeNameConstants
    {
        const char* TYPES [] = {"Number", "String", "Boolean", "Substack", "Type", "Command"};
    }
}

using namespace TypeNameConstants;

TypeElement::TypeElement (DataType type) : StackElement (StackElement::DataType::Type), data (type)
{}

TypeElement::operator const char* () const
{
    return TYPES[(int) data];
}