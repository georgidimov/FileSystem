#include "cluster.h"

Cluster :: Cluster(size_t clusterSize){
    size = clusterSize;
    prevClusterPosition = 0;
    nextClusterPosition = 0;

    dataSize = 0;
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

Value Cluster :: getData() const{
    return data;
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

void Cluster :: setPrev(size_t newPrev){
    prevClusterPosition = newPrev;
}

void Cluster :: setNext(size_t newNext){
    nextClusterPosition = newNext;
}
