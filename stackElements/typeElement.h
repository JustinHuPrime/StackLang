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

        TypeElement (DataType);

        operator const char* () const override;

        static const string TYPES [];

        private:

        DataType data;
    };
}

#endif /* TYPEELEMENT_H */