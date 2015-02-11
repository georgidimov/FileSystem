#pragma once
#include <ctime>
#include <value.h>

class FSElement{
    Value name;

    size_t positionInFile;

    size_t size;
    size_t sizeInFileSystem;

    time_t creationTime;
    time_t lastModifiedTime;
public:
    FSElement(Value newName, size_t position);
    virtual ~FSElement();

    Value getName() const;

    Value getCreationTime() const;
    Value getLastModifiedTime() const;

    size_t getSize() const;
    size_t getSizeInFileSystem() const;

    virtual Value getContent() const = 0;
};
