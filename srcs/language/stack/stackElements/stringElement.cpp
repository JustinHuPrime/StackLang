#include "language/stack/stackElements/stringElement.h"

#include "utils/stringUtils.h"

namespace StackLang
{
namespace StackElements
{
using Util::escape;

StringElement::StringElement(string s) :
    StackElement(StackElement::DataType::String),
    data(s)
{}

StringElement* StringElement::clone() const
{
    return new StringElement(*this);
}

StringElement::operator const string() const
{
    return "\"" + escape(data) + "\"";
}

string& StringElement::getData()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang