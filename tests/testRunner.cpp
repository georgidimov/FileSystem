#include <QTest>
#include <tst_filemanager.h>
#include <tst_file.h>

int main(){
    //tests for FileManager
    tst_FileManager fileManager;

    std :: cout << std :: endl;
    QTest :: qExec(&fileManager);


    //######################################
    //tests for File
    tst_File file;

    std :: cout << std :: endl;
    QTest :: qExec(&file);

    return 0;
}
