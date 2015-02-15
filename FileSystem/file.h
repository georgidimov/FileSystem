#pragma once
#include <ctime>
#include <value.h>

class File{
protected:
     Value name;

     size_t positionInFile;

     size_t size;
     size_t sizeInFileSystem;

     time_t creationTime;
     time_t lastModifiedTime;
public:
     File(Value newName, size_t newSize);
     File(Value newName, size_t newPosition, size_t newSize, size_t newSizeInFS);
     File(Value serialized);

     virtual ~File();

     void setName(Value newName);
     Value getName() const;

     Value getCreationTime() const;
     Value getLastModifiedTime() const;

     void setCreationTime(time_t newCreationTime);
     void setLastModifiedTime(time_t newLastModified);

     size_t getSize() const;
     void setSize(size_t newSize);

     size_t getSizeInFileSystem() const;
     void setSizeInFS(size_t newSize);

     File * getCopy() const;

     size_t getPosition() const;
     void setPosition(size_t newPosition);


     virtual Value serialize() const;
     virtual Value deserialize(Value serialized);

     void printContent() const;
 };

