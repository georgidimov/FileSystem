#pragma once
#include <stdexcept>
#include <directory.h>

class FSTree{
    Directory * root;
public:
    Directory * getDirectory(Value path, Directory * currentDir) const;
    Directory * getDirectory(Value path) const;
public:
    FSTree();
    ~FSTree();

    Value getSerializedTree() const;
    void deserialize(Value serialized);

    void addDirectory(Value path, Value name);
    void removeDirectory(Value path, Value name);
    void renameDirectory(Value path, Value name, Value newName);
    void moveDirectory(Value path, Value name, Value newPath);
    void copyDirectory(Value path, Value name, Value newPath);
    void printDirectoryInfo(Value path) const;

    void createFile(Value path, Value name, size_t position);
    void removeFile(Value path, Value name);
    void renameFile(Value path, Value name, Value newName);
    void moveFile(Value path, Value name, Value newPath);
    void copyFile(Value path, Value name, Value newPath);
    size_t getFilePosition(Value path, Value name) const;
    void setFilePosition(Value path, Value name, size_t position);
    void printFileInfo(Value path, Value name) const;
    Value getSerializedFile(Value path, Value name) const;

};


