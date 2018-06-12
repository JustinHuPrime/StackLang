#include "stackElements/typeElement.h"
#include <string>
using namespace StackElements;
using std::string;

const string TypeElement::TYPES [] = {"Number", "String", "Boolean", "Substack", "Type", "Command"};

TypeElement::TypeElement (DataType type) : StackElement (StackElement::DataType::Type), data (type)
{}

TypeElement::operator const char* () const
{
    return TYPES[(int) data].c_str ();
}