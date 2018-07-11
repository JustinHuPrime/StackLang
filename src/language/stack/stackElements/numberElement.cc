#include "language/stack/stackElements/numberElement.h"

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

#include <cmath>
#include <string>

namespace StackLang {
namespace StackElements {
using StackLang::Exceptions::ParserException;
using std::to_string;
using Util::removeChar;

const char* NumberElement::ALLOWED_NUMBER = "-+/1234567890.'";
const char* NumberElement::NUMBER_SIGNS = "-+";

NumberElement* NumberElement::parse(const string& s) {
  if (s.find_first_not_of(NumberElement::ALLOWED_NUMBER) ==
      string::npos)  // has allowed chars
  {
    if (count(s.begin(), s.end(), '.') + count(s.begin(), s.end(), '/') <= 1) {
      if (s.find_last_of(NumberElement::NUMBER_SIGNS) == 0 ||
          s.find_last_of(NumberElement::NUMBER_SIGNS) == string::npos) {
        if (removeChar(s, '\'').find_first_of('/') !=
            removeChar(s, '\'').length() - 1) {
          return new NumberElement(removeChar(s, '\''));
        } else {
          throw ParserException(
              "Looks like a fraction, but has an empty denominator.", s,
              s.find_last_of('/'));
        }
      } else {
        throw ParserException(
            "Looks like a number, but has a sign in the middle.", s,
            s.find_first_of(NumberElement::NUMBER_SIGNS));
      }
    } else {
      if (count(s.begin(), s.end(), '.') > 1)  // more than one dot
      {
        throw ParserException(
            "Looks like a number, but has more than one deminal point.", s,
            s.find('.', s.find('.') + 1));
      } else if (count(s.begin(), s.end(), '/') > 1)  // has more than one slash
      {
        throw ParserException(
            "Looks like a number, but has more than one fraction bar.", s,
            s.find('/', s.find('/') + 1));
      } else  // dot and slash
      {
        throw ParserException(
            "Looks like a number, but has a decimal point in a fraction.", s,
            s.find_first_of("."));
      }
    }
  } else {
    throw ParserException(
        "Looks like a number, but has an unexpected character.", s,
        s.find_first_not_of(NumberElement::ALLOWED_NUMBER));
  }
}

NumberElement::NumberElement(string d)
    : StackElement(StackElement::DataType::Number) {
  if (d.find('.') == string::npos) {
    data = mpq_class(d, 10);
  } else {
    int exponent = d.length() - 1 - d.find('.');
    data = mpq_class(d.erase(d.find('.'), 1));
    for (int i = 0; i < exponent; i++) {
      data /= 10;
    }
  }

  data.canonicalize();
}

NumberElement* NumberElement::clone() const {
  return new NumberElement(data.get_str());
}

NumberElement::operator const string() const { return data.get_str(); }

mpq_class& NumberElement::getData() { return data; }
}  // namespace StackElements
}  // namespace StackLang