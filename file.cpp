#include "file.h"

///fix other instance variables
File :: File(Value fileName){
    name = fileName;
    size = 0;

    time(&creationTime);
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

Value File :: getCreationTime() const{
    return Value( ctime(&creationTime) );
}
