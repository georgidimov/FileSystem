#pragma once
#include <iostream>
#include <string.h>

class Value{    ///Fix class name
    char * value;
    void clear();
public:
    Value();
    Value(char * passedValue);
    Value(char * passedValue, size_t end);
    Value(const Value & o);
    Value & operator=(const Value & o);
    ~Value();

    bool operator == (const Value & v) const;
    bool operator !=(const Value & v) const;

    friend std :: ostream & operator << (std :: ostream & out, const Value & v);


    char * getValue() const;
};

