#include "cluster.h"

Cluster :: Cluster(size_t clusterSize){
    dataSize = 0;
    prevClusterPosition = 0;
    nextClusterPosition = 0;
    size = clusterSize;
}

Cluster :: ~Cluster(){
    ;
}

void Cluster :: loadFromFile(std::fstream & file, size_t position){
    file.seekg(0, file.end);
    size_t lastPositionInFile = file.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }

    //send file pointer to position for reading
    file.seekg(position);

    char * dataFromFile = new char[size];

    //read data
    file.read((char *) & prevClusterPosition, sizeof(size_t));
    file.read((char *) & nextClusterPosition, sizeof(size_t));
    file.read((char *) & dataSize, sizeof(size_t));
    file.read(dataFromFile, dataSize * sizeof(char));

    data = Value(dataFromFile, dataSize);

    delete [] dataFromFile;
    dataFromFile = NULL;
}

void Cluster :: writeToFile(std::fstream & file, size_t position) const{
    file.seekg(0, file.end);
    size_t lastPositionInFile = file.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }

    //send file pointer to position for reading
    file.seekg(position);

    file.write((char *) & prevClusterPosition, sizeof(size_t));
    file.write((char *) & nextClusterPosition, sizeof(size_t));

    file.write((char *) & dataSize, sizeof(size_t));
    file.write(data.getValue(), sizeof(char) * dataSize);
}

Value Cluster :: getData() const{
    return data;
}

size_t Cluster :: getDataSize() const{
    return dataSize;
}

size_t Cluster :: getPrev() const{
    return prevClusterPosition;
}

size_t Cluster :: getNext() const{
    return nextClusterPosition;
}

void Cluster :: setData(Value newData){
    data = newData;
}

void Cluster :: setDataSize(size_t newDataSize){
    dataSize = newDataSize;
}

void Cluster :: setPrev(size_t newPrev){
    prevClusterPosition = newPrev;
}

void Cluster :: setNext(size_t newNext){
    nextClusterPosition = newNext;
}
