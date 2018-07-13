#ifndef STACKLANG_UI_CLAREADER_H_
#define STACKLANG_UI_CLAREADER_H_

#include <map>
#include <string>
#include <vector>

#include "language/language.h"
#include "language/stack.h"

namespace TermUI {
using StackLang::DefineMap;
using StackLang::Stack;
using std::map;
using std::string;
using std::vector;

// A commannd line argument reader that can infer the intended syntax of command
// line arguments. An option followed by another option is a flag, an option
// followed by something that doesn't start with a -, or a quoted string, is a
// option, and an option followed by many non-options is a long option. Combined
// flags are supported, but long option names and synonyms are not.
class ArgReader {
 public:
  ArgReader() = default;
  ArgReader(const ArgReader&) = default;
  ArgReader& operator=(const ArgReader&) = default;
  ArgReader(ArgReader&&) = default;
  ArgReader& operator=(ArgReader&&) = default;
  // Reads in to the object from argv. If restrictChar is true, then the reader
  // will only consider arguments with a dash and a char to be option flags
  void read(int, char* [], bool restrictChar = true);

  // Checks if the intended syntax was valid. Throws an argumentError if there
  // are any errors.
  void validate(const string& allowedFlag, const string& allowedOpts,
                const string& allowedLongOpts) const;

  // Getters. Will also throw ArgumentErrors.
  bool hasFlag(char) const;  // Doesn't check that given flag is valid
  bool hasOpt(char) const;
  bool hasLongOpt(char) const;
  string getOpt(char) const;
  vector<string> getLongOpt(char) const;

 private:
  vector<char> flags;
  map<char, string> options;
  map<char, vector<string>> longOpts;
};
}  // namespace TermUI

#endif  // STACKLANG_UI_CLAREADER_H_