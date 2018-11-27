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

// Special included file for implementation of command-related function
// primitives

PRIMDEF("command?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(
    new BooleanElement(elm->getType() == StackElement::DataType::Command));
})
PRIMDEF("quoted?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(
    elm->getType() == StackElement::DataType::Command &&
    dynamic_cast<CommandElement*>(elm.get())->isQuoted()));
})
PRIMDEF("defined?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
  s.push(new BooleanElement(prims.find(cmd->getName()) != prims.end() ||
    defs.find(cmd->getName()) != defs.end()));
})
PRIMDEF("local?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
  if (prims.find(cmd->getName()) != prims.end()) {
    s.push(new BooleanElement(false));
    return;
  }
  auto iter = defs.find(cmd->getName());
  if (iter == defs.end()) {
    s.push(new BooleanElement(false));
    return;
  }
  s.push(new BooleanElement(iter->second.context != nullptr));
})
PRIMDEF("unquote", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr elm(dynamic_cast<CommandElement*>(s.pop()));
  s.push(new CommandElement(elm->getName()));
})
PRIMDEF("command-to-string", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr elm(dynamic_cast<CommandElement*>(s.pop()));
  s.push(
    new StringElement((elm->isQuoted() ? "`" : "") + elm->getName()));
}) 
PRIMDEF("string-to-command", {
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
  if (result->getType() != StackElement::DataType::Command) {
    string elmString = static_cast<string>(*elm);
    throw RuntimeError("Parsing of element " + elmString +
                       " produced a non-command result.",
                       context);
  }
  s.push(result.release());
}) 
PRIMDEF("string-to-command*", {
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
  if (result->getType() != StackElement::DataType::Command) {
    string elmString = static_cast<string>(*elm);
    throw RuntimeError("Parsing of element " + elmString +
                       " produced a non-command result.",
                       context);
  }
  CommandPtr cmd(dynamic_cast<CommandElement*>(result.release()));
  s.push(new CommandElement(cmd->getName(), true));
}) 
PRIMDEF("arity", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
  auto primIter = prims.find(cmd->getName());
  if (primIter != prims.end()) {
    s.push(new NumberElement(primIter->second.first.size(), 0));
    return;
  }
  auto defIter = defs.find(cmd->getName());
  if (defIter != defs.end()) {
    s.push(new NumberElement(defIter->second.signature.size(), 0));
    return;
  }
  throw RuntimeError("Command " + cmd->getName() + " doesn't exist.",
                     context);
}) 
PRIMDEF("body", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
  auto defIter = defs.find(cmd->getName());
  if (defIter != defs.end()) {
    s.push(new SubstackElement(defIter->second.body));
    return;
  }
  throw RuntimeError("Command " + cmd->getName() + " isn't defined.",
                     context);
}) 
PRIMDEF("context", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
  auto defIter = defs.find(cmd->getName());
  if (defIter != defs.end()) {
    if (defIter->second.context == nullptr) {
      throw RuntimeError(
        "Command " + cmd->getName() + " is not a local function.",
          context);
      }
      s.push(defIter->second.context->clone());
      return;
    }
  throw RuntimeError("Command " + cmd->getName() + " isn't defined.",
                    context);
}) 
PRIMDEF("signature", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Command)});
  CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
  auto primIter = prims.find(cmd->getName());
  if (primIter != prims.end()) {
    s.push(new SubstackElement(primIter->second.first));
    return;
  }
  auto defIter = defs.find(cmd->getName());
  if (defIter != defs.end()) {
    s.push(new SubstackElement(defIter->second.signature));
    return;
  }
  throw RuntimeError("Command " + cmd->getName() + " doesn't exist.",
                     context);
}) 
