// Copyright 2018 Justin Hu
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The StackLang interpreter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the StackLang interpreter.  If not, see <https://www.gnu.org/licenses/>.

// Special included file for implementation of special primitives (define,
// include, etc.)

{"define",
 {Stack{new TypeElement(StackElement::DataType::Substack,
                        new TypeElement(StackElement::DataType::Type)),
        new TypeElement(StackElement::DataType::Substack),
        new TypeElement(StackElement::DataType::Command,
                        new TypeElement(StackElement::DataType::Quoted))},
  [](Stack& s, map<string, DefinedFunction>& defs, list<string>& context) {
    CommandPtr name(dynamic_cast<CommandElement*>(s.pop()));
    SubstackPtr body(dynamic_cast<SubstackElement*>(s.pop()));
    SubstackPtr sig(dynamic_cast<SubstackElement*>(s.pop()));
    auto iter = defs.find(name->getName());
    if (iter != defs.end()) {
      throw RuntimeError("Cannot redefine " + name->getName() + ".", context);
    }
    DefinedFunction def = DefinedFunction(sig->getData(), body->getData());
    defs[name->getName()] = def;
  }}},
    {"define*",
     {Stack{new TypeElement(StackElement::DataType::Substack,
                            new TypeElement(StackElement::DataType::Type)),
            new TypeElement(StackElement::DataType::Substack),
            new TypeElement(StackElement::DataType::Command,
                            new TypeElement(StackElement::DataType::Quoted)),
            new TypeElement(StackElement::DataType::Command,
                            new TypeElement(StackElement::DataType::Quoted))},
      [](Stack& s, map<string, DefinedFunction>& defs, list<string>& context) {
        CommandPtr name(dynamic_cast<CommandElement*>(s.pop()));
        CommandPtr contextReq(dynamic_cast<CommandElement*>(s.pop()));
        SubstackPtr body(dynamic_cast<SubstackElement*>(s.pop()));
        SubstackPtr sig(dynamic_cast<SubstackElement*>(s.pop()));
        auto iter = defs.find(name->getName());
        if (iter != defs.end()) {
          throw RuntimeError("Cannot redefine " + name->getName() + ".",
                             context);
        }
        DefinedFunction def = DefinedFunction(sig->getData(), body->getData(),
                                              contextReq.release());
        defs[name->getName()] = def;
      }}},
    {"undefine",
     {Stack{new TypeElement(StackElement::DataType::Command,
                            new TypeElement(StackElement::DataType::Quoted))},
      [](Stack& s, map<string, DefinedFunction>& defs, list<string>& context) {
        CommandPtr elm(dynamic_cast<CommandElement*>(s.pop()));
        auto iter = defs.find(elm->getName());
        if (iter == defs.end()) {
          throw RuntimeError(
              "Command " + elm->getName() + " was not previously defined.",
              context);
        } else {
          defs.erase(iter);
        }
      }}},
    {"include",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, map<string, DefinedFunction>& defs, list<string>& context) {
        return;
      }}},
    {"drop",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        delete s.pop();
      }}},
    {"drop*",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double remainder = num->getData();
        long double whole;
        remainder = modfl(remainder, &whole);
        if (remainder != 1 || whole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*num) + " instead.",
                             context);
        }
        while (whole-- > 0) delete s.pop();
      }}},
    {"clear",
     {Stack{}, [](Stack& s, map<string, DefinedFunction>&,
                  list<string>&) { s.clear(); }}},
    {"rotate",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double remainder = num->getData();
        long double whole;
        remainder = modfl(remainder, &whole);
        if (remainder != 0 || whole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*num) + " instead.",
                             context);
        }
        StackElement* grabbed;
        stack<StackElement*> rest;
        while (whole-- > 1) rest.push(s.pop());
        grabbed = s.pop();
        while (!rest.empty()) {
          s.push(rest.top());
          rest.pop();
        }
        s.push(grabbed);
      }}},
    {"rotate*",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        long double reachRemainder = first->getData();
        long double reachWhole;
        reachRemainder = modfl(reachRemainder, &reachWhole);
        if (reachRemainder != 0 || reachWhole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*second) + " instead.",
                             context);
        }
        long double countRemainder = second->getData();
        long double countWhole;
        countRemainder = modfl(countRemainder, &countWhole);
        if (countRemainder != 0 || countWhole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*first) + " instead.",
                             context);
        }
        while (countWhole-- > 0) {
          long double reach = reachWhole;
          StackElement* grabbed;
          stack<StackElement*> rest;
          while (reach-- > 1) rest.push(s.pop());
          grabbed = s.pop();
          while (!rest.empty()) {
            s.push(rest.top());
            rest.pop();
          }
          s.push(grabbed);
        }
      }}},
    {"duplicate",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        s.push(s.top()->clone());
      }}},
    {"error",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        StringPtr str(dynamic_cast<StringElement*>(s.pop()));
        throw new RuntimeError(str->getData(), context);
      }}},
    {"error*",
     {Stack{new TypeElement(StackElement::DataType::String),
            new TypeElement(StackElement::DataType::String),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr ctxIndex(dynamic_cast<NumberElement*>(s.pop()));
        StringPtr ctx(dynamic_cast<StringElement*>(s.pop()));
        StringPtr msg(dynamic_cast<StringElement*>(s.pop()));
        long double remainder = ctxIndex->getData();
        long double whole;
        remainder = modfl(remainder, &whole);
        if (remainder != 0 || whole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*ctxIndex) + " instead.",
                             context);
        }
        throw new RuntimeError(msg->getData(), ctx->getData(), whole, context);
      }}},
    {"null",
     {Stack{},
      [](Stack&, map<string, DefinedFunction>&, list<string>&) { return; }}},
    {"identity",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack&, map<string, DefinedFunction>&, list<string>&) {
        return;  // equivalent to popping then pushing.
      }}},