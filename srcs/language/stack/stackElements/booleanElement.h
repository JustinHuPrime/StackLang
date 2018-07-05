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
    BooleanElement(bool);
    BooleanElement* clone() const override;

    operator const string() const override;
    bool& getData();

    static const string TSTR;
    static const string FSTR;

  private:
    bool data;
};
} // namespace StackElements
} // namespace StackLang

#endif /* BOOLEANELEMENT_H */