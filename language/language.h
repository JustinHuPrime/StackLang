#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "language/stackElement.h"
#include <list>
#include <map>
using std::list;
using std::map;

namespace StackLang
{
    void execute (list<StackElement*>&);

    namespace Primitives
    {
        typedef void (*stackFunction) (list<StackElement*>&);
        const map<string, stackFunction> PRIMITIVES;
    }
}

#endif