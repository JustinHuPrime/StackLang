#include <iostream>

void testStringUtils() noexcept;
void testArgReader() noexcept;
void testLineEditor() noexcept;
void testStack() noexcept;
void testParsers() noexcept;
// void testTypeCheck() noexcept;
// void testPrimitives() noexcept;

int main() {
  using std::cout;

  cout << "Test suite for StackLang.\n";

  cout << "\nTesting utils:\n";
  cout << "stringUtils";
  testStringUtils();
  cout << " PASSED\n";

  cout << "\nTesting ui:\n";
  cout << "argReader";
  testArgReader();
  cout << " PASSED\n";
  cout << "lineEditor";
  testLineEditor();
  cout << " PASSED\n";

  cout << "\nTesting language:\n";
  cout << "stack";
  testStack();
  cout << " PASSED\n";
  cout << "parsers";
  testParsers();
  cout << " PASSED\n";

  // cout << "\nTesting language:\n";
  // cout << "Type checking";
  // testTypeCheck();
  // cout << " PASSED\n";
  // cout << "Primitives";
  // testPrimitives();
  // cout << " PASSED\n";

  cout << "\nALL TESTS PASSED\n\n";

  exit(EXIT_SUCCESS);
}