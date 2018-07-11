#include "language/stack/stackElement.h"

#include "language/exceptions/parserException.h"
#include "language/stack/stackElements/booleanElement.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/numberElement.h"
#include "language/stack/stackElements/stringElement.h"
#include "language/stack/stackElements/substackElement.h"
#include "language/stack/stackElements/typeElement.h"
#include "util/stringUtils.h"

#include <algorithm>
#include <csignal>
#include <iterator>
#include <stack>
#include <string>

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
                  s[0]) !=
                 NumberElement::NUMBER_SIGNS +
                     strlen(
                         NumberElement::NUMBER_SIGNS))  // looks like a number.
  {
    return NumberElement::parse(s);
  } else if (starts_with(s, "\""))  // starts with a quote
  {
    return StringElement::parse(s);
  } else if (starts_with(s, "<<") &&
             ends_with(s, ">>"))  // has a pair of substack delmiters on either
                                  // end - must be a substack.
  {
    return SubstackElement::parse(s);
  } else if (s == BooleanElement::TSTR ||
             s == BooleanElement::FSTR)  // it's either true or false
  {
    return new BooleanElement(s == "true");
  } else if (find(begin(TypeElement::TYPES()), end(TypeElement::TYPES()), s) !=
             end(TypeElement::TYPES()))  // exists in types
  {                                      // is a type
    return new TypeElement(DataType(
        find(begin(TypeElement::TYPES()), end(TypeElement::TYPES()), s) -
        begin(TypeElement::TYPES())));
  } else if (isalpha(s[0]))  // starts with a character
  {
    return CommandElement::parse(s);
  } else  // error case
  {
    throw ParserException(
        "Input doesn't look like any type - does it begin with a symbol?", s,
        0);
  }
}

StackElement::~StackElement() {}

StackElement::DataType StackElement::getType() const { return dataType; }

StackElement::StackElement(DataType type) : dataType(type) {}
}  // namespace StackLang