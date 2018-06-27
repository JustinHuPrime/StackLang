#ifndef TYPEELEMENT_H
#define TYPEELEMENT_H

#include "language/stack/stackElement.h"
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

            static string to_string (DataType);

            private:

            DataType data;
        };

        namespace TypeNameConstants
        {
            const string TYPES [] = {"Number", "String", "Boolean", "Substack", "Type", "Command", "Any"};
        }
    }
}

#endif /* TYPEELEMENT_H */