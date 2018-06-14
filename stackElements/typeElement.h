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

        operator const string () const override;
        const DataType& getData () const;

        private:

        DataType data;
    };

    namespace TypeNameConstants
    {
        const string TYPES [] = {"Number", "String", "Boolean", "Substack", "Type", "Command"};
    }
}

#endif /* TYPEELEMENT_H */