#include "language/stack/stackElements/stringElement.h"

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace StackLang {
namespace StackElements {
using StackLang::Exceptions::ParserException;
using Util::ends_with;
using Util::escape;
using Util::findImproperEscape;
using Util::unescape;

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

StringElement::StringElement(string s)
    : StackElement(StackElement::DataType::String), data(s) {}

StringElement* StringElement::clone() const { return new StringElement(data); }

StringElement::operator const string() const {
  return "\"" + escape(data) + "\"";
}

string& StringElement::getData() { return data; }
}  // namespace StackElements
}  // namespace StackLang