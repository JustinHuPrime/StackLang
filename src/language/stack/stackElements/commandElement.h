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

  static CommandElement* parse(const string&);

  explicit CommandElement(const string&, bool isQuoted = false) noexcept;
  CommandElement* clone() const noexcept override;

  explicit operator const string() const noexcept override;
  const string& getData() const noexcept;
  bool isQuoted() const noexcept;

  static const char QUOTE_CHAR;

 private:
  static const char* COMMAND_LDELIM;
  static const char* COMMAND_RDELIM;

  string data;
  bool quoted;
};
}  // namespace StackElements
}  // namespace StackLang

#endif  // STACKLANG_LANGUAGE_STACK_STACKELEMENTS_COMMANDELEMENT_H_