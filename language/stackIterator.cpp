#include "language/stackIterator.h"
using namespace StackLang;

Stack::StackIterator::StackIterator (Stack::Node* node) : curr (node)
{}

Stack::StackIterator::StackIterator (const StackIterator& other) : curr (other.curr)
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
    return curr != other.curr;
}

const StackElement* Stack::StackIterator::operator* () const
{
    return curr -> elm;
}

const StackElement& Stack::StackIterator::operator-> () const
{
    return *(curr -> elm);
}

Stack::StackIterator& Stack::StackIterator::operator++ ()
{
    if (curr != nullptr)
    {
        curr = curr -> next;
    }

    return *this;
}

Stack::StackIterator Stack::StackIterator::operator++ (int)
{
    StackIterator temp = *this;
    
    operator++ ();

    return temp;
}