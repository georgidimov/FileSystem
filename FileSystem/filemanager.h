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

    PriorityQueue<std :: streampos> emptyPositions;

    std :: streampos endOfFile() const;
//remove me
public:
    void replaceCluster(std :: streampos position, std :: streampos newPosition) const;
    void isValidPositionInFile(std :: streampos position) const;
public:
    FileManager(std :: fstream & file, std :: streampos fileBeginning);
    ~FileManager();

    size_t write(const char * data, size_t size);
    Value read(size_t position) const;
    void remove(size_t position);
};

