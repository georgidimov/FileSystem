#include "fselement.h"

///fix other instance variables
FSElement :: FSElement(Value newName, size_t position){
    name = newName;
    positionInFile = position;
    size = 0;

    time(&creationTime);
    time(&lastModifiedTime);
}

FSElement :: ~FSElement(){
    ;
}

Value FSElement :: getName() const{
    return name;
}

Value FSElement :: getCreationTime() const{
    return Value( ctime(&creationTime) );
}

Value FSElement :: getLastModifiedTime() const{
    return Value( ctime(&lastModifiedTime) );
}

size_t FSElement :: getSize() const{
    return size;
}

size_t FSElement :: getSizeInFileSystem() const{
    return sizeInFileSystem;
}
