#include <iostream>

void testStringUtils();
void testArgReader();
// void testLineEditor();
// void testStack();
// void testParsers();

int main() {
  using std::cout;

  cout << "Test suite for StackLang.\n";

  cout << "\nTesting utils.\n";
  cout << "stringUtils\n";
  testStringUtils();

  cout << "\nTesting ui\n";
  cout << "argReader\n";
  testArgReader();
  //   cout << "lineEditor\n";
  //   testLineEditor();

  //   cout << "\nTesting language\n";
  //   cout << "stack\n";
  //   testStack();
  //   cout << "parsers\n";
  //   testParsers();

  cout << "\nALL TESTS PASSED\n\n";

  exit(EXIT_SUCCESS);
}