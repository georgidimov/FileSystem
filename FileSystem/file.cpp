#include "file.h"

File :: File(Value newName, size_t newSize){
    name = newName;
    positionInFile = 0;
    size = newSize;
    sizeInFileSystem = 0;

    time(&creationTime);
    time(&lastModifiedTime);
}

File :: File(Value newName, std::streampos position, size_t newSize, size_t newSizeInFS){
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

void File :: setName(Value newName){
    name = newName;
}

Value File :: getName() const{
    return name;
}

std::streampos File::getPosition() const{
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
    //concatenate all variables
    Value result = name + ":" + Value(positionInFile) + ":" + Value(size)+ ":" + Value(sizeInFileSystem);

    result = result + ":" + Value(creationTime) + ":" + Value(lastModifiedTime);

    return Value(":") + Value(result.length() + 1) + ":" + result;
}

Value File :: deserialize(Value serialized){
    size_t length = serialized.length();

    //skip ':'
    size_t i = 1;
    for(; i < length; ++i){
        if(serialized[i] < '0' || serialized[i] > '9'){
            break;
        }
    }
    //skip ':'
    size_t serializedStringLength = Value(serialized, 1, i).toNumber();
    Value restOfTheString = Value(serialized, serializedStringLength + i, length);

    serialized = Value(serialized, i + 1, serializedStringLength + i + 1);

    size_t delimiter = serialized.find(':');


    //jump to positins of ':' in serilized string
    //and load value for every variable

    name = Value(serialized.getValue(), delimiter);
    i = delimiter + 1;
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

    lastModifiedTime = Value(serialized.getValue() + i, serializedStringLength - i - 1).toNumber();
    return restOfTheString;
}

void File :: printContent() const{
    std :: cout << "name:          " << name << "\n"
                << "size:          " << size << "\n"
                << "real size:     " << sizeInFileSystem << "\n"
                << "created:       " << getCreationTime()
                << "last modified: " << getLastModifiedTime();
}
