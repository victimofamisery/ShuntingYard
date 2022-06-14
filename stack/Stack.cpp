#include "Stack.h"


Stack::Stack(StackContainer container)
    : _containerType(container)
{
    switch (container)
    {
    case StackContainer::List: {
        _pimpl = static_cast<IStackImplementation*>(new ListStack());
        break;
    }
    case StackContainer::Vector: {
        _pimpl = static_cast<IStackImplementation*>(new VectorStack()); 
        break;
    }
    default:
        throw std::runtime_error("Неизвестный тип контейнера");
    }
}

Stack::Stack(const ValueType* valueArray, const size_t arraySize, StackContainer container)
    : _containerType(container)
{
    switch (container)
    {
    case StackContainer::List: {
        _pimpl = static_cast<IStackImplementation*>(new ListStack(valueArray, arraySize));  
        break;
    }
    case StackContainer::Vector: {
        _pimpl = static_cast<IStackImplementation*>(new VectorStack(valueArray, arraySize)); 
        break;
    }
    default:
        throw std::runtime_error("Неизвестный тип контейнера");
    }
}

Stack::Stack(const Stack& copyStack)
    : Stack(copyStack._containerType)
{
    switch (copyStack._containerType)
    {
    case StackContainer::List: {
        _pimpl = static_cast<IStackImplementation*>(new ListStack(*dynamic_cast<ListStack*>(copyStack._pimpl)));
        break;
    }
    case StackContainer::Vector: {
        _pimpl = static_cast<IStackImplementation*>(new VectorStack(*dynamic_cast<VectorStack*>(copyStack._pimpl)));
        break;
    }
    default:
        throw std::runtime_error("Неизвестный тип контейнера");
    }	 
}


Stack& Stack::operator=(const Stack& copyStack)
{
    if(&copyStack == this) {
        return *this;
    }
    delete _pimpl;
    Stack tmp(copyStack);
    _pimpl = tmp._pimpl;
    tmp._pimpl = nullptr;
    _containerType = tmp._containerType;
    return *this;
}

Stack::Stack(Stack&& moveStack) noexcept {
    _pimpl = moveStack._pimpl;
    moveStack._pimpl = nullptr;
    _containerType = moveStack._containerType;
    moveStack._containerType = StackContainer::Undefined;
}

Stack& Stack::operator=(Stack&& moveStack) noexcept {
    if(&moveStack == this) {
        return *this;
    }
	delete[] _pimpl;
    _pimpl = moveStack._pimpl;
    moveStack._pimpl = nullptr;
    _containerType = moveStack._containerType;
    moveStack._containerType = StackContainer::Undefined;
    return *this;
}

Stack::~Stack()
{
    delete _pimpl;
}

void Stack::push(const ValueType& value)
{
    _pimpl->push(value);
}

void Stack::pop()
{
    _pimpl->pop();
}

const ValueType& Stack::top() const
{
    return _pimpl->top();
}

bool Stack::isEmpty() const
{
    return _pimpl->isEmpty();
}

size_t Stack::size() const
{
    return _pimpl->size();
}
