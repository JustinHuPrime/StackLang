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

// Special included file for implementation of identifier- and command-related
// function primitives

PRIMDEF("identifier?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(
      new BooleanElement(elm->getType() == StackElement::DataType::Identifier));
})
PRIMDEF("quoted?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(
      elm->getType() == StackElement::DataType::Identifier &&
      dynamic_cast<IdentifierElement*>(elm.get())->isQuoted()));
})
PRIMDEF("bound?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr elm(dynamic_cast<IdentifierElement*>(s.pop()));
  try {
    e->lookup(elm->getName());
    s.push(new BooleanElement(1));
  } catch (...) {
    s.push(new BooleanElement(0));
  }
})
PRIMDEF("unquote", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr elm(dynamic_cast<IdentifierElement*>(s.pop()));
  s.push(new IdentifierElement(elm->getName()));
})
PRIMDEF("identifier-to-string", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr elm(dynamic_cast<IdentifierElement*>(s.pop()));
  s.push(new StringElement((elm->isQuoted() ? "`" : "") + elm->getName()));
})
PRIMDEF("string-to-identifier", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String)});
  StringPtr elm(dynamic_cast<StringElement*>(s.pop()));
  ElementPtr result;
  try {
    result = ElementPtr(StackElement::parse(elm->getData()));
  } catch (...) {
    throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
                       " produced an error.");
  }
  if (result->getType() != StackElement::DataType::Identifier) {
    string elmString = static_cast<string>(*elm);
    throw RuntimeError("Parsing of element " + elmString +
                       " produced a non-identifier result.");
  }
  s.push(result.release());
})
PRIMDEF("string-to-identifier*", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String)});
  StringPtr elm(dynamic_cast<StringElement*>(s.pop()));
  ElementPtr result;
  try {
    result = ElementPtr(StackElement::parse(elm->getData()));
  } catch (...) {
    throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
                       " produced an error.");
  }
  if (result->getType() != StackElement::DataType::Identifier) {
    string elmString = static_cast<string>(*elm);
    throw RuntimeError("Parsing of element " + elmString +
                       " produced a non-identifier result.");
  }
  IdentifierPtr id(dynamic_cast<IdentifierElement*>(result.release()));
  s.push(new IdentifierElement(id->getName(), true));
})
PRIMDEF("arity", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr id(dynamic_cast<IdentifierElement*>(s.pop()));
  StackElement* elem = e->lookup(id->getName());
  Stack elemStack;
  elemStack.push(elem);
  try {
    checkTypes(elemStack,
               Stack{new TypeElement(StackElement::DataType::Defined)});
  } catch (...) {
    delete elem;
    throw RuntimeError("Identifier " + id->getName() +
                       " is bound to a primitive command.");
  }
  DefinedCommandPtr defcmd(dynamic_cast<DefinedCommandElement*>(elem));
  s.push(new NumberElement(defcmd->getSig().size(), 0));
  delete elem;
  return;
})
PRIMDEF("body", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr id(dynamic_cast<IdentifierElement*>(s.pop()));
  StackElement* elem = e->lookup(id->getName());
  Stack elemStack;
  elemStack.push(elem);
  try {
    checkTypes(elemStack,
               Stack{new TypeElement(StackElement::DataType::Defined)});
  } catch (...) {
    delete elem;
    throw RuntimeError("Identifier " + id->getName() +
                       " is bound to a primitive command.");
  }
  DefinedCommandPtr defcmd(dynamic_cast<DefinedCommandElement*>(elem));
  s.push(new SubstackElement(defcmd->getBody()));
  delete elem;
  return;
})
PRIMDEF("signature", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr id(dynamic_cast<IdentifierElement*>(s.pop()));
  StackElement* elem = e->lookup(id->getName());
  Stack elemStack;
  elemStack.push(elem);
  try {
    checkTypes(elemStack,
               Stack{new TypeElement(StackElement::DataType::Defined)});
  } catch (...) {
    delete elem;
    throw RuntimeError("Identifier " + id->getName() +
                       " is bound to a primitive command.");
  }
  DefinedCommandPtr defcmd(dynamic_cast<DefinedCommandElement*>(elem));
  s.push(new SubstackElement(defcmd->getSig()));
  delete elem;
  return;
})
