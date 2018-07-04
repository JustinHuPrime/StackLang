#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/typeElement.h"

#include <list>
#include <map>
#include <utility>

namespace StackLang
{
using StackElements::TypeElement;
using std::list;
using std::map;
using std::pair;

typedef pair< list< TypeElement >, list< StackElement > > DefinedFunction;
typedef map< string, DefinedFunction > DefineMap;
typedef void (*StackFunction) (Stack&, DefineMap&);

/**
 * Executes the stack until it encounters a data element
 */
void execute (Stack&, DefineMap&);

const map< string, StackFunction > PRIMITIVES = {};
} // namespace StackLang

#endif