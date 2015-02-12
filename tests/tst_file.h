#include <QtTest/QtTest>
#include <file.h>

class tst_File : public QObject{
    Q_OBJECT
private slots:
    void testSerialization();
    void testDeserialization();
    void testRestOfDeserealization();
};

