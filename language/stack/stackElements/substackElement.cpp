#include "language/stack/stackElements/substackElement.h"
using namespace StackLang;
using namespace StackElements;
using namespace SubstackConstants;

SubstackElement::SubstackElement () :
    StackElement (StackElement::DataType::Substack)
{}

SubstackElement::~SubstackElement ()
{
    for (StackElement* elm : data)
    {
        delete elm;
    }
}

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

const list< StackElement* >& SubstackElement::getData ()
{
    return data;
}