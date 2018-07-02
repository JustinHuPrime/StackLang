#include "language/stack/stackElements/numberElement.h"

#include <cmath>
#include <string>

namespace StackLang
{
namespace StackElements
{
using std::to_string;

NumberElement::NumberElement (string d) :
    StackElement (StackElement::DataType::Number)
{
    if (d.find ('.') == string::npos)
    {
        data = mpq_class (d, 10);
    }
    else
    {
        int exponent = d.length () - 1 - d.find ('.');
        data = mpq_class (d.erase (d.find ('.'), 1));
        data /= pow (10, exponent);
    }
}

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