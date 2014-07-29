#include <QtTest/QtTest>
#include <QThread>

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

QTEST_MAIN(TestQString)
#include "testqstring.moc"
