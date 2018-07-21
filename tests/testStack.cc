#include "language/exceptions/stackOverflowError.h"
#include "language/exceptions/stackUnderflowError.h"
#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/numberElement.h"
#include "language/stack/stackIterator.h"

#include <assert.h>
#include <iostream>
#include <limits>

void testStack() {
  using StackLang::Stack;
  using StackLang::StackElement;
  using StackLang::Exceptions::StackOverflowError;
  using StackLang::Exceptions::StackUnderflowError;
  using StackLang::StackElements::NumberElement;
  using std::cerr;
  using std::numeric_limits;

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
  assert(s.pop() == elm1);
  assert(s.pop() == elm2);
  assert(s.pop() == elm3);
  assert(s.getLimit() == numeric_limits<unsigned long>().max());
  assert(s.size() == 0);
  s.push(elm1);
  s.push(elm2);
  assert(s.size() == 2);
  Stack sPrime(s);
  auto iter = sPrime.begin();
  assert((dynamic_cast<const NumberElement*>(*iter))->getData() == 2);
  iter++;
  assert((dynamic_cast<const NumberElement*>(*iter))->getData() == 1);
  ++iter;
  assert(iter == sPrime.end());
  assert(iter != s.begin());
  assert(iter != sPrime.begin());
  sPrime.clear();
  try {
    s.setLimit(3);
  } catch (...) {
    cerr << " FAILED: Exception thrown, but expected none.\n";
    assert(false);
  }
  assert(s.getLimit() == 3);
  try {
    s.setLimit(1);
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const StackOverflowError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
  try {
    s.setLimit(2);
  } catch (...) {
    cerr << " FAILED: Exception thrown, but expected none.\n";
    assert(false);
  }
  try {
    s.push(elm3);
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const StackOverflowError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
  try {
    s.pop();
    s.pop();
  } catch (...) {
    cerr << " FAILED: Exception thrown, but expected none.\n";
    assert(false);
  }
  try {
    s.pop();
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const StackUnderflowError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
  s.push(elm1);
  s.pop();
  try {
    s.top();
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const StackUnderflowError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
  delete elm1;
  delete elm2;
  delete elm3;
}