#include "language/exceptions/argumentError.h"
#include "ui/argReader.cc"

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

void testArgReader() {
  using StackLang::Exceptions::ArgumentError;
  using std::cerr;
  using std::string;
  using std::vector;
  using TermUI::ArgReader;

  ArgReader ar;

  char* argv[] = {"-a",
                  "-b",
                  "data",
                  "-c",
                  "more",
                  "stuff",
                  "-d",
                  "\"-words",
                  "in",
                  "quotes-\"",
                  "-e",
                  "\"part",
                  "one\"",
                  "\"part"
                  "two\"",
                  "three"};

  char* badArgv[] = {"-ab"};

  try {
    ar.read(1, argv);
    cerr << "Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }

  try {
    ar.read(15, argv);
    ar.validate("a", "bd", "ce");
  } catch (...) {
    cerr << "Threw exception, but shouldn't have.\n";
    assert(false);
  }

  try {
    ar.validate("", "bd", "ce");
    cerr << "Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }

  try {
    ar.validate("a", "d", "ce");
    cerr << "Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }

  try {
    ar.validate("a", "bd", "c");
    cerr << "Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }

  assert(ar.hasFlag('a'));
  assert(!ar.hasFlag('b'));
  assert(!ar.hasFlag('x'));

  assert(ar.hasOpt('b'));
  assert(!ar.hasOpt('c'));
  assert(!ar.hasOpt('x'));

  assert(ar.hasLongOpt('c'));
  assert(!ar.hasLongOpt('a'));
  assert(!ar.hasLongOpt('x'));

  assert(ar.getOpt('b') == "data");
  assert(ar.getOpt('d') == "-words in quotes-");
  try {
    ar.getOpt('c');
    cerr << "Expected an exception, but none thrown.\n";
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }
  try {
    ar.getOpt('x');
    cerr << "Expected an exception, but none thrown.\n";
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }

  assert(ar.getLongOpt('c') == vector<string>({"more", "stuff"}));
  assert(ar.getOpt('d') == "-words in quotes-");
  try {
    ar.getOpt('c');
    cerr << "Expected an exception, but none thrown.\n";
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }
  try {
    ar.getOpt('x');
    cerr << "Expected an exception, but none thrown.\n";
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << "Threw wrong exception.\n";
    assert(false);
  }
}