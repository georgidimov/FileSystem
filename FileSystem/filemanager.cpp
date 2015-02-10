#include "filemanager.h"

FileManager :: FileManager(std :: fstream & file, size_t fileBeginning) : sourceFile(file){


    if(!sourceFile.is_open()){
        throw std :: runtime_error("wrong sourcefile path");
    }

    clusterSize = 4;
    clusterSizeInFS = 3 * sizeof(size_t) + clusterSize;

    firstPositionInFile = fileBeginning;
}

FileManager :: ~FileManager(){
    ;
}


void FileManager :: isValidPositionInFile(size_t position) const{
    sourceFile.seekg(0, sourceFile.end);
    size_t lastPositionInFile = sourceFile.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }
}
size_t FileManager :: write(const char * data, size_t size) const{
    //define count of clusters needed for saving data
    size_t neededClusters = ceil( (double)size / clusterSize );

    sourceFile.seekg(0, sourceFile.end);
    size_t positionOfFirstCluster = sourceFile.tellg();

    size_t tempSize = size;
    size_t tempClusterSize = clusterSize;
    size_t clusterRealSize = sizeof(size_t) * 3 + tempClusterSize;
    size_t prev = 0;
    size_t next = positionOfFirstCluster + clusterRealSize;

    Cluster tempCluster(clusterSize);
    //for every cluster write position of previous, next,
    //size of storing data and data
    for(size_t i = 0; i < neededClusters; ++i){
        //define cluster size
        if(tempSize > clusterSize){
            tempClusterSize = clusterSize;
        }else{
            tempClusterSize = tempSize;
        }

        //stop cluster sequence in last cluster
        if(i == neededClusters - 1){
            next = 0;
        }

        tempCluster.setData(data + i * clusterSize);
        tempCluster.setDataSize(tempClusterSize);
        tempCluster.setPrev(prev);
        tempCluster.setNext(next);

        tempCluster.writeToFile(sourceFile, sourceFile.tellg());

        prev = next - clusterRealSize;
        next += clusterRealSize;
        tempSize -= tempClusterSize;
    }

    //add zeros to the end of the cluster
    sourceFile.write((char *) &tempSize, (clusterSize - tempClusterSize) * sizeof(char));

    return positionOfFirstCluster;
}

Value FileManager :: read(size_t position) const{
    isValidPositionInFile(position);

    //send file pointer to position for reading
    sourceFile.seekg(position);

    Value resultData = "";

    Cluster tempCluster(clusterSize);
    //read every cluster and append data in resultData
    do{
        tempCluster.loadFromFile(sourceFile, position);
        resultData = resultData + tempCluster.getData();
        position = tempCluster.getNext();

    }while(position > 0);

    return resultData + Value("\0");
}

void FileManager :: replaceCluster(size_t position, size_t newPosition) const{
    isValidPositionInFile(position);

    Cluster currentCluster(clusterSize);
    currentCluster.loadFromFile(sourceFile, position);

    if(currentCluster.getNext() == newPosition){
        throw std :: runtime_error("invalid position - loop in clusters sequence");
    }

    if(currentCluster.getPrev() == newPosition){
        throw std :: runtime_error("invalid position - breaking clusters sequence");
    }

    //write cluster to new place and fix next position in
    //it`s previous cluster
    currentCluster.writeToFile(sourceFile, newPosition);

    size_t previousClusterPosition = currentCluster.getPrev();
    currentCluster.loadFromFile(sourceFile, previousClusterPosition);

    currentCluster.setNext(newPosition);
    currentCluster.writeToFile(sourceFile, previousClusterPosition);
}

void FileManager :: remove(size_t position) const{
    isValidPositionInFile(position);


    Cluster tempCluster(clusterSize);
    tempCluster.loadFromFile(sourceFile, position);

    if(!tempCluster.isValidCluster(sourceFile, position)){
        throw std :: runtime_error("try to remove wrong cluster");
    }

    PriorityQueue<size_t> positionsOfClustersToRemove;
    positionsOfClustersToRemove.enqueue(position);

    //closest position to the end of the file
    size_t maxPosition = position;
    //closest position to the beginning of the file
    size_t minPosition = position;

    while (tempCluster.getNext() != 0) {
        position = tempCluster.getNext();

        tempCluster.loadFromFile(sourceFile, position);
        positionsOfClustersToRemove.enqueue(position);

        maxPosition = position > maxPosition ? position : maxPosition;
        minPosition = position < minPosition ? position : minPosition;
    }

    //go to the last cluster in the file
    sourceFile.seekg(0, sourceFile.end);

    size_t positionOfLastCluster = sourceFile.tellg() - clusterSizeInFS;
    tempCluster.loadFromFile(sourceFile, positionOfLastCluster);

    size_t currentPosition = positionsOfClustersToRemove.dequeue();
    tempCluster.loadFromFile(sourceFile, currentPosition);

    while(!positionsOfClustersToRemove.isEmpty() && positionOfLastCluster > maxPosition){
        replaceCluster(positionOfLastCluster, currentPosition);

        tempCluster.loadFromFile(sourceFile, positionOfLastCluster);
        positionOfLastCluster = tempCluster.getPrev();

        currentPosition = positionsOfClustersToRemove.dequeue();
        std :: cout << "cala";
    }


}
