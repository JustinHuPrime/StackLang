#include "language/exceptions/typeError.h"

namespace StackLang
{
namespace Exceptions
{
using StackLang::StackElements::TypeElement;

TypeError::TypeError (StackElement::DataType expected, StackElement* given) :
    LanguageError ("Expected " + TypeElement::to_string (expected) + "\nGiven " + string (*given))
{}

const string TypeError::getKind () const
{
    return "Type Mismatch:";
}
} // namespace Exceptions
} // namespace StackLang