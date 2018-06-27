#include "language/language.h"
using namespace StackLang;
using namespace StackElements;
using namespace Primitives;

namespace StackLang
{
const map< string, stackFunction > PRIMITIVES = {
        {"plus", [](Stack& s) {
             //do things
         }},
        {"divide", [](Stack& s) {
             //do other things
         }} //and so on.
};

void execute (Stack& s)
{
    switch (s.top ()->getType ())
    {
        case StackElement::DataType::Command:
        {
            CommandElement* command = dynamic_cast< CommandElement* > (s.top ());
            s.pop ();

            auto result = PRIMITIVES.find (command->getData ());

            if (result != PRIMITIVES.end ())
            {
                result->second (s);
            }
            else
            {
                //find from non-primitives
                if (false)
                {
                }
                else
                {}
            }

            delete command;
            break;
        }
        default:
        {
            return;
        }
    }
}
} // namespace StackLang