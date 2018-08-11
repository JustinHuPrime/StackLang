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

// Special included file for implementation of number-related function
// primitives

{"euler",
 {Stack{}, [](Stack& s, map<string, DefinedFunction>&,
              list<string>&) { s.push(new NumberElement(M_E, false)); }}},
    {"pi",
     {Stack{}, [](Stack& s, map<string, DefinedFunction>&,
                  list<string>&) { s.push(new NumberElement(M_PI, false)); }}},
    {"number?",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        ElementPtr elm(s.pop());
        s.push(new BooleanElement(elm->getType() ==
                                  StackElement::DataType::Number));
      }}},
    {"exact?",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        ElementPtr elm(s.pop());
        s.push(new BooleanElement(
            elm->getType() == StackElement::DataType::Number &&
            dynamic_cast<NumberElement*>(elm.get())->isExact()));
      }}},
    {"exact-to-inexact",
     {Stack{new TypeElement(StackElement::DataType::Number,
                            new TypeElement(StackElement::DataType::Exact))},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(elm->getData(), false));
      }}},
    {"inexact-to-exact",
     {Stack{new TypeElement(StackElement::DataType::Number,
                            new TypeElement(StackElement::DataType::Exact))},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(elm->getData(), true));
      }}},
    {"number-to-string",
     {Stack{new TypeElement(StackElement::DataType::Boolean)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new StringElement(static_cast<string>(*elm)));
      }}},
    {"string-to-number",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        StringPtr elm(dynamic_cast<StringElement*>(s.pop()));
        ElementPtr result;
        try {
          result = ElementPtr(StackElement::parse(elm->getData()));
        } catch (...) {
          throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
                                 " produced an error.",
                             context);
        }
        if (result->getType() != StackElement::DataType::Number) {
          throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
                                 " produced a non-number result.",
                             context);
        }
        s.push(result.release());
      }}},
    {"add",
     {Stack{},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(second->getData() + first->getData(),
                                 second->isExact() && first->isExact()));
      }}},
    {"subtract",
     {Stack{},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(second->getData() - first->getData(),
                                 second->isExact() && first->isExact()));
      }}},
    {"multiply",
     {Stack{},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(second->getData() * first->getData(),
                                 second->isExact() && first->isExact()));
      }}},
    {"divide",
     {Stack{},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(second->getData() / first->getData(),
                                 second->isExact() && first->isExact()));
      }}},
    {"modulo",
     {Stack{},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(second->getData() / first->getData(),
                                 second->isExact() && first->isExact()));
      }}},