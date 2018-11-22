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

// Implenetation of argument reader class

#include "ui/argReader.h"

#include <algorithm>

#include "language/exceptions/interpreterExceptions.h"

namespace terminalui {
namespace {
using stacklang::exceptions::ArgumentError;
using std::find;
using std::min;
using std::string;
using std::to_string;
using std::vector;
}  // namespace

ArgReader::ArgReader() noexcept : flags(), options(), longOpts() {}

void ArgReader::read(int argc, const char* argv[]) {
  vector<string> args;

  for (int i = 1; i < argc; i++) {
    args.push_back(string(argv[i]));
  }

  vector<string> acc;
  bool inQuote = false;
  char currentOpt = '\0';

  for (auto vIter = args.begin(); vIter != args.end(); ++vIter) {
    if (inQuote) {
      if (vIter->back() == '"' &&
          (vIter->size() < 2 || (*vIter)[vIter->size() - 2] !=
                                    '\\')) {  // ending a quote, not a backslash
        acc.back() += " " + vIter->substr(0, vIter->size() - 1);
        inQuote = false;
      } else {
        acc.back() += " " + *vIter;
      }
    } else {
      if (vIter->front() == '-') {  // Found an option
        // commit the past one if it has been written to
        if (currentOpt != '\0') {
          if (acc.size() == 0) {
            flags.push_back(currentOpt);
          } else if (acc.size() == 1) {
            options[currentOpt] = acc.front();
          } else {
            longOpts[currentOpt] = acc;
          }
        }
        acc.clear();

        if (vIter->size() != 2) {  // is a combined flag or something.
          throw ArgumentError("Expected an single character after the `-`.",
                              *vIter, min(vIter->size(), size_t{2}));
        } else if (*vIter == "--") {  // ignore end of long-opts.
          continue;
        }
        currentOpt = vIter->back();
      } else if (vIter->front() == '"') {  // not an option, starts a quote
        if (vIter->back() == '"') {        // ends a quote
          acc.push_back(
              vIter->substr(1, vIter->size() - 2));  // chop off the quotes
        } else {
          acc.push_back(vIter->substr(1));  // strip the first quote
          inQuote = true;
        }
      } else {  // not an option, not in a quote - add as parameter
        acc.push_back(*vIter);
      }
    }
  }

  if (currentOpt != '\0') {  // unless the args really are empty
    if (acc.size() == 0) {   // commit the last option
      flags.push_back(currentOpt);
    } else if (acc.size() == 1) {
      options[currentOpt] = acc.front();
    } else {
      longOpts[currentOpt] = acc;
    }
  }
}

void ArgReader::validate(const string& allowedFlag, const string& allowedOpts,
                         const string& allowedLongOpts) const {
  for (char c : flags) {
    if (allowedFlag.find(c) == string::npos) {
      string asStr;
      asStr += c;
      throw ArgumentError("Flag `-" + asStr + "` is not allowed as a flag.");
    }
  }
  for (auto c : options) {
    if (allowedOpts.find(c.first) == string::npos) {
      string asStr;
      asStr += c.first;
      throw ArgumentError("Option `-" + asStr +
                          "` is not allowed as an option.");
    }
  }
  for (auto c : longOpts) {
    if (allowedLongOpts.find(c.first) == string::npos) {
      string asStr;
      asStr += c.first;
      throw ArgumentError("Long option `-" + asStr +
                          "` is not allowed as an option.");
    }
  }
}

bool ArgReader::hasFlag(char c) const noexcept {
  return find(flags.begin(), flags.end(), c) != flags.end();
}
bool ArgReader::hasOpt(char c) const noexcept {
  return options.find(c) != options.end();
}
bool ArgReader::hasLongOpt(char c) const noexcept {
  return longOpts.find(c) != longOpts.end();
}
string ArgReader::getOpt(char c) const {
  auto atargst = options.find(c);
  if (atargst == options.end()) {
    throw ArgumentError("Could not find option `-" + to_string(c) + "`.");
  }
  return atargst->second;
}
vector<string> ArgReader::getLongOpt(char c) const {
  auto atargst = longOpts.find(c);
  if (atargst == longOpts.end()) {
    throw ArgumentError("Could not find long option `-" + to_string(c) + "`.");
  }
  return atargst->second;
}
}  // namespace terminalui
