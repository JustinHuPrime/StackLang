#ifndef SUBSTACKELEMENT_H
#define SUBSTACKELEMENT_H

#include "language/stack/stackElement.h"

#include <list>
#include <string>

namespace StackLang
{
namespace StackElements
{
using std::list;
using std::string;

class SubstackElement : public StackElement
{
  public:
    SubstackElement ();
    ~SubstackElement () override;

    operator const string () const override;
    const list< StackElement* >& getData ();

  private:
    list< StackElement* > data;
};
const string SUBSTACK_BEGIN = "<<";
const string SUBSTACK_END = ">>";
const string SUBSTACK_SEPARATOR = ", ";
} // namespace StackElements
} // namespace StackLang

#endif /* SUBSTACKELEMENT_H */