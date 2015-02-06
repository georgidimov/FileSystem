#pragma once
#include <value.h>

class File{
    Value name;
    Value content;

    size_t size;
    size_t sizeInFileSystem;

    time_t creationTime;
    time_t lastModified;
public:
    File(Value fileName);
    File(const char * fileName);
    File(const File & f);
    File & operator =(const File & f);
    ~File();

    Value getName() const;
};

