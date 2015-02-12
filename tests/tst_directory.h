#include <QtTest/QtTest>
#include <directory.h>

class tst_Directory : public QObject{
    Q_OBJECT
private slots:
    void testSingleFolderSerialization();
    void testMultipleFoldersSerialization();
    void testDeserialization();
    void testRestOfDeserealization();
};

