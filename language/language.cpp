#include "language/language.h"
#include "language/stackElement.h"
#include "language/stackElements/commandElement.h"
#include <list>
#include <map>
using namespace StackElements;
using namespace StackLang;
using namespace Primitives;
using std::list;
using std::map;

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