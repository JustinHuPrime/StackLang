#include "language/stack/stackElements/booleanElement.h"
using namespace StackLang::StackElements;
using namespace BooleanConstants;

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