#include "fstree.h"

FSTree :: FSTree(){
    root = new Directory("root", 0, NULL);
    //root = NULL;
}

FSTree :: ~FSTree(){
    delete root;
}

Value FSTree :: getSerializedTree() const{
    return root->serialize();
}

void FSTree :: deserialize(Value serialized){
    //create root folder
    delete root;
    root = new Directory(serialized);
}

Directory * FSTree :: getDirectory(Value path, Directory * currentDir) const{
    if(path == Value("")){
        return NULL;
    }

    if(path == "root"){
        return root;
    }

    char * tempTagCharP = strtok(path.getValue(), "/");
    Value tempTagPath(tempTagCharP);

    if(path == "root"){
        currentDir = root;
    }

    //slice path and search for child in current directory
    tempTagCharP = strtok(NULL, "/");
    while(tempTagCharP != NULL){


        tempTagPath = Value(tempTagCharP);

        currentDir = currentDir->getDirectory(tempTagPath);

        tempTagCharP = strtok(NULL, "/");
    }

    return currentDir;
}

Directory * FSTree :: getDirectory(Value path) const{
    return getDirectory(path, root);
}

void FSTree :: addDirectory(Value path, Value name){
    Directory * parent = getDirectory(path);

    Directory * newDirectory = new Directory(name, 0, parent);
    parent->addDirectory(newDirectory);
}

void FSTree :: removeDirectory(Value path, Value name){
    Directory * parent = getDirectory(path);
    parent->deleteDirectory(name);
}

void FSTree :: renameDirectory(Value path, Value name, Value newName){
    getDirectory(path + "/" + name)->setName(newName);
}

void FSTree :: moveDirectory(Value path, Value name, Value newPath){
    Directory * parent = getDirectory(path);
    Directory * directory = parent->detachDirectory(name);

    parent = getDirectory(newPath);
    parent->addDirectory(directory);
}

void FSTree :: copyDirectory(Value path, Value name, Value newPath){
    Directory * newDirectory = getDirectory(path + "/" + name);
    newDirectory = newDirectory->getCopy();

    Directory * parent = getDirectory(newPath);
    newDirectory->setParent(parent);




    ///FIX FILES
    parent->addDirectory(newDirectory);
}

void FSTree :: printDirectoryInfo(Value path) const{
    getDirectory(path)->printContent();
}

void FSTree :: createFile(Value path, Value name, size_t position){
    Directory * parent = getDirectory(path);

    File * file = new File(name, position, 0, 0);

    parent->addFile(file);
}

void FSTree :: removeFile(Value path, Value name){
    getDirectory(path)->deleteFile(name);
}

void FSTree :: printFileInfo(Value path, Value name) const{
    Directory * parent = getDirectory(path);
    File * file = parent->getFile(name);
    file->printContent();
}

Value FSTree :: getSerializedFile(Value path, Value name) const{
    Directory * parent = getDirectory(path);
    File * file =  parent->getFile(name);

    return file->serialize();
}
