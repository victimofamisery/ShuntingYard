#include "ListStack.h"


ListStack::ListStack(const ValueType* rawArray, const size_t size) {
    _list.insert(_list.begin(), rawArray, rawArray + size);
}

void ListStack::push(const ValueType& value) {
    _list.push_back(value);
}

void ListStack::pop() {
    _list.pop_back();
}

const ValueType& ListStack::top() const {
    return _list.back();
}

bool ListStack::isEmpty() const {    
    return _list.size() == 0;
}

size_t ListStack::size() const {
    return _list.size();
}