#include "language/stack.h"
#include "language/stackIterator.h"
#include "language/exceptions/stackOverflowError.h"
using namespace StackLang;
using namespace Exceptions;

Stack::Stack () : head (nullptr), dataSize (0), limit (__LONG_MAX__)
{}

Stack::Stack (unsigned long lim) : head (nullptr), dataSize (0), limit (lim)
{}

Stack::~Stack ()
{
    clear ();
}

void Stack::push (StackElement* ptr)
{
    if (dataSize >= limit)
    {
        throw StackOverflowError (limit);
    }

    Node* temp = new Node;
    temp -> elm = ptr;
    temp -> next = head;
    head = temp;
    dataSize++;
}

void Stack::pop ()
{
    if (head != nullptr)
    {
        Node* temp = head -> next;
        delete head;
        head = temp;
        dataSize--;
    }
}

StackElement* Stack::top ()
{
    return head -> elm;
}

unsigned long Stack::size () const
{
    return dataSize;
}

unsigned long Stack::getLimit () const
{
    return limit;
}

void Stack::setLimit (unsigned long newLimit)
{
    if (dataSize > newLimit)
    {
        throw StackOverflowError (newLimit);
    }

    limit = newLimit;
}

bool Stack::empty () const
{
    return dataSize == 0;
}

Stack::StackIterator Stack::begin () const
{
    return StackIterator (head);
}

Stack::StackIterator Stack::end () const
{
    return StackIterator (nullptr);
}

void Stack::clear ()
{

    while (head != nullptr)
    {
        delete head -> elm;
        pop ();
    }

    dataSize = 0;
}