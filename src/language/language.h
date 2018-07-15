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
typedef void (*const Prim)(Stack&, map<string, DefinedFunction>&);
typedef pair<list<StackElement::DataType>, Prim> PrimitiveFunction;

const map<string, PrimitiveFunction>& PRIMITIVES();
void execute(Stack&,
             map<string, DefinedFunction>&);  // Executes the stack until it
                                              // encounters a data element

extern bool stopFlag;
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_