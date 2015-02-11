#include "directory.h"

Directory :: Directory(Value newName, size_t size) : File(newName, size){
    ;
}

Directory :: Directory(Value newName, size_t position, size_t newSize, size_t newSizeInFS) :
    File(newName, position, newSize, newSizeInFS)
{

}

Directory :: ~Directory(){
    int filesCount = files.getSize();

    while(filesCount){
        delete files[0];
        files.removeAt(0);

        --filesCount;
    }

    int dirsCount = directies.getSize();

    while(dirsCount){
        delete directies[0];
        directies.removeAt(0);
        --dirsCount;
    }
}

void Directory :: addFile(File * file){
    files.addAt(0, file);
}

void Directory :: addDirectory(Directory * dir){
    directies.addAt(0, dir);
}

File * Directory :: getFile(Value name){
    for(List<File *> :: Iterator it = files.begin(); it; it++){
        if((*it)->getName() == name){
            return *it;
        }
    }

    return NULL;
}

Directory * Directory :: getDirectory(Value name){
    for(List<Directory *> :: Iterator it = files.begin(); it; it++){
        if((*it)->getName() == name){
            return *it;
        }
    }

    return NULL;
}
