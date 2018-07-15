#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_

#include <string>

namespace StackLang {
using std::string;

// Represents a element in the stack. Subclassed to make a specific element.
class StackElement {
 public:
  enum class DataType {
    Number,
    String,
    Boolean,
    Substack,
    Type,
    Command,
    Any,
  };

  // Produces a StackElement (of some type) from a terminal input string
  static StackElement* parse(const string&);

  StackElement(const StackElement&) = default;
  StackElement(StackElement&&) = default;
  virtual StackElement* clone() const = 0;

  StackElement& operator=(const StackElement&) = default;
  StackElement& operator=(StackElement&&) = default;

  virtual ~StackElement();

  // Produces a nicely formatted string of the string (for print to console)
  explicit virtual operator const string() const = 0;

  // Getter for the DataType
  DataType getType() const;

  // Gets the data from the element (returns different type depending on
  // dataType) should exist in all subclasses, where T is the closest dataTYpe
  // to the element's type
  // T getData () const;

  // Constructs an element of some type (takes different param type depending on
  // dataType) should exist in all subclasses.
  // Element (dataType);

 protected:
  // Constructs a StackElement by setting the dataType
  explicit StackElement(DataType);

  DataType dataType;
};
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_