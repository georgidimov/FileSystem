#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <math.h>
#include <value.h>
#include <cluster.h>
#include <priorityqueue.hpp>

class FileManager{
    std :: fstream & sourceFile;
    size_t clusterSize;
    size_t clusterSizeInFS;

    size_t firstPositionInFile;

    PriorityQueue<size_t> emptyPositions;

    size_t endOfFile() const;
//remove me
public:
    void replaceCluster(size_t position, size_t newPosition) const;
    void isValidPositionInFile(size_t position) const;
public:
    FileManager(std :: fstream & file, size_t fileBeginning);
    ~FileManager();

    size_t write(const char * data, size_t size);
    Value read(size_t position) const;
    void remove(size_t position);
};

