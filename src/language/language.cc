#include "language/language.h"

#include <algorithm>
#include <iterator>

#include "language/stack/stackElements/commandElement.h"

namespace StackLang {
using StackLang::StackElements::CommandElement;
using std::begin;
using std::end;
using std::find_if;

void execute(Stack& s, DefineMap& defines) {
  switch (s.top()->getType()) {
    case StackElement::DataType::Command: {
      CommandElement* command = dynamic_cast<CommandElement*>(s.pop());

      auto result =
          find_if(begin(PRIMITIVES), end(PRIMITIVES),
                  [&command](pair<const char* const, PrimitiveFunction> entry) {
                    return string(entry.first) == command->getData();
                  });

      if (result != end(PRIMITIVES)) {
        result->second(s, defines);
      } else {
        // find from non-primitives
        if (false) {
        } else {
        }
      }

      delete command;
      break;
    }
    default: { return; }
  }
}
}  // namespace StackLang