#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "language/stackElement.h"
#include "language/stackElements/commandElement.h"
#include <list>
#include <map>
using std::list;
using std::map;

namespace StackLang
{
    /**
     * Executes
     */
    void execute (list<StackElement*>&);

    namespace Primitives
    {
        typedef void (*stackFunction) (list<StackElement*>&);
        const map<string, stackFunction> PRIMITIVES;
    }
}

#endif