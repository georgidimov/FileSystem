#include "value.h"

Value :: Value(){
    value = NULL;
}

Value :: Value(const char * passedValue){
    value = new char[strlen(passedValue) + 1];
    strcpy(value, passedValue);
}

Value :: Value(const char * passedValue, size_t end){
    value = new char[end + 1];

    for(size_t i = 0; i < end; ++i){
        value[i] = passedValue[i];
    }

    value[end] = '\0';
}

Value :: Value(Value passedValue, size_t end){
    value = new char[end + 1];
    char * passedValueC = passedValue.getValue();
    for(size_t i = 0; i < end; ++i){
        value[i] = passedValueC[i];
    }

    value[end] = '\0';
}

Value :: Value(Value passedValue, size_t start, size_t end){
    value = new char[end + 1 - start];

    char * passedValueC = passedValue.getValue();

    for(size_t i = 0; i < end - start; ++i){
            value[i] = passedValueC[start + i];
    }

    value[end - start] = '\0';
}

Value :: Value(size_t passedValue){
    size_t digitsCount;

    //invalid values for log10
    if(passedValue == 0 || passedValue == 1){
        digitsCount = 1;
    }else{
        digitsCount = ceil(log10(passedValue));
    }

    value = new char[digitsCount + 1];

    value[digitsCount] = '\0';

    do{
        --digitsCount;
        value[digitsCount] = (char)( (passedValue % 10) + 48);

        passedValue /= 10;
    }while (passedValue > 0) ;

}

Value :: Value(const Value &o){
    //check for self-assignment
    if(&o == this){
        throw "invalid assignment";
    }

    value = new char[strlen(o.value) + 1];
    strcpy(value, o.value);
}

Value & Value :: operator =(const Value & o){

    //remove content and copy the new one
    clear();

    value = new char[strlen(o.value) + 1];
    strcpy(value, o.value);

    return *this;
}

Value :: ~Value(){
    clear();
}

void Value :: clear(){
    delete [] value;
    value = NULL;
}

bool Value :: operator ==(const Value & v) const{
    return !strcmp(value, v.value);
}

bool Value :: operator !=(const Value & v) const{
    return !(*this == v);
}

Value Value :: operator +(const Value & v) const{
    size_t newValueLength = strlen(value);
    newValueLength += strlen(v.getValue());

    char * newValue = new char[newValueLength + 1];
    strcpy(newValue, value);
    strcat(newValue, v.getValue());

    Value concatenatedValue(newValue);

    delete [] newValue;

    return concatenatedValue;
}

char Value :: operator [](size_t position){
    return value[position];
}

size_t Value :: toNumber() const{
    size_t result = 0;
    size_t length = strlen(value);

    for(size_t i = 0; i < length; ++i){
        result *= 10;
        result += (int)value[i] - 48;
    }

    return result;
}

std :: ostream & operator <<(std :: ostream & out, const Value & v){
    out << v.value;

    return out;
}

char * Value :: getValue() const{
    return value;
}

int Value::find(char delimiter, size_t pos) const{
    size_t length = strlen(value);

    for(; pos < length; ++pos){
        if(value[pos] == delimiter){
            return pos;
        }
    }

    return -1;
}

size_t Value :: length() const{
    return strlen(value);
}
