#include "stackElements/commandElement.h"
#include <string>
using namespace StackElements;
using std::string;

CommandElement::CommandElement (string s) : StackElement (StackElement::DataType::Command), data (s)
{}

CommandElement::operator const string () const
{
    return "<" + data + ">";
}

const string& CommandElement::getData () const
{
    return data;
}