#ifndef DATASTACK_H
#define DATASTACK_H

#include "stackElement.h"

namespace CustomADT
{
    class DataStack
    {
        public:
            DataStack ();
            ~DataStack ();

            void push (StackElement*);
            StackElement* pop ();
            StackElement* peek () const;
            int getLength () const;
            bool isEmpty () const;

            StackElement*& operator [] (int) const;

        private:
            struct Node
            {
                StackElement* data;
                Node* next;
            };

            Node* head;
            int length;
    };
}

#endif /* DATASTACK_H */