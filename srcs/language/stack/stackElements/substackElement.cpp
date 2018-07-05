#include "language/stack/stackElements/substackElement.h"

#include "language/stack/stackIterator.h"

namespace StackLang
{
namespace StackElements
{
using StackLang::StackElement;
using std::to_string;

const string SubstackElement::SUBSTACK_BEGIN = "<< ";
const string SubstackElement::SUBSTACK_END = " >>";
const string SubstackElement::SUBSTACK_SEPARATOR = ", ";
const string SubstackElement::SUBSTACK_EMPTY = "<< (empty) >>";

SubstackElement::SubstackElement() :
    StackElement(StackElement::DataType::Substack)
{}

SubstackElement* SubstackElement::clone() const
{
    return new SubstackElement(*this);
}

SubstackElement::operator const string() const
{
    if (data.size() == 0)
    {
        return SUBSTACK_EMPTY;
    }
    string buffer = SUBSTACK_BEGIN;

    for (auto elm : data)
    {
        buffer += string(*elm);
        buffer += SUBSTACK_SEPARATOR;
    }

    buffer.erase(buffer.length() - 2);

    buffer += SUBSTACK_END;

    return buffer; //return value optimization
}

Stack& SubstackElement::getData()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang