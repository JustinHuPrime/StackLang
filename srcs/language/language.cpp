#include "language/language.h"

#include "language/stack/stackElements/commandElement.h"

namespace StackLang
{
using StackLang::StackElements::CommandElement;

void execute(Stack& s, map< string, DefinedFunction >& defines)
{
    switch (s.top()->getType())
    {
        case StackElement::DataType::Command:
        {
            CommandElement* command = dynamic_cast< CommandElement* >(s.top());
            s.pop();

            auto result = PRIMITIVES.find(command->getData());

            if (result != PRIMITIVES.end())
            {
                result->second(s, defines);
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