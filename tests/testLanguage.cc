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
using std::list;
using std::map;
using std::string;
}  // namespace

TEST_CASE("check single type simple pass", "[language][checkType]") {
  NumberPtr elm(NumberElement::parse("3.14"));
  TypeElement t = TypeElement(StackElement::DataType::Number);
  REQUIRE(checkType(elm.get(), t, list<string>{}));
}

TEST_CASE("check single type simple fail", "[language][checkType]") {
  CommandPtr elm(CommandElement::parse("foldl"));
  TypeElement t = TypeElement(StackElement::DataType::Boolean);
  REQUIRE_FALSE(checkType(elm.get(), t, list<string>{}));
}

TEST_CASE("check command variants", "[language][checkType]") {
  CommandPtr raw(CommandElement::parse("raw-command"));
  CommandPtr quoted(CommandElement::parse("`quoted-command"));
  TypeElement anyT = TypeElement(StackElement::DataType::Command);
  TypeElement quotedT =
      TypeElement(StackElement::DataType::Command,
                  new TypeElement(StackElement::DataType::Quoted));
  TypeElement rawT =
      TypeElement(StackElement::DataType::Command,
                  new TypeElement(StackElement::DataType::Unquoted));
  REQUIRE(checkType(raw.get(), anyT, list<string>{}));
  REQUIRE(checkType(quoted.get(), anyT, list<string>{}));
  REQUIRE(checkType(raw.get(), rawT, list<string>{}));
  REQUIRE(checkType(quoted.get(), quotedT, list<string>{}));
  REQUIRE_FALSE(checkType(raw.get(), quotedT, list<string>{}));
  REQUIRE_FALSE(checkType(quoted.get(), rawT, list<string>{}));
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
  REQUIRE(checkType(numberStack.get(), anyT, list<string>{}));
  REQUIRE(checkType(commandStack.get(), anyT, list<string>{}));
  REQUIRE(checkType(numberStack.get(), substackT, list<string>{}));
  REQUIRE(checkType(commandStack.get(), substackT, list<string>{}));

  REQUIRE(checkType(numberStack.get(), numberT, list<string>{}));
  REQUIRE(checkType(commandStack.get(), commandT, list<string>{}));
  REQUIRE_FALSE(checkType(numberStack.get(), commandT, list<string>{}));
  REQUIRE_FALSE(checkType(commandStack.get(), numberT, list<string>{}));
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
  REQUIRE(checkType(s.get(), numberT, list<string>{}));
  REQUIRE(checkType(s.get(), commandT, list<string>{}));
  REQUIRE(checkType(s.get(), substackT, list<string>{}));
  REQUIRE(checkType(s.get(), anyT, list<string>{}));
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
  REQUIRE_FALSE(checkType(s.get(), numberT, list<string>{}));
  REQUIRE_FALSE(checkType(s.get(), commandT, list<string>{}));
  REQUIRE(checkType(s.get(), substackT, list<string>{}));
  REQUIRE(checkType(s.get(), anyT, list<string>{}));
}

TEST_CASE("check any matches any element", "[language][checkType]") {
  ElementPtr number(NumberElement::parse("3.14"));
  ElementPtr substack(SubstackElement::parse("<<\"string\", 2, `map>>"));
  ElementPtr command(CommandElement::parse("filter"));
  TypeElement anyT = TypeElement(StackElement::DataType::Any);
  REQUIRE(checkType(number.get(), anyT, list<string>{}));
  REQUIRE(checkType(substack.get(), anyT, list<string>{}));
  REQUIRE(checkType(command.get(), anyT, list<string>{}));
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

  REQUIRE_NOTHROW(checkTypes(things, types, list<string>{}));
  types.reverse();
  types.push(TypeElement::parse("Number"));
  types.reverse();
  REQUIRE_NOTHROW(checkTypes(things, types, list<string>{}));
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
  REQUIRE_THROWS_AS(checkTypes(things, types, list<string>{}), TypeError);
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
  REQUIRE_THROWS_AS(checkTypes(things, types, list<string>{}), TypeError);
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
  REQUIRE_THROWS_AS(checkTypes(things, types, list<string>{}), TypeError);
}

TEST_CASE("check context all allowed", "[language][checkContext]") {
  REQUIRE_NOTHROW(
      checkContext(GLOBAL_CONTEXT, nullptr, "some-name", list<string>{}));
  REQUIRE_NOTHROW(checkContext("map", nullptr, "some-name", list<string>{}));
}

TEST_CASE("check limiting context respected", "[language][checkContext]") {
  CommandPtr elm(CommandElement::parse("map"));
  REQUIRE_NOTHROW(checkContext("map", elm.get(), "map-local", list<string>{}));
  CommandPtr other(CommandElement::parse("foldl"));
  REQUIRE_THROWS_AS(
      checkContext("map", other.get(), "foldl-local", list<string>{}),
      SyntaxError);
  REQUIRE_THROWS_AS(
      checkContext(GLOBAL_CONTEXT, other.get(), "foldl-local", list<string>{}),
      SyntaxError);
}

TEST_CASE("check pairs between stack and Prim", "[language][PRIMTIVES]") {
  Stack stk = Stack{new TypeElement(StackElement::DataType::Number)};
  Prim pf = [](Stack&, map<string, DefinedFunction>&, list<string>&) {
    return;
  };

  PrimitiveFunction* p = new PrimitiveFunction{stk, pf};
  delete p;
}

TEST_CASE("check PRIMITIVES segfault special case", "[language][PRIMTIVES]") {
  Stack stk = Stack{new TypeElement(StackElement::DataType::Number)};
  Prim pf = [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
    return;
  };

  map<string, PrimitiveFunction>* manual =
      new map<string, PrimitiveFunction>{{"drop*", PrimitiveFunction{stk, pf}}};

  map<string, PrimitiveFunction>* prims = new map<string, PrimitiveFunction>{
      {"drop*",
       PrimitiveFunction{Stack{new TypeElement(StackElement::DataType::Number)},
                         [](Stack& s, map<string, DefinedFunction>&,
                            list<string>&) { return; }}},
  };
}