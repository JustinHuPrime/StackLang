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

// Special included file for implementation of string-related function
// primitives.

PRIMDEF("string?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(
    new BooleanElement(elm->getType() == StackElement::DataType::String));
})
PRIMDEF("empty-string?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(
    elm->getType() == StackElement::DataType::String &&
      dynamic_cast<StringElement*>(elm.get())->getData() == ""));
})
PRIMDEF("string-length", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String)});
  StringPtr str(dynamic_cast<StringElement*>(s.pop()));
  s.push(new NumberElement(str->getData().length(), 0));
})
PRIMDEF("string-ref", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  StringPtr str(dynamic_cast<StringElement*>(s.pop()));
  long double indexRemainder = num->getData();
  long double index;
  indexRemainder = modf(indexRemainder, &index);
  if (indexRemainder != 0 || index < 0)
    throw RuntimeError("Expected a non-negative integer, but got " +
                      static_cast<string>(*num) + " instead.");
  size_t i = static_cast<size_t>(index);
  if (i >= str->getData().size())
    throw RuntimeError("Index " + static_cast<string>(*num) +
                        " is out of range for the string " +
                        static_cast<string>(*str) + ".");
  s.push(new StringElement(string(1, str->getData()[i])));
})  
PRIMDEF("substring", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::Number),
                      new TypeElement(StackElement::DataType::Number)});
  NumberPtr start(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr end(dynamic_cast<NumberElement*>(s.pop()));
  StringPtr str(dynamic_cast<StringElement*>(s.pop()));
  long double startRemainder = start->getData();
  long double startIndex;
  startRemainder = modf(startRemainder, &startIndex);
  if (startRemainder != 0 || startIndex < 0)
    throw RuntimeError(
        "Expected a non-negative integer for the starting index, but "
        "got " +
        static_cast<string>(*start) + " instead.");
  long double endRemainder = end->getData();
  long double endIndex;
  endRemainder = modf(endRemainder, &endIndex);
  if (endRemainder != 0 || endIndex < 0)
    throw RuntimeError(
        "Expected a non-negative integer for the ending index, but got " +
        static_cast<string>(*start) + " instead.");
  size_t sidx = static_cast<size_t>(startIndex);
  size_t eidx = static_cast<size_t>(endIndex);
  if (sidx >= str->getData().size())
    throw RuntimeError("Starting index " + static_cast<string>(*start) +
                      " is out of range for the string " +
                      static_cast<string>(*str) + ".");
  if (eidx >= str->getData().size())
    throw RuntimeError("Ending index " + static_cast<string>(*end) +
                      " is out of range for the string " +
                      static_cast<string>(*str) + ".");
  if (eidx < sidx)
    throw RuntimeError(
        "Ending index (" + static_cast<string>(*end) +
        ") must not be less than the starting index (" +
        static_cast<string>(*start) + ").");
  s.push(new StringElement(str->getData().substr(sidx, eidx - sidx)));
})  
PRIMDEF("string-append", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr first(dynamic_cast<StringElement*>(s.pop()));
  StringPtr second(dynamic_cast<StringElement*>(s.pop()));
  s.push(new StringElement(second->getData() + first->getData()));
}) 
PRIMDEF("toupper", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String)});
  StringPtr str(dynamic_cast<StringElement*>(s.pop()));
  string rawStr = str->getData();
  for (char& c : rawStr) c = toupper(c);
  s.push(new StringElement(rawStr));
})  
PRIMDEF("tolower", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String)});
  StringPtr str(dynamic_cast<StringElement*>(s.pop()));
  string rawStr = str->getData();
  for (char& c : rawStr) c = tolower(c);
  s.push(new StringElement(rawStr));
})  
PRIMDEF("join", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Substack),
                      new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr splice(dynamic_cast<StringElement*>(s.pop()));
  SubstackPtr sub(dynamic_cast<SubstackElement*>(s.pop()));
  string acc;
  for (const StackElement* elm : sub->getData()) {
    const StringElement* str = dynamic_cast<const StringElement*>(elm);
    if (acc != "") acc += splice->getData();
    acc += str->getData();
  }
  s.push(new StringElement(acc));
}) 
PRIMDEF("split", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr splitter(dynamic_cast<StringElement*>(s.pop()));
  StringPtr str(dynamic_cast<StringElement*>(s.pop()));
  Stack sta;
  const string& delim = splitter->getData();
  const string& raw = str->getData();
  if (delim == "") {  // empty delimiter special case - or else infinite
                      // loop of blanks.
    for (const char& c : raw) {
      sta.push(new StringElement(string(1, c)));
    }
  } else {
    size_t current = raw.find(delim);
    size_t previous = 0;
    while (current != string::npos) {
      sta.push(
          new StringElement(raw.substr(previous, current - previous)));
      previous = current + delim.size();
      current = raw.find(delim, previous);
    }
    sta.push(new StringElement(raw.substr(previous, current - previous)));
  }
  sta.reverse();
  s.push(new SubstackElement(sta));
}) 
PRIMDEF("replace", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr from(dynamic_cast<StringElement*>(s.pop()));
  StringPtr to(dynamic_cast<StringElement*>(s.pop()));
  StringPtr target(dynamic_cast<StringElement*>(s.pop()));
  string str = target->getData();
  size_t foundLocation = str.find(from->getData());
  if (foundLocation == string::npos) {
    s.push(target.release());
  } else {
    str.replace(foundLocation, from->getData().size(), to->getData());
    s.push(new StringElement(str));
  }
}) 
PRIMDEF("build-string", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::Number)});
  NumberPtr reps(dynamic_cast<NumberElement*>(s.pop()));
  StringPtr str(dynamic_cast<StringElement*>(s.pop()));
  long double countRemainder = reps->getData();
  long double count;
  countRemainder = modf(countRemainder, &count);
  if (countRemainder != 0 || count < 0)
    throw RuntimeError(
        "Expected a non-negative integer for the number of repetitions, "
        "but got " +
            static_cast<string>(*reps) + " instead.");
  string acc;
  const string& toAdd = str->getData();
  while (count-- > 0) acc += toAdd;
  s.push(new StringElement(acc));
}) 
PRIMDEF("string-equal?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr a(dynamic_cast<StringElement*>(s.pop()));
  StringPtr b(dynamic_cast<StringElement*>(s.pop()));
  s.push(new BooleanElement(a->getData() == b->getData()));
}) 
PRIMDEF("string-alphabetic?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr a(dynamic_cast<StringElement*>(s.pop()));
  StringPtr b(dynamic_cast<StringElement*>(s.pop()));
  s.push(new BooleanElement(a->getData().compare(b->getData()) > 0));
})  
PRIMDEF("string-reverse-alphabetic?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr a(dynamic_cast<StringElement*>(s.pop()));
  StringPtr b(dynamic_cast<StringElement*>(s.pop()));
  s.push(new BooleanElement(a->getData().compare(b->getData()) < 0));
})  
PRIMDEF("string-contains?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr inner(dynamic_cast<StringElement*>(s.pop()));
  StringPtr outer(dynamic_cast<StringElement*>(s.pop()));
  s.push(new BooleanElement(outer->getData().find(inner->getData()) !=
    string::npos));
})  
PRIMDEF("string-prefix?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr prefix(dynamic_cast<StringElement*>(s.pop()));
  StringPtr outer(dynamic_cast<StringElement*>(s.pop()));
  s.push(new BooleanElement(
      starts_with(outer->getData(), prefix->getData())));
})
PRIMDEF("string-suffix?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String),
                      new TypeElement(StackElement::DataType::String)});
  StringPtr suffix(dynamic_cast<StringElement*>(s.pop()));
  StringPtr outer(dynamic_cast<StringElement*>(s.pop()));
  s.push(
    new BooleanElement(ends_with(outer->getData(), suffix->getData())));
})
