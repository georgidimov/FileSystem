#pragma once
#include <list.hpp>
#include <file.h>

class Directory : public File{
    List<File *> files;
    List<Directory *> directies;
public:
    Directory(Value newName, size_t size);
    Directory(Value newName, size_t position, size_t newSize, size_t newSizeInFS);

    ~Directory();

    void addFile(File * file);
    void addDirectory(Directory * dir);

    File * getFile(Value name);

};

