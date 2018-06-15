#include "language/language.h"
using namespace StackLang;
using namespace StackElements;
using namespace Primitives;

namespace StackLang
{
    const map<string, stackFunction> PRIMITIVES = {
        {"plus", [] (list<StackElement*>& s)
        {
            //do things
        }},
        {"divide", [] (list<StackElement*>& s)
        {
            //do other things
        }} //and so on.
    };

    void execute (list<StackElement*>& s)
    {
        switch (s.front () -> getType ())
        {
            case StackElement::DataType::Command:
            {
                CommandElement* command = dynamic_cast<CommandElement*> (s.front ());
                s.pop_front ();
                auto result = PRIMITIVES.find (command -> getData ());
                if (result != PRIMITIVES.end ())
                {
                    result -> second (s);
                }
                else
                {
                    //find from non-primitives
                    if (false)
                    {}
                    else
                    {}
                }
                break;
            }
            default:
            {
                return;
            }
        }
    }
}