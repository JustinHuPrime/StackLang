#ifndef STACKLANG_UI_CLAREADER_H_
#define STACKLANG_UI_CLAREADER_H_

#include <map>
#include <string>
#include <vector>

namespace TermUI {
using std::map;
using std::string;
using std::vector;

// A commannd line argument reader that can infer the intended syntax of command
// line arguments. An option followed by another option is a flag, an option
// followed by something that doesn't start with a -, or a quoted string, is a
// option, and an option followed by many non-options is a long option. Combined
// flags are not supported, not are long option names and synonyms.
class ArgReader {
 public:
  ArgReader() noexcept;
  ArgReader(const ArgReader&) noexcept = default;
  ArgReader& operator=(const ArgReader&) noexcept = default;
  ArgReader(ArgReader&&) noexcept = default;
  ArgReader& operator=(ArgReader&&) noexcept = default;
  // Reads in to the object from argv. If restrictChar is true, then the reader
  // will only consider arguments with a dash and a char to be option flags
  void read(int, char* []);

  // Checks if the intended syntax was valid. Throws an argumentError if there
  // are any errors.
  void validate(const string& allowedFlag, const string& allowedOpts,
                const string& allowedLongOpts) const;

  // Getters. Will also throw ArgumentErrors.
  bool hasFlag(char) const noexcept;  // Doesn't check that given flag is valid
  bool hasOpt(char) const noexcept;
  bool hasLongOpt(char) const noexcept;
  string getOpt(char) const;
  vector<string> getLongOpt(char) const;

 private:
  vector<char> flags;
  map<char, string> options;
  map<char, vector<string>> longOpts;
};
}  // namespace TermUI

#endif  // STACKLANG_UI_CLAREADER_H_