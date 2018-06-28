#include "language/language.h"

namespace StackLang
{
using StackLang::StackElements::CommandElement;

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