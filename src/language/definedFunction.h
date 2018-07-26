#ifndef STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_
#define STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_

#include "language/stack.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/typeElement.h"

#include <list>

namespace StackLang {
using StackLang::StackElements::CommandElement;
using StackLang::StackElements::TypeElement;
using std::list;

struct DefinedFunction {
  const list<TypeElement>& signature;
  const Stack& body;
  const CommandElement* context;
  DefinedFunction(const list<TypeElement>& sig, const Stack& b,
                  const CommandElement* ctx = nullptr) noexcept;
};
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_DEFINEDFUNCTION_H_