#ifndef STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_
#define STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_

#include "language/stack.h"

namespace StackLang {
using StackLang::Stack;

class Stack::StackIterator {
 public:
  StackIterator(Stack::Node*);
  StackIterator(const StackIterator&) = default;

  StackIterator& operator=(const StackIterator&) = default;

  friend bool operator==(const Stack::StackIterator&,
                         const Stack::StackIterator&);
  friend bool operator!=(const Stack::StackIterator&,
                         const Stack::StackIterator&);

  const StackElement* operator*();

  StackIterator& operator++();
  StackIterator operator++(int);

 private:
  Stack::Node* curr;
};

bool operator==(const Stack::StackIterator&, const Stack::StackIterator&);
bool operator!=(const Stack::StackIterator&, const Stack::StackIterator&);
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_