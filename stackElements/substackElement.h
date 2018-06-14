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

        operator const string () const override;
        list<StackElement*>& getData ();

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

#endif /* SUBSTACKELEMENT_H */