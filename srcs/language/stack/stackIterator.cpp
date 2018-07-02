#include "language/stack/stackIterator.h"

namespace StackLang
{
using StackLang::Stack;
using StackLang::StackElement;

Stack::StackIterator::StackIterator (Stack::Node* node) :
    curr (node)
{}

Stack::StackIterator::StackIterator (const StackIterator& other) :
    curr (other.curr)
{}

const Stack::StackIterator& Stack::StackIterator::operator= (const StackIterator& other)
{
    curr = other.curr;
    return *this;
}

bool Stack::StackIterator::operator== (const StackIterator& other) const
{
    return curr == other.curr;
}

bool Stack::StackIterator::operator!= (const StackIterator& other) const
{
    return !(*this == other);
}

const StackElement* Stack::StackIterator::operator* () const
{
    return curr->elm.get ();
}

Stack::StackIterator& Stack::StackIterator::operator++ ()
{
    if (curr != nullptr)
    {
        curr = curr->next;
    }

    return *this;
}

Stack::StackIterator Stack::StackIterator::operator++ (int)
{
    StackIterator temp = *this;

    operator++ ();

    return temp;
}
} // namespace StackLang