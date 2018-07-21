#include "util/stringUtils.h"

#include <assert.h>
#include <string>

void testStringUtils() noexcept {
  using std::string;
  using Util::ends_with;
  using Util::escape;
  using Util::findImproperEscape;
  using Util::removeChar;
  using Util::spaces;
  using Util::starts_with;
  using Util::trim;
  using Util::unescape;

  assert(starts_with("abcd", "ab"));
  assert(!starts_with("abcd", "ad"));
  assert(starts_with("abcd", "abcd"));
  assert(ends_with("abcd", "cd"));
  assert(!ends_with("abcd", "ce"));
  assert(ends_with("abcd", "abcd"));
  assert(escape("a\\b\"c\nd") == "a\\\\b\\\"c\\nd");
  assert(unescape("b\\\"a\\\\c\\nd") == "b\"a\\c\nd");
  assert(unescape(escape("a\\b\"c\n")) == "a\\b\"c\n");
  assert(escape(unescape("a\\\\c\\nb\\\"d")) == "a\\\\c\\nb\\\"d");
  assert(findImproperEscape("012345\\") == 6);
  assert(findImproperEscape("lotsa stuff \\\\") == string::npos);
  assert(findImproperEscape("0123456\\aother stuff goes here.") == 7);
  assert(findImproperEscape("012\\\\stuff after this...") == string::npos);
  assert(findImproperEscape("012\"stuff after this...") == 3);
  assert(findImproperEscape("012\\\\\"stuff after this...") == 5);
  assert(spaces(5) == "     ");
  assert(spaces(0) == "");
  assert(removeChar("aabaacdebba", 'a') == "bcdebb");
  assert(removeChar("doesn't contain that letter", 'j') ==
         "doesn't contain that letter");
  assert(trim(" \n\tabc \n\tdef\n \t") == "abc \n\tdef");
  assert(trim("no whitespace to trim") == "no whitespace to trim");
}