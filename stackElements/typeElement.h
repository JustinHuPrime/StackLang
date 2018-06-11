#ifndef TYPEELEMENT_H
#define TYPEELEMENT_H

#include "stackElement.h"
#include <string>
using std::string;

namespace StackElements
{
    class TypeElement : public StackElement
    {
        public:

        TypeElement ();

        operator const char* () const override;

        private:

        DataType data;
    };
}

#endif /* TYPEELEMENT_H */