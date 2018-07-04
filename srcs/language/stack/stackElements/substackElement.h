#ifndef SUBSTACKELEMENT_H
#define SUBSTACKELEMENT_H

#include "language/stack.h"
#include "language/stack/stackElement.h"

#include <string>

namespace StackLang
{
namespace StackElements
{
using std::string;

class SubstackElement : public StackElement
{
  public:
    SubstackElement ();
    SubstackElement* clone () const override;

    operator const string () const override;
    Stack& getData ();

  private:
    Stack data;
};
const string SUBSTACK_BEGIN = "<<";
const string SUBSTACK_END = ">>";
const string SUBSTACK_SEPARATOR = ", ";
} // namespace StackElements
} // namespace StackLang

#endif /* SUBSTACKELEMENT_H */