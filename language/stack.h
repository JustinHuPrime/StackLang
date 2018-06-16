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
        Stack (const Stack& other);

        ~Stack ();

        void push (StackElement* ptr);
        void pop ();
        StackElement* top ();

        StackIterator begin () const;
        StackIterator end () const;

        unsigned size () const;

        class StackIterator
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

        private:

        struct Node
        {
            StackElement* elm;
            Node* next;
        };

        Node* head;
        int dataSize;
    };
}

#endif /* STACK_H */