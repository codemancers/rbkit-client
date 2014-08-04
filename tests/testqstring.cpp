#include <QtTest/QtTest>
#include <QThread>
#include "AutoTest.h"

class TestQString: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};

void TestQString::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));

    QThread::usleep(1000 * 1000 * 10);
}

DECLARE_TEST(TestQString)
