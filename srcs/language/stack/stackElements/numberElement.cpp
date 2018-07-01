#include "language/stack/stackElements/numberElement.h"

#include <cmath>
#include <string>

namespace StackLang
{
namespace StackElements
{
using std::to_string;

NumberElement::NumberElement (string d) :
    StackElement (StackElement::DataType::Number), data (d, 10)
{}

NumberElement::operator const string () const
{
    return data.get_str ();
}

mpq_class NumberElement::getData ()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang