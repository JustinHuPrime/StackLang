#include "language/stack/stackElements/commandElement.h"

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace StackLang {
namespace StackElements {
using StackLang::Exceptions::ParserException;
using Util::removeChar;

const char* CommandElement::COMMAND_LDELIM = "<";
const char* CommandElement::COMMAND_RDELIM = ">";
const char* CommandElement::ALLOWED_COMMAND =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";
const char CommandElement::QUOTE_CHAR = '`';

CommandElement* CommandElement::parse(const string& s) {
  if (s.find_first_not_of(CommandElement::ALLOWED_COMMAND, 1) == string::npos &&
      (isalpha(s[0]) ||
       (s[0] == QUOTE_CHAR && s.length() >= 2 &&
        isalpha(s[1]))))  // has only allowed characters, starts with a
                          // quote char and a letter or a letter
  {
    return new CommandElement(removeChar(s, QUOTE_CHAR), s[0] == QUOTE_CHAR);
  } else {
    unsigned badIndex = s.find_first_not_of(CommandElement::ALLOWED_COMMAND,
                                            s[0] == QUOTE_CHAR ? 1 : 0);
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

CommandElement::CommandElement(const string& s, bool isQuoted) noexcept
    : StackElement(StackElement::DataType::Command),
      data(s),
      quoted(isQuoted) {}

CommandElement* CommandElement::clone() const noexcept {
  return new CommandElement(data, quoted);
}

CommandElement::operator const string() const noexcept {
  return (quoted ? string(1, QUOTE_CHAR) : "") + COMMAND_LDELIM + data +
         COMMAND_RDELIM;
}

const string& CommandElement::getData() const noexcept { return data; }
bool CommandElement::isQuoted() const noexcept { return quoted; }
}  // namespace StackElements
}  // namespace StackLang