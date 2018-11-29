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

// Special included file for implementation of identifier- and command-related function
// primitives

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
  s.push(new BooleanElement(e.find(elm->getName()) != e.end()));
})
PRIMDEF("unquote", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr elm(dynamic_cast<IdentifierElement*>(s.pop()));
  s.push(new IdentifierElement(elm->getName()));
})
PRIMDEF("identifier-to-string", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr elm(dynamic_cast<IdentifierElement*>(s.pop()));
  s.push(
    new StringElement((elm->isQuoted() ? "`" : "") + elm->getName()));
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
  CommandPtr cmd(getOrError(e, id->getName()));
  if (checkType(cmd, new TypeElement(StackElement::DataType::Defined))) {
    s.push(new NumberElement(cmd->getSig().size(), 0));
    delete cmd;
    return;
  }
  delete cmd;
  throw RuntimeError("Identifier " + id->getName() + " is bound to a primitive command.");
}) 
PRIMDEF("body", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr id(dynamic_cast<IdentifierElement*>(s.pop()));
  CommandPtr cmd(getOrError(e, id->getName()));
  if (checkType(cmd, new TypeElement(StackElement::DataType::Defined))) {
    s.push(new SubstackElement(cmd->getBody());
    delete cmd;
    return;
  }
  delete cmd;
  throw RuntimeError("Identifier " + id->getName() + " is bound to a primitive command.");
})
PRIMDEF("signature", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Identifier)});
  IdentifierPtr id(dynamic_cast<IdentifierElement*>(s.pop()));
  CommandPtr cmd(getOrError(e, id->getName()));
  if (checkType(cmd, new TypeElement(StackElement::DataType::Defined))) {
    s.push(new SubstackElement(cmd->getSig());
    delete cmd;
    return;
  }
  delete cmd;
  throw RuntimeError("Identifier " + id->getName() + " is bound a primitive command.");
}) 
