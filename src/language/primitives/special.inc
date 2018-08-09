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

//  {"NAME",
//   {Stack{INPUT TYPES...}, // vertically sorted, like the stack.
//    [](Stack& s, map<string, DefinedFunction>&) {
//      PRIMITIVE BODY...
//    }}},

// {"define",
//  {Stack{new TypeElement(StackElement::DataType::Substack,
//                         new TypeElement(StackElement::DataType::Type)),
//         new TypeElement(StackElement::DataType::Substack),
//         new TypeElement(StackElement::DataType::Command,
//                         new TypeElement(StackElement::DataType::Quoted))},
//   [](Stack& s, map<string, DefinedFunction>&) { return; }}},
//     {"define*",
//      {Stack{new TypeElement(StackElement::DataType::Substack,
//                             new TypeElement(StackElement::DataType::Type)),
//             new TypeElement(StackElement::DataType::Substack),
//             new TypeElement(StackElement::DataType::Command,
//                             new TypeElement(StackElement::DataType::Quoted)),
//             new TypeElement(StackElement::DataType::Command,
//                             new
//                             TypeElement(StackElement::DataType::Quoted))},
//       [](Stack& s, map<string, DefinedFunction>&) { return; }}},
// {"undefine",
//  {Stack{new TypeElement(StackElement::DataType::Command,
//                         new TypeElement(StackElement::DataType::Quoted))},
//   [](Stack& s, map<string, DefinedFunction>&) { return; }}},
{"include",
 {Stack{new TypeElement(StackElement::DataType::String)},
  [](Stack& s, map<string, DefinedFunction>&) { return; }}},
    {"drop",
     {Stack{},
      [](Stack& s, map<string, DefinedFunction>&) {
        if (!s.isEmpty()) delete s.pop();
      }}},
    {"drop*",
     {Stack{new TypeElement(StackElement::DataType::Number,
                            new TypeElement(StackElement::DataType::Exact))},
      [](Stack& s, map<string, DefinedFunction>&) {
        NumberElement* num = dynamic_cast<NumberElement*>(s.pop());
        mpq_class rawNum = num->getData();
        if (rawNum.get_den() != 1 || rawNum.get_num() <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                             static_cast<string>(*num) + " instead.");
        }
        mpz_class rawInt(rawNum.get_num());
        for (; rawInt > 0; rawInt--) {
          delete s.pop();
        }
        delete num;
      }}},