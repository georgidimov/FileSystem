#include "filesystemmanager.h"

FileSystemManager :: FileSystemManager(std :: fstream & file) : fileManager(file){
    size_t treePosition = fileManager.getTreePosition();
    if(treePosition){
        Value serializedTree = fileManager.read(treePosition);
        fstree.deserialize(serializedTree);
        //std :: cout << serializedTree << "\n";// << ":86:33:root:0:0:0:1424037741:1424037741:1:43:myFirstFile2:124:0:0:1424037760:1424037760:0";
    }
}

FileSystemManager :: ~FileSystemManager(){
    Value serialized = fstree.getSerializedTree();

    size_t treePos = fileManager.getTreePosition();
    if(treePos){
        //fileManager.remove(treePos);
    }
    //std :: cout << serialized << "\n";
    fileManager.writeTree(serialized.getValue(), serialized.length());
}

void FileSystemManager :: createDirectory(Value name, Value path){
    fstree.addDirectory(name, path);
}

void FileSystemManager :: removeDirectory(Value path, Value name){
    fstree.removeDirectory(path, name);
}

void FileSystemManager :: renameDirectory(Value path, Value name, Value newName){
    fstree.renameDirectory(path, name, newName);
}

void FileSystemManager :: moveDirectory(Value path, Value name, Value newPath){
    fstree.moveDirectory(path, name, newPath);
}

void FileSystemManager :: copyDirecoty(Value path, Value name, Value newPath){
    fstree.copyDirectory(path, name, newPath);
}

void FileSystemManager :: printDirectoryInfo(Value path) const{
    fstree.printDirectoryInfo(path);
}

void FileSystemManager :: createFile(Value path, Value name){
    fstree.createFile(path, name, fileManager.getEmptyPosition());

    Value serializedFile = fstree.getSerializedFile(path, name);
    fileManager.write(serializedFile.getValue(), serializedFile.length());
}


void FileSystemManager :: removeFile(Value path, Value name){
    fstree.removeFile(path, name);
}

void FileSystemManager :: printFileInfo(Value path, Value name) const{
    fstree.printFileInfo(path, name);
}