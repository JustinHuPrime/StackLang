#include "language/exceptions/argumentError.h"
#include "ui/argReader.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

void testArgReader() noexcept {
  using stacklang::exceptions::ArgumentError;
  using std::cerr;
  using std::string;
  using std::vector;
  using terminalui::ArgReader;

  ArgReader ar;

  char* argv[] = {"discard",
                  "-a",
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

  char* badArgv[] = {"discard", "-ab", "-f"};

  try {
    ar.read(3, badArgv);
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }

  try {
    ar.read(16, argv);
    ar.validate("a", "bd", "ce");
  } catch (...) {
    cerr << " FAILED: Threw exception, but shouldn't have.\n";
    assert(false);
  }

  try {
    ar.validate("", "bd", "ce");
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }

  try {
    ar.validate("a", "d", "ce");
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }

  try {
    ar.validate("a", "bd", "c");
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
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

  try {
    assert(ar.getOpt('b') == "data");
    assert(ar.getOpt('d') == "-words in quotes-");
  } catch (...) {
    cerr << " FAILED: Exception thrown, but expected none.\n";
    assert(false);
  }
  try {
    ar.getOpt('c');
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
  try {
    ar.getOpt('x');
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
  try {
    assert(ar.getLongOpt('c') == vector<string>({"more", "stuff"}));
    assert(ar.getOpt('d') == "-words in quotes-");
  } catch (...) {
    cerr << " FAILED: Exception thrown, but expected none.\n";
    assert(false);
  }
  try {
    ar.getOpt('c');
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
  try {
    ar.getOpt('x');
    cerr << " FAILED: Expected an exception, but none thrown.\n";
    assert(false);
  } catch (const ArgumentError&) {
    // do nothing.
  } catch (...) {
    cerr << " FAILED: Threw wrong exception.\n";
    assert(false);
  }
}