#include "language/stack.h"

#include <limits>
#include <memory>
#include <queue>

#include "language/exceptions/stackOverflowError.h"
#include "language/exceptions/stackUnderflowError.h"

namespace stacklang {
using stacklang::StackElement;
using stacklang::exceptions::StackOverflowError;
using stacklang::exceptions::StackUnderflowError;
using std::make_unique;
using std::numeric_limits;
using std::queue;

Stack::Stack() noexcept
    : head(nullptr), dataSize(0), limit(numeric_limits<size_t>().max()) {}

Stack::Stack(size_t lim) noexcept : head(nullptr), dataSize(0), limit(lim) {}

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

  Node* temp = new Node(ptr, head);
  head = temp;
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

StackElement* Stack::top() {
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
}  // namespace stacklang
