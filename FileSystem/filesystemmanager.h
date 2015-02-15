#pragma once
#include <fstree.h>
#include <filemanager.h>

class FileSystemManager{
    FSTree fstree;
    FileManager fileManager;
public:
    FileSystemManager(std :: fstream & file);
    ~FileSystemManager();

    void createDirectory(Value name, Value path);
    void removeDirectory(Value name, Value path);
    void renameDirectory(Value path, Value name, Value newName);
    void moveDirectory(Value path, Value name, Value newPath);
    void copyDirecoty(Value path, Value name, Value newPath);
    void printDirectoryInfo(Value path) const;

    void createFile(Value path, Value name);
    void removeFile(Value path, Value name);
    void printFileInfo(Value path, Value name) const;

};
