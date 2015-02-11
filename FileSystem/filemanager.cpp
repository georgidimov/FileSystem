#include "filemanager.h"

FileManager :: FileManager(std :: fstream & file, std::streampos fileBeginning) : sourceFile(file){
    if(!sourceFile.is_open()){
        throw std :: runtime_error("wrong sourcefile path");
    }

    //make zero position unvailable
    if(endOfFile() == 0){
        size_t nil = 0;

        sourceFile.seekg(0);
        sourceFile.write((char *) & nil, sizeof(size_t));
    }

    clusterSize = 4;
    clusterSizeInFS = 3 * sizeof(size_t) + clusterSize;

    firstPositionInFile = fileBeginning;
    /*
    emptyPositions.enqueue(52);
    emptyPositions.enqueue(68);
    emptyPositions.enqueue(84);
    emptyPositions.enqueue(100);
    emptyPositions.enqueue(116);*/
}

FileManager :: ~FileManager(){
    ;
}


void FileManager :: isValidPositionInFile(std::streampos position) const{
    size_t lastPositionInFile = endOfFile();

    if(position > lastPositionInFile){
        throw std :: runtime_error("invalid position in file");
    }
}

std::streampos FileManager::endOfFile() const{
    //save previous position, define end of file and
    //send pointer back to previous position
    std :: streampos previousPosition = sourceFile.tellg();

    sourceFile.seekg(0, sourceFile.end);
    std :: streampos end = sourceFile.tellg();

    sourceFile.seekg(previousPosition);

    return end;
}

size_t FileManager :: write(const char * data, size_t size){
    //define count of clusters needed for saving data
    size_t neededClusters = ceil( (double)size / clusterSize );

    bool clustersInSequence = true;
    size_t positionOfFirstCluster;
    ///FIX ME
    if(emptyPositions.isEmpty()){
        positionOfFirstCluster = endOfFile();
    }else{
        positionOfFirstCluster = emptyPositions.dequeue();
        clustersInSequence = false;
    }


    sourceFile.seekg(positionOfFirstCluster);

    size_t tempSize = size;
    size_t tempClusterSize = clusterSize;
    size_t clusterRealSize = sizeof(size_t) * 3 + tempClusterSize;
    std :: streampos prev = 0;
    std :: streampos next;// = positionOfFirstCluster + clusterRealSize;

    if(clustersInSequence){  //clusters are in sequence
        next = positionOfFirstCluster + clusterRealSize;
    }else{                  //clusters are not in sequence
        if(!emptyPositions.isEmpty()){
            next = emptyPositions.dequeue();
        }else{
            next = endOfFile();
        }
    }

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

        if(clustersInSequence){
            prev = next - (std :: streampos) clusterRealSize;
            next += clusterRealSize;
        }else{
            prev = sourceFile.tellg() - (std :: streampos) clusterSizeInFS;

            if(next == endOfFile()){
                sourceFile.seekg(endOfFile());
                next += clusterRealSize;
                clustersInSequence = true;
            }else{
                sourceFile.seekg(next);

                if(!emptyPositions.isEmpty()){
                    next = emptyPositions.dequeue();
                }else{
                    next = endOfFile();
                    clustersInSequence = true;
                }
            }
        }


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

void FileManager :: replaceCluster(std::streampos position, std::streampos newPosition) const{
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

void FileManager :: remove(size_t position){
    isValidPositionInFile(position);


    Cluster tempCluster(clusterSize);
    tempCluster.loadFromFile(sourceFile, position);
    tempCluster.markAsInvalid(sourceFile, position);

    //if(!tempCluster.isValidCluster(sourceFile, position)){
    //    throw std :: runtime_error("try to remove wrong cluster");
    //}

    if(!tempCluster.isFirstInSequence()){
        throw std :: runtime_error("start removing from not first cluster");
    }

    //closest position to the end of the file
    size_t maxPosition = position;

    //add clusters from the sequence to the queue of "empty" clusters
    while(!tempCluster.isLastInSequence()) {
        emptyPositions.enqueue(position);
        position = tempCluster.getNext();
        tempCluster.loadFromFile(sourceFile, position);
        tempCluster.markAsInvalid(sourceFile, position);
        maxPosition = position > maxPosition ? position : maxPosition;
    }
    //add also the last one
    emptyPositions.enqueue(position);

    //go to the last cluster in the file
    size_t positionOfLastCluster = endOfFile() - (std :: streampos) clusterSizeInFS;
    tempCluster.loadFromFile(sourceFile, positionOfLastCluster);
    sourceFile.seekg(positionOfLastCluster);

    size_t newPositionForTheCluster = emptyPositions.dequeue();
    size_t oldPositionOfTheCluster = positionOfLastCluster;

    while(oldPositionOfTheCluster > maxPosition && !emptyPositions.isEmpty()){

        if(!tempCluster.isFirstInSequence()){
            replaceCluster(oldPositionOfTheCluster, newPositionForTheCluster);

//            std :: cout << oldPositionOfTheCluster << ' ' << tempCluster.getData() << std :: endl;

//            std :: cout << "to postions " << newPositionForTheCluster << "\n";
            newPositionForTheCluster = emptyPositions.dequeue();
        }else{
            break;
        }

        oldPositionOfTheCluster -= clusterSizeInFS;
        tempCluster.loadFromFile(sourceFile, oldPositionOfTheCluster);
    }

    std :: cout << "limit " <<  oldPositionOfTheCluster + clusterSizeInFS << std :: endl;
    /*
    PriorityQueue<size_t> positionsOfClustersToRemove;
    positionsOfClustersToRemove.enqueue(position);

    //closest position to the end of the file
    size_t maxPosition = position;
    //closest position to the beginning of the file
    size_t minPosition = position;

    while (tempCluster.getNext() != 0) {
        position = tempCluster.getNext();

        tempCluster.loadFromFile(sourceFile, position);

        if(tempCluster.isFirstInSequence()){
            throw std :: runtime_error("invalid cluster sequence");
        }else{
            positionsOfClustersToRemove.enqueue(position);
        }

        maxPosition = position > maxPosition ? position : maxPosition;
        minPosition = position < minPosition ? position : minPosition;
    }
    */

    /*
    //go to the last cluster in the file
    size_t positionOfLastCluster = endOfFile() - clusterSizeInFS;
    tempCluster.loadFromFile(sourceFile, positionOfLastCluster);

    size_t currentPosition = positionsOfClustersToRemove.dequeue();
    tempCluster.loadFromFile(sourceFile, currentPosition);

    while(!positionsOfClustersToRemove.isEmpty() && positionOfLastCluster > maxPosition){
        if(tempCluster.isFirstInSequence()){
            emptyPositions.enqueue(currentPosition);
        }else{
            replaceCluster(positionOfLastCluster, currentPosition);
        }

        tempCluster.loadFromFile(sourceFile, positionOfLastCluster);
        positionOfLastCluster = tempCluster.getPrev();


        currentPosition = positionsOfClustersToRemove.dequeue();
    }*/
/*
    std :: cout << "positions to remove \n";
    while (!positionsOfClustersToRemove.isEmpty()) {
        std :: cout << positionsOfClustersToRemove.dequeue() << std :: endl;
    }*/

    std :: cout << "empty positions: \n";

    while (!emptyPositions.isEmpty()) {
        std :: cout << emptyPositions.dequeue() << std :: endl;

    }
}
