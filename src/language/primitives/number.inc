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

// Special included file for implementation of number-related function
// primitives

// {"euler",
//  {Stack{}, [](Stack& s, map<string, DefinedFunction>&,
//               list<string>&) { s.push(new NumberElement(M_E, false)); }}},
//     {"pi",
//      {Stack{}, [](Stack& s, map<string, DefinedFunction>&,
//                   list<string>&) { s.push(new NumberElement(M_PI, false));
//                   }}},
//     {"number?",
//      {Stack{new TypeElement(StackElement::DataType::Any)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
//         ElementPtr elm(s.pop());
//         s.push(new BooleanElement(elm->getType() ==
//                                   StackElement::DataType::Number));
//       }}},
//     {"exact?",
//      {Stack{new TypeElement(StackElement::DataType::Any)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
//         ElementPtr elm(s.pop());
//         s.push(new BooleanElement(
//             elm->getType() == StackElement::DataType::Number &&
//             dynamic_cast<NumberElement*>(elm.get())->isExact()));
//       }}},
//     {"exact-to-inexact",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
//         NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(elm->getData(), false));
//       }}},
//     {"inexact-to-exact",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
//         NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(elm->getData(), true));
//       }}},
//     {"number-to-string",
//      {Stack{new TypeElement(StackElement::DataType::Boolean)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>&) {
//         NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new StringElement(static_cast<string>(*elm)));
//       }}},
//     {"string-to-number",
//      {Stack{new TypeElement(StackElement::DataType::String)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         StringPtr elm(dynamic_cast<StringElement*>(s.pop()));
//         ElementPtr result;
//         try {
//           result = ElementPtr(StackElement::parse(elm->getData()));
//         } catch (...) {
//           throw RuntimeError("Parsing of element " +
//           static_cast<string>(*elm) +
//                                  " produced an error.",
//                              context);
//         }
//         if (result->getType() != StackElement::DataType::Number) {
//           throw RuntimeError("Parsing of element " +
//           static_cast<string>(*elm) +
//                                  " produced a non-number result.",
//                              context);
//         }
//         s.push(result.release());
//       }}},
//     {"add",
//      {Stack{new TypeElement(StackElement::DataType::Number),
//             new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(first->getData() + second->getData(),
//                                  first->isExact() && second->isExact()));
//       }}},
//     {"subtract",
//      {Stack{new TypeElement(StackElement::DataType::Number),
//             new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(second->getData() - first->getData(),
//                                  first->isExact() && second->isExact()));
//       }}},
//     {"multiply",
//      {Stack{new TypeElement(StackElement::DataType::Number),
//             new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(first->getData() * second->getData(),
//                                  first->isExact() && second->isExact()));
//       }}},
//     {"divide",
//      {Stack{new TypeElement(StackElement::DataType::Number),
//             new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         if (first->getData() == 0) {
//           throw RuntimeError("Attempted to divide by zero.", context);
//         }
//         s.push(new NumberElement(second->getData() / first->getData(),
//                                  first->isExact() && second->isExact()));
//       }}},
//     {"modulo",
//      {Stack{new TypeElement(StackElement::DataType::Number,
//                             new TypeElement(StackElement::DataType::Exact)),
//             new TypeElement(StackElement::DataType::Number,
//                             new TypeElement(StackElement::DataType::Exact))},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         mpq_class base = first->getData();
//         mpq_class target = second->getData();
//         if (base == 0) {
//           throw RuntimeError("Attempted to modulo by zero.", context);
//         }
//         int resultSign = sgn(base);
//         base = abs(base);
//         target = abs(target);
//         while (target >= base) target -= base;
//         if (resultSign < 0 && target != 0) target -= base;
//         s.push(new NumberElement(target));
//       }}},
//     {"floor",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new
//         NumberElement(mpq_class(floor(mpf_class(num->getData())))));
//       }}},
//     {"ceil",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new
//         NumberElement(mpq_class(ceil(mpf_class(num->getData())))));
//       }}},
//     {"round",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         mpq_class numRaw = num->getData();
//         mpz_class numFloored = mpz_class(floor(mpf_class(numRaw)));
//         mpz_class numCeiled = mpz_class(ceil(mpf_class(numRaw)));
//         if (abs(numRaw - numFloored) < abs(numRaw - numCeiled))
//           s.push(new NumberElement(mpq_class(numFloored)));
//         else if (abs(numRaw - numFloored) > abs(numRaw - numCeiled))
//           s.push(new NumberElement(mpq_class(numCeiled)));
//         else
//           s.push(new NumberElement(
//               mpq_class(numFloored % 2 == 0 ? numFloored : numCeiled)));
//       }}},
//     {"round*",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         mpq_class numRaw = num->getData();
//         mpz_class numFloored = mpz_class(floor(mpf_class(numRaw)));
//         mpz_class numCeiled = mpz_class(ceil(mpf_class(numRaw)));
//         if (abs(numRaw - numFloored) < abs(numRaw - numCeiled))
//           s.push(new NumberElement(mpq_class(numFloored)));
//         else
//           s.push(new NumberElement(mpq_class(numCeiled)));
//       }}},
//     {"trunc",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new
//         NumberElement(mpq_class(trunc(mpf_class(num->getData())))));
//       }}},
//     {"abs",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(abs(num->getData()), num->isExact()));
//       }}},
//     {"sign",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(mpq_class(sgn(num->getData()))));
//       }}},
//     {"max",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(first->getData() > second->getData() ? first.release()
//                                                     : second.release());
//       }}},
//     {"min",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(first->getData() < second->getData() ? first.release()
//                                                     : second.release());
//       }}},
//     {"gcd",
//      {Stack{new TypeElement(StackElement::DataType::Number,
//                             new TypeElement(StackElement::DataType::Exact)),
//             new TypeElement(StackElement::DataType::Number,
//                             new TypeElement(StackElement::DataType::Exact))},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(mpq_class(
//             gcd(first->getData().get_num(), second->getData().get_num()),
//             lcm(first->getData().get_den(), second->getData().get_den()))));
//       }}},
//     {"lcm",
//      {Stack{new TypeElement(StackElement::DataType::Number,
//                             new TypeElement(StackElement::DataType::Exact)),
//             new TypeElement(StackElement::DataType::Number,
//                             new TypeElement(StackElement::DataType::Exact))},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
//         NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
//         mpz_class den =
//             lcm(first->getData().get_den(), second->getData().get_den());
//         mpq_class result = mpq_class(
//             lcm(first->getData().get_num() * den /
//             first->getData().get_den(),
//                 second->getData().get_num() * den /
//                     second->getData().get_den()),
//             den);
//         result.canonicalize();
//         s.push(new NumberElement(result));
//       }}},
//     {"numerator",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(mpq_class(num->getData().get_num())));
//       }}},
//     {"denominator",
//      {Stack{new TypeElement(StackElement::DataType::Number)},
//       [](Stack& s, map<string, DefinedFunction>&, list<string>& context) {
//         NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
//         s.push(new NumberElement(mpq_class(num->getData().get_den())));
//       }}},