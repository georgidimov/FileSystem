#include "filemanager.h"

FileManager :: FileManager(std :: fstream & file, size_t fileBeginning) : sourceFile(file){


    if(!sourceFile.is_open()){
        throw std :: runtime_error("wrong source file path");
    }

    clusterSize = 4;
    firstPositionInFile = fileBeginning;
}

FileManager :: ~FileManager(){

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

        sourceFile.write((char *) & prev, sizeof(size_t));
        sourceFile.write((char *) & next, sizeof(size_t));

        sourceFile.write((char *) & tempClusterSize, sizeof(size_t));
        sourceFile.write(data + i * clusterSize, sizeof(char) * tempClusterSize);

        prev = next - clusterRealSize;
        next += clusterRealSize;
        tempSize -= tempClusterSize;
    }

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

    char * data = new char[clusterSize];

    size_t positionOfPrevCluster;
    size_t positionOfNextCluster;
    size_t dataSize;

    do{
        sourceFile.read((char *) & positionOfPrevCluster, sizeof(size_t));
        sourceFile.read((char *) & positionOfNextCluster, sizeof(size_t));
        sourceFile.read((char *) & dataSize, sizeof(size_t));
        sourceFile.read(data, dataSize * sizeof(char));

        resultData = resultData + Value(data, dataSize);

        sourceFile.seekg(positionOfNextCluster);
    }while(positionOfNextCluster > 0);


    delete [] data;
    data = NULL;

    return resultData + Value("\0");
}
