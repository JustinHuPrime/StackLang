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

// Implementation of the stack, iterator, and base StackElement, including the
// parse method.

#include "language/stack/stack.h"

#include <algorithm>
#include <cstring>
#include <queue>
#include <string>

#include "language/exceptions/interpreterExceptions.h"
#include "language/exceptions/languageExceptions.h"
#include "language/stack/stackElements.h"
#include "util/stringUtils.h"

namespace stacklang {
namespace {
using stacklang::StackElement;
using stacklang::exceptions::ParserException;
using stacklang::exceptions::StackOverflowError;
using stacklang::exceptions::StackUnderflowError;
using stacklang::stackelements::BooleanElement;
using stacklang::stackelements::CommandElement;
using stacklang::stackelements::NumberElement;
using stacklang::stackelements::StringElement;
using stacklang::stackelements::SubstackElement;
using stacklang::stackelements::TypeElement;
using std::any_of;
using std::find;
using std::initializer_list;
using std::make_unique;
using std::numeric_limits;
using std::queue;
using std::string;
using util::ends_with;
using util::starts_with;
}  // namespace

// Acts as a dispatcher for specific types, and handles parsing of trivially
// parsed types (currently just Booleans).
StackElement* StackElement::parse(const string& s) {
  if (s.empty()) {
    throw ParserException("Given input is empty.", s, 0);
  } else if (isdigit(s[0]) ||
             (find(NumberElement::NUMBER_SIGNS,
                   NumberElement::NUMBER_SIGNS +
                       strlen(NumberElement::NUMBER_SIGNS),
                   s[0]) != NumberElement::NUMBER_SIGNS +
                                strlen(NumberElement::NUMBER_SIGNS) &&
              any_of(s.begin(), s.end(), isdigit)))  // looks like a number.
  {
    return NumberElement::parse(s);
  } else if (starts_with(
                 s,
                 string(1, StringElement::QUOTE_CHAR)))  // starts with a quote
  {
    return StringElement::parse(s);
  } else if (starts_with(s, SubstackElement::SUBSTACK_BEGIN) &&
             ends_with(
                 s,
                 SubstackElement::SUBSTACK_END))  // has a pair of substack
                                                  // delmiters on either end
                                                  // - must be a substack.
  {
    return SubstackElement::parse(s);
  } else if (s == BooleanElement::TSTR ||
             s == BooleanElement::FSTR)  // it's either true or false
  {
    return new BooleanElement(s == string(BooleanElement::TSTR));
  } else if (s.find_first_of(TypeElement::PARENS) != string::npos ||
             find(TypeElement::TYPES().begin(), TypeElement::TYPES().end(),
                  s) != TypeElement::TYPES()
                            .end())  // has a subtype, or exists in types
  {                                  // is a type
    return TypeElement::parse(s);
  } else if (isalpha(s[0]) ||
             (s[0] == CommandElement::QUOTE_CHAR &&
              s.length() > 1))  // starts with a character or
                                // a command-quote and at least one other thing.
  {
    return CommandElement::parse(s);
  } else  // error case
  {
    throw ParserException(
        "Input doesn't look like any type - does it begin with a symbol?", s,
        0);
  }
}

StackElement::~StackElement() = default;

StackElement::DataType StackElement::getType() const noexcept {
  return dataType;
}

StackElement::StackElement(DataType type) noexcept : dataType(type) {}

Stack::Stack(size_t lim) noexcept : head(nullptr), dataSize(0), limit(lim) {}

Stack::Stack(initializer_list<StackElement*> initList) noexcept
    : head(nullptr), dataSize(0), limit(numeric_limits<size_t>().max()) {
  for (StackElement* elm : initList) {
    push(elm);
  }
}

Stack::~Stack() noexcept { clear(); }

Stack::Stack(const Stack& other) noexcept
    : head(copy(other.head)), dataSize(other.dataSize), limit(other.limit) {}

Stack::Stack(Stack&& other) noexcept {
  head = other.head;
  dataSize = other.dataSize;
  limit = other.limit;
  other.head = nullptr;
}

Stack& Stack::operator=(const Stack& other) noexcept {
  clear();
  head = copy(other.head);
  dataSize = other.dataSize;
  limit = other.limit;
  return *this;
}

Stack& Stack::operator=(Stack&& other) noexcept {
  clear();
  head = other.head;
  dataSize = other.dataSize;
  limit = other.limit;
  other.head = nullptr;
  return *this;
}

void Stack::push(StackElement* ptr) {
  if (dataSize >= limit) {
    throw StackOverflowError(limit);
  }

  head = new Node(ptr, head);
  dataSize++;
}

StackElement* Stack::pop() {
  if (head != nullptr) {
    StackElement* retval = head->elm.release();
    Node* temp = head->next;
    delete head;
    head = temp;
    dataSize--;
    return retval;
  }

  throw StackUnderflowError();
}

const StackElement* Stack::top() {
  if (head != nullptr) {
    return head->elm.get();
  }

  throw StackUnderflowError();
}

size_t Stack::size() const noexcept { return dataSize; }

size_t Stack::getLimit() const noexcept { return limit; }

void Stack::setLimit(size_t newLimit) {
  if (dataSize > newLimit) {
    throw StackOverflowError(newLimit);
  }

  limit = newLimit;
}

void Stack::reverse() noexcept {
  queue<StackElement*> s;
  while (!isEmpty()) {
    s.push(pop());
  }
  while (!s.empty()) {
    push(s.front());
    s.pop();
  }
}

bool Stack::isEmpty() const noexcept { return dataSize == 0; }

Stack::StackIterator Stack::begin() const noexcept {
  return StackIterator(head);
}

Stack::StackIterator Stack::end() const noexcept {
  return StackIterator(nullptr);
}

void Stack::clear() noexcept {
  while (head != nullptr) {
    Node* temp = head->next;
    delete head;
    head = temp;
  }

  dataSize = 0;
}

Stack::Node* Stack::copy(Node* other) noexcept {
  if (other == nullptr) {
    return nullptr;
  } else {
    return new Node(other->elm->clone(), copy(other->next));
  }
}

Stack::Node::Node(StackElement* ptr, Node* nxt) noexcept
    : elm(ptr), next(nxt) {}

Stack::StackIterator::StackIterator(const Stack::Node* node) noexcept
    : curr(node) {}

const StackElement* Stack::StackIterator::operator*() noexcept {
  return curr->elm.get();
}

Stack::StackIterator& Stack::StackIterator::operator++() noexcept {
  if (curr != nullptr) {
    curr = curr->next;
  }

  return *this;
}

Stack::StackIterator Stack::StackIterator::operator++(int) noexcept {
  StackIterator temp = *this;

  operator++();

  return temp;
}

bool operator==(const Stack::StackIterator& first,
                const Stack::StackIterator& second) noexcept {
  return first.curr == second.curr;
}

bool operator!=(const Stack::StackIterator& first,
                const Stack::StackIterator& second) noexcept {
  return !(first == second);
}
}  // namespace stacklang
