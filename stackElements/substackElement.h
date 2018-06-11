#ifndef SUBSTACKELEMENT_H
#define SUBSTACKELEMENT_H

#include "stackElement.h"
#include <list>
using std::list;

namespace StackElements
{
    class SubstackElement : public StackElement
    {
        public:

        SubstackElement ();

        operator const char* () const override;

        private:

        list<StackElement*> data;
    };
}

#endif /* SUBSTACKELEMENT_H */