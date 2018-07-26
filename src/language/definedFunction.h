#ifndef STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_
#define STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_

#include "language/stack.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/typeElement.h"

#include <list>

namespace stacklang {
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::TypeElement;
using std::list;

struct DefinedFunction {
  const list<TypeElement>& signature;
  const Stack& body;
  const CommandElement* context;
  DefinedFunction(const list<TypeElement>& sig, const Stack& b,
                  const CommandElement* ctx = nullptr) noexcept;
};
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_