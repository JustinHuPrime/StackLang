#include "language/exceptions/languageExceptions.h"
#include "language/language.h"

#include <map>
#include <string>

#include "catch.hpp"

namespace {
using stacklang::checkContext;
using stacklang::checkType;
using stacklang::checkTypes;
using stacklang::DefinedFunction;
using stacklang::ElementPtr;
using stacklang::GLOBAL_CONTEXT;
using stacklang::Prim;
using stacklang::PrimitiveFunction;
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::exceptions::SyntaxError;
using stacklang::exceptions::TypeError;
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::CommandPtr;
using stacklang::stackelements::NumberElement;
using stacklang::stackelements::NumberPtr;
using stacklang::stackelements::SubstackElement;
using stacklang::stackelements::TypeElement;
using std::map;
using std::string;
using std::vector;
}  // namespace

TEST_CASE("check single type simple pass", "[language][checkType]") {
  NumberPtr elm(NumberElement::parse("3.14"));
  TypeElement t = TypeElement(StackElement::DataType::Number);
  REQUIRE(checkType(elm.get(), t, vector<string>{}));
}

TEST_CASE("check single type simple fail", "[language][checkType]") {
  CommandPtr elm(CommandElement::parse("foldl"));
  TypeElement t = TypeElement(StackElement::DataType::Boolean);
  REQUIRE_FALSE(checkType(elm.get(), t, vector<string>{}));
}

TEST_CASE("check substack variants", "[language][checkType]") {
  ElementPtr numberStack(SubstackElement::parse("<<12, 10>>"));
  ElementPtr commandStack(SubstackElement::parse("<<add, subtract>>"));
  TypeElement numberT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Number));
  TypeElement commandT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Command));
  TypeElement substackT = TypeElement(StackElement::DataType::Substack);
  TypeElement anyT = TypeElement(StackElement::DataType::Substack,
                                 new TypeElement(StackElement::DataType::Any));
  REQUIRE(checkType(numberStack.get(), anyT, vector<string>{}));
  REQUIRE(checkType(commandStack.get(), anyT, vector<string>{}));
  REQUIRE(checkType(numberStack.get(), substackT, vector<string>{}));
  REQUIRE(checkType(commandStack.get(), substackT, vector<string>{}));

  REQUIRE(checkType(numberStack.get(), numberT, vector<string>{}));
  REQUIRE(checkType(commandStack.get(), commandT, vector<string>{}));
  REQUIRE_FALSE(checkType(numberStack.get(), commandT, vector<string>{}));
  REQUIRE_FALSE(checkType(commandStack.get(), numberT, vector<string>{}));
}

TEST_CASE("check empty substack always matches substack",
          "[language][checkType]") {
  ElementPtr s(SubstackElement::parse("<<>>"));
  TypeElement numberT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Number));
  TypeElement commandT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Command));
  TypeElement substackT = TypeElement(StackElement::DataType::Substack);
  TypeElement anyT = TypeElement(StackElement::DataType::Substack,
                                 new TypeElement(StackElement::DataType::Any));
  REQUIRE(checkType(s.get(), numberT, vector<string>{}));
  REQUIRE(checkType(s.get(), commandT, vector<string>{}));
  REQUIRE(checkType(s.get(), substackT, vector<string>{}));
  REQUIRE(checkType(s.get(), anyT, vector<string>{}));
}

TEST_CASE("check mixed substack only matched by any", "[language][checkType]") {
  ElementPtr s(SubstackElement::parse("<<\"string\", 2, `map>>"));
  TypeElement anyT = TypeElement(StackElement::DataType::Substack,
                                 new TypeElement(StackElement::DataType::Any));
  TypeElement numberT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Number));
  TypeElement commandT =
      TypeElement(StackElement::DataType::Substack,
                  new TypeElement(StackElement::DataType::Command));
  TypeElement substackT = TypeElement(StackElement::DataType::Substack);
  REQUIRE_FALSE(checkType(s.get(), numberT, vector<string>{}));
  REQUIRE_FALSE(checkType(s.get(), commandT, vector<string>{}));
  REQUIRE(checkType(s.get(), substackT, vector<string>{}));
  REQUIRE(checkType(s.get(), anyT, vector<string>{}));
}

TEST_CASE("check any matches any element", "[language][checkType]") {
  ElementPtr number(NumberElement::parse("3.14"));
  ElementPtr substack(SubstackElement::parse("<<\"string\", 2, `map>>"));
  ElementPtr command(CommandElement::parse("filter"));
  TypeElement anyT = TypeElement(StackElement::DataType::Any);
  REQUIRE(checkType(number.get(), anyT, vector<string>{}));
  REQUIRE(checkType(substack.get(), anyT, vector<string>{}));
  REQUIRE(checkType(command.get(), anyT, vector<string>{}));
}

TEST_CASE("check vector of types good", "[language][checkTypes]") {
  Stack things = Stack{
      StackElement::parse("12"),
      StackElement::parse("Number"),
      StackElement::parse("validate"),
  };
  Stack types = Stack{
      TypeElement::parse("Type"),
      TypeElement::parse("Command"),
  };

  REQUIRE_NOTHROW(checkTypes(things, types, vector<string>{}));
  types.reverse();
  types.push(TypeElement::parse("Number"));
  types.reverse();
  REQUIRE_NOTHROW(checkTypes(things, types, vector<string>{}));
}

TEST_CASE("check vector of types mismatch", "[language][checkTypes]") {
  Stack things = Stack{
      StackElement::parse("12"),
      StackElement::parse("Number"),
      StackElement::parse("validate"),
  };
  Stack types = Stack{
      TypeElement::parse("Number"),
      TypeElement::parse("Command"),
  };
  REQUIRE_THROWS_AS(checkTypes(things, types, vector<string>{}), TypeError);
}

TEST_CASE("check vector of types too long", "[language][checkTypes]") {
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
  REQUIRE_THROWS_AS(checkTypes(things, types, vector<string>{}), TypeError);
}

TEST_CASE("check vector of types too long and no match",
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
  REQUIRE_THROWS_AS(checkTypes(things, types, vector<string>{}), TypeError);
}

TEST_CASE("check context all allowed", "[language][checkContext]") {
  REQUIRE_NOTHROW(
      checkContext(GLOBAL_CONTEXT, nullptr, "some-name", vector<string>{}));
  REQUIRE_NOTHROW(checkContext("map", nullptr, "some-name", vector<string>{}));
}

TEST_CASE("check limiting context respected", "[language][checkContext]") {
  CommandPtr elm(CommandElement::parse("map"));
  REQUIRE_NOTHROW(
      checkContext("map", elm.get(), "map-local", vector<string>{}));
  CommandPtr other(CommandElement::parse("foldl"));
  REQUIRE_THROWS_AS(
      checkContext("map", other.get(), "foldl-local", vector<string>{}),
      SyntaxError);
  REQUIRE_THROWS_AS(checkContext(GLOBAL_CONTEXT, other.get(), "foldl-local",
                                 vector<string>{}),
                    SyntaxError);
}