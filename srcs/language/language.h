#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/commandElement.h"

#include <map>

namespace StackLang
{
using std::map;

/**
 * Executes the top element of the stack
 */
void execute (Stack&);

typedef void (*stackFunction) (Stack&);
const map< string, stackFunction > PRIMITIVES = {};
} // namespace StackLang

#endif