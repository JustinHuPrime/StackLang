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
    CommandElement* clone () const override;

    operator const string () const override;
    string& getData ();

    static const string ALLOWED_COMMAND;

  private:
    string data;
};
} // namespace StackElements
} // namespace StackLang

#endif /* COMMANDELEMENT_H */