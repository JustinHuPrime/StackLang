#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/numberElement.h"

#include <assert.h>

void testStack() {
  using StackLang::Stack;
  using StackLang::StackElement;
  using StackLang::StackElements::NumberElement;

  Stack s;
  NumberElement* elm1 = new NumberElement("1");
  NumberElement* elm2 = new NumberElement("2");

  assert(s.isEmpty());
  s.push(elm1);
  assert(!s.isEmpty());
  assert(s.top() == elm1);
  s.push(elm2);
  StackElement* disc = s.pop();
  assert(disc == elm2);
  assert(s.top() == elm1);
  delete disc;
  s.clear();
  assert(s.isEmpty());

  elm1 = new NumberElement("1");
  elm2 = new NumberElement("2");
  StackElement* elm3 = new NumberElement("3");
  s.push(elm1);
  s.push(elm2);
  s.push(elm3);
  s.reverse();

  // test copy, iterators, getSize, getLimit, setLimit,
  // exceptions on s.push, s.pop, s.top, s.setLimit
}