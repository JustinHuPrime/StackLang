#include "language/stack/stackElements/substackElement.h"

#include "language/stack/stackIterator.h"

namespace StackLang
{
namespace StackElements
{
using StackLang::StackElement;
using std::to_string;

SubstackElement::SubstackElement () :
    StackElement (StackElement::DataType::Substack)
{}

SubstackElement::~SubstackElement ()
{}

SubstackElement::operator const string () const
{
    string buffer = SUBSTACK_BEGIN;

    for (auto elm : data)
    {
        buffer += string (*elm);
        buffer += SUBSTACK_SEPARATOR;
    }

    buffer.erase (buffer.length () - 2);

    buffer += SUBSTACK_END;

    return buffer; //return value optimization
}

const Stack& SubstackElement::getData ()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang