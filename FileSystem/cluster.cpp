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


void Cluster :: isValidPositionInFile(std::fstream & file, std::streampos position) const{
    file.seekg(0, file.end);
    std :: streampos lastPositionInFile = file.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }
}

void Cluster :: loadFromFile(std::fstream & file, std::streampos position){
    isValidPositionInFile(file, position);

    //send file pointer to position for reading
    file.seekg(position);

    char * dataFromFile = new char[size];

    int tempDataSize;
    //read data
    file.read((char *) & prevClusterPosition, sizeof(size_t));
    file.read((char *) & nextClusterPosition, sizeof(size_t));
    file.read((char *) & tempDataSize, sizeof(int));

    if(tempDataSize != -1){  //cluster is valid
        dataSize = tempDataSize;
    }else {
        dataSize = 0;
    }

    file.read(dataFromFile, dataSize * sizeof(char));

    data = Value(dataFromFile, dataSize);

    delete [] dataFromFile;
    dataFromFile = NULL;
}

size_t Cluster::writeToFile(std::fstream & file, std::streampos position) const{

    isValidPositionInFile(file, position);

    file.seekg(0, file.end);
    size_t lastPositionInFile = file.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }

    //send file pointer to position for writing
    file.seekg(position);

    file.write((char *) & prevClusterPosition, sizeof(size_t));
    file.write((char *) & nextClusterPosition, sizeof(size_t));

    file.write((char *) & dataSize, sizeof(size_t));
    file.write(data.getValue(), sizeof(char) * dataSize);

    return file.tellg();
}

Value Cluster :: getData() const{
    return data;
}

std :: streampos Cluster :: getDataSize() const{
    return dataSize;
}

std :: streampos Cluster :: getPrev() const{
    return prevClusterPosition;
}

std :: streampos Cluster :: getNext() const{
    return nextClusterPosition;
}

void Cluster :: setData(Value newData){
    data = newData;
}

void Cluster :: setDataSize(size_t newDataSize){
    dataSize = newDataSize;
}

void Cluster :: setPrev(std :: streampos newPrev){
    prevClusterPosition = newPrev;
}

void Cluster :: setNext(std :: streampos newNext){
    nextClusterPosition = newNext;
}


bool Cluster :: isValidCluster(std::fstream & file, std::streampos position) const{
    isValidPositionInFile(file, position);

    //set pointer to rigth place
    file.seekg(position);

    //read previous and next cluster position
    size_t uselessVariable;
    file.read((char *) & uselessVariable, sizeof(size_t));
    file.read((char *) & uselessVariable, sizeof(size_t));

    int size;
    file.read((char *) & size, sizeof(int));

    if(size == -1){
        return false;
    }

    return true;
}

void Cluster :: markAsInvalid(std::fstream & file, std::streampos position) const{
    isValidPositionInFile(file, position);

    //skip prev and next pointer
    file.seekg(position + (std :: streampos)(2 * sizeof(size_t)));
    int uselessVar = -1;

    file.write((char *) & uselessVar, sizeof(int));
}

bool Cluster :: isLastInSequence() const{
    return !nextClusterPosition;
}

bool Cluster :: isFirstInSequence() const{
    return prevClusterPosition == 0;
}

bool Cluster :: isLastInSequence(std::fstream & file, std::streampos position) const{
    isValidPositionInFile(file, position);

    //skip prev pointer
    file.seekg(position);

    size_t next;

    file.read((char *) & next, sizeof(size_t));

    return !next;
}

bool Cluster :: isFirstInSequence(std::fstream & file, std::streampos position) const{
    isValidPositionInFile(file, position);

    file.seekg(position);
    size_t prev;
    file.read((char *) & prev, sizeof(size_t));

    return prev == 0 && isValidCluster(file, position);
}
