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

{"euler",
 {Stack{},
  [](Stack& s, Defines&, list<string>&) { s.push(new NumberElement(M_E)); }}},
    {"pi",
     {Stack{}, [](Stack& s, Defines&,
                  list<string>&) { s.push(new NumberElement(M_PI)); }}},
    {"number?",
     {Stack{new TypeElement(StackElement::DataType::Any)},
      [](Stack& s, Defines&, list<string>&) {
        ElementPtr elm(s.pop());
        s.push(new BooleanElement(elm->getType() ==
                                  StackElement::DataType::Number));
      }}},
    {"number-to-string",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new StringElement(static_cast<string>(*elm)));
      }}},
    {"string-to-number",
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
        if (result->getType() != StackElement::DataType::Number) {
          throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
                                 " produced a non-number result.",
                             context);
        }
        s.push(result.release());
      }}},
    {"precision",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(elm->getPrecision(), 0));
      }}},
    {"set-precision",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>& context) {
        NumberPtr newPrec(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr target(dynamic_cast<NumberElement*>(s.pop()));
        long double remainder = newPrec->getData();
        long double whole;
        remainder = modf(remainder, &whole);
        if (remainder != 0 || whole <= 0) {
          throw RuntimeError("Expected a positive integer, but got " +
                                 static_cast<string>(*newPrec) + " instead.",
                             context);
        }
        s.push(new NumberElement(target->getData(), whole));
      }}},
    {"add",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(
            first->getData() + second->getData(),
            max(first->getPrecision(), second->getPrecision())));
      }}},
    {"subtract",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(
            second->getData() - first->getData(),
            max(first->getPrecision(), second->getPrecision())));
      }}},
    {"multiply",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(
            new NumberElement(first->getData() * second->getData(),
                              first->getPrecision() + second->getPrecision()));
      }}},
    {"divide",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>& context) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        if (first->getData() == 0) {
          throw RuntimeError("Attempted to divide by zero.", context);
        }
        s.push(
            new NumberElement(second->getData() / first->getData(),
                              first->getPrecision() + second->getPrecision()));
      }}},
    {"modulo",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>& context) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        long double base = first->getData();
        long double target = second->getData();
        if (base == 0) {
          throw RuntimeError("Attempted to modulo by zero.", context);
        }
        long double resultSign = copysign(1, base);  // base won't be zero.
        base = abs(base);
        target = abs(target);
        while (target >= base) target -= base;
        if (resultSign < 0 && target != 0) target -= base;
        s.push(new NumberElement(target, second->getPrecision()));
      }}},
    {"floor",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(floor(num->getData()), 0));
      }}},
    {"ceil",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(ceil(num->getData()), 0));
      }}},
    {"round",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double numRaw = num->getData();
        long double numFloored = floor(numRaw);
        long double numCeiled = ceil(numRaw);
        if (abs(numRaw - numFloored) < abs(numRaw - numCeiled)) {
          s.push(new NumberElement(numFloored, 0));
        } else if (abs(numRaw - numFloored) > abs(numRaw - numCeiled)) {
          s.push(new NumberElement(numCeiled, 0));
        } else {
          s.push(new NumberElement(
              fmod(numFloored, 2) == 0 ? numFloored : numCeiled, 0));
        }
      }}},
    {"round*",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(round(num->getData()), 0));
      }}},
    {"trunc",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(trunc(num->getData()), 0));
      }}},
    {"abs",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(abs(num->getData()), num->getPrecision()));
      }}},
    {"sign",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(
            num->getData() == 0 ? 0 : copysign(1, num->getData()), 0));
      }}},
    {"max",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(first->getData() > second->getData() ? first.release()
                                                    : second.release());
      }}},
    {"min",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(first->getData() < second->getData() ? first.release()
                                                    : second.release());
      }}},
    {"pow",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        if (fmod(first->getData(), 1) == 0)
          s.push(new NumberElement(
              pow(second->getData(), first->getData()),
              ceil(second->getPrecision() * first->getData())));
        else
          s.push(new NumberElement(pow(second->getData(), first->getData())));
      }}},
    {"log",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(
            new NumberElement(log(second->getData()) / log(first->getData())));
      }}},
    {"equal?",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new BooleanElement(
            spaceship(first->getData(), second->getData(),
                      pow(10, -max(first->getPrecision(),
                                   second->getPrecision()))) == 0));
      }}},
    {"less-than?",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new BooleanElement(
            spaceship(second->getData(), first->getData(),
                      pow(10, -max(first->getPrecision(),
                                   second->getPrecision()))) == -1));
      }}},
    {"greater-than?",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new BooleanElement(
            spaceship(second->getData(), first->getData(),
                      pow(10, -max(first->getPrecision(),
                                   second->getPrecision()))) == -1));
      }}},
    {"sine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(sin(num->getData())));
      }}},
    {"cosine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(cos(num->getData())));
      }}},
    {"tangent",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(tan(num->getData())));
      }}},
    {"arcsine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double result = asin(num->getData());
        if (isnan(result))
          throw RuntimeError("The value " + static_cast<string>(*num) +
                                 " is not in the range for arcsine.",
                             context);
        s.push(new NumberElement(result));
      }}},
    {"arccosine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double result = acos(num->getData());
        if (isnan(result))
          throw RuntimeError("The value " + static_cast<string>(*num) +
                                 "is not in the range for arccosine.",
                             context);
        s.push(new NumberElement(result));
      }}},
    {"arctangent",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(atan(num->getData())));
      }}},
    {"arctangent2",
     {Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr x(dynamic_cast<NumberElement*>(s.pop()));
        NumberPtr y(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(atan2(y->getData(), x->getData())));
      }}},
    {"hyperbolic-sine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(sinh(num->getData())));
      }}},
    {"hyperbolic-cosine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(cosh(num->getData())));
      }}},
    {"hyperbolic-tangent",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(tanh(num->getData())));
      }}},
    {"hyperbolic-arcsine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>&) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        s.push(new NumberElement(asinh(num->getData())));
      }}},
    {"hyperbolic-arccosine",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double result = acosh(num->getData());
        if (isnan(result))
          throw RuntimeError(
              "The value " + static_cast<string>(*num) +
                  "is not in the range for the hyperbolic arccosines.",
              context);
        s.push(new NumberElement(result));
      }}},
    {"hyperbolic-arctangent",
     {Stack{new TypeElement(StackElement::DataType::Number)},
      [](Stack& s, Defines&, list<string>& context) {
        NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
        long double result = atanh(num->getData());
        if (isnan(result))
          throw RuntimeError(
              "The value " + static_cast<string>(*num) +
                  "is not in the range for the hyperbolic arctangent.",
              context);
        s.push(new NumberElement(result));
      }}},
    {"random",
     {Stack{}, [](Stack& s, Defines&, list<string>&) {
        random_device rd;
        s.push(new NumberElement(static_cast<long double>(rd()) / rd.max()));
      }}},