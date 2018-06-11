#include "stackElements/substackElement.h"
#include <string>
using namespace StackElements;
using std::string;
using std::to_string;

namespace StackElements
{
    namespace SubstackConstants
    {
        const string SUBSTACK_BEGIN = "<<";
        const string SUBSTACK_END = ">>";
        const string SUBSTACK_SEPARATOR = ", ";
    }
}

using namespace SubstackConstants;

SubstackElement::SubstackElement () : StackElement (StackElement::DataType::Substack)
{}

SubstackElement::operator const char* () const
{
    string buffer = SUBSTACK_BEGIN;

    for (auto elm : data)
    {
        buffer += (const char*) *elm;
        buffer += SUBSTACK_SEPARATOR;
    }

    buffer.erase (buffer.length () - 2);

    buffer += SUBSTACK_END;

    return buffer.c_str ();
}