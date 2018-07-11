#include "language/stack/stackIterator.h"

namespace StackLang {
using StackLang::Stack;
using StackLang::StackElement;

Stack::StackIterator::StackIterator(Stack::Node* node) : curr(node) {}

const StackElement* Stack::StackIterator::operator*() {
  return curr->elm.get();
}

Stack::StackIterator& Stack::StackIterator::operator++() {
  if (curr != nullptr) {
    curr = curr->next;
  }

  return *this;
}

Stack::StackIterator Stack::StackIterator::operator++(int) {
  StackIterator temp = *this;

  operator++();

  return temp;
}

bool operator==(const Stack::StackIterator& first,
                const Stack::StackIterator& second) {
  return first.curr == second.curr;
}

bool operator!=(const Stack::StackIterator& first,
                const Stack::StackIterator& second) {
  return !(first == second);
}
}  // namespace StackLang