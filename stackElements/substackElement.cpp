#include "stackElements/substackElement.h"
#include "stackElement.h"
#include <string>
#include <list>
using namespace StackElements;
using std::string;
using std::to_string;
using std::list;

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

const list<StackElement*>& SubstackElement::getData () const
{
    return data;
}