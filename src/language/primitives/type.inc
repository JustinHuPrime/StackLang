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

// Special included file for implementation of type-related function
// primitives

PRIMDEF("type?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(elm->getType() == StackElement::DataType::Type));
})
PRIMDEF("specialized?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(
  elm->getType() == StackElement::DataType::Type &&
  dynamic_cast<TypeElement*>(elm.get())->getSpecialization() !=
   nullptr));
})
PRIMDEF("get-specialization?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Type)});
  TypePtr elm(dynamic_cast<TypeElement*>(s.pop()));
  s.push(elm->getSpecialization()->clone());
})
PRIMDEF("add-specialization?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Type),
            new TypeElement(StackElement::DataType::Type)});
  TypePtr added(dynamic_cast<TypeElement*>(s.pop()));
  TypePtr base(dynamic_cast<TypeElement*>(s.pop()));
  vector<StackElement::DataType> trace;

  const TypeElement* curr = base.get();
  while (curr != nullptr) {
  trace.push_back(curr->getBase());
  curr = curr->getSpecialization();
  }

  if (trace.back() != StackElement::DataType::Substack)
  throw RuntimeError(
  "Cannot have a specialzation except on a Substack.", context);

  TypeElement* result =
  added.release();  // dangerous, but inefficient to use and release
                              // a unique_ptr every time.
  while (!trace.empty()) {
  result = new TypeElement(trace.back(), result);
  trace.pop_back();
  }
  s.push(result);
})
PRIMDEF("base?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Type)});
  TypePtr elm(dynamic_cast<TypeElement*>(s.pop()));
  s.push(new TypeElement(elm->getBase()));
})
PRIMDEF("check-type", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any),
            new TypeElement(StackElement::DataType::Type)});
  TypePtr type(dynamic_cast<TypeElement*>(s.pop()));
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(checkType(elm.get(), *type, context)));
}) 
PRIMDEF("typeof", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new TypeElement(elm->getType()));
})
