#ifndef STACKELEMENT_H
#define STACKELEMENT_H

#include <string>
using std::string;

class StackElement
{
    public:

    static StackElement* parse (string);

    virtual operator const char* () const = 0;

    protected:

    enum class DataType
    {
        Number,
        String,
        Boolean,
        Substack,
        Type,
        Command,
    };

    StackElement (DataType);

    DataType dataType;
};

#endif /* STACKELEMENT_H */