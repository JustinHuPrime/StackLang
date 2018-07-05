#ifndef STACKELEMENT_H
#define STACKELEMENT_H

#include <string>

namespace StackLang
{
using std::string;

/**
 * Represents a element in the stack. Subclassed to make a specific element.
 */
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
        Any,
    };

    /**
     * destroys this object and anything managed by it
     */
    virtual ~StackElement();

    /**
     * clone function - virtual constructor idiom
     */
    virtual StackElement* clone() const = 0;

    /**
     * Produces a StackElement (of some type) from a terminal input string
     * Assumes that given string is not empty
     */
    static StackElement* parse(const string&);

    /**
     * Produces a nicely formatted string of the string (for print to console)
     */
    virtual operator const string() const = 0;

    /**
     * Getter for the DataType
     */
    DataType getType() const;

    /**
     * Gets the data from the element (returns different type depending on dataType)
     * should exist in all subclasses, where T is the closest dataTYpe to the element's type
     */
    //T getData () const;

    /**
     * Constructs an element of some type (takes different param type depending on dataType)
     * should exist in all subclasses.
     */
    //Element (dataType);

  protected:
    /**
     * Constructs a StackElement by setting the dataType
     */
    StackElement(DataType);

    /**
     * What type the Element is
     */
    DataType dataType;
};
} // namespace StackLang

#endif /* STACKELEMENT_H */