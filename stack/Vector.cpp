#include "VectorStack.h"

//Vector


Vector::Vector(const Value* rawArray, const size_t size, float coef)
:_size(size), _multiplicativeCoef(coef) {
	_reserveBackend(_size);
	for(auto i = 0; i < _size; i++) {
		_data[i] = rawArray[i];
	}
}



Vector::Vector(const Vector& other) {
	_size = other._size;
	_capacity = other._size;
	_multiplicativeCoef = other._multiplicativeCoef;
	_data = new Value[_capacity];
	for(auto i = 0; i < _size; i++) {
		_data[i] = other._data[i];
	}
}

Vector& Vector::operator=(const Vector& other) {
	if(&other == this) {
		return *this;
	}
	delete[] _data;
	_size = other._size;
	_capacity = other._size;
	_multiplicativeCoef = other._multiplicativeCoef;
	_data = new Value[_capacity];
	for(auto i = 0; i < _size; i++) {
		_data[i] = other._data[i]; 
	}
	return *this;
}

Vector::Vector(Vector&& other) noexcept {
	_size = other._size;
	_capacity = other._size;
	_multiplicativeCoef = other._multiplicativeCoef;
	_data = other._data;
	other._data = nullptr;
	other._size = 0;
	other._capacity = 0;
}

Vector& Vector::operator=(Vector&& other) noexcept {
	if(&other == this) {
		return *this;
	}
	delete[] _data;
	_size = other._size;
	_capacity = other._size;
	_multiplicativeCoef = other._multiplicativeCoef;
	_data = other._data;
	other._data = nullptr;
	other._size = 0;
	other._capacity = 0;	
	return *this;	
}


Vector::~Vector() {
	delete[] _data;
}

void Vector::pushBack(const Value& value) {
	insert(value, _size);
}

void Vector::pushFront(const Value& value) {
	insert(value, 0);
}

void Vector::insert(const Value& value, size_t pos) {
	insert(&value, 1, pos);
}

void Vector::insert(const Value* values, size_t size, size_t pos) {
	if(pos > _size) {
		throw std::out_of_range("pos > _size");
		return;
	}
	_reserveBackend(_size + size);
	_size += size;
	for(auto i = _size - 1; i > pos; i--) {
		_data[i] = std::move(_data[i - size]);
	}
	for(auto i = 0; i < size; i++) {
		_data[pos + i] = values[i];
	}	
}

void Vector::insert(const Vector& vector, size_t pos) {
	insert(vector._data, vector._size, pos);
}


void Vector::popFront() {
	erase(0);
}

void Vector::popBack() {
	if(_size > 0) {
		_size--;
	}
	else {
		throw std::out_of_range("size = 0");
	}
}

void Vector::erase(size_t pos, size_t count) {
	if(pos >= _size) {
		throw std::out_of_range("pos >= _size");
		return;
	}
	if(pos + count > _size) {
		count = _size - pos;
	}
	_size -= count;
	for(auto i = pos; i < _size; i++) {
		_data[i] = std::move(_data[i + count]);
	}
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
	if(beginPos >= endPos) {
		throw std::out_of_range("beginPos >= endPos");
		return;
	}
	erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const {
	return _size;
}

size_t Vector::capacity() const {
	return _capacity;
}

double Vector::loadFactor() const {
	return  double(_size) / double(_capacity);
}


Value& Vector::operator[](size_t idx) {
	return _data[idx];
}


const Value& Vector::operator[](size_t idx) const {
	return _data[idx];
}


long long Vector::find(const Value& value) const {
	for(auto i = 0; i < _size; i++) {
		if(value == _data[i]) {
			return i;
		}
	}
	return -1;
}

void Vector::reserve(size_t capacity) {
	if(capacity > _capacity) {
		_capacity = capacity;
		_realloc();
	}
}


void Vector::_reserveBackend(size_t capacity) {
	if(capacity > _capacity) {
		if(_capacity == 0) {
			_capacity = 1 * _multiplicativeCoef;
		}
		while(capacity > _capacity) {
			_capacity *= _multiplicativeCoef;
		}
		_realloc();
	}
}

void Vector::shrinkToFit() {
	_capacity = _size;
	_realloc();
}

void Vector::_realloc() {
	if(_capacity == 0) {
		_data = nullptr;
	}
	if(_data == nullptr) {
		_data = new Value[_capacity];
		return;
	}
	Value* newData = new Value[_capacity];
	for(auto i = 0; i < _size; i++) {
		newData[i] = _data[i];
	}
	delete[] _data;
	_data = newData;
}

Vector::Iterator Vector::begin() const {
	return Vector::Iterator(&_data[0]);
}

Vector::Iterator Vector::end() const {
	return Vector::Iterator(&_data[_size]);
}

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
	for(auto&& elem : vector) {
		out << elem << ' ';
	}
	return out;
}

//Iterator

Vector::Iterator::Iterator(Value* ptr) : _ptr(ptr) {
}

Value& Vector::Iterator::operator*() {
	return *_ptr;
}

const Value& Vector::Iterator::operator*() const {
	return *_ptr;
}

Value* Vector::Iterator::operator->() {
	return _ptr;
}

const Value* Vector::Iterator::operator->() const {
	return _ptr;
}



Vector::Iterator Vector::Iterator::operator+(int x) {
	Vector::Iterator bufIt(_ptr + x); 
	return bufIt;
}

void Vector::Iterator::operator+=(int x) {
	*this = *this + x;
}

Vector::Iterator Vector::Iterator::operator-(int x) {
	Vector::Iterator bufIt(_ptr - x); 
	return bufIt;
}

void Vector::Iterator::operator-=(int x) {
	*this = *this - x;
}

Vector::Iterator Vector::Iterator::operator++() {
	++_ptr;
	return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
	Vector::Iterator bufIt = *this;
	++*this;
	return bufIt;
}

Vector::Iterator Vector::Iterator::operator--() {
	--_ptr;
	return *this;
}

Vector::Iterator Vector::Iterator::operator--(int) {
	Vector::Iterator bufIt = *this;
	--*this;
	return bufIt;
}

bool Vector::Iterator::operator==(const Vector::Iterator& other) const {
	return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Vector::Iterator& other) const {
	return !(*this == other);
}
