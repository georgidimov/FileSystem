#pragma once
#include <list.hpp>
#include <file.h>

class Directory : public File{
    Directory * parent;
    List<File *> files;
    List<Directory *> directories;

    void clear();
public:
    Directory(Value newName, size_t size, Directory * newParent);
    Directory(Value newName, size_t position, size_t newSize, size_t newSizeInFS, Directory * nParent);
    Directory(Value serializedString);
    ~Directory();

    void addFile(File * file);
    void addDirectory(Directory * dir);

    File * getFile(Value name);
    Directory * getDirectory(Value name);

    void deleteFile(Value name);
    void deleteDirectory(Value name);

    void setParent(Directory * newParent);
    Directory * getParent() const;

    virtual Value serialize() const;
    virtual Value deserialize(Value serialized);

    void printContent() const;
};

