#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/commandElement.h"

#include <map>
using std::map;

namespace StackLang
{
/**
 * Executes the top element of the stack
 */
void execute (Stack&);

namespace Primitives
{
typedef void (*stackFunction) (Stack&);
const map< string, stackFunction > PRIMITIVES;
} // namespace Primitives
} // namespace StackLang

#endif