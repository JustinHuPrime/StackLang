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

// Special included file for implementation of substack-related function
// primitives

PRIMDEF("substack?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(
      new BooleanElement(elm->getType() == StackElement::DataType::Substack));
})
PRIMDEF("empty?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(
      elm->getType() == StackElement::DataType::Substack &&
      dynamic_cast<SubstackElement*>(elm.get())->getData().isEmpty()));
})
PRIMDEF("contains-type?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack),
                      new TypeElement(StackElement::DataType::Type)});
  TypePtr internal(dynamic_cast<TypeElement*>(s.pop()));
  SubstackPtr sub(dynamic_cast<SubstackElement*>(s.pop()));
  s.push(new BooleanElement(checkType(
      sub.get(),
      TypeElement(StackElement::DataType::Substack, internal.release()))));
})
PRIMDEF("push", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack),
                      new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  SubstackPtr sub(dynamic_cast<SubstackElement*>(s.pop()));
  Stack sta = sub->getData();
  try {
    sta.push(elm.release());
  } catch (const StackOverflowError&) {
    throw StackOverflowError(sta.getLimit());
  }
  s.push(new SubstackElement(sta));
})
PRIMDEF("top", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack)});
  SubstackPtr sub(dynamic_cast<SubstackElement*>(s.pop()));
  Stack sta = sub->getData();
  try {
    s.push(sta.pop());
  } catch (const StackUnderflowError&) {
    throw StackUnderflowError();
  }
})
PRIMDEF("pop", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack)});
  SubstackPtr sub(dynamic_cast<SubstackElement*>(s.pop()));
  Stack sta = sub->getData();
  try {
    delete sta.pop();
  } catch (const StackUnderflowError&) {
    throw StackUnderflowError();
  }
  s.push(new SubstackElement(sta));
})
PRIMDEF("pop*", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack)});
  SubstackPtr sub(dynamic_cast<SubstackElement*>(s.pop()));
  Stack sta = sub->getData();
  StackElement* popped;
  try {
    popped = sta.pop();
  } catch (const StackUnderflowError&) {
    throw StackUnderflowError();
  }
  s.push(new SubstackElement(sta));
  s.push(popped);
})
PRIMDEF("make-substack", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any),
                      new TypeElement(StackElement::DataType::Any)});
  ElementPtr a(s.pop());
  ElementPtr b(s.pop());
  s.push(new SubstackElement(Stack{b.release(), a.release()}));
})
PRIMDEF("length", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack)});
  SubstackPtr sub(dynamic_cast<SubstackElement*>(s.pop()));
  s.push(new NumberElement(sub->getData().size(), 0));
})
PRIMDEF("substack-ref", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack),
                      new TypeElement(StackElement::DataType::Number)});
  NumberPtr index(dynamic_cast<NumberElement*>(s.pop()));
  SubstackPtr sta(dynamic_cast<SubstackElement*>(s.pop()));
  long double remainder = index->getData();
  long double whole;
  remainder = modf(remainder, &whole);
  if (remainder != 0 || whole < 0)
    throw RuntimeError(
        "Expected a non-negative integer for the index, but got " +
        static_cast<string>(*index) + " instead.");
  if (whole >= sta->getData().size())
    throw RuntimeError("Index " + static_cast<string>(*index) +
                       " is out of range for the substack ( " +
                       to_string(sta->getData().size()) + " long).");
  for (const StackElement* elm : sta->getData()) {
    if (whole-- == 0) {
      s.push(elm->clone());
    }
  }
})
PRIMDEF("sub-substack", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack),
                      new TypeElement(StackElement::DataType::Number),
                      new TypeElement(StackElement::DataType::Number)});
  NumberPtr start(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr end(dynamic_cast<NumberElement*>(s.pop()));
  SubstackPtr sta(dynamic_cast<SubstackElement*>(s.pop()));
  long double startRemainder = start->getData();
  long double startIndex;
  startRemainder = modf(startRemainder, &startIndex);
  if (startRemainder != 0 || startIndex < 0)
    throw RuntimeError(
        "Expected a non-negative integer for the starting index, but got " +
        static_cast<string>(*start) + " instead.");
  long double endRemainder = end->getData();
  long double endIndex;
  endRemainder = modf(endRemainder, &endIndex);
  if (endRemainder != 0 || endIndex < 0)
    throw RuntimeError(
        "Expected a non-negative integer for the ending index, but got " +
        static_cast<string>(*start) + " instead.");
  if (startIndex >= sta->getData().size())
    throw RuntimeError("Starting index " + static_cast<string>(*start) +
                       " is out of range for the substack ( " +
                       to_string(sta->getData().size()) + " long).");
  if (endIndex >= sta->getData().size())
    throw RuntimeError("Ending index " + static_cast<string>(*end) +
                       " is out of range for the substack ( " +
                       to_string(sta->getData().size()) + " long).");
  if (endIndex < startIndex)
    throw RuntimeError("Ending index (" + static_cast<string>(*end) +
                       ") must not be less than the starting index (" +
                       static_cast<string>(*start) + ").");
  Stack result;
  auto iter = sta->getData().begin();
  size_t index = 0;
  while (index++ != startIndex) iter++;
  while (index++ != endIndex + 1) {
    result.push((*iter++)->clone());
  }
  result.reverse();
  s.push(new SubstackElement(result));
})
PRIMDEF("append", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack),
                      new TypeElement(StackElement::DataType::Substack)});
  SubstackPtr later(dynamic_cast<SubstackElement*>(s.pop()));
  SubstackPtr base(dynamic_cast<SubstackElement*>(s.pop()));
  Stack first = base->getData();
  first.reverse();
  Stack second = later->getData();
  while (!second.isEmpty()) first.push(second.pop());
  first.reverse();
  s.push(new SubstackElement(first));
})
PRIMDEF("reverse", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack)});
  SubstackPtr sta(dynamic_cast<SubstackElement*>(s.pop()));
  Stack result = sta->getData();
  result.reverse();
  s.push(new SubstackElement(result));
})
PRIMDEF("insert", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack),
                      new TypeElement(StackElement::DataType::Number),
                      new TypeElement(StackElement::DataType::Substack)});
  SubstackPtr inserted(dynamic_cast<SubstackElement*>(s.pop()));
  NumberPtr index(dynamic_cast<NumberElement*>(s.pop()));
  SubstackPtr base(dynamic_cast<SubstackElement*>(s.pop()));
  long double remainder = index->getData();
  long double whole;
  remainder = modf(remainder, &whole);
  if (remainder != 0 || whole < 0)
    throw RuntimeError(
        "Expected a non-negative integer for the index, but got " +
        static_cast<string>(*index) + " instead.");
  Stack baseStack = base->getData();
  Stack insertStack = inserted->getData();
  Stack result;
  while (whole-- > 0) result.push(baseStack.pop());
  while (!insertStack.isEmpty()) result.push(insertStack.pop());
  while (!baseStack.isEmpty()) result.push(baseStack.pop());
  result.reverse();
  s.push(new SubstackElement(result));
})
