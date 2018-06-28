#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#include "language/stack/stackElement.h"

#include <string>

namespace StackLang
{
namespace StackElements
{
using std::string;

class NumberElement : public StackElement
{
  public:
    NumberElement (double);

    operator const string () const override;
    double getData ();

  private:
    double data;
};
const string ALLOWED_NUMBER = "1234567890.'";
} // namespace StackElements
} // namespace StackLang

#endif /* NUMBERELEMENT_H */