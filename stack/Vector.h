#pragma once

#include <iostream>

using Value = double;

class Vector
{
public:
    class Iterator
    {
        Value* _ptr;
    public:
        explicit Iterator(Value* ptr);

        Value& operator*();


        const Value& operator*() const;


        Value* operator->();


        const Value* operator->() const;

        Iterator operator+(int x);

        void operator+=(int x);

        Iterator operator-(int x);

        void operator-=(int x);

        Iterator operator++();


        Iterator operator++(int);

        Iterator operator--();

        Iterator operator--(int);

        bool operator==(const Iterator& other) const;

        bool operator!=(const Iterator& other) const;

    };

    Vector() = default;
    Vector(const Value* rawArray, const size_t size, float coef = 2.0f);
    explicit Vector(const Vector& other);
    Vector& operator=(const Vector& other);

    explicit Vector(Vector&& other) noexcept;
    Vector& operator=(Vector&& other) noexcept;

    ~Vector();


    void pushBack(const Value& value);


    void pushFront(const Value& value);


    void insert(const Value& value, size_t pos);
    void insert(const Value* values, size_t size, size_t pos);
    void insert(const Vector& vector, size_t pos);


    void popBack();


    void popFront();


    void erase(size_t pos, size_t count = 1);


    void eraseBetween(size_t beginPos, size_t endPos);


    size_t size() const;


    size_t capacity() const;


    double loadFactor() const;


    Value& operator[](size_t idx);
    const Value& operator[](size_t idx) const;


    long long find(const Value& value) const;
   

    void reserve(size_t capacity);


    void shrinkToFit();

    Iterator begin() const;
    Iterator end() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector& vector);

    
private:
    Value* _data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;
    float _multiplicativeCoef = 2.0f;

    void _realloc();
    void _reserveBackend(size_t capacity);
};