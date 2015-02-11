#include "file.h"

File :: File(Value newName, size_t position, size_t newSize, size_t newSizeInFS){
    name = newName;
    positionInFile = position;
    size = newSize;
    sizeInFileSystem = newSizeInFS;

    time(&creationTime);
    time(&lastModifiedTime);
}

File :: File(Value serialized){
    deserialize(serialized);
}

File :: ~File(){
    ;
}

Value File :: getName() const{
    return name;
}

size_t File :: getPosition() const{
    return positionInFile;
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

Value File :: serialize() const{
    Value result = name + ":" + Value(positionInFile) + ":" + Value(size)+ ":" + Value(sizeInFileSystem);

    return result + ":" + Value(creationTime) + ":" + Value(lastModifiedTime);
}

void File :: deserialize(Value serialized){
    size_t delimiter = serialized.find(':');


    name = Value(serialized.getValue(), delimiter);
    size_t i = delimiter + 1;
    delimiter = serialized.find(':', delimiter + 1);

    positionInFile = Value(serialized.getValue() + i, delimiter - i).toNumber();
    i = delimiter + 1;
    delimiter = serialized.find(':', delimiter + 1);

    size = Value(serialized.getValue() + i, delimiter - i).toNumber();
    i = delimiter + 1;
    delimiter = serialized.find(':', delimiter + 1);

    sizeInFileSystem = Value(serialized.getValue() + i, delimiter - i).toNumber();
    i = delimiter + 1;
    delimiter = serialized.find(':', delimiter + 1);

    creationTime = Value(serialized.getValue() + i, delimiter - i).toNumber();
    i = delimiter + 1;
    delimiter = serialized.find(':', delimiter + 1);

    lastModifiedTime = Value(serialized.getValue() + i).toNumber();

    Value result = name + ":" + Value(positionInFile) + ":" + Value(size)+ ":" + Value(sizeInFileSystem);

    result = result + ":" + Value(creationTime) + ":" + Value(lastModifiedTime);
}
