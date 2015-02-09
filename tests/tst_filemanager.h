#include <QtTest/QtTest>
#include <filemanager.h>

class tst_FileManager : public QObject{
    Q_OBJECT
private slots:
    void testReading();
    void testWriting();
};

