#include "commadsparser.h"

CommadsParser :: CommadsParser(FileSystemManager & newFSManager) : FSmanager(newFSManager){
    commands = NULL;
    commandsCount = 0;
}

CommadsParser :: ~CommadsParser(){
    clear();
}

void CommadsParser :: clear(){
    for(size_t i = 0; i < commandsCount; ++i){
        delete [] commands[i];
        commands[i] = NULL;
    }

    delete [] commands;
}
