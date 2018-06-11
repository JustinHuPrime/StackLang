#ifndef STACKELEMENT_H
#define STACKELEMENT_H

class StackElement
{
    public:

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