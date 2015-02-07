#include "filemanager.h"

FileManager :: FileManager(std :: fstream & file, size_t fileBeginning) : sourceFile(file){


    if(!sourceFile.is_open()){
        throw std :: runtime_error("wrong source file path");
    }

    clusterSize = 4;
    firstPositionInFile = fileBeginning;
}

FileManager :: ~FileManager(){
    sourceFile.close();
}

void FileManager :: write(const char * data, size_t size){
    //define count of clusters needed for saving data
    size_t neededClusters = ceil( (double)size / clusterSize );

    sourceFile.seekg(sourceFile.end);

    size_t tempSize = size;
    size_t tempClusterSize = clusterSize;
    size_t clusterRealSize = sizeof(size_t) * 3 + tempClusterSize;
    size_t prev = 0;
    size_t next = sourceFile.end + clusterRealSize;

    //for every cluster write position of previous, next,
    //size of storing data and data
    for(size_t i = 0; i < neededClusters; ++i){
        if(tempSize > clusterSize){
            tempClusterSize = clusterSize;
        }else{
            tempClusterSize = tempSize;
        }
        sourceFile.write((char *) & prev, sizeof(size_t));
        sourceFile.write((char *) & next, sizeof(size_t));

        sourceFile.write((char *) & tempClusterSize, sizeof(size_t));
        sourceFile.write(data + i * clusterSize, sizeof(char) * tempClusterSize);

        prev = next - clusterRealSize;
        next += clusterRealSize;
        tempSize -= tempClusterSize;
    }
}
