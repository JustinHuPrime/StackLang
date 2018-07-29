// Copyright 2018 Justin Hu
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The StackLang interpreter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the StackLang interpreter.  If not, see <https://www.gnu.org/licenses/>.

// Catch2 main file - kept separate despite triviality. Only contains
// CATCH_CONFIG_MAIN and the catch.hpp include.

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// #include <iostream>

// void testStringUtils() noexcept;
// void testArgReader() noexcept;
// void testLineEditor() noexcept;
// void testStack() noexcept;
// void testParsers() noexcept;
// // void testTypeCheck() noexcept;
// // void testPrimitives() noexcept;

// int main() {
//   using std::cout;

//   cout << "Test suite for StackLang.\n";

//   cout << "\nTesting utils:\n";
//   cout << "stringUtils";
//   testStringUtils();
//   cout << " PASSED\n";

//   cout << "\nTesting ui:\n";
//   cout << "argReader";
//   testArgReader();
//   cout << " PASSED\n";
//   cout << "lineEditor";
//   testLineEditor();
//   cout << " PASSED\n";

//   cout << "\nTesting language:\n";
//   cout << "stack";
//   testStack();
//   cout << " PASSED\n";
//   cout << "parsers";
//   testParsers();
//   cout << " PASSED\n";

//   // cout << "\nTesting language:\n";
//   // cout << "Type checking";
//   // testTypeCheck();
//   // cout << " PASSED\n";
//   // cout << "Primitives";
//   // testPrimitives();
//   // cout << " PASSED\n";

//   cout << "\nALL TESTS PASSED\n\n";

//   exit(EXIT_SUCCESS);
// }