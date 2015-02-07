#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <math.h>
#include <value.h>

struct Cluster{
    size_t size;
    size_t clustersCount;

    size_t prevClusterPosition;
    size_t nextClusterPosition;
};

class FileManager{
    std :: fstream & sourceFile;
    size_t clusterSize;


    size_t firstPositionInFile;
public:
    FileManager(std :: fstream & file, size_t fileBeginning);
    ~FileManager();

    size_t write(const char * data, size_t size) const;
    Value read(size_t position) const;
};

