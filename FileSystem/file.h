#pragma once
#include <ctime>
#include <value.h>

class File{
protected:
     Value name;

     std :: streampos positionInFile;

     size_t size;
     size_t sizeInFileSystem;

     time_t creationTime;
     time_t lastModifiedTime;
public:
     File(Value newName, size_t newSize);
     File(Value newName, std :: streampos position, size_t newSize, size_t newSizeInFS);
     File(Value serialized);

     virtual ~File();

     void setName(Value newName);
     Value getName() const;

     std :: streampos getPosition() const;

     Value getCreationTime() const;
     Value getLastModifiedTime() const;

     size_t getSize() const;
     size_t getSizeInFileSystem() const;

     virtual Value serialize() const;
     virtual Value deserialize(Value serialized);

     void printContent() const;
 };

