#include "directory.h"

Directory :: Directory(Value newName, size_t size, Directory * newParent) : File(newName, size){
    parent = newParent;
}

Directory :: Directory(Value newName, size_t position, size_t newSize, size_t newSizeInFS, Directory *nParent) :
    File(newName, position, newSize, newSizeInFS)
{
    parent = nParent;
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

    return *it;
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

void Directory :: setParent(Directory * newParent){
    parent = newParent;
}

Directory * Directory :: getParent() const{
    return parent;
}

Value Directory :: serialize() const{
    Value serialized = File :: serialize();

    //serialized = serialized + ":" + Value(files.getSize()) + ":";
    serialized = serialized + ":" + Value(files.getSize());
    for(List<File *> :: Iterator it = files.begin(); it; it++){
        serialized = serialized + (*it)->serialize();
    }

    serialized = serialized + ":" + Value(directories.getSize());
    for(List<Directory *> :: Iterator it = directories.begin(); it; it++){
        //serialized = serialized + (*it)->serialize().length() + (*it)->serialize();
        serialized = serialized + (*it)->serialize();
    }

    return Value(":") + Value(serialized.length()) + serialized;
    //return Value(":") + serialized;
}

Value Directory::deserialize(Value serialized){

    std :: cout << "now " << serialized << "\n";
    clear();
    size_t length = serialized.length();

    size_t i = 1;
    for(; i < length; ++i){
        if(serialized[i] < '0' || serialized[i] > '9'){
            break;
        }
    }

    size_t serializedStringLength = Value(serialized, 1, i).toNumber();

    Value restOfTheString = Value(serialized, serializedStringLength + i, length);
    serialized = Value(serialized, i, serializedStringLength + i);

    serialized = File :: deserialize(serialized);

    for(i = 1; i < length; ++i){
        if(serialized[i] < '0' || serialized[i] > '9'){
            break;
        }
    }
    size_t delimiter = serialized.find(':', 1);

    size_t filesCount = Value(serialized, 1, i).toNumber();
    serialized = Value(serialized, delimiter, serialized.length());

    File * tempFile = NULL;

    for(i = 0; i < filesCount; ++i){
        tempFile = new File("", 0);
        serialized = tempFile->deserialize(serialized) + Value(":");
        addFile(tempFile);

    }
    tempFile = NULL;

    for(i = 1; i < length; ++i){
        if(serialized[i] < '0' || serialized[i] > '9'){
            break;
        }
    }
    delimiter = serialized.find(':', 1);

    size_t directoriesCount = Value(serialized, 1, i).toNumber();


    serialized = Value(serialized, delimiter, serialized.length());
    //std :: cout << "\n" << directoriesCount << "\n" << serialized;



    Directory * tempDir = NULL;
    //:44:36:Sub Dir:0:0:0:1423751368:1423751368:0:0:
    //std :: cout << "\n:44:36:Sub Dir:0:0:0:1423751368:1423751368:0:0:\n";

    for(i = 0; i < directoriesCount; ++i){
        tempDir = new Directory("", 0, NULL);
        serialized = tempDir->deserialize(serialized); //+ Value(":");
        addDirectory(tempDir);

    }
    tempDir = NULL;

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
