#ifndef STACKITERATOR_H
#define STACKITERATOR_H

#include "language/stack.h"
using StackLang::Stack;

namespace StackLang
{
class Stack::StackIterator
{
  public:
    StackIterator (Stack::Node*);
    StackIterator (const StackIterator&);

    const StackIterator& operator= (const StackIterator&);

    bool operator== (const StackIterator&) const;
    bool operator!= (const StackIterator&) const;

    const StackElement* operator* () const;
    const StackElement& operator-> () const;

    StackIterator& operator++ ();
    StackIterator operator++ (int);

  private:
    Stack::Node* curr;
};
} // namespace StackLang

#endif /* STACKITERATOR_H */