#include "VectorStack.h"


VectorStack::VectorStack(const ValueType* rawArray, const size_t size) 
:_vector(rawArray, size){
}

void VectorStack::push(const ValueType& value) {
    _vector.pushBack(value);
}

void VectorStack::pop() {
    _vector.popBack();
}

const ValueType& VectorStack::top() const {
    return  _vector[size() - 1];
}

bool VectorStack::isEmpty() const {    
    return _vector.size() == 0;
}

size_t VectorStack::size() const {
    return _vector.size();
}