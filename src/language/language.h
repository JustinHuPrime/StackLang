#ifndef STACKLANG_LANGUAGE_LANGUAGE_H_
#define STACKLANG_LANGUAGE_LANGUAGE_H_

#include <array>
#include <list>
#include <map>
#include <utility>

#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

namespace StackLang {
using StackElements::TypeElement;
using std::array;
using std::list;
using std::map;
using std::pair;

typedef pair<list<TypeElement>, Stack> DefinedFunction;
typedef map<string, DefinedFunction> DefineMap;
typedef void (*PrimitiveFunction)(Stack&, DefineMap&);

// Executes the stack until it encounters a data element
void execute(Stack&, DefineMap&);

const pair<const char*, PrimitiveFunction> PRIMITIVES[] = {
    {"plus", [](Stack&, DefineMap&) { return; }}};
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_