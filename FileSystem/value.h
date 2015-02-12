#pragma once
#include <iostream>
#include <math.h>
#include <string.h>

class Value{    ///Fix class name
    char * value;
    void clear();
public:
    Value();
    Value(const char * passedValue);
    Value(const char * passedValue, size_t end);
    Value(Value passedValue, size_t end);
    Value(Value passedValue, size_t start, size_t end);
    Value(size_t passedValue);
    Value(const Value & o);
    Value & operator=(const Value & o);
    ~Value();

    bool operator == (const Value & v) const;
    bool operator !=(const Value & v) const;
    Value operator +(const Value & v) const;
    char operator [](size_t position);
    size_t toNumber() const;

    friend std :: ostream & operator << (std :: ostream & out, const Value & v);

    char * getValue() const;
    int find(char delimiter, size_t pos = 0) const;
    size_t length() const;
};

