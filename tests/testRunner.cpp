#include <QTest>
#include <tst_filemanager.h>
#include <tst_file.h>
#include <tst_directory.h>

int main(){
    //######################################
    //tests for FileManager
    tst_FileManager fileManager;

    std :: cout << std :: endl;
    QTest :: qExec(&fileManager);


    //######################################
    //tests for File
    tst_File file;

    std :: cout << std :: endl;
    QTest :: qExec(&file);

    //######################################
    //tests for Directory
    tst_Directory dir;

    std :: cout << std :: endl;
    QTest :: qExec(&dir);
    return 0;
}
