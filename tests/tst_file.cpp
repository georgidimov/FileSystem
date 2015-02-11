#include "tst_file.h"

void tst_File :: testSerialization(){
    Value fileName = "fil3_NAme!";
    size_t position = 2;
    size_t size = 123;
    size_t realSize = 1235;

    File file(fileName, position, size, realSize);

    //find delimiter before timestamps
    size_t delimiter = fileName.length() + ceil(log10(position)) + ceil(log10(size)) + ceil(log10(realSize));
    //add also ':'
    delimiter += 4;

    QString result(Value(file.serialize(), delimiter).getValue());
    QCOMPARE(result, QString("fil3_NAme!:2:123:1235:"));
}

void tst_File :: testDeserialization(){
    File file(Value("File_Name:1:0:15:1423655595:1423655597"));

    Value fileName = "File_Name";
    size_t position = 1;
    size_t size = 0;
    size_t realSize = 15;
    time_t created = 1423655595;
    time_t lastModified = 1423655597;

    QVERIFY2(file.getName() == fileName, "problem with file name");
    QVERIFY2(file.getPosition() == position, "problem with file position");
    QVERIFY2(file.getSize() == size, "problem with file size");
    QVERIFY2(file.getSizeInFileSystem() == realSize, "problem with file size in FS");
    QVERIFY2(file.getCreationTime() == Value( ctime(&created) ), "problem with file creation time stamp");
    QVERIFY2(file.getLastModifiedTime() == Value( ctime(&lastModified) ), "problem with file last modified time stamp");
}

