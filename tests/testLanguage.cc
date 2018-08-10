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
  REQUIRE(checkType(elm, t, list<CommandElement*>{}));
  delete elm;
}

TEST_CASE("check single type simple fail", "[language][checkType]") {
  CommandElement* elm = new CommandElement("foldl");
  TypeElement t = TypeElement(StackElement::DataType::Boolean);
  REQUIRE_FALSE(checkType(elm, t, list<CommandElement*>{}));
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
  REQUIRE(checkType(exact, exactT, list<CommandElement*>{}));
  REQUIRE(checkType(inexact, inexactT, list<CommandElement*>{}));
  REQUIRE_FALSE(checkType(exact, inexactT, list<CommandElement*>{}));
  REQUIRE_FALSE(checkType(inexact, exactT, list<CommandElement*>{}));
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
  REQUIRE(checkType(raw, anyT, list<CommandElement*>{}));
  REQUIRE(checkType(quoted, anyT, list<CommandElement*>{}));
  REQUIRE(checkType(raw, rawT, list<CommandElement*>{}));
  REQUIRE(checkType(quoted, quotedT, list<CommandElement*>{}));
  REQUIRE_FALSE(checkType(raw, quotedT, list<CommandElement*>{}));
  REQUIRE_FALSE(checkType(quoted, rawT, list<CommandElement*>{}));
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
  REQUIRE(checkType(numberStack, anyT, list<CommandElement*>{}));
  REQUIRE(checkType(commandStack, anyT, list<CommandElement*>{}));
  REQUIRE(checkType(numberStack, substackT, list<CommandElement*>{}));
  REQUIRE(checkType(commandStack, substackT, list<CommandElement*>{}));

  REQUIRE(checkType(numberStack, numberT, list<CommandElement*>{}));
  REQUIRE(checkType(commandStack, commandT, list<CommandElement*>{}));
  REQUIRE_FALSE(checkType(numberStack, commandT, list<CommandElement*>{}));
  REQUIRE_FALSE(checkType(commandStack, numberT, list<CommandElement*>{}));
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
  REQUIRE(checkType(s, numberT, list<CommandElement*>{}));
  REQUIRE(checkType(s, commandT, list<CommandElement*>{}));
  REQUIRE(checkType(s, substackT, list<CommandElement*>{}));
  REQUIRE(checkType(s, anyT, list<CommandElement*>{}));
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
  REQUIRE_FALSE(checkType(s, numberT, list<CommandElement*>{}));
  REQUIRE_FALSE(checkType(s, commandT, list<CommandElement*>{}));
  REQUIRE(checkType(s, substackT, list<CommandElement*>{}));
  REQUIRE(checkType(s, anyT, list<CommandElement*>{}));
  delete s;
}

TEST_CASE("check any matches any element", "[language][checkType]") {
  StackElement* number = new NumberElement("22/7");
  StackElement* substack = SubstackElement::parse("<<\"string\", 2, `map>>");
  StackElement* command = new CommandElement("filter");
  TypeElement anyT = TypeElement(StackElement::DataType::Any);
  REQUIRE(checkType(number, anyT, list<CommandElement*>{}));
  REQUIRE(checkType(substack, anyT, list<CommandElement*>{}));
  REQUIRE(checkType(command, anyT, list<CommandElement*>{}));
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

  REQUIRE_NOTHROW(checkTypes(things, types, list<CommandElement*>{}));
  types.reverse();
  types.push(TypeElement::parse("Number"));
  types.reverse();
  REQUIRE_NOTHROW(checkTypes(things, types, list<CommandElement*>{}));
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
  REQUIRE_THROWS_AS(checkTypes(things, types, list<CommandElement*>{}),
                    TypeError);
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
  REQUIRE_THROWS_AS(checkTypes(things, types, list<CommandElement*>{}),
                    TypeError);
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
  REQUIRE_THROWS_AS(checkTypes(things, types, list<CommandElement*>{}),
                    TypeError);
}

TEST_CASE("check context all allowed", "[language][checkContext]") {
  REQUIRE_NOTHROW(
      checkContext(nullptr, nullptr, "some-name", list<CommandElement*>{}));
  CommandElement* elm = new CommandElement("map");
  REQUIRE_NOTHROW(
      checkContext(elm, nullptr, "some-name", list<CommandElement*>{}));
  delete elm;
}

TEST_CASE("check limiting context respected", "[language][checkContext]") {
  CommandElement* elm = new CommandElement("map");
  REQUIRE_NOTHROW(checkContext(elm, elm, "map-local", list<CommandElement*>{}));
  CommandElement* other = new CommandElement("foldl");
  REQUIRE_THROWS_AS(
      checkContext(elm, other, "foldl-local", list<CommandElement*>{}),
      SyntaxError);
  REQUIRE_THROWS_AS(
      checkContext(nullptr, other, "foldl-local", list<CommandElement*>{}),
      SyntaxError);
  delete elm;
  delete other;
}

TEST_CASE("check pairs between stack and Prim", "[language][PRIMTIVES]") {
  Stack stk =
      Stack{new TypeElement(StackElement::DataType::Number,
                            new TypeElement(StackElement::DataType::Exact))};
  Prim pf = [](Stack&, map<string, DefinedFunction>&, list<CommandElement*>&) {
    return;
  };

  PrimitiveFunction* p = new PrimitiveFunction{stk, pf};
  delete p;
}

TEST_CASE("check PRIMITIVES segfault special case", "[language][PRIMTIVES]") {
  Stack stk =
      Stack{new TypeElement(StackElement::DataType::Number,
                            new TypeElement(StackElement::DataType::Exact))};
  Prim pf = [](Stack& s, map<string, DefinedFunction>&,
               list<CommandElement*>&) { return; };

  map<string, PrimitiveFunction>* manual =
      new map<string, PrimitiveFunction>{{"drop*", PrimitiveFunction{stk, pf}}};

  map<string, PrimitiveFunction>* prims = new map<string, PrimitiveFunction>{
      {"drop*",
       PrimitiveFunction{Stack{new TypeElement(
                             StackElement::DataType::Number,
                             new TypeElement(StackElement::DataType::Exact))},
                         [](Stack& s, map<string, DefinedFunction>&,
                            list<CommandElement*>&) { return; }}},
  };
}