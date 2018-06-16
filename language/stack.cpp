#include "language/stack.h"
#include "language/stackIterator.h"
using namespace StackLang;

Stack::Stack () : head (nullptr), dataSize (0)
{}

Stack::~Stack ()
{
    while (head != nullptr)
    {
        delete head -> elm;
        pop ();
    }
}

void Stack::push (StackElement* ptr)
{
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

unsigned Stack::size () const
{
    return dataSize;
}

Stack::StackIterator Stack::begin () const
{
    return StackIterator (head);
}

Stack::StackIterator Stack::end () const
{
    return StackIterator (nullptr);
}