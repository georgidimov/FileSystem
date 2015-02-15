#include "tst_filemanager.h"

void tst_FileManager :: testReading(){
    std :: fstream sourceFile("files/source_file",
                              std :: ios :: in | std :: ios :: out | std :: ios :: binary);
    FileManager f(sourceFile);

    //check reading from first position
    QString result(f.read(4).getValue());
    QCOMPARE(result, QString("123456789"));

    //check reading from "random" position
    result = f.read(44).getValue();
    QCOMPARE(result, QString("someStrangeTextHere"));
}


void tst_FileManager :: testWriting(){
    std :: fstream sourceFile("files/temp_file",
                              std :: ios :: in | std :: ios :: out | std :: ios :: binary);
    FileManager f(sourceFile);

    //check with random text and length
    size_t pos = f.write("test writing in file", 20);
    QString result = f.read(pos).getValue();
    QCOMPARE(result, QString("test writing in file"));

    //check with special chars
    pos = f.write("and\n", 4);
    result = f.read(pos).getValue();
    QCOMPARE(result, QString("and\n"));

    //check with numbers and strange symbols
    pos = f.write("`0@0!", 5);
    result = f.read(pos).getValue();
    QCOMPARE(result, QString("`0@0!"));

    //check with only null-terminated char
    pos = f.write("", 1);
    result = f.read(pos).getValue();
    QCOMPARE(result, QString(""));
}
