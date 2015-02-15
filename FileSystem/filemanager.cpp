#include "filemanager.h"

FileManager :: FileManager(std :: fstream & file, std::streampos fileBeginning) : sourceFile(file){
    if(!sourceFile.is_open()){
        throw std :: runtime_error("wrong sourcefile path");
    }

    //make zero position unvailable - prevent writing over "system information"
    if(endOfFile() == 0){
        size_t nil = 0;
///REMOVE MEE!
        std :: cout << "call";
        sourceFile.seekg(0);
        sourceFile.write((char *) & nil, sizeof(size_t));
    }
    clusterSize = 8;
    ///Fix me - add positions as empty
    //load configuration from the file
    deserialize();
    std :: cout << "\n";
    for(List<size_t> :: Iterator it = emptyPositions.begin(); it; ++it){
        std :: cout << (*it) << ' ';
    }

    //for(size_t j = 0; j < emptyPositions.getSize(); ++j){
    //    std :: cout << emptyPositions[j] << ' ';
    //}


/*
    clusterSizeInFS = 3 * sizeof(size_t) + clusterSize;

    firstPositionInFile = fileBeginning;
    emptyPositions.enqueue(4);
    emptyPositions.enqueue(24);
*/

/*
    emptyPositions.enqueue(52);
    emptyPositions.enqueue(68);
    emptyPositions.enqueue(84);
    emptyPositions.enqueue(101);
    emptyPositions.enqueue(116);
*/
}

FileManager :: ~FileManager(){
    serialize();
}

void FileManager :: serialize(){
    Value serialized = "";
    serialized = Value(clusterSize) + ":" + Value(clusterSizeInFS) + ":" + Value(firstPositionInFile) + ":";
    serialized = serialized + Value(emptyPositions.getSize());

    for(List<size_t> :: Iterator it = emptyPositions.begin(); it; ++it){
        serialized = serialized + Value(":") + *it;
    }

    serialized = serialized + ":";
/*
    //make temporary queue, that will be used to save values from empty positions.
    //write method won`t use them
    PriorityQueue<size_t> tempQueue;
    while(!emptyPositions.isEmpty()){
        tempQueue.enqueue(emptyPositions.dequeue());
    }
*/

    //std :: cout << serialized;

    size_t positionOfSerializedString = write(serialized.getValue(), serialized.length());
    //std :: cout << "\npositionOfSerializedString" << positionOfSerializedString << "\n";
//    sourceFile.seekg(endOfFile());


    sourceFile.seekg(0);
    sourceFile.write((char *) &positionOfSerializedString, sizeof(size_t));
/*
    while(!tempQueue.isEmpty()){
        emptyPositions.enqueue(tempQueue.dequeue());
    }
*/
}

void FileManager :: deserialize(){
    //4:16:2:5:116:101:84:68:52:26
    //sourceFile.seekg(endOfFile() - (std ::streampos)sizeof(size_t));
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
    firstPositionInFile = Value(serialized, i, delimiter).toNumber();

    i = delimiter + 1;
    delimiter = serialized.find(':', i);
    size_t positionsCount = Value(serialized, i, delimiter).toNumber();

    size_t position;
    for(size_t j = 0; j < positionsCount; ++j){
        i = delimiter + 1;
        delimiter = serialized.find(':', i);
        position = Value(serialized, i, delimiter).toNumber();

        //emptyPositions.addAt(0, position);
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
    ///FIX ME
    if(emptyPositions.isEmpty()){
        positionOfFirstCluster = endOfFile();
    }else{
        //positionOfFirstCluster = emptyPositions.getAt(0);
        positionOfFirstCluster = emptyPositions.dequeue();
        clustersInSequence = false;
    }


    sourceFile.seekg(positionOfFirstCluster);

    size_t tempSize = size;
    size_t tempClusterSize = clusterSize;
    //size_t clusterRealSize = sizeof(size_t) * 3 + tempClusterSize;
    size_t clusterRealSize = clusterSizeInFS;
    std :: streampos prev = 0;
    std :: streampos next;// = positionOfFirstCluster + clusterRealSize;

    if(clustersInSequence){  //clusters are in sequence
        next = positionOfFirstCluster + clusterRealSize;
    }else{                  //clusters are not in sequence
        if(!emptyPositions.isEmpty()){
            //next = emptyPositions.getAt(0);
            next = emptyPositions.peek();
        }else{
            next = endOfFile();
            //clustersInSequence = true;
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
            //prev = emptyPositions.removeAt(0);
            //prev = emptyPositions.dequeue();

            if(next == endOfFile()){
                sourceFile.seekg(endOfFile());
                next += clusterRealSize;
                clustersInSequence = true;
            }else{
                //sourceFile.seekg(next);
                sourceFile.seekg(emptyPositions.dequeue());

                if(!emptyPositions.isEmpty()){
                    //next = emptyPositions.getAt(0);
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

void FileManager :: remove(std::streampos position){
    isValidPositionInFile(position);


    Cluster tempCluster(clusterSize);
    tempCluster.loadFromFile(sourceFile, position);
    tempCluster.markAsInvalid(sourceFile, position);

    //if(!tempCluster.isValidCluster(sourceFile, position)){
    //    throw std :: runtime_error("try to remove wrong cluster");
    //}

    if(!tempCluster.isFirstInSequence()){
        //throw std :: runtime_error("start removing from not first cluster");
    }

    //closest position to the end of the file
    std :: streampos maxPosition = position;

    //add clusters from the sequence to the queue of "empty" clusters
    while(!tempCluster.isLastInSequence()) {
        //emptyPositions.addAt(0, position);
        emptyPositions.enqueue(position);
        position = tempCluster.getNext();
        tempCluster.loadFromFile(sourceFile, position);
        tempCluster.markAsInvalid(sourceFile, position);
        maxPosition = position > maxPosition ? position : maxPosition;
    }

    //add also the last one
    //emptyPositions.addAt(0, position);
    emptyPositions.enqueue(position);

    /*
    //go to the last cluster in the file
    size_t positionOfLastCluster = endOfFile() - (std :: streampos) clusterSizeInFS;
    tempCluster.loadFromFile(sourceFile, positionOfLastCluster);
    sourceFile.seekg(positionOfLastCluster);

    size_t newPositionForTheCluster = emptyPositions.getAt(0);
    size_t oldPositionOfTheCluster = positionOfLastCluster;

    while(oldPositionOfTheCluster > maxPosition && !emptyPositions.isEmpty()){

        if(!tempCluster.isFirstInSequence()){
            replaceCluster(oldPositionOfTheCluster, newPositionForTheCluster);

//            std :: cout << oldPositionOfTheCluster << ' ' << tempCluster.getData() << std :: endl;

//            std :: cout << "to postions " << newPositionForTheCluster << "\n";
            newPositionForTheCluster = emptyPositions.getAt(0);
        }else{
            break;
        }

        oldPositionOfTheCluster -= clusterSizeInFS;
        tempCluster.loadFromFile(sourceFile, oldPositionOfTheCluster);
    }

    std :: cout << "limit " <<  oldPositionOfTheCluster + clusterSizeInFS << std :: endl;

    std :: cout << "empty positions: \n";

    while (!emptyPositions.isEmpty()) {
        std :: cout << emptyPositions.getAt(0) << std :: endl;

    }*/
}
