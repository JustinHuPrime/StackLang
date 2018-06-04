#include "dataStack.h"
#include "stackElement.h"
using namespace CustomADT;

DataStack::DataStack (): head {nullptr}, length {0}
{}

DataStack::~DataStack ()
{
    Node* temp = head -> next;
    
    while (head != nullptr)
    {
        temp = head -> next;
        delete head;
        head = temp;
    }
}

void DataStack::push (StackElement* value)
{
    Node* node = new Node;

    node -> next = head;
    node -> data = value;

    head = node;

    length++;
}

StackElement* DataStack::pop ()
{
    if (isEmpty ())
    {
        throw "DataStack::pop: stack is empty - cannot pop.";
    }

    Node* victim = head;
    head = victim -> next;

    StackElement* victimData = victim -> data;

    delete victim;
    
    length--;

    return victimData;
}

StackElement* DataStack::peek () const
{
    return head -> data;
}

int DataStack::getLength () const
{
    return length;
}

bool DataStack::isEmpty () const
{
    return length == 0;
}

StackElement*& DataStack::operator[] (int index) const
{
    if (index >= length)
    {
        throw "DataStack::operator[]: requested index is past the end of the stack.";
    }

    Node* curr = head;

    for (; index > 0; index--)
    {
        curr = curr -> next;
    }

    return curr -> data;
}