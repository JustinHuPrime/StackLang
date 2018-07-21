#include "language/stack/stackElements/typeElement.h"

#include <algorithm>
#include <array>
#include <string>

#include "language/exceptions/parserException.h"
#include "util/stringUtils.h"

namespace StackLang {
namespace StackElements {
using StackLang::StackElement;
using StackLang::Exceptions::ParserException;
using std::array;
using std::distance;
using std::find;
using std::string;
using Util::starts_with;

const char* TypeElement::PARENS = "()";

TypeElement* TypeElement::parse(const string& s) {
  if (s.find('(') == string::npos) {
    auto value = find(TYPES().begin(), TYPES().end(), s);
    if (value == TYPES().end()) {
      throw ParserException("Input is not a type.", s, 0);
    }
    return new TypeElement(static_cast<DataType>(value - begin(TYPES())));
  } else if (starts_with(
                 s, "Substack")) {  // has to be a substack - no way around it.
    return new TypeElement(
        DataType::Substack,
        parse(s.substr(s.find_first_of('(') + 1,
                       s.length() - s.find_first_of('(') - 2)));
  } else {
    throw ParserException("Cannot have a specialzation except on Substacks.", s,
                          s.find('('));
  }
}

TypeElement::TypeElement(DataType type)
    : StackElement(StackElement::DataType::Type),
      data(type),
      specialization(nullptr) {}

TypeElement::TypeElement(DataType type, TypeElement* subType)
    : StackElement(StackElement::DataType::Type),
      data(type),
      specialization(subType) {}

TypeElement::TypeElement(const TypeElement& other)
    : StackElement(StackElement::DataType::Type), data(other.data) {
  if (other.specialization == nullptr)
    specialization = nullptr;
  else
    specialization = new TypeElement(*(other.specialization));
}

TypeElement& TypeElement::operator=(const TypeElement& other) {
  data = other.data;
  if (specialization == nullptr) delete specialization;
  specialization = other.specialization == nullptr
                       ? nullptr
                       : new TypeElement(*(other.specialization));
  return *this;
}

TypeElement::TypeElement(TypeElement&& other)
    : StackElement(StackElement::DataType::Type), data(other.data) {
  TypeElement* temp = specialization;
  specialization = other.specialization;
  other.specialization = temp;
}

TypeElement& TypeElement::operator=(TypeElement&& other) {
  data = other.data;
  TypeElement* temp = specialization;
  specialization = other.specialization;
  other.specialization = temp;
  return *this;
}

TypeElement::~TypeElement() { delete specialization; }

TypeElement* TypeElement::clone() const { return new TypeElement(data); }

TypeElement::operator const string() const {
  if (specialization == nullptr)
    return to_string(data);
  else
    return to_string(data) + "(" + string(*specialization) + ")";
}

StackElement::DataType TypeElement::getData() const { return data; }

string TypeElement::to_string(StackElement::DataType type) {
  return TYPES()[static_cast<unsigned>(type)];
}

const vector<string>& TypeElement::TYPES() {
  static vector<string>* TYPES = new vector<string>{
      "Number", "String", "Boolean", "Substack", "Type", "Command", "Any"};
  return *TYPES;
}
}  // namespace StackElements
}  // namespace StackLang