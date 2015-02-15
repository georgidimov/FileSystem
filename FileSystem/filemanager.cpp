#include "filemanager.h"

FileManager :: FileManager(std :: fstream & file) : sourceFile(file){
    if(!sourceFile.is_open()){
        throw std :: runtime_error("wrong sourcefile path");
    }

    //make zero position unvailable - prevent writing over "system information"
    if(endOfFile() == 0){
        size_t nil = 0;
        sourceFile.seekg(0);
        sourceFile.write((char *) & nil, sizeof(size_t));
    }
    clusterSize = 8;
    clusterSizeInFS = 3 * sizeof(size_t) + clusterSize;
    treePosition = 0;

    //load configuration from the file
    if(endOfFile()){
        deserialize();
    }
}

FileManager :: ~FileManager(){
    serialize();
}

void FileManager :: serialize(){
    Value serialized = "";
    serialized = Value(clusterSize) + ":" + Value(clusterSizeInFS) + ":" + Value(treePosition) + ":";
    serialized = serialized + Value(emptyPositions.getSize());

    for(List<size_t> :: Iterator it = emptyPositions.begin(); it; ++it){
        serialized = serialized + Value(":") + *it;       
    }

    serialized = serialized + ":";

    size_t positionOfSerializedString = write(serialized.getValue(), serialized.length());

    sourceFile.seekg(0);
    sourceFile.write((char *) &positionOfSerializedString, sizeof(size_t));
}

void FileManager :: deserialize(){
    sourceFile.seekg(0);
    size_t positionOfSerializedString;
    //read how long is serialized string
    sourceFile.read((char *) &positionOfSerializedString, sizeof(size_t));

    Value serialized = read(positionOfSerializedString);
    //remove(positionOfSerializedString);

    size_t i;
    size_t delimiter = serialized.find(':', 1);
    clusterSize = Value(serialized, delimiter).toNumber();

    i = delimiter + 1;
    delimiter = serialized.find(':', i);
    clusterSizeInFS = Value(serialized, i, delimiter).toNumber();

    i = delimiter + 1;
    delimiter = serialized.find(':', i);
    treePosition = Value(serialized, i, delimiter).toNumber();

    i = delimiter + 1;
    delimiter = serialized.find(':', i);
    size_t positionsCount = Value(serialized, i, delimiter).toNumber();

    size_t position;
    for(size_t j = 0; j < positionsCount; ++j){
        i = delimiter + 1;
        delimiter = serialized.find(':', i);
        position = Value(serialized, i, delimiter).toNumber();

        emptyPositions.enqueue(position);
    }

    Cluster temp(clusterSize);

    do{
        if(!emptyPositions.contains(positionOfSerializedString)){
            emptyPositions.enqueue(positionOfSerializedString);
        }

        temp.loadFromFile(sourceFile, positionOfSerializedString);
        positionOfSerializedString = temp.getNext();
    }while(positionOfSerializedString);      
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

    if(emptyPositions.isEmpty()){
        positionOfFirstCluster = endOfFile();
    }else{
        positionOfFirstCluster = emptyPositions.dequeue();
        clustersInSequence = false;
    }


    sourceFile.seekg(positionOfFirstCluster);

    size_t tempSize = size;
    size_t tempClusterSize = clusterSize;

    size_t clusterRealSize = clusterSizeInFS;
    std :: streampos prev = 0;
    std :: streampos next;

    if(clustersInSequence){  //clusters are in sequence
        next = positionOfFirstCluster + clusterRealSize;
    }else{                  //clusters are not in sequence
        if(!emptyPositions.isEmpty()){
            next = emptyPositions.peek();
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
                sourceFile.seekg(emptyPositions.dequeue());

                if(!emptyPositions.isEmpty()){
                    next = emptyPositions.peek();
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

size_t FileManager :: writeTree(const char * data, size_t size){
    size_t position = write(data, size);
    treePosition  = position;
    return position;
}

Value FileManager :: read(std :: streampos position) const{
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

void FileManager :: append(const char * data, size_t size, size_t start){
    Cluster tempCluster(clusterSize);
    tempCluster.loadFromFile(sourceFile, start);

    while(tempCluster.getNext()){
        start = tempCluster.getNext();
        tempCluster.loadFromFile(sourceFile, start);
    }

    //tempCluster.loadFromFile(sourceFile, start);
    size_t next = write(data, size);

    tempCluster.loadFromFile(sourceFile, start);
    tempCluster.setNext(next);
    tempCluster.writeToFile(sourceFile, start);

    tempCluster.loadFromFile(sourceFile, next);
    tempCluster.setPrev(start);
    tempCluster.writeToFile(sourceFile, next);
}


void FileManager :: remove(std::streampos position){
    isValidPositionInFile(position);


    Cluster tempCluster(clusterSize);
    tempCluster.loadFromFile(sourceFile, position);
    tempCluster.markAsInvalid(sourceFile, position);


    //add clusters from the sequence to the queue of "empty" clusters
    while(!tempCluster.isLastInSequence()) {
        emptyPositions.enqueue(position);
        position = tempCluster.getNext();
        tempCluster.loadFromFile(sourceFile, position);
        tempCluster.markAsInvalid(sourceFile, position);
    }

    //add also the last one
    emptyPositions.enqueue(position);
}

size_t FileManager :: getEmptyPosition() const{
    if(!emptyPositions.isEmpty()){
        return emptyPositions.peek();
    }else{
        return endOfFile();
    }
}

size_t FileManager :: getTreePosition() const{
    return treePosition;
}
