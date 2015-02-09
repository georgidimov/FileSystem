#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <math.h>
#include <value.h>
#include <cluster.h>

class FileManager{
    std :: fstream & sourceFile;
    size_t clusterSize;

    size_t firstPositionInFile;
    void replaceCluster(size_t position, size_t newPosition) const;
    bool isValidPositionInFile(size_t position) const;
public:
    FileManager(std :: fstream & file, size_t fileBeginning);
    ~FileManager();

    size_t write(const char * data, size_t size) const;
    Value read(size_t position) const;
    void remove(size_t position) const;
};

