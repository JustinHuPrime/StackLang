#include "language/exceptions/typeError.h"
using namespace StackLang;
using namespace Exceptions;
using StackElements::TypeElement;

TypeError::TypeError (StackElement::DataType expected, StackElement* given) :
    LanguageError ("Expected " + TypeElement::to_string (expected) + "\nGiven " + string (*given))
{}

const string TypeError::getKind () const
{
    return "Type Mismatch:";
}