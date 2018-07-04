#ifndef BOOLEANELEMENT_H
#define BOOLEANELEMENT_H

#include "language/stack/stackElement.h"

#include <string>

namespace StackLang
{
namespace StackElements
{
using std::string;

class BooleanElement : public StackElement
{
  public:
    BooleanElement (bool);
    BooleanElement* clone () const override;

    operator const string () const override;
    bool& getData ();

  private:
    bool data;
};
const string TSTR = "true";
const string FSTR = "false";
} // namespace StackElements
} // namespace StackLang

#endif /* BOOLEANELEMENT_H */