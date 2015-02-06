#include "file.h"

///fix other instance variables
File :: File(Value fileName){
    name = fileName;
    content = "";
    size = 0;

    time(&creationTime);
    time(&lastModifiedTime);
}

File :: File(const char * fileName){
    name = Value(fileName);
    content = "";
    size = 0;

    time(&creationTime);
    time(&lastModifiedTime);
}

File :: File(const File & f){
    name = f.name;
    content = f.content;
    size = f.size;

    time(&creationTime);
    time(&lastModifiedTime);
}

File & File :: operator =(const File & f){
    name = f.name;
    content = f.content;
    size = f.size;

    creationTime = f.creationTime;
    time(&lastModifiedTime);

    return * this;
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

void File :: setContent(Value newContent){
    content = newContent;
    size = strlen( newContent.getValue() ) * sizeof(char);

    time(&lastModifiedTime);
}

void File :: append(Value data){
    content = content + data;
    size += strlen( data.getValue() ) * sizeof(char);

    time(&lastModifiedTime);
}
