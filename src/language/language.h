#ifndef STACKLANG_LANGUAGE_LANGUAGE_H_
#define STACKLANG_LANGUAGE_LANGUAGE_H_

#include <array>
#include <list>
#include <map>
#include <utility>

#include "language/definedFunction.h"
#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

namespace StackLang {
using StackElements::TypeElement;
using std::array;
using std::list;
using std::map;
using std::pair;

typedef void (*const Prim)(Stack&, map<string, DefinedFunction>&);
typedef pair<list<TypeElement>, Prim> PrimitiveFunction;

const map<string, PrimitiveFunction>& PRIMITIVES() noexcept;
bool checkType(const StackElement*, const TypeElement) noexcept;
void checkTypes(const Stack&, const list<TypeElement>&);
void execute(Stack&, map<string, DefinedFunction>&,
             list<CommandElement*> = list<CommandElement*>(
                 {nullptr}));  // Executes the stack until it
                               // encounters a data element

extern bool stopFlag;  // signal handlers set this to stop execution.
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_