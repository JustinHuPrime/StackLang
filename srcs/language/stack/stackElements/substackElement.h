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
    SubstackElement();
    SubstackElement* clone() const override;

    operator const string() const override;
    Stack& getData();

  private:
    Stack data;
    static const string SUBSTACK_BEGIN;
    static const string SUBSTACK_END;
    static const string SUBSTACK_SEPARATOR;
    static const string SUBSTACK_EMPTY;
};
} // namespace StackElements
} // namespace StackLang

#endif /* SUBSTACKELEMENT_H */