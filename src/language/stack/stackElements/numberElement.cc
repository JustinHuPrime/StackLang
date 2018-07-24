#include "language/stack/stackElements/numberElement.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace StackLang {
namespace StackElements {
using StackLang::Exceptions::ParserException;
using std::fixed;
using std::setprecision;
using std::stringstream;
using std::to_string;
using Util::removeChar;

const char* NumberElement::ALLOWED_NUMBER = "~-+/1234567890.'";
const char* NumberElement::NUMBER_SIGNS = "-+";
const char NumberElement::INEXACT_SIGNAL = '~';

NumberElement* NumberElement::parse(const string& s) {
  if (s.find_first_not_of(ALLOWED_NUMBER) == string::npos)  // has allowed chars
  {
    if (count(s.begin(), s.end(), '.') + count(s.begin(), s.end(), '/') <=
        1) {  // only one dot or decimal point
      if (count(s.begin(), s.end(),
                INEXACT_SIGNAL) <= 1) {  // only one inexact signal
        if (s.find_last_of(INEXACT_SIGNAL) == 0 ||
            s.find_last_of(INEXACT_SIGNAL) ==
                string::npos) {  // and it's at the start, if anywhere
          if (s.find_last_of(NUMBER_SIGNS) == 0 ||
              (removeChar(s, '\'').find_last_of(NUMBER_SIGNS) == 1 &&
               s.find_last_of(INEXACT_SIGNAL) == 0) ||
              s.find_last_of(NumberElement::NUMBER_SIGNS) ==
                  string::npos) {  // sign is first, second but has a tilde
                                   // before it, or doesn't exist
            if (removeChar(s, '\'').find_first_of('/') !=
                removeChar(s, '\'').length() -
                    1) {  // doesn't have a blank denominator
              if (s.find_first_of('/') == string::npos ||
                  s.substr(s.find_first_of('/') + 1).find_first_not_of('0') !=
                      string::npos) {  // doesn't have all zeroes in the
                                       // denominator.
                return new NumberElement(
                    removeChar(removeChar(s, '\''), INEXACT_SIGNAL),
                    s.find_first_of(INEXACT_SIGNAL) == string::npos);
              } else {
                throw ParserException(
                    "Fractions may not have a zero in the denominator.", s,
                    s.find_first_of('0', s.find_first_of('/')));
              }
            } else {
              throw ParserException(
                  "Looks like a fraction, but has an empty denominator.", s,
                  s.find_last_of('/'));
            }
          } else {
            throw ParserException(
                "Looks like a number, but has a sign in the middle.", s,
                s.find_last_of(NUMBER_SIGNS));
          }
        } else {
          throw ParserException(
              "Looks like a number, but has a tilde in the middle.", s,
              s.find_last_of(INEXACT_SIGNAL));
        }
      } else {
        throw ParserException(
            "Looks like a number, but has more than one tilde.", s,
            s.find(INEXACT_SIGNAL, s.find(INEXACT_SIGNAL) + 1));
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

NumberElement::NumberElement(string d, bool isExact) noexcept
    : StackElement(StackElement::DataType::Number), exact(isExact) {
  d = removeChar(d, '+');
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

NumberElement::NumberElement(const mpq_class& d, bool isExact) noexcept
    : StackElement(StackElement::DataType::Number), data(d), exact(isExact) {}

NumberElement* NumberElement::clone() const noexcept {
  return new NumberElement(data.get_str(), exact);
}

NumberElement::operator const string() const noexcept {
  if (exact) {
    return data.get_str();
  } else {
    stringstream stream;
    stream << fixed << setprecision(data.get_prec()) << data.get_d();
    string inexactSig;
    inexactSig += INEXACT_SIGNAL;
    return inexactSig + stream.str() + "...";
  }
}

const mpq_class& NumberElement::getData() const noexcept { return data; }
}  // namespace StackElements
}  // namespace StackLang