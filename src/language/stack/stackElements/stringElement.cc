#include "language/stack/stackElements/stringElement.h"

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace stackelements {
using stacklang::exceptions::ParserException;
using util::ends_with;
using util::escape;
using util::findImproperEscape;
using util::unescape;

const char StringElement::QUOTE_CHAR = '"';

StringElement* StringElement::parse(const string& s) {
  if (ends_with(s, "\""))  // has quotes on either end
  {
    if (findImproperEscape(s.substr(1, s.length() - 2)) == string::npos) {
      return new StringElement(unescape(s.substr(1, s.length() - 2)));
    } else {
      throw ParserException(
          "Looks like a string, but has an invalid escape sequence", s,
          findImproperEscape(s));
    }
  } else {
    throw ParserException(
        "Looks like a string, but is missing a closing quote.", s, s.length());
  }
}

StringElement::StringElement(string s) noexcept
    : StackElement(StackElement::DataType::String), data(s) {}

StringElement* StringElement::clone() const noexcept {
  return new StringElement(data);
}

StringElement::operator string() const noexcept {
  return "\"" + escape(data) + "\"";
}

const string& StringElement::getData() const noexcept { return data; }
}  // namespace stackelements
}  // namespace stacklang