#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_

#include <string>

namespace stacklang {
using std::string;

// Represents a element in the stack. Subclassed to make a specific element.
class StackElement {
 public:
  static const unsigned NUM_PRIM_TYPES;
  enum class DataType {
    Number,
    String,
    Boolean,
    Substack,
    Type,
    Command,
    Any,
    Exact,
    Inexact,
    Quoted,
  };

  // Produces a StackElement (of some type) from a terminal input string
  static StackElement* parse(const string&);

  StackElement(const StackElement&) noexcept = default;
  StackElement(StackElement&&) noexcept = default;
  virtual StackElement* clone() const noexcept = 0;

  StackElement& operator=(const StackElement&) noexcept = default;
  StackElement& operator=(StackElement&&) noexcept = default;

  virtual ~StackElement() noexcept = default;

  // Produces a nicely formatted string of the string (for print to console)
  explicit virtual operator string() const noexcept = 0;

  // Getter for the DataType
  DataType getType() const noexcept;

  // Gets the data from the element (returns different type depending on
  // dataType) should exist in all subclasses, where T is the closest
  // dataTYpe to the element's type T getData () const;

  // Constructs an element of some type (takes different param type
  // depending on dataType) should exist in all subclasses. Element
  // (dataType);

 protected:
  // Constructs a StackElement by setting the dataType
  explicit StackElement(DataType) noexcept;

  DataType dataType;
};
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENT_H_