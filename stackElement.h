#ifndef STACKELEMENT_H
#define STACKELEMENT_H

#include <string>
using std::string;

class StackElement
{
    public:

    enum class DataType
    {
        Number,
        String,
        Boolean,
        Substack,
        Type,
        Command,
    };

    static StackElement* parse (string);

    virtual operator const string () const = 0;
    DataType getType () const;

    protected:

    StackElement (DataType);

    DataType dataType;
};

#endif /* STACKELEMENT_H */