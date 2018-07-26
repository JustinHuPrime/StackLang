#include "language/definedFunction.h"

namespace stacklang {
DefinedFunction::DefinedFunction(const list<TypeElement>& sig, const Stack& b,
                                 const CommandElement* ctx) noexcept
    : signature(sig), body(b), context(ctx) {}
}  // namespace stacklang