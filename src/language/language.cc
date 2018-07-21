#include "language/language.h"

#include <algorithm>
#include <iterator>

#include <gmp.h>
#include <gmpxx.h>

#include "language/exceptions/stopError.h"
#include "language/exceptions/syntaxError.h"
#include "language/exceptions/typeError.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/numberElement.h"
#include "language/stack/stackIterator.h"

namespace StackLang {
using StackLang::Exceptions::StopError;
using StackLang::Exceptions::SyntaxError;
using StackLang::Exceptions::TypeError;
using StackLang::StackElements::CommandElement;
using StackLang::StackElements::NumberElement;
using std::begin;
using std::end;
using std::find_if;

bool stopFlag = false;

const map<string, PrimitiveFunction>& PRIMITIVES() {
  static map<string, PrimitiveFunction>* prims =
      new map<string, PrimitiveFunction>{
#include "language/primitives/boolean.inc"
#include "language/primitives/command.inc"
#include "language/primitives/number.inc"
#include "language/primitives/special.inc"
#include "language/primitives/string.inc"
#include "language/primitives/substack.inc"
#include "language/primitives/type.inc"
      };
  return *prims;
}

void execute(Stack& s, map<string, DefinedFunction>& defines) {
  if (stopFlag) {
    stopFlag = false;
    throw StopError();
  }

  const auto& PRIMS = PRIMITIVES();

  if (s.top()->getType() == StackElement::DataType::Command &&
      !dynamic_cast<CommandElement*>(s.top())->isQuoted()) {
    CommandElement* command = dynamic_cast<CommandElement*>(s.pop());
    const auto& primResult =
        find_if(PRIMS.begin(), PRIMS.end(),
                [&command](const pair<string, PrimitiveFunction>& entry) {
                  return entry.first == command->getData();
                });

    if (primResult != PRIMS.end()) {
      const auto& types = primResult->second.first;

      auto typeIter = types.begin();
      auto stackIter = s.begin();

      for (; typeIter != types.end() && stackIter != s.end();
           typeIter++, stackIter++) {
        if (*typeIter != (*stackIter)->getType()) {
          throw TypeError(*typeIter, *stackIter);
        }
      }
      if (typeIter != types.end()) {
        throw TypeError(*typeIter);
      }

      primResult->second.second(s, defines);
    } else {
      // find from non-primitives
      const auto& defResult =
          find_if(defines.begin(), defines.end(),
                  [&command](const pair<string, DefinedFunction>& entry) {
                    return entry.first == command->getData();
                  });

      if (defResult != defines.end()) {
        const auto& types = defResult->second.first;
        const auto& commands = defResult->second.second;

        auto typeIter = types.begin();
        auto stackIter = s.begin();

        for (; typeIter != types.end() && stackIter != s.end();
             typeIter++, stackIter++) {
          if (typeIter->getData() != (*stackIter)->getType()) {
            throw TypeError(typeIter->getData(), *stackIter);
          }
        }
        if (typeIter != types.end()) {
          throw TypeError(typeIter->getData());
        }

        for (auto c : commands) {
          s.push(c->clone());
          execute(s, defines);
        }
      } else {
        throw SyntaxError("Given command is not recognized.",
                          command->getData(), 0);
      }
    }

    delete command;
  }
}
}  // namespace StackLang