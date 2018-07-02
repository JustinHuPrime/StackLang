#ifndef TYPEELEMENT_H
#define TYPEELEMENT_H

#include "language/stack/stackElement.h"

#include <string>
using std::string;

namespace StackLang
{
namespace StackElements
{
class TypeElement : public StackElement
{
  public:
    TypeElement (DataType);
    TypeElement* clone () const override;

    operator const string () const override;
    DataType getData ();

    static string to_string (DataType);

  private:
    DataType data;
};

const string TYPES[] = {"Number", "String", "Boolean", "Substack", "Type", "Command", "Any"};
} // namespace StackElements
} // namespace StackLang

#endif /* TYPEELEMENT_H */