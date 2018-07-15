#include "language/stack/stackElements/commandElement.h"

#include "language/exceptions/parserException.h"

namespace StackLang {
namespace StackElements {
using StackLang::Exceptions::ParserException;

const char* CommandElement::COMMAND_LDELIM = "<";
const char* CommandElement::COMMAND_RDELIM = ">";
const char* CommandElement::ALLOWED_COMMAND =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";

CommandElement* CommandElement::parse(const string& s) {
  if (s.find_first_not_of(CommandElement::ALLOWED_COMMAND) ==
      string::npos)  // has only allowed characters
  {
    return new CommandElement(s);
  } else {
    unsigned badIndex = s.find_first_not_of(CommandElement::ALLOWED_COMMAND);
    if (s[badIndex] == ' ')  // has a space
    {
      throw ParserException("Input looks like a command, but has a space.", s,
                            badIndex);
    } else {
      throw ParserException(
          "Input looks like a command, but has a symbol "
          "that is not `-`, `?`, or `*`.",
          s, badIndex);
    }
  }
}

CommandElement::CommandElement(string s)
    : StackElement(StackElement::DataType::Command), data(s) {}

CommandElement* CommandElement::clone() const {
  return new CommandElement(data);
}

CommandElement::operator const string() const {
  return COMMAND_LDELIM + data + COMMAND_RDELIM;
}

const string& CommandElement::getData() const { return data; }
}  // namespace StackElements
}  // namespace StackLang