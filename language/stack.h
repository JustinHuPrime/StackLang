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

        /**
         * Creates a stack, optionally with a limit on the number of elements.
         */
        Stack ();
        Stack (unsigned long);

        ~Stack ();
        
        /**
         * Manipulates stack elements.
         * Pop does not delete the element, but the stack destructor does assume that when the stack is destroyed, it's managed elements should be deallocated.
         */
        void push (StackElement* ptr);
        void pop ();
        StackElement* top ();

        /**
         * Stack iterator functions
         */
        StackIterator begin () const;
        StackIterator end () const;

        /**
         * Gets the stack's size, size limit
         */
        unsigned long size () const;
        unsigned long getLimit () const;

        /**
         * Sets a new limit. Throws StackOverflowError if too big.
         */
        void setLimit (unsigned long);

        /**
         * Returns true if the stack is empty
         */
        bool empty () const;

        private:

        Stack (const Stack&);
        Stack& operator= (const Stack&);

        struct Node
        {
            StackElement* elm;
            Node* next;
        };

        Node* head;
        unsigned long dataSize;
        unsigned long limit;

        void clear ();
    };
}

#endif /* STACK_H */