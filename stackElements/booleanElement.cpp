#include "stackElements/booleanElement.h"
#include <string>
using namespace StackElements;
using std::string;

namespace StackElements
{
    namespace BooleanConstants
    {
        const string TCSTR = "true";
        const string FCSTR = "false";
    }
}

using namespace BooleanConstants;

BooleanElement::BooleanElement () : StackElement (StackElement::DataType::Boolean)
{}

BooleanElement::operator const char* () const
{
    return (data ? TCSTR : FCSTR).c_str ();
}