#ifndef COMMANDELEMENT_H
#define COMMANDELEMENT_H

#include "stackElement.h"
#include <string>
using std::string;

namespace StackElements
{
    class CommandElement : public StackElement
    {
        public:

        CommandElement (string);

        operator const string () const override;
        const string& getData () const;

        private:

        string data;
    };
}

#endif /* COMMANDELEMENT_H */