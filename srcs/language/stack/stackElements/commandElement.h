#ifndef COMMANDELEMENT_H
#define COMMANDELEMENT_H

#include "language/stack/stackElement.h"

#include <string>

namespace StackLang
{
namespace StackElements
{
using std::string;

class CommandElement : public StackElement
{
  public:
    CommandElement (string);

    operator const string () const override;
    const string& getData ();

  private:
    string data;
};
const string ALLOWED_COMMAND = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-?*";
} // namespace StackElements
} // namespace StackLang

#endif /* COMMANDELEMENT_H */