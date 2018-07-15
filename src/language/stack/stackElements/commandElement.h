#ifndef STACKLANG_LANGUAGE_STACK_STACKELEMENTS_COMMANDELEMENT_H_
#define STACKLANG_LANGUAGE_STACK_STACKELEMENTS_COMMANDELEMENT_H_

#include <string>

#include "language/stack/stackElement.h"

namespace StackLang {
namespace StackElements {
using std::string;

class CommandElement : public StackElement {
 public:
  static const char* ALLOWED_COMMAND;

  static CommandElement* parse(const string& s);

  explicit CommandElement(string);
  CommandElement* clone() const override;

  explicit operator const string() const override;
  const string& getData() const;

 private:
  static const char* COMMAND_LDELIM;
  static const char* COMMAND_RDELIM;

  string data;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_COMMANDELEMENT_H_