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

// Special included file for implementation of special primitives (define,
// include, etc.)

{"define",
 {Stack{new TypeElement(StackElement::DataType::Substack,
                        new TypeElement(StackElement::DataType::Type)),
        new TypeElement(StackElement::DataType::Substack),
        new TypeElement(StackElement::DataType::Command)},
  [](Stack& s, Defines& defs, vector<string>& context) {
    CommandPtr name(dynamic_cast<CommandElement*>(s.pop()));
    SubstackPtr body(dynamic_cast<SubstackElement*>(s.pop()));
    SubstackPtr sig(dynamic_cast<SubstackElement*>(s.pop()));
    auto iter = defs.find(name->getName());
    if (iter != defs.end()) {
      throw RuntimeError("Cannot redefine " + name->getName() + ".", context);
    }
    DefinedFunction def = DefinedFunction(sig->getData(), body->getData());
    defs[name->getName()] = def;
  }}},
    {"define*",
     {Stack{new TypeElement(StackElement::DataType::Substack,
                            new TypeElement(StackElement::DataType::Type)),
            new TypeElement(StackElement::DataType::Substack),
            new TypeElement(StackElement::DataType::Command),
            new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines& defs, vector<string>& context) {
        CommandPtr name(dynamic_cast<CommandElement*>(s.pop()));
        CommandPtr contextReq(dynamic_cast<CommandElement*>(s.pop()));
        SubstackPtr body(dynamic_cast<SubstackElement*>(s.pop()));
        SubstackPtr sig(dynamic_cast<SubstackElement*>(s.pop()));
        auto iter = defs.find(name->getName());
        if (iter != defs.end()) {
          throw RuntimeError("Cannot redefine " + name->getName() + ".",
                             context);
        }
        DefinedFunction def = DefinedFunction(sig->getData(), body->getData(),
                                              contextReq.release());
        defs[name->getName()] = def;
      }}},
    {"undefine",
     {Stack{new TypeElement(StackElement::DataType::Command)},
      [](Stack& s, Defines& defs, vector<string>& context) {
        CommandPtr elm(dynamic_cast<CommandElement*>(s.pop()));
        auto iter = defs.find(elm->getName());
        if (iter == defs.end()) {
          throw RuntimeError(
              "Command " + elm->getName() + " was not previously defined.",
              context);
        } else {
          defs.erase(iter);
        }
      }}},
    {"include",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, Defines& defs, vector<string>& context) {
        StringPtr given(dynamic_cast<StringElement*>(s.pop()));
        string path = given->getData();
        ifstream fin;

        // change this to be more portable.
        if (!fin.is_open()) fin.open(path);
        if (!fin.is_open()) fin.open(path + ".sta");
        if (path.find('/') ==
            string::npos) {  // filename - check file within libs folder
          if (!fin.is_open()) fin.open("libs/" + path);
          if (!fin.is_open()) fin.open("libs/" + path + ".sta");
        }
        if (!fin.is_open())
          throw RuntimeError("Could not open include file " + path + ".",
                             context);

        string buffer;
        size_t lineNumber = 1;
        int substackLevel = 0;
        bool inString = false;
        bool inComment = false;
        bool setPrevChar = true;
        char prevChar = '\0';
        while (fin.peek() != EOF) {
          char c = fin.get();
          if (c == ';' && !inString) {
            inComment = true;
            continue;
          } else if (c == '"' && !inComment && !inString) {
            inString = true;
          } else if (c == '"' && !inComment && inString && prevChar != '\\') {
            inString = false;
          } else if (c == '\\' && inString && prevChar == '\\') {
            prevChar = '\0';
            setPrevChar = false;
          } else if (c == '<' && prevChar == '<' && !inString && !inComment) {
            prevChar = '\0';
            setPrevChar = false;
            substackLevel++;
          } else if (c == '>' && prevChar == '>' && !inString && !inComment) {
            prevChar = '\0';
            setPrevChar = false;
            if (--substackLevel < 0)
              throw ParserException(
                  path + ":" + to_string(lineNumber) +
                      ": Found unmatched closing substack delimiter.",
                  "", 0, context);
          } else if (c == '\n' && substackLevel == 0 && trim(buffer) != "") {
            try {
              s.push(StackElement::parse(trim(buffer)));
            } catch (const StackOverflowError&) {
              throw StackOverflowError(s.getLimit(), context);
            } catch (const ParserException& e) {
              throw ParserException(
                  path + ":" + to_string(lineNumber) + ": " + e.getMessage(),
                  e.getContext(), e.getLocation(), context);
            }
            buffer.erase();
            inString = false;
            inComment = false;
            prevChar = '\0';
            execute(s, defs, context);
            lineNumber++;
            continue;  // skip end block
          } else if (c == '\n' && substackLevel == 0 && trim(buffer) == "") {
            buffer.erase();
            inString = false;
            inComment = false;
            prevChar = '\0';
            lineNumber++;
            continue;  // skip end block
          } else if (c == '\n' && substackLevel > 0) {
            if (buffer.back() != ',' && !inString && !ends_with(buffer, "<<"))
              buffer += ',';
            prevChar = '\0';
            inString = false;
            inComment = false;
            lineNumber++;
            continue;
          }

          if (!inComment) {
            buffer += c;
            if (setPrevChar) prevChar = c;
            setPrevChar = true;
          }
        }

        if (substackLevel == 0 && trim(buffer) != "") {
          try {
            s.push(StackElement::parse(trim(buffer)));
          } catch (const StackOverflowError&) {
            throw StackOverflowError(s.getLimit(), context);
          } catch (const ParserException& e) {
            throw ParserException(
                path + ":" + to_string(lineNumber) + ": " + e.getMessage(),
                e.getContext(), e.getLocation(), context);
          }
          execute(s, defs, context);
        } else if (substackLevel > 0) {
          throw ParserException(path + ":" + to_string(lineNumber) +
                                    ": Missing " + to_string(substackLevel) +
                                    " closing substack delimiter" +
                                    (substackLevel > 1 ? "s" : "") + ".",
                                "", 0, context);
        }

        fin.close();

        execute(s, defs, context);
      }}},
    {"drop",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, Defines&, vector<string>&) { delete s.pop(); }}},
    {"drop*",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, vector<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double remainder = num->getData();
        long double whole;
        remainder = modf(remainder, &whole);
        if (remainder != 0 || whole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*num) + " instead.",
                             context);
        }
        while (whole-- > 0) delete s.pop();
      }}},
    {"clear",
     {Stack{}, [](Stack& s, Defines&, vector<string>&) { s.clear(); }}},
    {"rotate",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, vector<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double remainder = num->getData();
        long double whole;
        remainder = modf(remainder, &whole);
        if (remainder != 0 || whole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*num) + " instead.",
                             context);
        }
        StackElement* grabbed;
        stack<StackElement*> rest;
        while (whole-- > 1) rest.push(s.pop());
        grabbed = s.pop();
        while (!rest.empty()) {
          s.push(rest.top());
          rest.pop();
        }
        s.push(grabbed);
      }}},
    {"rotate*",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, vector<string>& context) {
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        long double reachRemainder = first->getData();
        long double reachWhole;
        reachRemainder = modf(reachRemainder, &reachWhole);
        if (reachRemainder != 0 || reachWhole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*second) + " instead.",
                             context);
        }
        long double countRemainder = second->getData();
        long double countWhole;
        countRemainder = modf(countRemainder, &countWhole);
        if (countRemainder != 0 || countWhole <= 0)
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*first) + " instead.",
                             context);

        while (countWhole-- > 0) {
          long double reach = reachWhole;
          StackElement* grabbed;
          stack<StackElement*> rest;
          while (reach-- > 1) rest.push(s.pop());
          grabbed = s.pop();
          while (!rest.empty()) {
            s.push(rest.top());
            rest.pop();
          }
          s.push(grabbed);
        }
      }}},
    {"duplicate",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, Defines&, vector<string>&) { s.push(s.top()->clone()); }}},
    {"error",
     {Stack{new TypeElement(StackElement::DataType::String)},
      [](Stack& s, Defines&, vector<string>& context) {
        StringPtr str(dynamic_cast<StringElement*>(s.pop()));
        throw RuntimeError(str->getData(), context);
      }}},
    {"error*",
     {Stack{new TypeElement(StackElement::DataType::String),
            new TypeElement(StackElement::DataType::String),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, vector<string>& context) {
        NumberPtr ctxIndex(dynamic_cast<NumberElement*>(s.pop()));
        StringPtr ctx(dynamic_cast<StringElement*>(s.pop()));
        StringPtr msg(dynamic_cast<StringElement*>(s.pop()));
        long double remainder = ctxIndex->getData();
        long double whole;
        remainder = modf(remainder, &whole);
        if (remainder != 0 || whole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*ctxIndex) + " instead.",
                             context);
        }
        throw RuntimeError(msg->getData(), ctx->getData(), whole, context);
      }}},
    {"null", {Stack{}, [](Stack&, Defines&, vector<string>&) { return; }}},
    {"identity",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack&, Defines&, vector<string>&) {
        return;  // equivalent to popping then pushing.
      }}},