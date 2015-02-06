#include "file.h"

///fix other instance variables
File :: File(Value fileName){
    name = fileName;
    size = 0;

    time(&creationTime);
    time(&lastModifiedTime);
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

Value File :: getContent() const{
    return content;
}

Value File :: getCreationTime() const{
    return Value( ctime(&creationTime) );
}

Value File :: getLastModifiedTime() const{
    return Value( ctime(&lastModifiedTime) );
}

size_t File :: getSize() const{
    return size;
}

size_t File :: getSizeInFileSystem() const{
    return sizeInFileSystem;
}
