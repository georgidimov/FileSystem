#include "directory.h"

Directory :: Directory(Value newName, size_t size, Directory * newParent) : File(newName, size){
    parent = newParent;
}

Directory :: Directory(Value newName, size_t position, size_t newSize, size_t newSizeInFS, Directory *nParent) :
    File(newName, position, newSize, newSizeInFS)
{
    parent = nParent;
}

Directory :: Directory(Value serializedString) : File("", 0){
    deserialize(serializedString);
}

Directory :: ~Directory(){
    clear();
}

void Directory :: clear(){
    for(List<File *> :: Iterator it = files.begin(); it; it++){
        delete *it;
    }

    for(List<Directory *> :: Iterator it = directories.begin(); it; it++){
        delete *it;
    }

    size_t size = files.getSize();

    while(size){
        files.removeAt(0);
        --size;
    }

    size = directories.getSize();
    while(size){
        directories.removeAt(0);
        --size;
    }
}

void Directory :: addFile(File * file){
    files.addAt(0, file);
}

void Directory :: addDirectory(Directory * dir){
    directories.addAt(0, dir);
}

File * Directory :: getFile(Value name){
    List<File *> :: Iterator it = files.begin();
    for(; it; it++){
        if((*it)->getName() == name){
            return *it;
        }
    }

    return *it;
}

Directory * Directory :: getDirectory(Value name){
    List<Directory *> :: Iterator it = directories.begin();
    for(; it; it++){
        if((*it)->getName() == name){
            return *it;
        }
    }

    throw std :: runtime_error("invalid directory name or path");
}

void Directory :: deleteFile(Value name){
    //FIX US!
    size_t size = files.getSize();

    for(size_t i = 0; i < size; ++i){
        if(files.getAt(i)->getName() == name){
            delete files[i];
            files.removeAt(i);
            return;
        }
    }
}

void Directory :: deleteDirectory(Value name){
    size_t size = directories.getSize();

    for(size_t i = 0; i < size; ++i){
        if(directories.getAt(i)->getName() == name){
            delete directories[i];
            directories.removeAt(i);

            return;
        }
    }
}

Directory * Directory :: detachDirectory(Value name){
    size_t size = directories.getSize();

    for(size_t i = 0; i < size; ++i){
        if(directories.getAt(i)->getName() == name){
            return directories.removeAt(i);

        }
    }

    throw std :: runtime_error("wrong directory name or path");
}

File * Directory :: detachFile(Value name){
    //FIX US!
    size_t size = files.getSize();

    for(size_t i = 0; i < size; ++i){
        if(files.getAt(i)->getName() == name){
            return files.removeAt(i);
        }
    }

    throw std :: runtime_error("wrong file name or path");
}

void Directory :: setParent(Directory * newParent){
    parent = newParent;
}

Directory * Directory :: getParent() const{
    return parent;
}

Directory * Directory :: getCopy() const{
    Directory * newDirectory = new Directory(name, positionInFile, size, sizeInFileSystem, NULL);

    newDirectory->setCreationTime(creationTime);
    newDirectory->setLastModifiedTime(lastModifiedTime);

    Directory * tempChild = NULL;
    for(List<Directory *> :: Iterator it = directories.begin(); it; it++){
        tempChild = (*it)->getCopy();
        tempChild->setParent(newDirectory);
        newDirectory->addDirectory(tempChild);
    }

    File * tempFile = NULL;
    for(List<File *> :: Iterator it = files.begin(); it; it++){
        tempFile = (*it)->getCopy();
        newDirectory->addFile(tempFile);
    }

    return newDirectory;
}

Value Directory :: serialize() const{
    Value serialized = File :: serialize();

    serialized = serialized + ":" + Value(files.getSize());
    for(List<File *> :: Iterator it = files.begin(); it; it++){
        serialized = serialized + (*it)->serialize();
    }

    serialized = serialized + ":" + Value(directories.getSize());
    for(List<Directory *> :: Iterator it = directories.begin(); it; it++){
        serialized = serialized + (*it)->serialize();
    }

    //return Value(":") + Value(serialized.length() + 1) + serialized;
    return Value(":") + Value(serialized.length()) + serialized;
}

Value Directory::deserialize(Value serialized){
    //remove current content
    clear();

    //define part of the string for this folder
    size_t length = serialized.length();

    size_t i = 1;
    for(; i < length; ++i){
        if(serialized[i] < '0' || serialized[i] > '9'){
            break;
        }
    }

    size_t serializedStringLength = Value(serialized, 1, i).toNumber();

    //split part of the string that is not needed
    Value restOfTheString = Value(serialized, serializedStringLength + i, length);

    //load information for the current folder - name, size and etc.
    serialized = Value(serialized, i, serializedStringLength + i);
    serialized = File :: deserialize(serialized);


    //read part for the files in the folder
    for(i = 1; i < length; ++i){
        if(serialized[i] < '0' || serialized[i] > '9'){
            break;
        }
    }
    size_t delimiter = serialized.find(':', 1);
    size_t filesCount = Value(serialized, 1, i).toNumber();

    serialized = Value(serialized, delimiter, serialized.length());

    File * tempFile = NULL;
    //add files in the folder
    for(i = 0; i < filesCount; ++i){
        tempFile = new File("", 0);
        serialized = tempFile->deserialize(serialized) + Value(":");
        addFile(tempFile);

    }
    tempFile = NULL;


    //read part for the subfolders
    for(i = 1; i < length; ++i){
        if(serialized[i] < '0' || serialized[i] > '9'){
            break;
        }
    }

    delimiter = serialized.find(':', 1);

    size_t directoriesCount = Value(serialized, 1, i).toNumber();
    serialized = Value(serialized, delimiter, serialized.length());
    //std :: cout <<"\n\n s"<< serialized << "d " << delimiter << "l " << serialized.length() << "\n";
    //add subfolders
    Directory * tempDir = NULL;
    for(i = 0; i < directoriesCount; ++i){
        tempDir = new Directory("", 0, NULL);
        serialized = tempDir->deserialize(serialized);
        addDirectory(tempDir);

    }

    tempDir = NULL;

    //return the part of the string that is for the other folders
    return restOfTheString;
}

void Directory :: printContent() const{
    File :: printContent();

    std :: cout << "\nFiles:" << files.getSize() << "\n";
    for(List<File *> :: Iterator it = files.begin(); it; it++){
        std :: cout << (*it)->getName() << "\n";
    }

    std :: cout << "\nDirectories:" <<directories.getSize() << "\n";
    for(List<Directory *> :: Iterator it = directories.begin(); it; it++){
        std :: cout << (*it)->getName() << "\n";
    }
}
