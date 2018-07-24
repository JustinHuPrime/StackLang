#include "language/stack/stackElement.h"

#include <algorithm>
#include <iterator>
#include <stack>
#include <string>

#include "language/exceptions/parserException.h"
#include "language/stack/stackElements/booleanElement.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/numberElement.h"
#include "language/stack/stackElements/stringElement.h"
#include "language/stack/stackElements/substackElement.h"
#include "language/stack/stackElements/typeElement.h"
#include "util/stringUtils.h"

namespace StackLang {
using Exceptions::ParserException;
using StackElements::BooleanElement;
using StackElements::CommandElement;
using StackElements::NumberElement;
using StackElements::StringElement;
using StackElements::SubstackElement;
using StackElements::TypeElement;
using std::any_of;
using std::begin;
using std::end;
using std::find;
using std::stack;
using std::stod;
using std::to_string;
using Util::ends_with;
using Util::findImproperEscape;
using Util::removeChar;
using Util::starts_with;
using Util::trim;
using Util::unescape;

// ASSUME: s is not empty
StackElement* StackElement::parse(const string& s) {
  if (s.empty()) {
    throw ParserException("Given input is empty.", s, 0);
  } else if (isdigit(s[0]) ||
             find(NumberElement::NUMBER_SIGNS,
                  NumberElement::NUMBER_SIGNS +
                      strlen(NumberElement::NUMBER_SIGNS),
                  s[0]) != NumberElement::NUMBER_SIGNS +
                               strlen(NumberElement::NUMBER_SIGNS) ||
             s[0] == NumberElement::INEXACT_SIGNAL)  // looks like a number.
  {
    return NumberElement::parse(s);
  } else if (starts_with(
                 s,
                 string(1, StringElement::QUOTE_CHAR)))  // starts with a quote
  {
    return StringElement::parse(s);
  } else if (starts_with(s, SubstackElement::SUBSTACK_BEGIN) &&
             ends_with(
                 s,
                 SubstackElement::SUBSTACK_END))  // has a pair of substack
                                                  // delmiters on either end
                                                  // - must be a substack.
  {
    return SubstackElement::parse(s);
  } else if (s == BooleanElement::TSTR ||
             s == BooleanElement::FSTR)  // it's either true or false
  {
    return new BooleanElement(s == string(BooleanElement::TSTR));
  } else if (s.find_first_of(TypeElement::PARENS) != string::npos ||
             find(TypeElement::TYPES().begin(), TypeElement::TYPES().end(),
                  s) != TypeElement::TYPES()
                            .end())  // has a subtype, or exists in types
  {                                  // is a type
    return TypeElement::parse(s);
  } else if (isalpha(s[0]) ||
             s[0] == CommandElement::QUOTE_CHAR)  // starts with a character or
                                                  // a command-quote
  {
    return CommandElement::parse(s);
  } else  // error case
  {
    throw ParserException(
        "Input doesn't look like any type - does it begin with a symbol?", s,
        0);
  }
}

StackElement::DataType StackElement::getType() const noexcept {
  return dataType;
}

StackElement::StackElement(DataType type) noexcept : dataType(type) {}
}  // namespace StackLang