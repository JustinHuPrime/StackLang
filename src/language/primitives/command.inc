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

{"command?",
 {Stack{new TypeElement(StackElement::DataType::Any)},
  [](Stack& s, Defines&, list<string>&) {
    ElementPtr elm(s.pop());
    s.push(
        new BooleanElement(elm->getType() == StackElement::DataType::Command));
  }}},
    {"quoted?",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, Defines&, list<string>&) {
        ElementPtr elm(s.pop());
        s.push(new BooleanElement(
            elm->getType() == StackElement::DataType::Command &&
            dynamic_cast<CommandElement*>(elm.get())->isQuoted()));
      }}},
    {"local?",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, Defines& defs, list<string>& context) {
        ElementPtr elm(s.pop());
        if (elm->getType() != StackElement::DataType::Command) {
          s.push(new BooleanElement(false));
          return;
        }
        CommandElement* cmd(dynamic_cast<CommandElement*>(elm.release()));
        if (prims->find(cmd->getName()) != prims->end()) {
          s.push(new BooleanElement(false));
          return;
        }
        auto iter = defs.find(cmd->getName());
        if (iter == defs.end()) {
          s.push(new BooleanElement(false));
          return;
        }
        s.push(new BooleanElement(iter->second.context != nullptr));
      }}},
    {"unquote",
     {Stack{new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines&, list<string>&) {
        CommandPtr elm(dynamic_cast<CommandElement*>(s.pop()));
        s.push(new CommandElement(elm->getName()));
      }}},
    {"command-to-string",
     {Stack{new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines&, list<string>&) {
        CommandPtr elm(dynamic_cast<CommandElement*>(s.pop()));
        s.push(
            new StringElement((elm->isQuoted() ? "`" : "") + elm->getName()));
      }}},
    {"string-to-command",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, Defines&, list<string>& context) {
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
      }}},
    {"string-to-command*",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, Defines&, list<string>& context) {
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
      }}},
    {"arity",
     {Stack{new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines& defs, list<string>& context) {
        CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
        auto primIter = prims->find(cmd->getName());
        if (primIter != prims->end()) {
          s.push(new NumberElement(primIter->second.first.size()));
          return;
        }
        auto defIter = defs.find(cmd->getName());
        if (defIter != defs.end()) {
          s.push(new NumberElement(defIter->second.signature.size()));
          return;
        }
        throw RuntimeError("Command " + cmd->getName() + " doesn't exist.",
                           context);
      }}},
    {"body",
     {Stack{new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines& defs, list<string>& context) {
        CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
        auto defIter = defs.find(cmd->getName());
        if (defIter != defs.end()) {
          s.push(new SubstackElement(defIter->second.body));
          return;
        }
        throw RuntimeError("Command " + cmd->getName() + " isn't defined.",
                           context);
      }}},
    {"context",
     {Stack{new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines& defs, list<string>& context) {
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
      }}},
    {"signature",
     {Stack{new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines& defs, list<string>& context) {
        CommandPtr cmd(dynamic_cast<CommandElement*>(s.pop()));
        auto primIter = prims->find(cmd->getName());
        if (primIter != prims->end()) {
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
      }}},