#include "ui/argReader.h"

#include <algorithm>

#include "language/exceptions/argumentError.h"

namespace TermUI {
using StackLang::Exceptions::ArgumentError;
using std::find;
using std::min;
using std::to_string;

void ArgReader::read(int argc, char* argv[], bool restrictChar) {
  vector<string> args;

  for (int i = 0; i < argc; i++) {
    args.push_back(string(argv[i]));
  }

  vector<string> acc;
  bool inQuote;
  char currentOpt;

  for (auto vIter = args.begin(); vIter != args.end(); ++vIter) {
    if ((*vIter)[0] == '-') {  // I see an option here, maybe.
      if (inQuote) {
        if (vIter->back() == '"' &&
            (vIter->size() < 2 ||
             (*vIter)[vIter->size() - 2] !=
                 '\\')) {  // ending a quote, not a backslash
          acc.back() += vIter->substr(0, vIter->size() - 1);
          inQuote = false;
        } else {
          acc.back() += *vIter;
        }
      } else {
        if ((*vIter)[0] == '-') {  // Found an option
          // commit the past one
          if (acc.size() == 0) {
            flags.push_back(currentOpt);
          } else if (acc.size() == 1) {
            options[currentOpt] = acc.front();
          } else {
            longOpts[currentOpt] = acc;
          }
          acc.clear();

          if (vIter->size() != 2) {  //
            throw ArgumentError("Expected an single character after the `-`.",
                                *vIter, min(vIter->size(), 2UL));
          } else if (*vIter == "--") {  // ignore end of long-opts.
            continue;
          }
          currentOpt = (*vIter)[1];
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
  }

  if (acc.size() == 0) {  // again, commit the last option
    flags.push_back(currentOpt);
  } else if (acc.size() == 1) {
    options[currentOpt] = acc.front();
  } else {
    longOpts[currentOpt] = acc;
  }
}

void ArgReader::validate(const string& allowedFlag, const string& allowedOpts,
                         const string& allowedLongOpts) const {
  for (char c : flags) {
    if (allowedFlag.find(c) == string::npos) {
      throw ArgumentError("Flag `-" + to_string(c) +
                          "` is not allowed as a flag.");
    }
  }
  for (auto c : options) {
    if (allowedOpts.find(c.first) == string::npos) {
      throw ArgumentError("Option `-" + to_string(c.first) +
                          "` is not allowed as an option.");
    }
  }
  for (auto c : longOpts) {
    if (allowedLongOpts.find(c.first) == string::npos) {
      throw ArgumentError("Long option `-" + to_string(c.first) +
                          "` is not allowed as an option.");
    }
  }
}

bool ArgReader::hasFlag(char c) const {
  return find(flags.begin(), flags.end(), c) != flags.end();
}
bool ArgReader::hasOpt(char c) const {
  return find(options.begin(), options.end(), c) != options.end();
}
bool ArgReader::hasLongOpt(char c) const {
  return find(longOpts.begin(), longOpts.end(), c) != longOpts.end();
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
}  // namespace TermUI