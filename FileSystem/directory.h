#pragma once
#include <list.hpp>
#include <file.h>

class Directory : public File{
    List<File *> files;
    List<Directory *> directories;
public:
    Directory(Value newName, size_t position, size_t newSize, size_t newSizeInFS);
};


