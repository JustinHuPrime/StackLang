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

PRIMDEF("boolean?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(
  new BooleanElement(elm->getType() == StackElement::DataType::Boolean));
  }})
PRIMDEF("false?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(
    elm->getType() == StackElement::DataType::Boolean &&
    !dynamic_cast<BooleanElement*>(elm.get())->getData()));
})
PRIMDEF("true?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(
    elm->getType() == StackElement::DataType::Boolean &&
    dynamic_cast<BooleanElement*>(elm.get())->getData()));
})
PRIMDEF("boolean-to-string", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Boolean)});
  BooleanPtr elm(dynamic_cast<BooleanElement*>(s.pop()));
  s.push(new StringElement(static_cast<string>(*elm)));
})
PRIMDEF("string-to-boolean", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String)});
  StringPtr elm(dynamic_cast<StringElement*>(s.pop()));
  ElementPtr result;
  try {
    result = ElementPtr(StackElement::parse(elm->getData()));
    } catch (...) {
      throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
      " produced an error.",
      context);
      }
      if (result->getType() != StackElement::DataType::Boolean) {
        throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
        " produced a non-boolean result.",
        context);
        }
        s.push(result.release());
})
PRIMDEF("if", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any),
            new TypeElement(StackElement::DataType::Any),
            new TypeElement(StackElement::DataType::Boolean)});
  BooleanPtr boolean(dynamic_cast<BooleanElement*>(s.pop()));
  ElementPtr falseCase(s.pop());
  ElementPtr trueCase(s.pop());
  s.push(boolean->getData() ? trueCase.release() : falseCase.release());
  }});
  BooleanPtr elm(dynamic_cast<BooleanElement*>(s.pop()));
  s.push(new StringElement(static_cast<string>(*elm)));
})
PRIMDEF("not", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Boolean)});
  BooleanPtr a(dynamic_cast<BooleanElement*>(s.pop()));
  s.push(new BooleanElement(!a->getData()));
})
PRIMDEF("or", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Boolean),
            new TypeElement(StackElement::DataType::Boolean)});
  BooleanPtr a(dynamic_cast<BooleanElement*>(s.pop()));
  BooleanPtr b(dynamic_cast<BooleanElement*>(s.pop()));
  s.push(a->getData() ? a.release() : b.release());
})
PRIMDEF("and", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Boolean),
            new TypeElement(StackElement::DataType::Boolean)});
  BooleanPtr a(dynamic_cast<BooleanElement*>(s.pop()));
  BooleanPtr b(dynamic_cast<BooleanElement*>(s.pop()));
  s.push(a->getData() ? b.release() : a.release());
})
PRIMDEF("xor", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Boolean),
            new TypeElement(StackElement::DataType::Boolean)});
  BooleanPtr a(dynamic_cast<BooleanElement*>(s.pop()));
  BooleanPtr b(dynamic_cast<BooleanElement*>(s.pop()));
  s.push(new BooleanElement(a->getData() != b->getData()));
})
PRIMDEF("eqv?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any),
            new TypeElement(StackElement::DataType::Any)});
  ElementPtr a(s.pop());
  ElementPtr b(s.pop());
  s.push(new BooleanElement(*a == *b));
})
