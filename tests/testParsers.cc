#include "language/exceptions/parserException.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/booleanElement.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/numberElement.h"
#include "language/stack/stackElements/stringElement.h"
#include "language/stack/stackElements/substackElement.h"
#include "language/stack/stackElements/typeElement.h"

#include <assert.h>
#include <iostream>

void testBoolean() noexcept;
void testCommand() noexcept;
void testNumber() noexcept;
void testString() noexcept;
void testSubstack() noexcept;
void testType() noexcept;

void testParsers() noexcept {
  using StackLang::StackElement;
  using StackLang::Exceptions::ParserException;
  using StackLang::StackElements::NumberElement;
  using std::cerr;

  testBoolean();
  testNumber();
  testCommand();
  testString();
  testSubstack();
  testType();

  StackElement* elm;

  try {
    StackElement::parse("");
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ParserException& e) {
    assert(e.getContext() == "");
    assert(e.getLocation() == 0);
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }

  try {
    elm = StackElement::parse("22/7");
    NumberElement* num = dynamic_cast<NumberElement*>(elm);
    assert(num != nullptr);
    assert(num->getData() == mpq_class("22/7"));
  } catch (...) {
    cerr << " FAILED: Exception thrown, but expected none.\n";
    assert(false);
  }
}

void testBoolean() noexcept {
  using StackLang::Exceptions::ParserException;
  using StackLang::StackElements::NumberElement;
  using std::cerr;
}

void testCommand() noexcept {}

void testNumber() noexcept {
  using StackLang::StackElement;
  using StackLang::Exceptions::ParserException;
  using StackLang::StackElements::NumberElement;
  using std::cerr;

  StackElement* elm;

  try {
    NumberElement::parse("0x10c");
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ParserException& e) {
    assert(e.getContext() == "0x10c");
    assert(e.getLocation() == 1);
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
}

void testString() noexcept {}
void testSubstack() noexcept {}
void testType() noexcept {}