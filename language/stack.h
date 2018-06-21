#ifndef STACK_H
#define STACK_H

#include "language/stackElement.h"

namespace StackLang
{
    class Stack
    {
        struct Node;

        public:

        class StackIterator;

        Stack ();
        Stack (unsigned long);
        Stack (const Stack& other);

        ~Stack ();

        void push (StackElement* ptr);
        void pop ();
        StackElement* top ();

        StackIterator begin () const;
        StackIterator end () const;

        unsigned long size () const;
        unsigned long getLimit () const;

        private:

        struct Node
        {
            StackElement* elm;
            Node* next;
        };

        Node* head;
        unsigned long dataSize;
        unsigned long limit;
        bool limited;
    };
}

#endif /* STACK_H */