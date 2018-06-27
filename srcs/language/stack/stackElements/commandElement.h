#ifndef COMMANDELEMENT_H
#define COMMANDELEMENT_H

#include "language/stack/stackElement.h"
#include <string>
using std::string;

namespace StackLang
{
    namespace StackElements
    {
        class CommandElement : public StackElement
        {
            public:

            CommandElement (string);

            operator const string () const override;
            const string& getData ();

            private:

            string data;
        };
    }

    namespace CharSet
    {
        const string ALLOWED_COMMAND = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";
    }
}

#endif /* COMMANDELEMENT_H */