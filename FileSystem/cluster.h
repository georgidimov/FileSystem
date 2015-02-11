#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <value.h>
#include <string.h>

class Cluster{
    size_t size;
    size_t prevClusterPosition;
    size_t nextClusterPosition;

    Value data;
    size_t dataSize;

    void isValidPositionInFile(std :: fstream & file, size_t position) const;
public:
    Cluster(size_t clusterSize);
    ~Cluster();

    void loadFromFile(std :: fstream & file, size_t position);
    size_t writeToFile(std :: fstream & file, size_t position) const;

    Value getData() const;
    size_t getDataSize() const;
    size_t getPrev() const;
    size_t getNext() const;

    void setData(Value newData);
    void setDataSize(size_t newDataSize);
    void setPrev(size_t newPrev);
    void setNext(size_t newNext);

    bool isValidCluster(std :: fstream & file, size_t position) const;
    void markAsInvalid(std :: fstream & file, size_t position) const;

    bool isLastInSequence() const;
    bool isFirstInSequence() const;

    bool isLastInSequence(std :: fstream & file, size_t position) const;
    bool isFirstInSequence(std :: fstream & file, size_t position) const;
};

