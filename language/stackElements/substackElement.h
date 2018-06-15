#ifndef SUBSTACKELEMENT_H
#define SUBSTACKELEMENT_H

#include "language/stackElement.h"
#include <list>
#include <string>
using std::list;
using std::string;
using std::to_string;

namespace StackLang
{
    namespace StackElements
    {
        class SubstackElement : public StackElement
        {
            public:

            SubstackElement ();

            operator const string () const override;
            const list<StackElement*>& getData ();

            private:

            list<StackElement*> data;
        };
        
        namespace SubstackConstants
        {
            const string SUBSTACK_BEGIN = "<<";
            const string SUBSTACK_END = ">>";
            const string SUBSTACK_SEPARATOR = ", ";
        }
    }
}

#endif /* SUBSTACKELEMENT_H */