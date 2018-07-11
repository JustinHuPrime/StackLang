#ifndef STACKLANG_UI_CLAREADER_H_
#define STACKLANG_UI_CLAREADER_H_

#include "language/language.h"
#include "language/stack.h"

#include <map>
#include <string>
#include <vector>

namespace TermUI {
using StackLang::DefineMap;
using StackLang::Stack;
using std::map;
using std::string;
using std::vector;

void claReader(const vector<string>&, DefineMap*);
}  // namespace TermUI

#endif  // STACKLANG_UI_CLAREADER_H_