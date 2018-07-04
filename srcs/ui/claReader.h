#ifndef CLAREADER_H
#define CLAREADER_H

#include "language/language.h"
#include "language/stack.h"

#include <map>
#include <string>
#include <vector>

namespace TermUI
{
using StackLang::DefineMap;
using StackLang::Stack;
using std::map;
using std::string;
using std::vector;

void claReader (const vector< string >&, DefineMap&);
} // namespace TermUI

#endif /* CLAREADER_H */