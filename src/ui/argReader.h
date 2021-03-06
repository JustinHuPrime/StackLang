// Copyright 2018 Justin Hu, Bronwyn Damm, Jacques Marais, Ramon Rakow, and Jude
// Sidloski
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

// Command line argument parser - parses arguments into a nicer form for access.

#ifndef STACKLANG_UI_CLAREADER_H_
#define STACKLANG_UI_CLAREADER_H_

#include <map>
#include <string>
#include <vector>

namespace terminalui {
// A commannd line argument reader that can infer the intended syntax of command
// line arguments. An option followed by another option is a flag, an option
// followed by something that doesn't start with a -, or a quoted string, is a
// option, and an option followed by many non-options is a long option. Combined
// flags are not supported, not are long option names and synonyms.
class ArgReader {
 public:
  ArgReader() noexcept;
  ArgReader(const ArgReader&) = default;
  ArgReader& operator=(const ArgReader&) = default;
  ArgReader(ArgReader&&) = default;
  ArgReader& operator=(ArgReader&&) = default;
  // Reads in to the object from argv. If restrictChar is true, then the reader
  // will only consider arguments with a dash and a char to be option flags
  void read(int, const char*[]);

  // Checks if the intended syntax was valid. Throws an argumentError if there
  // are any errors.
  void validate(const std::string& allowedFlag, const std::string& allowedOpts,
                const std::string& allowedLongOpts) const;

  // Getters. Will also throw ArgumentErrors.
  bool hasFlag(char) const noexcept;  // Doesn't check that given flag is valid
  bool hasOpt(char) const noexcept;
  bool hasLongOpt(char) const noexcept;
  std::string getOpt(char) const;
  std::vector<std::string> getLongOpt(char) const;

 private:
  std::vector<char> flags;
  std::map<char, std::string> options;
  std::map<char, std::vector<std::string>> longOpts;
};
}  // namespace terminalui

#endif  // STACKLANG_UI_CLAREADER_H_
