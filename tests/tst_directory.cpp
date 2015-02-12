#include "tst_directory.h"

void tst_Directory :: testSingleFolderSerialization(){
    Directory dir("D!rName", 1, 2, 3, NULL);
    Value serialiazed = dir.serialize();
    time_t currentTime;
    time(&currentTime);
    Value result = ":44:36:D!rName:1:2:3:";
    result = result +  Value(currentTime) + ":" + Value(currentTime) + ":0:0";

    QCOMPARE(QString(serialiazed.getValue()), QString(result.getValue()));
}

void tst_Directory :: testMultipleFoldersSerialization(){
    File * f1 = new File("f111", 102);
    File * f2 = new File("f2", 0);
    File * f3 = new File("f3", 0);

    Directory * dir = new Directory("dir", 1, NULL);
    Directory * subDir = new Directory("subDir", 5, dir);

    dir->addFile(f1);
    dir->addFile(f2);
    subDir->addFile(f3);
    dir->addDirectory(subDir);
    Value serialized = dir->serialize();
    time_t currentTime;
    time(&currentTime);
    Value time = Value(currentTime) + ":" + Value(currentTime);
    Value result = ":191:32:dir:0:1:0:";
    result = result + time + ":2:31:f2:0:0:0:" + time + ":35:f111:0:102:0:" + time + ":1:77:35:subDir:0:5:0:" + time + ":1:31:f3:0:0:0:" + time + ":0";

    QCOMPARE(QString(serialized.getValue()), QString(result.getValue()));

    delete dir;
}

void tst_Directory :: testDeserialization(){
    Directory dir(":76:35:subDir:0:5:0:1423770641:1423770641:1:31:f3:1:2:3:1423771641:1423775641:0");

    Value dirName = "subDir";
    size_t position = 0;
    size_t size = 5;
    size_t realSize = 0;
    time_t created = 1423770641;
    time_t lastModified = 1423770641;

    QVERIFY2(dir.getName() == dirName, "problem with directory name");
    QVERIFY2(dir.getPosition() == position, "problem with directory position");
    QVERIFY2(dir.getSize() == size, "problem with directory size");
    QVERIFY2(dir.getSizeInFileSystem() == realSize, "problem with directory size in FS");
    QVERIFY2(dir.getCreationTime() == Value( ctime(&created) ), "problem with directory creation time stamp");
    QVERIFY2(dir.getLastModifiedTime() == Value( ctime(&lastModified) ), "problem with directory last modified time stamp");

    File * file = dir.getFile("f3");

    Value fileName = "f3";
    position = 1;
    size = 2;
    realSize = 3;
    created = 1423771641;
    lastModified = 1423775641;

    QVERIFY2(file->getName() == fileName, "problem with file name");
    QVERIFY2(file->getPosition() == position, "problem with file position");
    QVERIFY2(file->getSize() == size, "problem with file size");
    QVERIFY2(file->getSizeInFileSystem() == realSize, "problem with file size in FS");
    QVERIFY2(file->getCreationTime() == Value( ctime(&created) ), "problem with file creation time stamp");
    QVERIFY2(file->getLastModifiedTime() == Value( ctime(&lastModified) ), "problem with file last modified time stamp");
}
    
void tst_Directory :: testRestOfDeserealization(){
    Directory dir("name", 0, NULL);
    Value serialized = ":76:35:subDir:0:5:0:1423770641:1423770641:1:31:f3:1:2:3:1423771641:1423775641:0A!123";
    Value rest = dir.deserialize(serialized);
    QVERIFY2(rest == Value("A!123"), "problem return in deserialization");
}

