#ifndef STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_
#define STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_

#include "language/stack/stackElement.h"
#include "language/stackImpl.h"

namespace StackLang {
using StackLang::Stack;
using StackLang::StackElement;

class Stack::StackIterator {
 public:
  typedef size_t difference_type;
  typedef StackElement* value_type;
  typedef StackElement** pointer;
  typedef StackElement*& reference;
  typedef size_t size_type;
  typedef std::input_iterator_tag iterator_category;

  StackIterator(Stack::Node*) noexcept;
  StackIterator(const StackIterator&) noexcept = default;

  StackIterator& operator=(const StackIterator&) noexcept = default;

  friend bool operator==(const Stack::StackIterator&,
                         const Stack::StackIterator&) noexcept;
  friend bool operator!=(const Stack::StackIterator&,
                         const Stack::StackIterator&) noexcept;

  const StackElement* operator*() noexcept;

  StackIterator& operator++() noexcept;
  StackIterator operator++(int) noexcept;

 private:
  Stack::Node* curr;
};

bool operator==(const Stack::StackIterator&,
                const Stack::StackIterator&) noexcept;
bool operator!=(const Stack::StackIterator&,
                const Stack::StackIterator&) noexcept;
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKITERATOR_H_