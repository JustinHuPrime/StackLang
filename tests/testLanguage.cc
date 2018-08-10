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
using stacklang::GLOBAL_CONTEXT;
using stacklang::Prim;
using stacklang::PrimitiveFunction;
using stacklang::Stack;
using stacklang::StackElement;
using stacklang::exceptions::SyntaxError;
using stacklang::exceptions::TypeError;
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::NumberElement;
using stacklang::stackelements::SubstackElement;
using stacklang::stackelements::TypeElement;
using std::list;
using std::map;
using std::string;
}  // namespace

TEST_CASE("check single type simple pass", "[language][checkType]") {
  NumberElement* elm = new NumberElement("22/7");
  TypeElement t = TypeElement(StackElement::DataType::Number);
  REQUIRE(checkType(elm, t, list<string>{}));
  delete elm;
}

TEST_CASE("check single type simple fail", "[language][checkType]") {
  CommandElement* elm = new CommandElement("foldl");
  TypeElement t = TypeElement(StackElement::DataType::Boolean);
  REQUIRE_FALSE(checkType(elm, t, list<string>{}));
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
  REQUIRE(checkType(exact, exactT, list<string>{}));
  REQUIRE(checkType(inexact, inexactT, list<string>{}));
  REQUIRE_FALSE(checkType(exact, inexactT, list<string>{}));
  REQUIRE_FALSE(checkType(inexact, exactT, list<string>{}));
  delete exact;
  delete inexact;
}

TEST_CASE("check command variants", "[language][checkType]") {
  CommandElement* raw = new CommandElement("raw-command");
  CommandElement* quoted = CommandElement::parse("`quoted-command");
  TypeElement anyT = TypeElement(StackElement::DataType::Command);
  TypeElement quotedT =
      TypeElement(StackElement::DataType::Command,
                  new TypeElement(StackElement::DataType::Quoted));
  TypeElement rawT =
      TypeElement(StackElement::DataType::Command,
                  new TypeElement(StackElement::DataType::Unquoted));
  REQUIRE(checkType(raw, anyT, list<string>{}));
  REQUIRE(checkType(quoted, anyT, list<string>{}));
  REQUIRE(checkType(raw, rawT, list<string>{}));
  REQUIRE(checkType(quoted, quotedT, list<string>{}));
  REQUIRE_FALSE(checkType(raw, quotedT, list<string>{}));
  REQUIRE_FALSE(checkType(quoted, rawT, list<string>{}));
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
  REQUIRE(checkType(numberStack, anyT, list<string>{}));
  REQUIRE(checkType(commandStack, anyT, list<string>{}));
  REQUIRE(checkType(numberStack, substackT, list<string>{}));
  REQUIRE(checkType(commandStack, substackT, list<string>{}));

  REQUIRE(checkType(numberStack, numberT, list<string>{}));
  REQUIRE(checkType(commandStack, commandT, list<string>{}));
  REQUIRE_FALSE(checkType(numberStack, commandT, list<string>{}));
  REQUIRE_FALSE(checkType(commandStack, numberT, list<string>{}));
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
  REQUIRE(checkType(s, numberT, list<string>{}));
  REQUIRE(checkType(s, commandT, list<string>{}));
  REQUIRE(checkType(s, substackT, list<string>{}));
  REQUIRE(checkType(s, anyT, list<string>{}));
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
  REQUIRE_FALSE(checkType(s, numberT, list<string>{}));
  REQUIRE_FALSE(checkType(s, commandT, list<string>{}));
  REQUIRE(checkType(s, substackT, list<string>{}));
  REQUIRE(checkType(s, anyT, list<string>{}));
  delete s;
}

TEST_CASE("check any matches any element", "[language][checkType]") {
  StackElement* number = new NumberElement("22/7");
  StackElement* substack = SubstackElement::parse("<<\"string\", 2, `map>>");
  StackElement* command = new CommandElement("filter");
  TypeElement anyT = TypeElement(StackElement::DataType::Any);
  REQUIRE(checkType(number, anyT, list<string>{}));
  REQUIRE(checkType(substack, anyT, list<string>{}));
  REQUIRE(checkType(command, anyT, list<string>{}));
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
  CommandElement* elm = new CommandElement("map");
  REQUIRE_NOTHROW(checkContext("map", elm, "map-local", list<string>{}));
  CommandElement* other = new CommandElement("foldl");
  REQUIRE_THROWS_AS(checkContext("map", other, "foldl-local", list<string>{}),
                    SyntaxError);
  REQUIRE_THROWS_AS(
      checkContext(GLOBAL_CONTEXT, other, "foldl-local", list<string>{}),
      SyntaxError);
  delete elm;
  delete other;
}

TEST_CASE("check pairs between stack and Prim", "[language][PRIMTIVES]") {
  Stack stk =
      Stack{new TypeElement(StackElement::DataType::Number,
                            new TypeElement(StackElement::DataType::Exact))};
  Prim pf = [](Stack&, map<string, DefinedFunction>&, list<string>&) {
    return;
  };

  PrimitiveFunction* p = new PrimitiveFunction{stk, pf};
  delete p;
}

TEST_CASE("check PRIMITIVES segfault special case", "[language][PRIMTIVES]") {
  Stack stk =
      Stack{new TypeElement(StackElement::DataType::Number,
                            new TypeElement(StackElement::DataType::Exact))};
  Prim pf = [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
    return;
  };

  map<string, PrimitiveFunction>* manual =
      new map<string, PrimitiveFunction>{{"drop*", PrimitiveFunction{stk, pf}}};

  map<string, PrimitiveFunction>* prims = new map<string, PrimitiveFunction>{
      {"drop*",
       PrimitiveFunction{Stack{new TypeElement(
                             StackElement::DataType::Number,
                             new TypeElement(StackElement::DataType::Exact))},
                         [](Stack& s, map<string, DefinedFunction>&,
                            list<string>&) { return; }}},
  };
}