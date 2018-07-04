#ifndef NUMBERELEMENT_H
#define NUMBERELEMENT_H

#include "language/stack/stackElement.h"

#include <gmp.h>
#include <gmpxx.h>
#include <string>

namespace StackLang
{
namespace StackElements
{
using std::string;

class NumberElement : public StackElement
{
  public:
    NumberElement (string);
    NumberElement* clone () const override;

    operator const string () const override;
    mpq_class& getData ();

  private:
    mpq_class data;
};
const string ALLOWED_NUMBER = "-+/1234567890.'";
const string NUMBER_SIGNS = "-+";
} // namespace StackElements
} // namespace StackLang

#endif /* NUMBERELEMENT_H */