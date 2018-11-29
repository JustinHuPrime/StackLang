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

PRIMDEF("euler", {
  s.push(new NumberElement(M_E));
})
PRIMDEF("pi", {
  s.push(new NumberElement(M_PI));
})
PRIMDEF("number?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Any)});
  ElementPtr elm(s.pop());
  s.push(new BooleanElement(elm->getType() ==
    StackElement::DataType::Number));
})
PRIMDEF("number-to-string", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new StringElement(static_cast<string>(*elm)));
})
PRIMDEF("string-to-number", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::String)});
  StringPtr elm(dynamic_cast<StringElement*>(s.pop()));
  ElementPtr result;
  try {
    result = ElementPtr(StackElement::parse(elm->getData()));
  } catch (...) {
    throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
      " produced an error.", context);
        }
    if (result->getType() != StackElement::DataType::Number) {
      throw RuntimeError("Parsing of element " + static_cast<string>(*elm) +
        " produced a non-number result.", context);
        }
    s.push(result.release());
})
PRIMDEF("precision", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr elm(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(elm->getPrecision(), 0));
})
PRIMDEF("set-precision", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr newPrec(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr target(dynamic_cast<NumberElement*>(s.pop()));
  long double remainder = newPrec->getData();
  long double whole;
  remainder = modf(remainder, &whole);
  if (remainder != 0 || whole <= 0) {
  throw RuntimeError("Expected a positive integer, but got " +
    static_cast<string>(*newPrec) + " instead.", context);
  }
  s.push(new NumberElement(target->getData(), whole));
})
PRIMDEF("add", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(
    first->getData() + second->getData(),
    max(first->getPrecision(), second->getPrecision())));
})
PRIMDEF("subtract", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(
    second->getData() - first->getData(),
    max(first->getPrecision(), second->getPrecision())));
})
PRIMDEF("multiply", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(
    new NumberElement(first->getData() * second->getData(),
    first->getPrecision() + second->getPrecision()));
})
PRIMDEF("divide", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  if (first->getData() == 0) {
  throw RuntimeError("Attempted to divide by zero.", context);
  }
  s.push(
    new NumberElement(second->getData() / first->getData(),
    first->getPrecision() + second->getPrecision()));
})
PRIMDEF("modulo", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
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
})  
PRIMDEF("floor", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(floor(num->getData()), 0));
})
PRIMDEF("ceil", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(ceil(num->getData()), 0));
})
PRIMDEF("round", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
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
})
PRIMDEF("round*", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(round(num->getData()), 0));
})
PRIMDEF("trunc", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(trunc(num->getData()), 0));
})
PRIMDEF("abs", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(abs(num->getData()), num->getPrecision()));
})
PRIMDEF("sign", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
 s.push(new NumberElement(
    num->getData() == 0 ? 0 : copysign(1, num->getData()), 0));
})
PRIMDEF("max", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(first->getData() > second->getData() ? first.release()
    : second.release());
})
PRIMDEF("min", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(first->getData() < second->getData() ? first.release()
    : second.release());
})
PRIMDEF("pow", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  if (fmod(first->getData(), 1) == 0)
  s.push(new NumberElement(
    pow(second->getData(), first->getData()),
    ceil(second->getPrecision() * first->getData())));
  else
    s.push(new NumberElement(pow(second->getData(), first->getData())));
})
PRIMDEF("log", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(
    new NumberElement(log(second->getData()) / log(first->getData())));
})
PRIMDEF("equal?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new BooleanElement(
    spaceship(first->getData(), second->getData(),
    pow(10, -max(first->getPrecision(),
    second->getPrecision()))) == 0));
})
PRIMDEF("less-than?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new BooleanElement(
  spaceship(second->getData(), first->getData(),
  pow(10, -max(first->getPrecision(),
  second->getPrecision()))) == -1));
}) 
PRIMDEF("greater-than?", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new BooleanElement(
  spaceship(second->getData(), first->getData(),
  pow(10, -max(first->getPrecision(),
  second->getPrecision()))) == -1));
}) 
PRIMDEF("sine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(sin(num->getData())));
})
PRIMDEF("cosine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(cos(num->getData())));
})
PRIMDEF("tangent", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(tan(num->getData())));
})
PRIMDEF("arcsine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  long double result = asin(num->getData());
  if (isnan(result))
  throw RuntimeError("The value " + static_cast<string>(*num) +
                                 " is not in the range for arcsine.",
                                 context);
  s.push(new NumberElement(result));
})
PRIMDEF("arccosine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  long double result = acos(num->getData());
  if (isnan(result))
 throw RuntimeError("The value " + static_cast<string>(*num) +
                                 "is not in the range for arccosine.",
                                 context);
  s.push(new NumberElement(result));
})
PRIMDEF("arctangent", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(atan(num->getData())));
})
PRIMDEF("arctangent2", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number),
            new TypeElement(StackElement::DataType::Number)});
  NumberPtr first(dynamic_cast<NumberElement*>(s.pop()));
  NumberPtr second(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new BooleanElement(
    spaceship(first->getData(), second->getData(),
    pow(10, -max(first->getPrecision(),
    second->getPrecision()))) == 0));
})
PRIMDEF("hyperbolic-sine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(sinh(num->getData())));
})    
PRIMDEF("hyperbolic-cosine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(cosh(num->getData())));
}) 
PRIMDEF("hyperbolic-tangent", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(tanh(num->getData())));
}) 
PRIMDEF("hyperbolic-arcsine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  s.push(new NumberElement(asinh(num->getData())));
}) 
PRIMDEF("hyperbolic-arccosine", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  long double result = acosh(num->getData());
  if (isnan(result))
  throw RuntimeError(
    "The value " + static_cast<string>(*num) +
      "is not in the range for the hyperbolic arccosines.",
              context);
        s.push(new NumberElement(result));
}) 
PRIMDEF("hyperbolic-arctangent", {
  checkTypes(s, Stack{new TypeElement(StackElement::DataType::Number)});
  NumberPtr num(dynamic_cast<NumberElement*>(s.pop()));
  long double result = atanh(num->getData());
  if (isnan(result))
  throw RuntimeError(
   "The value " + static_cast<string>(*num) +
   "is not in the range for the hyperbolic arctangent.", context);
  s.push(new NumberElement(result));
}) 
PRIMDEF("random", {
  random_device rd;
  s.push(new NumberElement(static_cast<long double>(rd()) / rd.max()));
})
