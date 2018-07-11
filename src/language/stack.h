#ifndef STACKLANG_LANGUAGE_STACK_H_
#define STACKLANG_LANGUAGE_STACK_H_

#include "language/stack/stackElement.h"

#include <memory>

namespace StackLang {
using std::unique_ptr;

class Stack {
  struct Node;

 public:
  class StackIterator;

  // Creates a stack, optionally with a limit on the number of elements.
  Stack();
  explicit Stack(unsigned long);
  Stack(const Stack&);
  Stack(Stack&&);

  ~Stack();

  Stack& operator=(const Stack&);
  Stack& operator=(Stack&&);

  // Manipulates stack elements. Pop does not delete the element, but the stack
  // destructor does assume that when the stack is destroyed, it's managed
  // elements should be deallocated.
  void push(StackElement*);
  StackElement* pop();
  StackElement* top();

  StackIterator begin() const;
  StackIterator end() const;

  unsigned long size() const;
  unsigned long getLimit() const;

  // Sets a new limit. Throws StackOverflowError if too big.
  void setLimit(unsigned long);

  void reverse();

  bool empty() const;

 private:
  struct Node {
    unique_ptr<StackElement> elm;
    Node* next;
    Node(StackElement*, Node*);
  };

  void clear();
  Node* copy(Node*);

  Node* head;
  unsigned long dataSize;
  unsigned long limit;
};
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_H