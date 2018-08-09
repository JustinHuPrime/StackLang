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

//  {"NAME",
//   {Stack{INPUT TYPES...}, // vertically sorted, like the stack.
//    [](Stack& s, map<string, DefinedFunction>&) {
//      PRIMITIVE BODY...
//    }}},

{"add",
 {Stack{new TypeElement(StackElement::DataType::Number),
        new TypeElement(StackElement::DataType::Number)},
  [](Stack& s, map<string, DefinedFunction>&) {
    NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
    NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
    NumberElement* result =
        new NumberElement(lower->getData() + higher->getData());
    delete lower;
    delete higher;
    s.push(result);
  }}},
    {"subtract",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&) {
        NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result =
            new NumberElement(higher->getData() - lower->getData());
        delete lower;
        delete higher;
        s.push(result);
      }}},
    {"multiply",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&) {
        NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result =
            new NumberElement(higher->getData() * lower->getData());
        delete lower;
        delete higher;
        s.push(result);
      }}},
    {"divide",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&) {
        if (dynamic_cast<NumberElement*>(s.top())->getData() == 0) {
          throw SyntaxError("Attempted division by zero.");
        }
        NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result =
            new NumberElement(higher->getData() / lower->getData());
        delete lower;
        delete higher;
        s.push(result);
      }}},
    {"negate",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, map<string, DefinedFunction>&) {
        NumberElement* input = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result = new NumberElement(-input->getData());
        delete input;
        s.push(result);
      }}},