#include "language/stackElements/commandElement.h"
using namespace StackLang::StackElements;

CommandElement::CommandElement (string s) : StackElement (StackElement::DataType::Command), data (s)
{}

CommandElement::operator const string () const
{
    return "<" + data + ">";
}

string& CommandElement::getData ()
{
    return data;
}