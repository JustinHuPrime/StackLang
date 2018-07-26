#ifndef STACKLANG_LANGUAGE_STACKIMPL_H_
#define STACKLANG_LANGUAGE_STACKIMPL_H_

#include <memory>

#include "language/stack/stackElement.h"

namespace stacklang {
using std::unique_ptr;

class Stack {
  struct Node;

 public:
  class StackIterator;

  // Creates a stack, optionally with a limit on the number of elements.
  Stack() noexcept;
  explicit Stack(size_t) noexcept;
  Stack(const Stack&) noexcept;
  Stack(Stack&&) noexcept;

  ~Stack() noexcept;

  Stack& operator=(const Stack&) noexcept;
  Stack& operator=(Stack&&) noexcept;

  // Manipulates stack elements. Pop does not delete the element, but the stack
  // destructor does assume that when the stack is destroyed, it's managed
  // elements should be deallocated.
  void push(StackElement*);
  StackElement* pop();
  StackElement* top();

  void clear() noexcept;

  StackIterator begin() const noexcept;
  StackIterator end() const noexcept;

  size_t size() const noexcept;
  size_t getLimit() const noexcept;

  // Sets a new limit. Throws StackOverflowError if too big.
  void setLimit(size_t);

  void reverse() noexcept;

  bool isEmpty() const noexcept;

 private:
  struct Node {
    unique_ptr<StackElement> elm;
    Node* next;
    Node(StackElement*, Node*) noexcept;
  };
  Node* copy(Node*) noexcept;

  Node* head;
  size_t dataSize;
  size_t limit;
};
}  // namespace stacklang

#endif  // STACKLANG_LANGUAGE_STACKIMPL_H_