#include "language/stack/stackElements/commandElement.h"

namespace StackLang
{
namespace StackElements
{
const string CommandElement::ALLOWED_COMMAND = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";

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