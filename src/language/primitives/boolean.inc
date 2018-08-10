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

// Special included file for implementation of boolean-related function
// primitives

//  {"NAME",
//   {Stack{INPUT TYPES...}, // vertically sorted, like the stack.
//    [](Stack& s, map<string, DefinedFunction>&) {
//      PRIMITIVE BODY...
//    }}},

{"boolean?",
 {Stack{new TypeElement(StackElement::DataType::Any)},
  [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
    ElementPtr elm(s.pop());
    s.push(
        new BooleanElement(elm->getType() == StackElement::DataType::Boolean));
  }}},
    {"false?",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        ElementPtr elm(s.pop());
        s.push(new BooleanElement(
            elm->getType() == StackElement::DataType::Boolean &&
            !dynamic_cast<const BooleanElement*>(elm.get())->getData()));
      }}},
    {"true?",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        StackElement* elm = s.pop();
        BooleanElement* result = new BooleanElement(
            elm->getType() == StackElement::DataType::Boolean &&
            dynamic_cast<BooleanElement*>(elm)->getData());
        delete elm;
        s.push(result);
      }}},
    {"boolean-to-string",
     {Stack{new TypeElement(StackElement::DataType::Boolean)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        BooleanElement* elm = dynamic_cast<BooleanElement*>(s.pop());
        StringElement* result = new StringElement(static_cast<string>(*elm));
        delete elm;
        s.push(result);
      }}},
    {"string-to-boolean",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
        StringElement* elm = dynamic_cast<StringElement*>(s.pop());
        StackElement* result;
        try {
          result = StackElement::parse(elm->getData());
        } catch (...) {
          string elmString = static_cast<string>(*elm);
          delete elm;
          throw RuntimeError(
              "Parsing of element " + elmString + " produced an error.",
              context);
        }
        if (result->getType() != StackElement::DataType::Boolean) {
          string elmString = static_cast<string>(*elm);
          delete elm;
          delete result;
          throw RuntimeError("Parsing of element " + elmString +
                                 " produced a non-boolean result.",
                             context);
        }
        delete elm;
        s.push(result);
      }}},
    {"if",
     {Stack{new TypeElement(StackElement::DataType::Any),
            new TypeElement(StackElement::DataType::Any),
            new TypeElement(StackElement::DataType::Boolean)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        BooleanElement* boolean = dynamic_cast<BooleanElement*>(s.pop());
        StackElement* falseCase = s.pop();
        StackElement* trueCase = s.pop();
        if (boolean->getData()) {
          delete falseCase;
          s.push(trueCase);
        } else {
          delete trueCase;
          s.push(falseCase);
        }
        delete boolean;
      }}},
    {"not",
     {Stack{new TypeElement(StackElement::DataType::Boolean)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        BooleanElement* a = dynamic_cast<BooleanElement*>(s.pop());
        BooleanElement* result = new BooleanElement(!a->getData());
        delete a;
        s.push(result);
      }}},
    {"or",
     {Stack{new TypeElement(StackElement::DataType::Boolean),
            new TypeElement(StackElement::DataType::Boolean)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        BooleanElement* a = dynamic_cast<BooleanElement*>(s.pop());
        BooleanElement* b = dynamic_cast<BooleanElement*>(s.pop());
        if (a->getData()) {  // a is true
          s.push(a);
          delete b;
        } else {  // a is false, result is b
          s.push(b);
          delete a;
        }
      }}},
    {"and",
     {Stack{new TypeElement(StackElement::DataType::Boolean),
            new TypeElement(StackElement::DataType::Boolean)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        BooleanElement* a = dynamic_cast<BooleanElement*>(s.pop());
        BooleanElement* b = dynamic_cast<BooleanElement*>(s.pop());
        if (!a->getData()) {  // a is false
          s.push(a);
          delete b;
        } else {  // a is true, result is b
          s.push(b);
          delete a;
        }
      }}},
    {"xor",
     {Stack{new TypeElement(StackElement::DataType::Boolean),
            new TypeElement(StackElement::DataType::Boolean)},
      [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
        BooleanElement* a = dynamic_cast<BooleanElement*>(s.pop());
        BooleanElement* b = dynamic_cast<BooleanElement*>(s.pop());
        BooleanElement* result =
            new BooleanElement(a->getData() != b->getData());
        delete a;
        delete b;
        s.push(result);
      }}},