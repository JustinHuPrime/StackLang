#include "language/stack/stackElements/commandElement.h"

namespace StackLang
{
namespace StackElements
{

CommandElement::CommandElement (string s) :
    StackElement (StackElement::DataType::Command), data (s)
{}

CommandElement* CommandElement::clone () const
{
    return new CommandElement (*this);
}

CommandElement::operator const string () const
{
    return "<" + data + ">";
}

string& CommandElement::getData ()
{
    return data;
}
} // namespace StackElements
} // namespace StackLang