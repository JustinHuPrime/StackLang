#include "stackElements/booleanElement.h"
#include <string>
using namespace StackElements;
using std::string;

const string BooleanElement::TSTR = "true";
const string BooleanElement::FSTR = "false";

BooleanElement::BooleanElement (bool b) : StackElement (StackElement::DataType::Boolean), data (b)
{}

BooleanElement::operator const char* () const
{
    return (data ? TSTR : FSTR).c_str ();
}