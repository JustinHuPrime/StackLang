#include "language/exceptions/languageExceptions.h"
#include "language/language.h"

#include "catch.hpp"

namespace {
using stacklang::checkContext;
using stacklang::checkType;
using stacklang::checkTypes;
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::exceptions::SyntaxError;
using stacklang::exceptions::TypeError;
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::NumberElement;
using stacklang::stackelements::SubstackElement;
using stacklang::stackelements::TypeElement;
}  // namespace

TEST_CASE("check single type simple pass", "[language][checkType]") {
  NumberElement* elm = new NumberElement("22/7");
  TypeElement t = TypeElement(StackElement::DataType::Number);
  REQUIRE(checkType(elm, t));
  delete elm;
}

TEST_CASE("check single type simple fail", "[language][checkType]") {
  CommandElement* elm = new CommandElement("foldl");
  TypeElement t = TypeElement(StackElement::DataType::Boolean);
  REQUIRE_FALSE(checkType(elm, t));
  delete elm;
}

TEST_CASE("check number types", "[language][checkType]") {
  NumberElement* exact = new NumberElement("22/7");
  NumberElement* inexact = NumberElement::parse("~22/7");
  TypeElement exactT =
      TypeElement(StackElement::DataType::Number,
                  new TypeElement(StackElement::DataType::Exact));
  TypeElement inexactT =
      TypeElement(StackElement::DataType::Number,
                  new TypeElement(StackElement::DataType::Inexact));
  REQUIRE(checkType(exact, exactT));
  REQUIRE(checkType(inexact, inexactT));
  REQUIRE_FALSE(checkType(exact, inexactT));
  REQUIRE_FALSE(checkType(inexact, exactT));
  delete exact;
  delete inexact;
}

TEST_CASE("check command variants", "[language][checkType]") {
  CommandElement* raw = new CommandElement("raw-command");
  CommandElement* quoted = CommandElement::parse("`quoted-command");
  TypeElement rawT = TypeElement(StackElement::DataType::Command);
  TypeElement quotedT =
      TypeElement(StackElement::DataType::Command,
                  new TypeElement(StackElement::DataType::Quoted));
  REQUIRE(checkType(raw, rawT));
  REQUIRE(checkType(quoted, quotedT));
  REQUIRE_FALSE(checkType(raw, quotedT));
  REQUIRE_FALSE(checkType(quoted, rawT));
  delete raw;
  delete quoted;
}

TEST_CASE("check substack variants", "[language][checkType]") {
  StackElement* numberStack = new SubstackElement(
      Stack{new NumberElement("12"), new NumberElement("10")});
  StackElement* commandStack = new SubstackElement(
      Stack{new CommandElement("add"), new CommandElement("subtract")});
  TypeElement numberT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Number));
  TypeElement commandT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Command));
  TypeElement substackT = TypeElement(StackElement::DataType::Substack);
  TypeElement anyT = TypeElement(StackElement::DataType::Substack,
                                 new TypeElement(StackElement::DataType::Any));
  REQUIRE(checkType(numberStack, anyT));
  REQUIRE(checkType(commandStack, anyT));
  REQUIRE(checkType(numberStack, substackT));
  REQUIRE(checkType(commandStack, substackT));

  REQUIRE(checkType(numberStack, numberT));
  REQUIRE(checkType(commandStack, commandT));
  REQUIRE_FALSE(checkType(numberStack, commandT));
  REQUIRE_FALSE(checkType(commandStack, numberT));
  delete numberStack;
  delete commandStack;
}

TEST_CASE("check empty substack always matches substack",
          "[language][checkType]") {
  StackElement* s = new SubstackElement(Stack{});
  TypeElement numberT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Number));
  TypeElement commandT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Command));
  TypeElement substackT = TypeElement(StackElement::DataType::Substack);
  TypeElement anyT = TypeElement(StackElement::DataType::Substack,
                                 new TypeElement(StackElement::DataType::Any));
  REQUIRE(checkType(s, numberT));
  REQUIRE(checkType(s, commandT));
  REQUIRE(checkType(s, substackT));
  REQUIRE(checkType(s, anyT));
  delete s;
}

TEST_CASE("check mixed substack only matched by any", "[language][checkType]") {
  StackElement* s = SubstackElement::parse("<<\"string\", 2, `map>>");
  TypeElement anyT = TypeElement(StackElement::DataType::Substack,
                                 new TypeElement(StackElement::DataType::Any));
  TypeElement numberT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Number));
  TypeElement commandT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Command));
  TypeElement substackT = TypeElement(StackElement::DataType::Substack);
  REQUIRE_FALSE(checkType(s, numberT));
  REQUIRE_FALSE(checkType(s, commandT));
  REQUIRE(checkType(s, substackT));
  REQUIRE(checkType(s, anyT));
  delete s;
}

TEST_CASE("check any matches any element", "[language][checkType]") {
  StackElement* number = new NumberElement("22/7");
  StackElement* substack = SubstackElement::parse("<<\"string\", 2, `map>>");
  StackElement* command = new CommandElement("filter");
  TypeElement anyT = TypeElement(StackElement::DataType::Any);
  REQUIRE(checkType(number, anyT));
  REQUIRE(checkType(substack, anyT));
  REQUIRE(checkType(command, anyT));
  delete number;
  delete substack;
  delete command;
}

TEST_CASE("check list of types good", "[language][checkTypes]") {
  Stack things = Stack{
      StackElement::parse("12"),
      StackElement::parse("Number"),
      StackElement::parse("validate"),
  };
  Stack types = Stack{
      TypeElement::parse("Type"),
      TypeElement::parse("Command"),
  };

  REQUIRE_NOTHROW(checkTypes(things, types));
  types.reverse();
  types.push(TypeElement::parse("Number"));
  types.reverse();
  REQUIRE_NOTHROW(checkTypes(things, types));
}

TEST_CASE("check list of types mismatch", "[language][checkTypes]") {
  Stack things = Stack{
      StackElement::parse("12"),
      StackElement::parse("Number"),
      StackElement::parse("validate"),
  };
  Stack types = Stack{
      TypeElement::parse("Number"),
      TypeElement::parse("Command"),
  };
  REQUIRE_THROWS_AS(checkTypes(things, types), TypeError);
}

TEST_CASE("check list of types too long", "[language][checkTypes]") {
  Stack things = Stack{
      StackElement::parse("12"),
      StackElement::parse("Number"),
      StackElement::parse("validate"),
  };
  Stack types = Stack{
      TypeElement::parse("String"),
      TypeElement::parse("Number"),
      TypeElement::parse("Type"),
      TypeElement::parse("Command"),
  };
  REQUIRE_THROWS_AS(checkTypes(things, types), TypeError);
}

TEST_CASE("check list of types too long and no match",
          "[language][checkTypes]") {
  Stack things = Stack{
      StackElement::parse("12"),
      StackElement::parse("Number"),
      StackElement::parse("validate"),
  };
  Stack types = Stack{
      TypeElement::parse("String"),
      TypeElement::parse("Number"),
      TypeElement::parse("Number"),
      TypeElement::parse("Command"),
  };
  REQUIRE_THROWS_AS(checkTypes(things, types), TypeError);
}

TEST_CASE("check context all allowed", "[language][checkContext]") {
  REQUIRE_NOTHROW(checkContext(nullptr, nullptr, "some-name"));
  CommandElement* elm = new CommandElement("map");
  REQUIRE_NOTHROW(checkContext(elm, nullptr, "some-name"));
  delete elm;
}

TEST_CASE("check limiting context respected", "[language][checkContext]") {
  CommandElement* elm = new CommandElement("map");
  REQUIRE_NOTHROW(checkContext(elm, elm, "map-local"));
  CommandElement* other = new CommandElement("foldl");
  REQUIRE_THROWS_AS(checkContext(elm, other, "foldl-local"), SyntaxError);
  REQUIRE_THROWS_AS(checkContext(nullptr, other, "foldl-local"), SyntaxError);
}