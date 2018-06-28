#include "language/stack/stackElements/numberElement.h"

#include <cmath>
#include <string>

namespace StackLang
{
namespace StackElements
{
using std::to_string;

NumberElement::NumberElement (double d) :
    StackElement (StackElement::DataType::Number), data (d)
{}

NumberElement::operator const string () const
{
    return to_string (data);
}

double NumberElement::getData ()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang