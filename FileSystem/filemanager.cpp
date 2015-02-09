#include "filemanager.h"

FileManager :: FileManager(std :: fstream & file, size_t fileBeginning) : sourceFile(file){


    if(!sourceFile.is_open()){
        throw std :: runtime_error("wrong sourcefile path");
    }

    clusterSize = 4;
    firstPositionInFile = fileBeginning;
}

FileManager :: ~FileManager(){
    ;
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
    sourceFile.seekg(0, sourceFile.end);
    size_t lastPositionInFile = sourceFile.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }

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
    sourceFile.seekg(0, sourceFile.end);
    size_t lastPositionInFile = sourceFile.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }

    Cluster currentCluster(clusterSize);
    currentCluster.loadFromFile(sourceFile, position);

    if(currentCluster.getNext() == newPosition){
        throw std :: runtime_error("invalid position - loop in clusters sequence");
    }

    if(currentCluster.getPrev() == newPosition){
        throw std :: runtime_error("invalid position - breaking clusters sequence");
    }

    currentCluster.writeToFile(sourceFile, newPosition);

    size_t previousClusterPosition = currentCluster.getPrev();
    currentCluster.loadFromFile(sourceFile, previousClusterPosition);

    currentCluster.setNext(newPosition);
    currentCluster.writeToFile(sourceFile, previousClusterPosition);
}

void FileManager :: remove(size_t position) const{
    sourceFile.seekg(0, sourceFile.end);
    size_t lastPositionInFile = sourceFile.tellg();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }
}
