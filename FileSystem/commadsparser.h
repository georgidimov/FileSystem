#pragma once
#include <filesystemmanager.h>

class CommadsParser{
    FileSystemManager & FSmanager;
    size_t commandsCount;
    char ** commands;
    void clear();
public:
    CommadsParser(FileSystemManager & newFSManager);
    ~CommadsParser();
};
