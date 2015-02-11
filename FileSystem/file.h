#pragma once
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
     File(Value newName, size_t position, size_t newSize, size_t newSizeInFS);
     ~File();

     Value getName() const;

     Value getCreationTime() const;
     Value getLastModifiedTime() const;

     size_t getSize() const;
     size_t getSizeInFileSystem() const;
 };

