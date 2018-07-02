#ifndef STRINGELEMENT_H
#define STRINGELEMENT_H

#include "language/stack/stackElement.h"
#include "utils/stringUtils.h"

#include <string>

namespace StackLang
{
namespace StackElements
{
using std::string;

class StringElement : public StackElement
{
  public:
    StringElement (string);
    StringElement* clone () const override;

    operator const string () const override;
    const string& getData ();

  private:
    string data;
};
} // namespace StackElements
} // namespace StackLang

#endif /* STRINGELEMENT_H */