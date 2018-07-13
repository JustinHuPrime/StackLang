#include "language/stack.h"

#include <memory>
#include <queue>

#include "language/exceptions/stackOverflowError.h"
#include "language/exceptions/stackUnderflowError.h"
#include "language/stack/stackIterator.h"

namespace StackLang {
using StackLang::StackElement;
using StackLang::Exceptions::StackOverflowError;
using StackLang::Exceptions::StackUnderflowError;
using std::make_unique;
using std::queue;

Stack::Stack() : head(nullptr), dataSize(0), limit(__LONG_MAX__) {}

Stack::Stack(unsigned long lim) : head(nullptr), dataSize(0), limit(lim) {}

Stack::~Stack() { clear(); }

Stack::Stack(const Stack& other)
    : head(copy(other.head)), dataSize(other.dataSize), limit(other.limit) {}

Stack::Stack(Stack&& other) {
  head = other.head;
  dataSize = other.dataSize;
  limit = other.limit;
  other.head = nullptr;
  other.dataSize = 0;
  other.limit = 0;
}

Stack& Stack::operator=(const Stack& other) {
  clear();
  head = copy(other.head);
  dataSize = other.dataSize;
  limit = other.limit;
  return *this;
}

Stack& Stack::operator=(Stack&& other) {
  clear();
  head = other.head;
  dataSize = other.dataSize;
  limit = other.limit;
  other.head = nullptr;
  other.dataSize = 0;
  other.limit = 0;
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

unsigned long Stack::size() const { return dataSize; }

unsigned long Stack::getLimit() const { return limit; }

void Stack::setLimit(unsigned long newLimit) {
  if (dataSize > newLimit) {
    throw StackOverflowError(newLimit);
  }

  limit = newLimit;
}

void Stack::reverse() {
  queue<StackElement*> s;
  while (!empty()) {
    s.push(pop());
  }
  while (!s.empty()) {
    push(s.front());
    s.pop();
  }
}

bool Stack::empty() const { return dataSize == 0; }

Stack::StackIterator Stack::begin() const { return StackIterator(head); }

Stack::StackIterator Stack::end() const { return StackIterator(nullptr); }

void Stack::clear() {
  while (head != nullptr) {
    Node* temp = head->next;
    delete head;
    head = temp;
  }

  dataSize = 0;
}

Stack::Node* Stack::copy(Node* other) {
  if (other == nullptr) {
    return nullptr;
  } else {
    return new Node(other->elm->clone(), copy(other->next));
  }
}

Stack::Node::Node(StackElement* ptr, Node* nxt) : elm(ptr), next(nxt) {}
}  // namespace StackLang
