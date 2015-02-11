#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <value.h>
#include <string.h>

class Cluster{
    size_t size;
    std :: streampos prevClusterPosition;
    std :: streampos nextClusterPosition;

    Value data;
    size_t dataSize;

    void isValidPositionInFile(std :: fstream & file, std :: streampos position) const;
public:
    Cluster(size_t clusterSize);
    ~Cluster();

    void loadFromFile(std :: fstream & file, std :: streampos position);
    size_t writeToFile(std :: fstream & file, std :: streampos position) const;

    Value getData() const;
    std :: streampos getDataSize() const;
    std :: streampos getPrev() const;
    std :: streampos getNext() const;

    void setData(Value newData);
    void setDataSize(size_t newDataSize);
    void setPrev(std :: streampos newPrev);
    void setNext(std :: streampos newNext);

    bool isValidCluster(std :: fstream & file, std :: streampos position) const;
    void markAsInvalid(std :: fstream & file, std :: streampos position) const;

    bool isLastInSequence() const;
    bool isFirstInSequence() const;

    bool isLastInSequence(std :: fstream & file, std :: streampos position) const;
    bool isFirstInSequence(std :: fstream & file, std :: streampos position) const;
};

