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

    size_t treePosition;

    PriorityQueue<size_t> emptyPositions;

    std :: streampos endOfFile() const;


//remove me
public:
    void replaceCluster(std :: streampos position, std :: streampos newPosition) const;
    void isValidPositionInFile(std :: streampos position) const;
    void serialize();
    void deserialize();
public:
    FileManager(std :: fstream & file);
    ~FileManager();

    size_t write(const char * data, size_t size);
    size_t writeTree(const char * data, size_t size);
    Value read(std :: streampos position) const;
    void remove(std :: streampos position);
    size_t getEmptyPosition() const;
    size_t getTreePosition() const;
};

