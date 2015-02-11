#include "file.h"

File :: File(Value newName, size_t position, size_t newSize, size_t newSizeInFS){
    name = newName;
    positionInFile = position;
    size = newSize;
    sizeInFileSystem = newSizeInFS;

    time(&creationTime);
    time(&lastModifiedTime);
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

Value File :: getLastModifiedTime() const{
    return Value( ctime(&lastModifiedTime) );
}

size_t File :: getSize() const{
    return size;
}

size_t File :: getSizeInFileSystem() const{
    return sizeInFileSystem;
}
