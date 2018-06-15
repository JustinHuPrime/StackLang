#include "language/stackElements/substackElement.h"
using namespace StackLang;
using namespace StackElements;
using namespace SubstackConstants;

SubstackElement::SubstackElement () : StackElement (StackElement::DataType::Substack)
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

    return buffer.c_str ();
}

const list<StackElement*>& SubstackElement::getData ()
{
    return data;
}