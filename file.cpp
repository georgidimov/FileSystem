#include "file.h"

File :: File(Value fileName){
    name = fileName;
}

File :: File(const char * fileName){
    name = Value(fileName);
}

File :: File(const File & f){
    ///add implementation
}

File & File :: operator =(const File & f){
    ///add implementation
}

File :: ~File(){
    ;
}

Value File :: getName() const{
    return name;
}
