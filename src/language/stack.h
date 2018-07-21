#ifndef STACKLANG_LANGUAGE_STACK_H_
#define STACKLANG_LANGUAGE_STACK_H_

#include <memory>

#include "language/stack/stackElement.h"

namespace StackLang {
using std::unique_ptr;

class Stack {
  struct Node;

 public:
  class StackIterator;

  // Creates a stack, optionally with a limit on the number of elements.
  Stack() noexcept;
  explicit Stack(unsigned long) noexcept;
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

  unsigned long size() const noexcept;
  unsigned long getLimit() const noexcept;

  // Sets a new limit. Throws StackOverflowError if too big.
  void setLimit(unsigned long);

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
  unsigned long dataSize;
  unsigned long limit;
};
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_H