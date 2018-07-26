#include "language/definedFunction.h"

namespace StackLang {
DefinedFunction::DefinedFunction(const list<TypeElement>& sig, const Stack& b,
                                 const CommandElement* ctx) noexcept
    : signature(sig), body(b), context(ctx) {}
}  // namespace StackLang