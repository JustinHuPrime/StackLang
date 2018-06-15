#ifndef TYPEELEMENT_H
#define TYPEELEMENT_H

#include "language/stackElement.h"
#include <string>
using std::string;

namespace StackLang
{
    namespace StackElements
    {
        class TypeElement : public StackElement
        {
            public:

            TypeElement (DataType);

            operator const string () const override;
            DataType getData ();

            private:

            DataType data;
        };

        namespace TypeNameConstants
        {
            const string TYPES [] = {"Number", "String", "Boolean", "Substack", "Type", "Command"};
        }
    }
}

#endif /* TYPEELEMENT_H */