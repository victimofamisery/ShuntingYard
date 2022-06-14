#pragma once

#include "StackImplementation.h"
#include "Vector.h"

class VectorStack : public IStackImplementation {
public:
    VectorStack() = default;
    VectorStack(const ValueType* rawArray, const size_t size);

    void push(const ValueType& value) override;

    void pop() override;

    const ValueType& top() const override;

    bool isEmpty() const override;

    size_t size() const override;
private:
    Vector _vector;
};
