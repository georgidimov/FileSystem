#pragma once
#include <fstream>
#include <ctime>
#include <value.h>

class File{
    Value name;

    size_t positionInFile;

    size_t size;
    size_t sizeInFileSystem;

    time_t creationTime;
    time_t lastModifiedTime;
public:
    File(Value fileName, size_t position);
    File(const File & f);
    File & operator =(const File & f);
    ~File();

    Value getName() const;
    Value getContent() const;

    Value getCreationTime() const;
    Value getLastModifiedTime() const;

    size_t getSize() const;
    size_t getSizeInFileSystem() const;

    void setContent(Value newContent);
    void append(Value data);
};
